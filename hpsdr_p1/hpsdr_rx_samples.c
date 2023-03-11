/*
 * Copyright 2022 Emiliano Gonzalez LU3VEA (lu3vea @ gmail . com))
 * * Project Site: https://github.com/hiperiondev/OpenHPSDR_P1 *
 *
 * This is based on other projects:
 *    HPSDR simulator (https://github.com/g0orx/pihpsdr)
 *    Lock-free ring buffer (https://github.com/QuantumLeaps/lock-free-ring-buffer)
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

#include "hpsdr_utils.h"
#include "hpsdr_p1.h"
#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_ring_buf_IQ.h"

void hpsdr_get_rx_samples(hpsdr_config_t *cfg, int samples_qty, uint8_t *pointer) {
    int j, k;
    iq_t sample;

    // TODO: complete
    for (j = 0; j < samples_qty; j++) {

        if (circular_buf_get(*(cfg->rxbuff_iq), &sample) != 0) {
            sample.i.s24 = 0;
            sample.q.s24 = 0;
        }

        uint8_t *bi = (uint8_t*) &sample.i.s24;
        uint8_t *bq = (uint8_t*) &sample.q.s24;

        for (k = 0; k < cfg->ep2_value[EP2_RECEIVERS]; k++) {
            *pointer++ = (*bi >> 16) & 0xff;
            *pointer++ = (*bi >> 8) & 0xff;
            *pointer++ = *bi & 0xff;

            *pointer++ = (*bq >> 16) & 0xff;
            *pointer++ = (*bq >> 8) & 0xff;
            *pointer++ = *bq & 0xff;
        }
    }
    // TODO: implement microphone
    pointer += 2;
}
