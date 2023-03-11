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
#include "hpsdr_utils.h"

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
    iq_t *sample = malloc(sizeof(iq_t));
    FILE *fp = NULL;

    if (ofile) {
        fp = fopen(ofilename, "w+");
        if (NULL == fp) {
            printf("WARNING: output file \"%s\" can't be opened\n", ifilename);
            exit(1);
        }

        while (1) {
            if (hpsdr_txbuffer_read(&cfg, &sample))
                fwrite(&sample, sizeof(iq_t), 1, fp);
        }
    } else
        while (1)
            usleep(1000);

    free(sample);

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
    iq_t *sample = malloc(sizeof(iq_t));
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
            if (feof(fp)) rewind(fp);
            fread(sample, sizeof(iq_t), 1, fp);
        } else {
            if (cfg->global.replay) {
                // make some noise
                sample->i.s16 = (rand() % 10000);
                sample->q.s16 = (rand() % 10000);
            } else {
                sample->i.s16 = 0;
                sample->q.s16 = 0;
            }
        }

        sample->i.s24 = convert_s16_to_s24(sample->i.s16);
        sample->q.s24 = convert_s16_to_s24(sample->q.s16);

        hpsdr_rxbuffer_write(&cfg, *sample);
    }

    free(sample);

    return NULL;
}
