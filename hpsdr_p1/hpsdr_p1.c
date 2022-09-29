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

#include "hpsdr_debug.h"
#include "hpsdr_internals.h"
#include "hpsdr_network.h"
#include "hpsdr_p1.h"
#include "cbuffer.h"

hpsdr_config_t *cfg;
int enable_thread;
int active_thread;
double c1, c2;

pthread_t iqtransmitter_thread_id;
pthread_t iqreceiver_thread_id;
pthread_t network_thread_id;

void hpsdr_init(hpsdr_config_t **cfg) {
    uint8_t res;

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

    (*cfg)->txbuff = cbuf_new(BUFFLEN * sizeof(float _Complex));
    (*cfg)->rxbuff = cbuf_new(BUFFLEN * sizeof(float _Complex));

    res = (*cfg)->cb.tx_init();
    if (res == -1)
        hpsdr_dbg_printf(1, "WARNING: tx_init failed");
    pthread_create(&iqtransmitter_thread_id, NULL, (*cfg)->cb.tx_thread, NULL);
    pthread_detach(iqtransmitter_thread_id);

    res = (*cfg)->cb.rx_init();
    if (res == -1)
        hpsdr_dbg_printf(1, "WARNING: rx_init failed");
    pthread_create(&iqreceiver_thread_id, NULL, (*cfg)->cb.rx_thread, NULL);
    pthread_detach(iqreceiver_thread_id);

    pthread_create(&network_thread_id, NULL, hpsdr_network_handler, NULL);
    pthread_detach(network_thread_id);
}

void hpsdr_deinit(hpsdr_config_t **cfg) {
    uint8_t res;

    pthread_cancel(network_thread_id);

    pthread_cancel(iqreceiver_thread_id);
    res = (*cfg)->cb.rx_deinit();
    if (res == -1)
        hpsdr_dbg_printf(1, "WARNING: rx_deinit failed");

    pthread_cancel(iqtransmitter_thread_id);
    res = (*cfg)->cb.tx_deinit();
    if (res == -1)
        hpsdr_dbg_printf(1, "WARNING: tx_deinit failed");

    free((*cfg)->txbuff);
    free((*cfg)->rxbuff);
}
