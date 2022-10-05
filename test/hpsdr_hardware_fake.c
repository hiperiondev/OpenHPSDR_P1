/*
 * Copyright 2022 Emiliano Gonzalez LU3VEA (lu3vea @ gmail . com))
 * * Project Site: https://github.com/hiperiondev/OpenHPSDR_P1 *
 *
 * This is based on other projects:
 *    HPSDR simulator (https://github.com/g0orx/pihpsdr)
 *    Others: see individual files
 *
 *    please contact their authors for more information.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 */

#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "hpsdr_p1.h"

bool ifile = false;
bool ofile = false;
char ifilename[254];
char ofilename[254];

uint8_t iqtransmitter_init(void) {
    return 0;
}

uint8_t iqtransmitter_deinit(void) {
    return 0;
}

void* iqtransmitter_thread(void *data) {
    hpsdr_config_t *cfg = (hpsdr_config_t*) data;
    double _Complex csample;
    FILE *fp = NULL;

    if (ofile) {
        fp = fopen(ofilename, "w+");
        if (NULL == fp) printf("WARNING: output file \"%s\" can't be opened\n", ifilename);

        while (1) {
            while (!hpsdr_rxbuffer_read(&cfg, &csample));
            fwrite(&csample, sizeof(double _Complex), 1, fp);
        }
    } else
        while (1)
            usleep(1000);

      return NULL;
}

uint8_t iqreceiver_init(void) {
    return 0;
}

uint8_t iqreceiver_deinit(void) {
    return 0;
}

void* iqreceiver_thread(void *data) {
    hpsdr_config_t *cfg = (hpsdr_config_t*) data;
    double _Complex csample;
    FILE *fp = NULL;

    if (ifile) {
        fp = fopen(ifilename, "r");
        if (NULL == fp) {
            printf("WARNING: input file \"%s\" can't be opened\n", ifilename);
        }
    } else {
        time_t t;
        srand((unsigned) time(&t));
    }

    while (1) {
        if (ifile) {
            if(feof(fp))
                rewind(fp);
            fread(&csample, sizeof(float _Complex), 1, fp);
        } else {
            // make some noise
            csample = (rand() % 10000) + (rand() % 10000) * I;
        }

        while(!hpsdr_rxbuffer_write(&cfg, &csample));
    }

    return NULL;
}
