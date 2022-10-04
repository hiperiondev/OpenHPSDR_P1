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

#include <stdint.h>
#include <stddef.h>
#include <complex.h>
#include <stdio.h>

#include "hpsdr_p1.h"
#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_ring_buf.h"

void hpsdr_get_rx_samples(hpsdr_config_t *cfg, int n, uint8_t *pointer) {
    int j; //, k;
    float _Complex csample;
    uint8_t itemp;

    // TODO: complete
    for (j = 0; j < n; j++) {
        if (!RingBuf_get(&(cfg->rxbuff), &csample))
            csample = 0 + 0 * I;

        itemp = creal(csample) >= 0.0 ? (uint8_t) floor((double) creal(csample) * 32767.0 + 0.5) : (uint8_t) ceil((double) creal(csample) * 32767.0 - 0.5);
        *pointer++ = (uint8_t) ((itemp >> 16) & 0xff);
        *pointer++ = (uint8_t) ((itemp >> 8) & 0xff);
        *pointer++ = (uint8_t) (itemp & 0xff);

        itemp = cimag(csample) >= 0.0 ? (uint8_t) floor((double) cimag(csample) * 32767.0 + 0.5) : (uint8_t) ceil((double) cimag(csample) * 32767.0 - 0.5);
        *pointer++ = (uint8_t) ((itemp >> 16) & 0xff);
        *pointer++ = (uint8_t) ((itemp >> 8) & 0xff);
        *pointer++ = (uint8_t) (itemp & 0xff);
    }
    // TODO: implement microphone
    pointer += 2;
}
