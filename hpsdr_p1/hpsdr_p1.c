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

    (*cfg)->settings.AlexTXrel = -1;
    (*cfg)->settings.alexRXout = -1;
    (*cfg)->settings.alexRXant = -1;
    (*cfg)->settings.MicTS = -1;
    (*cfg)->settings.duplex = -1;
    (*cfg)->settings.receivers = -1;
    (*cfg)->settings.rate = -1;
    (*cfg)->settings.preamp = -1;
    (*cfg)->settings.LTdither = -1;
    (*cfg)->settings.LTrandom = -1;
    (*cfg)->settings.ref10 = -1;
    (*cfg)->settings.src122 = -1;
    (*cfg)->settings.PMconfig = -1;
    (*cfg)->settings.MicSrc = -1;
    (*cfg)->settings.txdrive = 255;
    (*cfg)->settings.txatt = 0;
    (*cfg)->settings.sidetone_volume = -1;
    (*cfg)->settings.cw_internal = -1;
    (*cfg)->settings.rx_att[0] = -1;
    (*cfg)->settings.rx_att[1] = -1;
    (*cfg)->settings.rx1_attE = -1;
    (*cfg)->settings.rx_preamp[0] = -1;
    (*cfg)->settings.rx_preamp[1] = -1;
    (*cfg)->settings.rx_preamp[2] = -1;
    (*cfg)->settings.rx_preamp[3] = -1;
    (*cfg)->settings.MerTxATT0 = -1;
    (*cfg)->settings.MerTxATT1 = -1;
    (*cfg)->settings.MetisDB9 = -1;
    (*cfg)->settings.PeneSel = -1;
    (*cfg)->settings.PureSignal = -1;
    (*cfg)->settings.LineGain = -1;
    (*cfg)->settings.MicPTT = -1;
    (*cfg)->settings.tip_ring = -1;
    (*cfg)->settings.MicBias = -1;
    (*cfg)->settings.ptt = 0;
    (*cfg)->settings.AlexAtt = -1;
    (*cfg)->settings.TX_class_E = -1;
    (*cfg)->settings.OpenCollectorOutputs = -1;
    (*cfg)->settings.tx_freq = -1;
    (*cfg)->settings.rx_freq[0] = -1;
    (*cfg)->settings.rx_freq[1] = -1;
    (*cfg)->settings.rx_freq[2] = -1;
    (*cfg)->settings.rx_freq[3] = -1;
    (*cfg)->settings.rx_freq[4] = -1;
    (*cfg)->settings.rx_freq[5] = -1;
    (*cfg)->settings.rx_freq[6] = -1;
    (*cfg)->settings.hermes_config = -1;
    (*cfg)->settings.alex_lpf = -1;
    (*cfg)->settings.alex_hpf = -1;
    (*cfg)->settings.alex_manual = -1;
    (*cfg)->settings.alex_bypass = -1;
    (*cfg)->settings.lna6m = -1;
    (*cfg)->settings.alexTRdisable = -1;
    (*cfg)->settings.vna = -1;
    (*cfg)->settings.c25_ext_board_i2c_data = -1;
    (*cfg)->settings.rx_adc[0] = -1;
    (*cfg)->settings.rx_adc[1] = -1;
    (*cfg)->settings.rx_adc[2] = -1;
    (*cfg)->settings.rx_adc[3] = -1;
    (*cfg)->settings.rx_adc[4] = -1;
    (*cfg)->settings.rx_adc[5] = -1;
    (*cfg)->settings.rx_adc[6] = -1;
    (*cfg)->settings.cw_hang = -1;
    (*cfg)->settings.cw_reversed = -1;
    (*cfg)->settings.cw_speed = -1;
    (*cfg)->settings.cw_mode = -1;
    (*cfg)->settings.cw_weight = -1;
    (*cfg)->settings.cw_spacing = -1;
    (*cfg)->settings.cw_delay = -1;
    (*cfg)->settings.CommonMercuryFreq = -1;
    (*cfg)->settings.freq = -1;
    (*cfg)->settings.diversity = 0;

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
