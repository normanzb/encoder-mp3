
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "error.h"
#include "wave.h"
#include "layer3.h"

#include "AS3/AS3.h"

#include "as3api.h"

config_t config;

static void set_defaults()
{
    config.byte_order = DetermineByteOrder();
    if(config.byte_order==order_unknown) ERROR("Can't determine byte order");

    config.mpeg.type = 1;
    config.mpeg.layr = 2;
    config.mpeg.mode = 2;
    // TODO
    config.mpeg.bitr = 128;
    config.mpeg.psyc = 2;
    config.mpeg.emph = 0; 
    config.mpeg.crc  = 0;
    config.mpeg.ext  = 0;
    config.mpeg.mode_ext  = 0;
    config.mpeg.copyright = 0;
    config.mpeg.original  = 1;
}

static int find_samplerate_index(long freq)
{
    static long mpeg1[3] = {44100, 48000, 32000};
    int i;

    for(i=0;i<3;i++)
        if(freq==mpeg1[i]) return i;

    ERROR("Invalid samplerate");
    return -1;
}

static int find_bitrate_index(int bitr)
{
    static long mpeg1[15] = {0,32,40,48,56,64,80,96,112,128,160,192,224,256,320};
    int i;

    for(i=0;i<15;i++)
        if(bitr==mpeg1[i]) return i;

    ERROR("Invalid bitrate");
    return -1;
}


static void check_config()
{
    static char *mode_names[4]    = { "stereo", "j-stereo", "dual-ch", "mono" };
    static char *layer_names[3]   = { "I", "II", "III" };
    static char *version_names[2] = { "MPEG-II (LSF)", "MPEG-I" };
    static char *psy_names[3]     = { "", "MUSICAM", "Shine" };
    static char *demp_names[4]    = { "none", "50/15us", "", "CITT" };

    config.mpeg.samplerate_index = find_samplerate_index(config.wave.samplerate);
    config.mpeg.bitrate_index    = find_bitrate_index(config.mpeg.bitr);

   printf("%s layer %s, %s  Psychoacoustic Model: %s\n",
           version_names[config.mpeg.type],
           layer_names[config.mpeg.layr], 
           mode_names[config.mpeg.mode],
           psy_names[config.mpeg.psyc]);
   printf("Bitrate=%d kbps  ",config.mpeg.bitr );
   printf("De-emphasis: %s   %s %s\n",
          demp_names[config.mpeg.emph], 
          ((config.mpeg.original)?"Original":""),
          ((config.mpeg.copyright)?"(C)":""));

}

void AS3_Compress()
{
    size_t readResult;
    unsigned char * byteArrayWave;
    unsigned char * byteArrayMPEG;
    unsigned long byteArrayWaveSize;
    unsigned long byteArrayMPEGSize;

    // get current time
    time(&config.start_time);

    // ==== START ====

    // retrieve arguments interoperationly
    AS3_GetByteArray( wave, byteArrayWave, byteArrayWaveSize );

    // read wave as a FILE
    config.wave.file = tmpfile();
    fwrite( byteArrayWave, sizeof(char), byteArrayWaveSize, config.wave.file );
    rewind( config.wave.file );
    
    // try to open config.wave.file
    if(!wave_open()) ERROR("Unable to open input file...");

    // ==== PROCESSING ====

    config.pause = false;

    set_defaults();

    check_config();
    
    // compress the wave and store it in config.mpeg.file
    compress();

    // obtain the size of the file
    fseek(config.mpeg.file , 0 , SEEK_END);
    byteArrayMPEGSize = ftell(config.mpeg.file);
    rewind(config.mpeg.file);

    // allocate memory to contain the whole file:
    byteArrayMPEG = (char *)malloc( sizeof(char) * byteArrayWaveSize );
    if (byteArrayMPEG == NULL) ERROR("Memory error...");

    // read bytes out of the file
    readResult = fread(byteArrayMPEG , sizeof(char), byteArrayMPEGSize, config.mpeg.file);
    if (readResult != byteArrayMPEGSize) ERROR("Unable to read the compressed file... ");

    AS3_SetByteArray( result, byteArrayMPEG, byteArrayMPEGSize );

    // ==== CLEANUP ====
    
    wave_close();

    fclose(config.wave.file);

    fclose(config.mpeg.file);

    free(byteArrayWave);

    free(byteArrayMPEG);

    time(&config.end_time);
    config.end_time -= config.start_time;

    // fprintf(stdout," Finished in %2ld:%2ld:%2ld\n",
    //     end_time/3600,(end_time/60)%60,end_time%60);


    AS3_ReturnAS3Var( result );
}
