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

#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <netinet/in.h>

#include "hpsdr_debug.h"
#include "hpsdr_utils.h"
#include "hpsdr_internals.h"
#include "hpsdr_p1.h"
#include "hpsdr_network.h"
#include "hpsdr_rx_samples.h"

void* ep6_handler(void *arg) {
    hpsdr_dbg_printf(1, "Start handler ep6\n");

    hpsdr_config_t *cfg = (hpsdr_config_t*) arg;

    static double txlevel;
    int i, j;
    int samples_qty;
    int size;
    int header_offset;
    int bitsPerCycle;
    long nanosPerCycle;
    struct timespec ts_sleep, rm_sleep;
    uint32_t counter;
    uint8_t buffer[1032];
    uint8_t *pointer;

    uint8_t id[4] = { //
            0xef,     //
            0xfe,     //
            1,        //
            6         //
    };

    uint8_t header[40] = { //
        //  C0   C1   C2   C3   C4
            127, 127, 127, 0,   0,   //
            33,  17,  21,  127, 127, //
            127, 8,   0,   0,   0,   //
            0,   127, 127, 127, 16,  //
            0,   0,   0,   0,   127, //
            127, 127, 24,  0,   0,   //
            0,   0,   127, 127, 127, //
            32,  66,  66,  66,  66   //
    };

    memcpy(buffer, id, 4);
    header_offset = 0;
    counter = 0;

    while (1) {
        if (!enable_thread) break;

        long cycleStart = get_nanos();

        size = (cfg->ep2_value[EP2_RECEIVERS] * 6) + 2;
        samples_qty = 504 / size;  // number of samples per 512-byte-block

        bitsPerCycle = 8 * size;
        nanosPerCycle = (long) (((double) bitsPerCycle) / ((double) (48000 * (2 ^ (cfg->ep2_value[EP2_RATE])))) * 10000000000L);

        // plug in sequence numbers
        *(uint32_t*) (buffer + 4) = htonl(counter);
        ++counter;

        for (i = 0; i < 2; ++i) {
            pointer = buffer + i * 516 - i % 2 * 4 + 8;
            memcpy(pointer, header + header_offset, 8);

            switch (header_offset) {
                case 0: //
                    // do not set ptt and cw in c0
                    // do not set adc overflow in c1
                    if (cfg->global.emulation == DEVICE_HERMES_LITE2) {
                        *(pointer + 5) = (0 >> 8) & 0x7F;
                        *(pointer + 6) = 0 & 0xFF;
                    }
                    header_offset = 8;
                    break;
                case 8: //
                    if (cfg->global.emulation == DEVICE_HERMES_LITE2) {
                        // hl2: temperature
                        *(pointer + 4) = 0;
                        *(pointer + 5) = 0 & 0x7F;  // pseudo random number
                    } else {
                        // ain5: exciter power
                        *(pointer + 4) = 0;  // about 500 mW
                        *(pointer + 5) = cfg->ep2_value[EP2_TXDRIVE];
                    }
                    // ain1: forward power
                    j = (int) ((4095.0 / c1) * sqrt(100.0 * txlevel * c2));
                    *(pointer + 6) = (j >> 8) & 0xFF;
                    *(pointer + 7) = (j) & 0xFF;
                    header_offset = 16;
                    break;
                case 16: //
                    // ain2: reverse power
                    // ain3:
                    header_offset = 24;
                    break;
                case 24: //
                    // ain4:
                    // ain5: supply voltage
                    *(pointer + 6) = 0;
                    *(pointer + 7) = 63;
                    header_offset = 32;
                    break;
                case 32: //
                    header_offset = 0;
                    break;
            }

            pointer += 8;
            memset(pointer, 0, 504);

            hpsdr_get_rx_samples(cfg, samples_qty, pointer);
        }

        hpsdr_network_send(buffer, 1032);

        ts_sleep.tv_sec = 0;
        ts_sleep.tv_nsec = nanosPerCycle - (get_nanos() - cycleStart);
        nanosleep(&ts_sleep, &rm_sleep);
    }
    active_thread = 0;
    seqnum = 0;
    last_seqnum = 0xffffffff;

    hpsdr_dbg_printf(1, "Stop handler_ep6\n");

    return NULL;
}

