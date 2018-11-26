#pragma once

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
void initPlayer(unsigned int sample, unsigned int bitrate, unsigned int channels);
void initMad();
struct stat get_metadata(int fd, char* filename, FILE* fp);
void decode();
void free_mad();