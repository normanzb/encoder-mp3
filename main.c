//    Shine is an MP3 encoder
//    Copyright (C) 1999-2000  Gabriel Bouvigne
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Library General Public
//    License as published by the Free Software Foundation; either
//    version 2 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Library General Public License for more details.


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "types.h"
#include "error.h"
#include "wave.h"
#include "layer3.h"

config_t config;




static void print_header()
{
    fprintf(stderr,"Shine 0.1.4\n");
}

static void print_usage()
{
    fprintf(stderr,"USAGE   :  Shine [options] <infile> <outfile>\n");
    fprintf(stderr,"OPTIONS : -h            this help message\n");
    fprintf(stderr,"          -b <bitrate>  set the bitrate [32-320], default 128kbit\n");
    fprintf(stderr,"          -c            set copyright flag, default off\n");
    fprintf(stderr,"\n"); 
}

static void set_defaults()
{
    config.byte_order = DetermineByteOrder();
    if(config.byte_order==order_unknown) ERROR("Can't determine byte order");

    config.mpeg.type = 1;
    config.mpeg.layr = 2;
    config.mpeg.mode = 2;
    config.mpeg.bitr = 128;
    config.mpeg.psyc = 2;
    config.mpeg.emph = 0; 
    config.mpeg.crc  = 0;
    config.mpeg.ext  = 0;
    config.mpeg.mode_ext  = 0;
    config.mpeg.copyright = 0;
    config.mpeg.original  = 1;
}

static bool parse_command(int argc, char** argv)
{
    int i = 0;

    if(argc<3) return false;

    while(argv[++i][0]=='-')
        switch(argv[i][1])
        {
            case 'b' : config.mpeg.bitr = atoi(argv[++i]);
                       break;
            case 'c' : config.mpeg.copyright = 1;
                       break;
            case 'h' :
            default  : return false;
       }

    if((argc-i)!=2) return false;
    config.infile  = argv[i++];
    config.outfile = argv[i];
    return true;
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

int main(int argc, char **argv)
{
	time_t end_time;

    time(&config.start_time);
    print_header();
    set_defaults();
    if(!parse_command(argc,argv)) { print_usage(); return -1; }
    if(!wave_open()) ERROR("Unable to open input file...");
    check_config();

    printf("Encoding \"%s\" to \"%s\"\n", config.infile, config.outfile);
	
	compress();
    
	wave_close();

    time(&end_time);
    end_time -= config.start_time;
    fprintf(stdout," Finished in %2ld:%2ld:%2ld\n",
            end_time/3600,(end_time/60)%60,end_time%60);
    return 0;
} 

