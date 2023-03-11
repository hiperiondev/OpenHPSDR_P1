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
#include <time.h>
#include <string.h>
#include <math.h>
#include <complex.h>

#include "hpsdr_debug.h"
#include "hpsdr_p1.h"
#include "hpsdr_ring_buf_IQ.h"
#include "hpsdr_utils.h"

static uint8_t *bp;
static int j;
static int16_t samplei, sampleq;
static int16_t idelta, qdelta;
static int16_t last_i_sample = 0.0;
static int16_t last_q_sample = 0.0;
static int16_t isample, qsample;
static iq_t sample;

#define TX_REPLAY
/*
		    if (cfg->global.replay) {                            \
		        sample.i.s24 = convert_s16_s24(sample.i.s16);    \
		        sample.q.s24 = convert_s16_s24(sample.i.s16);    \
		        circular_buf_try_put(*(cfg->rxbuff_iq), sample); \
		    }
*/

void hpsdr_get_tx_samples(hpsdr_config_t *cfg, uint8_t *buffer) {
    // Put TX IQ samples into the ring buffer
    // In the protocol 1, samples come in groups of 8 bytes L1 L0 R1 R0 I1 I0 Q1 Q0
    // Here, L1/L0 and R1/R0 are audio samples, and I1/I0 and Q1/Q0 are the TX iq samples
    // I1 contains bits 8-15 and I0 bits 0-7 of a signed 16-bit integer.
    // We convert this here to double.
    // If the RX sample rate is larger than the TX on, we perform a
    // simple linear interpolation between the last and current sample.
    // Note that this interpolation causes weak "sidebands" at 48/96/... kHz distance
    // (the strongest ones at 48 kHz).

    bp = buffer + 16;  // skip 8 header and 8 SYNC/C&C bytes

    for (j = 0; j < 126; j++) {
        bp += 4;
        samplei = (int) ((signed char) *bp++) << 8;
        samplei |= (int) ((signed char) *bp++ & 0xFF);
        sampleq = (int) ((signed char) *bp++) << 8;
        sampleq |= (int) ((signed char) *bp++ & 0xFF);
        sample.i.s16 = samplei;
        sample.q.s16 = sampleq;

        switch (cfg->ep2_value[EP2_RATE]) {
            case 0:  // RX sample rate = TX sample rate = 48000
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                break;
            case 1:  // RX sample rate = 96000; TX sample rate = 48000
                idelta = 0.5 * (isample - last_i_sample);
                qdelta = 0.5 * (qsample - last_q_sample);
                isample = last_i_sample + idelta;
                qsample = last_q_sample + qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                break;
            case 2:  // RX sample rate = 192000; TX sample rate = 48000
                idelta = 0.25 * (isample - last_i_sample);
                qdelta = 0.25 * (qsample - last_q_sample);
                isample = last_i_sample + idelta;
                qsample = last_q_sample + qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 2.0 * idelta;
                qsample = last_q_sample + 2.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 3.0 * idelta;
                qsample = last_q_sample + 3.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                break;
            case 3:  // RX sample rate = 384000; TX sample rate = 48000
                idelta = 0.125 * (isample - last_i_sample);
                qdelta = 0.125 * (qsample - last_q_sample);
                isample = last_i_sample + idelta;
                qsample = last_q_sample + qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 2.0 * idelta;
                qsample = last_q_sample + 2.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 3.0 * idelta;
                qsample = last_q_sample + 3.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 4.0 * idelta;
                qsample = last_q_sample + 4.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 5.0 * idelta;
                qsample = last_q_sample + 5.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 6.0 * idelta;
                qsample = last_q_sample + 6.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                isample = last_i_sample + 7.0 * idelta;
                qsample = last_q_sample + 7.0 * qdelta;
                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                sample.i.s16 = isample;
                sample.q.s16 = qsample;
                circular_buf_try_put(*(cfg->txbuff_iq), sample);
                TX_REPLAY;

                break;
        }

        last_i_sample = isample;
        last_q_sample = qsample;

        if (j == 62) bp += 8;  // skip 8 SYNC/C&C bytes of second block
    }
}
