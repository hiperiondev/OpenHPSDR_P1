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
pthread_t iqtransmitter_thread_id;
pthread_t iqreceiver_thread_id;
pthread_t network_thread_id;

void hpsdr_init(hpsdr_config_t **cfg) {
    uint8_t res;

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
