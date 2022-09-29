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

#ifndef HPSDR_EP2_H_
#define HPSDR_EP2_H_

#include <stdint.h>

#include "hpsdr_p1.h"

#define FR_EP2_PTT(fr)                  (fr[0] & 1)
#define FR_EP2_RATE(fr)                 ((fr[1] & 0x03) >> 0)
#define FR_EP2_REF10(fr)                ((fr[1] & 0x0C) >> 3)
#define FR_EP2_SRC122(fr)               ((fr[1] & 0x10) >> 4)
#define FR_EP2_PMCONFIG(fr)             ((fr[1] & 0x60) >> 5)
#define FR_EP2_MICSRC(fr)               ((fr[1] & 0x80) >> 7)
#define FR_EP2_TXCLASSC(fr)             (fr[2] & 1)
#define FR_EP2_OPENCOLLECTOROUTPUTS(fr) ((fr[2] & 0xfe) >> 1)
#define FR_EP2_RECEIVERS(fr)            (((fr[4] >> 3) & 7) + 1)
#define FR_EP2_MICTS(fr)                (((fr[4] >> 6) & 1))
#define FR_EP2_COMMONMERCURYFREQ(fr)    ((fr[4] >> 7) & 1)
#define FR_EP2_ALEXATT(fr)              (fr[3] & 0x03)
#define FR_EP2_PREAMP(fr)               ((fr[3] & 0x04) >> 2)
#define FR_EP2_LTDITHER(fr)             ((fr[3] & 0x08) >> 3)
#define FR_EP2_LTRANDOM(fr)             ((fr[3] & 0x10) >> 4)
#define FR_EP2_ALEXRXANT(fr)            ((fr[3] & 0x60) >> 5)
#define FR_EP2_ALEXXRXOUT(fr)           ((fr[3] & 0x80) >> 7)
#define FR_EP2_ALETXREL(fr)             ((fr[4] >> 0) & 3)
#define FR_EP2_DUPLEX(fr)               ((fr[4] >> 2) & 1)
#define FR_EP2_TXFREQ(fr)               ((fr[4] | (fr[3] << 8) | (fr[2] << 16) | (fr[1] << 24)))
#define FR_EP2_RXFREQ(fr)               (fr[4] | (fr[3] << 8)  | (fr[2] << 16) | (fr[1] << 24))
#define FR_EP2_TXDRIVE(fr)              (fr[1])
#define FR_EP2_HERMESCONFIG(fr)         (fr[2] & 0x3F)
#define FR_EP2_ALEXMANUALHPFLPF(fr)     ((fr[2] >> 6) & 0x01)
#define FR_EP2_VNA(fr)                  ((fr[2] >> 7) & 0x01)
#define FR_EP2_ALEXHPF(fr)              (fr[3] & 0x1F)
#define FR_EP2_ALEXBYPASS(fr)           ((fr[3] >> 5) & 0x01)
#define FR_EP2_LNA6M(fr)                ((fr[3] >> 6) & 0x01)
#define FR_EP2_ALEXTRDISABLE(fr)        ((fr[3] >> 7) & 0x01)
#define FR_EP2_ALEXLPF(fr)              (fr[4])
#define FR_EP2_ADC1PREAMP(fr)           ((fr[1] & 0x01) >> 0)
#define FR_EP2_ADC2PREAMP(fr)           ((fr[1] & 0x02) >> 1)
#define FR_EP2_ADC3PREAMP(fr)           ((fr[1] & 0x04) >> 2)
#define FR_EP2_ADC4PREAMP(fr)           ((fr[1] & 0x08) >> 3)
#define FR_EP2_TIPRING(fr)              ((fr[1] & 0x10) >> 4)
#define FR_EP2_MICBIAS(fr)              ((fr[1] & 0x20) >> 5)
#define FR_EP2_MICPTT(fr)               ((fr[1] & 0x40) >> 6)
#define FR_EP2_LINEGAIN(fr)             ((fr[2] & 0x1F) >> 0)
#define FR_EP2_MERTXATT0(fr)            ((fr[2] & 0x20) >> 5)
#define FR_EP2_PURESIGNAL(fr)           ((fr[2] & 0x40) >> 6)
#define FR_EP2_PENESEL(fr)              ((fr[2] & 0x80) >> 7)
#define FR_EP2_METISDB9(fr)             ((fr[3] & 0x0F) >> 0)
#define FR_EP2_MERTXATT1(fr)            ((fr[3] & 0x10) >> 4)
#define FR_EP2_RX1HLATTGAIN(fr)         (37 - (fr[4] & 0x3F))
#define FR_EP2_RX1ATT(fr)               ((fr[4] & 0x1F) >> 0x0F)
#define FR_EP2_RX1ATTENABLE(fr)         ((fr[4] & 0x20) >> 5)
#define FR_EP2_RX2ATT(fr)               (fr[1] & 0x1f)
#define FR_EP2_CWREV(fr)                ((fr[2] >> 6) & 1)
#define FR_EP2_CWSPEED(fr)              (fr[3] & 63)
#define FR_EP2_CWMODE(fr)               ((fr[3] >> 6) & 3)
#define FR_EP2_CWWEIGHT(fr)             (fr[4] & 127)
#define FR_EP2_CWSPACING(fr)            ((fr[4] >> 7) & 1)
#define FR_EP2_C25EXTBOARDDATA(fr)      ((fr[2] << 8) | fr[1])
#define FR_EP2_RX1ADC(fr)               ((fr[1] & 0x03) >> 0)
#define FR_EP2_RX2ADC(fr)               ((fr[1] & 0x0C) >> 2)
#define FR_EP2_RX3ADC(fr)               ((fr[1] & 0x30) >> 4)
#define FR_EP2_RX4ADC(fr)               ((fr[1] & 0xC0) >> 6)
#define FR_EP2_RX5ADC(fr)               ((fr[2] & 0x03) >> 0)
#define FR_EP2_RX6ADC(fr)               ((fr[2] & 0x0C) >> 2)
#define FR_EP2_RX7ADC(fr)               ((fr[2] & 0x30) >> 4)
#define FR_EP2_TXATT(fr)                ((fr[3] & 0x1f))
#define FR_EP2_CWINT(fr)                (fr[1] & 1)
#define FR_EP2_CWDELAY(fr)              (frame[3])
#define FR_EP2_SIDETONEVOLUME(fr)       (fr[2])
#define FR_EP2_CWHANG(fr)               ((fr[1] << 2) | (fr[2] & 3))
#define FR_EP2_SIDETONEFREQ(fr)         ((fr[3] << 4) | (fr[4] & 255))

/**
 * @brief EP2 handler
 *
 * EP2 handler.
 *
 * @param frame The received frame.
 */
void ep2_handler(hpsdr_config_t *cfg, uint8_t *frame);

#endif /* HPSDR_EP2_H_ */
