#include "player.h"
int main(int argc, char **argv) {
  char *filename = argv[1];
  int sample_rate = atoi(argv[2]);
  int bitrate = atoi(argv[3]);

  std::cout << std::to_string(sample_rate) << std::endl;
  std::cout << std::to_string(bitrate) << std::endl;
  std::cout << std::to_string(channels) << std::endl;

  initPlayer(sample_rate, bitrate, channels);
  initMad();

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
    for (int j = 0 ; j < channels ; j++) {
      signed int sample = pcm->samples[j][i];
      stream.push_back((sample >> 0) & 0xff);
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
 * Cette fonction permet d'initialiser le player.
 * On choisi d'utiliser du 32 bits car le Alsa sur la FPGA ne sait lire que les samples en 32 bits.
 * En plus, cela fonctionne correctement si un appareil ne peut lire que des samples de 16 bits. L'inverse n'est pas vrai (ralentissement et perte)
 * 
 */ 
void initPlayer(unsigned int sample_rate, unsigned int bitrate, unsigned int channels) {
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

  if ((error = snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &sample_rate, 0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n",
      snd_strerror (error));
    exit (1);
  }
    
    // set stereo / mono
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
  int nbErrorBufLen = 0;
  while (1) {
      // Decode frame from the stream
    if (mad_frame_decode(&mad_frame, &mad_stream)) {
      if (MAD_RECOVERABLE(mad_stream.error)) {
        continue;
      } else if (mad_stream.error == MAD_ERROR_BUFLEN) {
        if (nbErrorBufLen++ > 10)
          break;
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
}