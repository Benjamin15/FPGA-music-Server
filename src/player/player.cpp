#include <ao/ao.h>
#include <mpg123.h>
#include <iostream>
#define BITS 8

int main(int argc, char *argv[])
{
    if(argc < 2)
        exit(0);
    mpg123_handle *handle;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;
    char* musicPath = argv[1];
    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    handle = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(handle);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(handle, musicPath);
    mpg123_getformat(handle, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while (mpg123_read(handle, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(dev, (char*)buffer, done);
    }

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(handle);
    mpg123_delete(handle);
    mpg123_exit();
    ao_shutdown();

    return 0;
}