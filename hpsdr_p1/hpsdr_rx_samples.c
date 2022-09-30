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

#include "hpsdr_p1.h"
#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "cbuffer.h"

void hpsdr_get_rx_samples(hpsdr_config_t *cfg, int n, uint8_t *pointer) {
    int j, k;
    float _Complex csample;

    int32_t dacisample = 0, dacqsample = 0;
    int32_t myisample, myqsample;

    // TODO: complete
    for (j = 0; j < n; j++) {
        void *cs = cbuf_poll(cfg->rxbuff, 1);
        if (cs != NULL)
            csample = *((_Complex float*) cs);
        else
            csample = 0 + 0 * I;

        for (k = 0; k < cfg->settings.receivers; k++) {
            myisample = 0;
            myqsample = 0;
            switch (cfg->settings.rx_adc[k]) {
                case 0: // ADC1
                    myisample = creal(csample);
                    myqsample = cimag(csample);
                    break;
                case 1: // ADC2
                    myisample = creal(csample);
                    myqsample = cimag(csample);
                    break;
                default:
                    myisample = 0;
                    myqsample = 0;
                    break;
            }
            if ((cfg->global.emulation == DEVICE_METIS || cfg->global.emulation == DEVICE_HERMES_LITE) && cfg->settings.ptt && (k == 1)) {
                // METIS: TX DAC signal goes to RX2 when TXing
                myisample = dacisample;
                myqsample = dacqsample;
            }
            if ((cfg->global.emulation == DEVICE_HERMES || cfg->global.emulation == DEVICE_GRIFFIN || cfg->global.emulation == DEVICE_C25
                    || cfg->global.emulation == DEVICE_HERMES_LITE2) && cfg->settings.ptt && (k == 3)) {
                // HERMES: TX DAC signal goes to RX4 when TXing
                myisample = dacisample;
                myqsample = dacqsample;
            }
            if ((cfg->global.emulation == DEVICE_ANGELIA || cfg->global.emulation == DEVICE_ORION || cfg->global.emulation == DEVICE_ORION2)
                    && cfg->settings.ptt && (k == 4)) {
                // ANGELIA and beyond: TX DAC signal goes to RX5 when TXing
                myisample = dacisample;
                myqsample = dacqsample;
            }

            *pointer++ = (myisample >> 16) & 0xFF;
            *pointer++ = (myisample >> 8) & 0xFF;
            *pointer++ = (myisample >> 0) & 0xFF;
            *pointer++ = (myqsample >> 16) & 0xFF;
            *pointer++ = (myqsample >> 8) & 0xFF;
            *pointer++ = (myqsample >> 0) & 0xFF;
        }
        // TODO: implement microphone
        pointer += 2;
    }
}
