#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mad.h>
#include <alsa/asoundlib.h>
#include <linux/membarrier.h>

int error;
snd_pcm_t *playback_handle = NULL;
snd_pcm_hw_params_t *hw_params = NULL;
struct mad_stream mad_stream;
struct mad_frame mad_frame;
struct mad_synth mad_synth;

void output(struct mad_header const *header, struct mad_pcm *pcm);

int main(int argc, char **argv) {
    // Parse command-line arguments
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename.mp3]", argv[0]);
        return 255;
    }

    // Set up Alsa 16-bit 44.1kHz stereo output
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

    if ((error = snd_pcm_hw_params_set_format (playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
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


    snd_pcm_uframes_t frames = 32;
	if ((error = snd_pcm_hw_params_set_period_size_near(playback_handle, hw_params, &frames, 0)) < 0) {
		printf("cannot set period size (%s)\n", snd_strerror(error));
		return -1;
    }

    if ((error = snd_pcm_hw_params (playback_handle, hw_params)) < 0) {
        fprintf (stderr, "cannot set parameters (%s)\n",
                snd_strerror (error));
        exit (1);
    }
    // Initialize MAD library
    mad_stream_init(&mad_stream);
    mad_synth_init(&mad_synth);
    mad_frame_init(&mad_frame);

    // Filename pointer
    char *filename = argv[1];

    // File pointer
    FILE *fp = fopen(filename, "r");
    int fd = fileno(fp);

    // Fetch file size, etc
    struct stat metadata;
    if (fstat(fd, &metadata) >= 0) {
        printf("File size %d bytes\n", (int)metadata.st_size);
    } else {
        printf("Failed to stat %s\n", filename);
        fclose(fp);
        return 254;
    }

    // Let kernel do all the dirty job of buffering etc, map file contents to memory
    unsigned char *input_stream = static_cast<unsigned char*> (mmap(0, metadata.st_size, PROT_READ, MAP_SHARED, fd, 0));

    // Copy pointer and length to mad_stream struct
    mad_stream_buffer(&mad_stream, input_stream, metadata.st_size);

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
        output(&mad_frame.header, &mad_synth.pcm);
    }

    // Close
    fclose(fp);

    // Free MAD structs
    mad_synth_finish(&mad_synth);
    mad_frame_finish(&mad_frame);
    mad_stream_finish(&mad_stream);

    // Close PulseAudio output
    if (playback_handle)
        snd_pcm_close (playback_handle);

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
    std::vector<char> stream;
    if (pcm->channels == 2) {
        while (nsamples--) {
            signed int sample;
            sample = scale(*left_ch++);
            stream.push_back((sample >> 0) & 0xff);
            stream.push_back((sample >> 8) & 0xff);
            sample = scale(*right_ch++);
            stream.push_back(((sample >> 0) & 0xff));
            stream.push_back((sample >> 8) & 0xff);
        }
        std::cout << "test : " << stream.size()<< std::endl;
        
            if ((error = snd_pcm_writei (playback_handle, stream.data(), pcm->length)) != pcm->length) {
                fprintf (stderr, "write to audio interface failed (%s)\n",
                        snd_strerror (error));
                exit (1);
            }
        std::cout << "a ecrit" << std::endl;
    } else {
        printf("Mono not supported!");
    }
}