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

#include <pthread.h>
#include <stdio.h>
#include <math.h>

#include "hpsdr_debug.h"
#include "hpsdr_internals.h"
#include "hpsdr_network.h"
#include "hpsdr_p1.h"
#include "hpsdr_ring_buf.h"

#define ARRAY_LEN(a_) (sizeof(a_)/sizeof(a_[0]))

int enable_thread;
int active_thread;
double c1, c2;

pthread_t network_thread_id;

double _Complex rxbuf[BUFFLEN];
double _Complex txbuf[BUFFLEN];

void hpsdr_clear_config(hpsdr_config_t **cfg) {
    for (int n = 0; n < 72; n++)
        (*cfg)->ep2_value[n] = 0;

    (*cfg)->ep2_cb = NULL;
}

void hpsdr_init(hpsdr_config_t **cfg) {
    hpsdr_dbg_printf(1, "hpsdr_init\n");

    switch ((*cfg)->global.emulation) {

        case DEVICE_METIS:
            hpsdr_dbg_printf(1, "DEVICE is METIS\n");
            c1 = 3.3;
            c2 = 0.090;
            break;

        case DEVICE_HERMES:
            hpsdr_dbg_printf(1, "DEVICE is HERMES\n");
            c1 = 3.3;
            c2 = 0.095;
            break;

        case DEVICE_GRIFFIN:
            hpsdr_dbg_printf(1, "DEVICE is GRIFFIN\n");
            c1 = 3.3;
            c2 = 0.095;
            break;

        case DEVICE_ANGELIA:
            hpsdr_dbg_printf(1, "DEVICE is ANGELIA\n");
            c1 = 3.3;
            c2 = 0.095;
            break;

        case DEVICE_HERMES_LITE:
            hpsdr_dbg_printf(1, "DEVICE is HermesLite V1\n");
            c1 = 3.3;
            c2 = 0.095;
            break;

        case DEVICE_HERMES_LITE2:
            hpsdr_dbg_printf(1, "DEVICE is HermesLite V2\n");
            c1 = 3.3;
            c2 = 0.095;
            break;

        case DEVICE_ORION:
            hpsdr_dbg_printf(1, "DEVICE is ORION\n");
            c1 = 5.0;
            c2 = 0.108;
            break;

        case DEVICE_ORION2:
            hpsdr_dbg_printf(1, "DEVICE is ORION MkII\n");
            c1 = 5.0;
            c2 = 0.108;
            break;

        case DEVICE_C25:
            hpsdr_dbg_printf(1, "DEVICE is STEMlab/C25\n");
            c1 = 3.3;
            c2 = 0.090;
            break;
    }

    RingBuf_ctor(&((*cfg)->txbuff), txbuf, ARRAY_LEN(txbuf));
    RingBuf_ctor(&((*cfg)->rxbuff), rxbuf, ARRAY_LEN(txbuf));
}

void hpsdr_deinit(hpsdr_config_t **cfg) {
    hpsdr_dbg_printf(1, "hpsdr_init\n");
}

void hpsdr_start(hpsdr_config_t **cfg) {
    hpsdr_dbg_printf(1, "hpsdr_start\n");

    pthread_create(&network_thread_id, NULL, hpsdr_network_handler, (void*) (*cfg));
    pthread_detach(network_thread_id);
}

void hpsdr_stop(void) {
    hpsdr_dbg_printf(1, "hpsdr_stop\n");

    pthread_cancel(network_thread_id);
}

bool hpsdr_txbuffer_write(hpsdr_config_t **cfg, double _Complex *iq) {
    return RingBuf_put(&((*cfg)->txbuff), *iq);
}

bool hpsdr_rxbuffer_write(hpsdr_config_t **cfg, double _Complex *iq) {
    return RingBuf_put(&((*cfg)->rxbuff), *iq);
}

bool hpsdr_txbuffer_read(hpsdr_config_t **cfg, double _Complex *iq) {
    return RingBuf_get(&((*cfg)->txbuff), iq);
}

bool hpsdr_rxbuffer_read(hpsdr_config_t **cfg, double _Complex *iq) {
    return RingBuf_get(&((*cfg)->rxbuff), iq);
}
