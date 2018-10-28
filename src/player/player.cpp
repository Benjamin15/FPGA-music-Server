<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mad.h>
#include <pulse/simple.h>
#include <pulse/error.h>

pa_simple *device = NULL;
int ret = 1;
int error;
struct mad_stream mad_stream;
struct mad_frame mad_frame;
struct mad_synth mad_synth;

void output(struct mad_header const *header, struct mad_pcm *pcm);

=======
#include "player.h"
>>>>>>> 6840440f5a8f619dfef525e1a952cb606b67dce9
int main(int argc, char **argv) {

    initPlayer();
    initMad();
    char *filename = argv[1];
    FILE *fp = fopen(filename, "r");
    int fd = fileno(fp);
    struct stat metadata = get_metadata(fd, filename, fp);

    // On laisse le kernel faire le travail d'optimistation de la mémoire.
    unsigned char *input_stream = static_cast<unsigned char*> (mmap(0, metadata.st_size, PROT_READ, MAP_SHARED, fd, 0));

    mad_stream_buffer(&mad_stream, input_stream, metadata.st_size);

    decode();

    // On libere les ressources
    fclose(fp);
    free_mad();
    if (playback_handle)
        snd_pcm_close (playback_handle);

    return EXIT_SUCCESS;
}

/**
 * La méthode permet de créer un vecteur de sample et de l'envoyer au player alsa.
 * @param pcm : pointeur de pcm qui content la taille, les channels, les samples...
 */ 
void output(struct mad_pcm *pcm) {
    std::vector<char> stream;
    for (int i = 0 ; i < pcm->length ; i++) {
        signed int sample;
        sample = pcm->samples[0][i];
        stream.push_back((sample >> 0) & 0xff);
        stream.push_back((sample >> 8));
        stream.push_back((sample >> 16));
        stream.push_back((sample >> 24));

        if (pcm->channels == 2) {
            sample = pcm->samples[1][i];
            stream.push_back(((sample >> 0) & 0xff));
            stream.push_back((sample >> 8));
            stream.push_back((sample >> 16));
            stream.push_back((sample >> 24));
        }
    }
    
    if ((error = snd_pcm_writei (playback_handle, stream.data(), pcm->length)) != pcm->length) {
        fprintf (stderr, "write to audio interface failed (%s)\n",
                snd_strerror (error));
        exit (1);
    }
}

/**
 * Cette fonction permet d'initialiser le player. On l'initialise en 32 bit à 44.1kHz, avec un son en stéréo.
 * On choisi d'utiliser du 32 bits car le Alsa sur la FPGA ne sait lire que les samples en 32 bits.
 * En plus, cela fonctionne correctement si un appareil ne peut lire que des samples de 16 bits. L'inverse n'est pas vrai (ralentissement et perte)
 * 
 */ 
void initPlayer() {
        // Set up Alsa 32-bit 44.1kHz stereo output
    if ((error = snd_pcm_open (&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        fprintf (stderr, "cannot open audio device %s (%s)\n", 
                "default",
                snd_strerror (error));
        exit (1);
    }
		   
    if ((error = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
        fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
                snd_strerror (error));
        exit (1);
    }
                
    if ((error = snd_pcm_hw_params_any (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
                snd_strerror (error));
        exit (1);
    }

    if ((error = snd_pcm_hw_params_set_access (playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        fprintf (stderr, "cannot set access type (%s)\n",
                snd_strerror (error));
        exit (1);
    }

    if ((error = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S32_LE)) < 0) {
        fprintf (stderr, "cannot set sample format (%s)\n",
                snd_strerror (error));
        exit (1);
    }

    unsigned int rate = 44100;
    if ((error = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &rate, 0)) < 0) {
        fprintf (stderr, "cannot set sample rate (%s)\n",
                snd_strerror (error));
        exit (1);
    }

    if ((error = snd_pcm_hw_params_set_channels (playback_handle, hw_params, 2)) < 0) {
        fprintf (stderr, "cannot set channel count (%s)\n",
                snd_strerror (error));
        exit (1);
    }

    if ((error = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                snd_strerror (error));
        exit (1);
    }
}


/**
 * Cette fonction permet d'initialiser les variables de libmad pour le décodage
 * 
 */ 
void initMad() {
    mad_stream_init(&mad_stream);
    mad_synth_init(&mad_synth);
    mad_frame_init(&mad_frame);
}

/**
 * Cette fonction permet d'obtenir les metadatas d'un fichier.
 * @param fd : numero du fichier
 * @param filename : nom du fichier
 * @param fp : le fichier
 * @return metadata
 */  
struct stat get_metadata(int fd, char* filename, FILE* fp) {
    struct stat metadata;
    if (fstat(fd, &metadata) >= 0) {
        printf("File size %d bytes\n", (int)metadata.st_size);
    } else {
        printf("Failed to stat %s\n", filename);
        fclose(fp);
    }
    return metadata;
}

/**
 * Fonction permettant de décoder l'audio et de l'envoyer au player progressivement
 * 
 */ 
void decode() {
        // Decode frame and synthesize loop
    while (1) {

        // Decode frame from the stream
        if (mad_frame_decode(&mad_frame, &mad_stream)) {
            if (MAD_RECOVERABLE(mad_stream.error)) {
                continue;
            } else if (mad_stream.error == MAD_ERROR_BUFLEN) {
                continue;
            } else {
                break;
            }
        }
        // Synthesize PCM data of frame
        mad_synth_frame(&mad_synth, &mad_frame);
        output(&mad_synth.pcm);
    }
}

/**
 * Fonction permettant de libérer la mémoire affilé aux structures libmad.
 * 
 */ 
void free_mad() {
    mad_synth_finish(&mad_synth);
    mad_frame_finish(&mad_frame);
    mad_stream_finish(&mad_stream);
<<<<<<< HEAD

    // Close PulseAudio output
    if (device)
        pa_simple_free(device);

    return EXIT_SUCCESS;
}

// Some helper functions, to be cleaned up in the future
int scale(mad_fixed_t sample) {
     /* round */
     sample += (1L << (MAD_F_FRACBITS - 16));
     /* clip */
     if (sample >= MAD_F_ONE)
         sample = MAD_F_ONE - 1;
     else if (sample < -MAD_F_ONE)
         sample = -MAD_F_ONE;
     /* quantize */
     return sample >> (MAD_F_FRACBITS + 1 - 16);
}
void output(struct mad_header const *header, struct mad_pcm *pcm) {
    register int nsamples = pcm->length;
    mad_fixed_t const *left_ch = pcm->samples[0], *right_ch = pcm->samples[1];
    static char stream[1152*4];
    if (pcm->channels == 2) {
        while (nsamples--) {
            signed int sample;
            sample = scale(*left_ch++);
            stream[(pcm->length-nsamples)*4 ] = ((sample >> 0) & 0xff);
            stream[(pcm->length-nsamples)*4 +1] = ((sample >> 8) & 0xff);
            sample = scale(*right_ch++);
            stream[(pcm->length-nsamples)*4+2 ] = ((sample >> 0) & 0xff);
            stream[(pcm->length-nsamples)*4 +3] = ((sample >> 8) & 0xff);
        }
        if (pa_simple_write(device, stream, (size_t)1152*4, &error) < 0) {
            fprintf(stderr, "pa_simple_write() failed: %s\n", pa_strerror(error));
            return;
        }
    } else {
        printf("Mono not supported!");
    }
=======
>>>>>>> 6840440f5a8f619dfef525e1a952cb606b67dce9
}