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
#include <math.h>

#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_p1.h"
#include "hpsdr_protocol.h"
#include "hpsdr_ep2.h"

// floating-point represents of tx att, rx att, and rx preamp settings
double txatt_dbl = 1.0;
double rxatt_dbl[4] = { 1.0, 1.0, 1.0, 1.0 };  // this reflects both att and preamp
double txdrv_dbl = 0.99;

#define chk_data(a,b,c,fun) \
		    if ((a) != b) { \
		        b = a; \
		        cfg->cb.ep2(fun, c); \
		    }

void ep2_handler(uint8_t *frame) {
    uint16_t data;
    int rc;
    int mod;

    chk_data(FR_EP2_PTT(frame), cfg->settings.ptt, "PTT", EP2_PTT);
    switch (frame[0]) {
    case 0:
    case 1:
        chk_data(FR_EP2_RATE(frame)                , cfg->settings.rate                , "SampleRate"             , EP2_RATE);
        chk_data(FR_EP2_REF10(frame)               , cfg->settings.ref10               , "Ref10MHz"               , EP2_REF10);
        chk_data(FR_EP2_SRC122(frame)              , cfg->settings.src122              , "Source122MHz"           , EP2_SRC122);
        chk_data(FR_EP2_PMCONFIG(frame)            , cfg->settings.PMconfig            , "Penelope/Mercury config", EP2_PMCONFIG);
        chk_data(FR_EP2_MICSRC(frame)              , cfg->settings.MicSrc              , "MicSource"              , EP2_MICSRC);
        chk_data(FR_EP2_TXCLASSC(frame)            , cfg->settings.TX_class_E          , "TX CLASS-E"             , EP2_TXCLASSC);
        chk_data(FR_EP2_OPENCOLLECTOROUTPUTS(frame), cfg->settings.OpenCollectorOutputs, "OpenCollector"          , EP2_OPENCOLLECTOROUTPUTS);
        chk_data(FR_EP2_RECEIVERS(frame)           , cfg->settings.receivers           , "RECEIVERS"              , EP2_RECEIVERS);
        chk_data(FR_EP2_MICTS(frame)               , cfg->settings.MicTS               , "TimeStampMic"           , EP2_MICTS);
        chk_data(FR_EP2_COMMONMERCURYFREQ(frame)   , cfg->settings.CommonMercuryFreq   , "Common Mercury Freq"    , EP2_COMMONMERCURYFREQ);

        mod = 0;
        rc = FR_EP2_ALEXATT(frame);
        if (rc != cfg->settings.AlexAtt) {
            mod = 1;
            cfg->settings.AlexAtt = rc;
            cfg->cb.ep2(EP2_ALEXATT, "AlexAtt");
        }
        rc = FR_EP2_PREAMP(frame);
        if (rc != cfg->settings.preamp) {
            mod = 1;
            cfg->settings.preamp = rc;
            cfg->cb.ep2(EP2_PREAMP, "Preamp");
        }
        rc = FR_EP2_LTDITHER(frame);
        if (rc != cfg->settings.LTdither) {
            mod = 1;
            cfg->settings.LTdither = rc;
            cfg->cb.ep2(EP2_LTDITHER, "LTdither");
        }

        rc = FR_EP2_LTRANDOM(frame);
        if (rc != cfg->settings.LTrandom) {
            mod = 1;
            cfg->settings.LTrandom = rc;
            cfg->cb.ep2(EP2_LTRANDOM, "LTrandom");
        }
        if (mod)
            hpsdr_dbg_printf(1, "AlexAtt=%d Preamp=%d Dither=%d Random=%d\n", cfg->settings.AlexAtt, cfg->settings.preamp, cfg->settings.LTdither, cfg->settings.LTrandom);

        mod = 0;
        rc = FR_EP2_ALEXRXANT(frame);
        if (rc != cfg->settings.alexRXant) {
            mod = 1;
            cfg->settings.alexRXant = rc;
            cfg->cb.ep2(EP2_ALEXRXANT, "AlexRXant");
        }
        rc = FR_EP2_ALEXXRXOUT(frame);
        if (rc != cfg->settings.alexRXout) {
            mod = 1;
            cfg->settings.alexRXout = rc;
            cfg->cb.ep2(EP2_ALEXXRXOUT, "AlexRXout");
        }
        rc = FR_EP2_ALETXREL(frame);
        if (rc != cfg->settings.AlexTXrel) {
            mod = 1;
            cfg->settings.AlexTXrel = rc;
            cfg->cb.ep2(EP2_ALETXREL, "AlexTXrel");
        }
        rc = FR_EP2_DUPLEX(frame);
        if (rc != cfg->settings.duplex) {
            mod = 1;
            cfg->settings.duplex = rc;
            cfg->cb.ep2(EP2_DUPLEX, "Duplex");
        }
        if (mod)
            hpsdr_dbg_printf(1, "RXout=%d RXant=%d TXrel=%d Duplex=%d\n", cfg->settings.alexRXout, cfg->settings.alexRXant, cfg->settings.AlexTXrel, cfg->settings.duplex);

        if (cfg->global.emulation == DEVICE_C25) {
            // charly25: has two 18-dB preamps that are switched with "preamp" and "dither"
            //           and two attenuators encoded in alex-att
            //           both only applies to rx1!
            rxatt_dbl[0] = pow(10.0, -0.05 * (12 * cfg->settings.AlexAtt - 18 * cfg->settings.LTdither - 18 * cfg->settings.preamp));
            rxatt_dbl[1] = 1.0;
        } else {
            // assume that it has alex attenuators in addition to the step attenuators
            rxatt_dbl[0] = pow(10.0, -0.05 * (10 * cfg->settings.AlexAtt + cfg->settings.rx_att[0]));
            rxatt_dbl[1] = 1.0;
        }
        break;

    case 2:
    case 3:
        chk_data(FR_EP2_TXFREQ(frame), cfg->settings.tx_freq   , "TX FREQ", EP2_TXFREQ);
        break;

    case 4:
    case 5:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[0], "RX FREQ1", EP2_RXFREQ);
        break;

    case 6:
    case 7:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[1], "RX FREQ2", EP2_RXFREQ2);
        break;

    case 8:
    case 9:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[2], "RX FREQ3", EP2_RXFREQ3);
        break;

    case 10:
    case 11:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[3], "RX FREQ4", EP2_RXFREQ4);
        break;

    case 12:
    case 13:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[4], "RX FREQ5", EP2_RXFREQ5);
        break;

    case 14:
    case 15:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[5], "RX FREQ6", EP2_RXFREQ6);
        break;

    case 16:
    case 17:
        chk_data(FR_EP2_RXFREQ(frame), cfg->settings.rx_freq[6], "RX FREQ7", EP2_RXFREQ7);
        break;

    case 18:
    case 19:
        chk_data(FR_EP2_TXDRIVE(frame)         , cfg->settings.txdrive       , "TX DRIVE"           , EP2_TXDRIVE);
        chk_data(FR_EP2_HERMESCONFIG(frame)    , cfg->settings.hermes_config , "HERMES CONFIG"      , EP2_HERMESCONFIG);
        chk_data(FR_EP2_ALEXMANUALHPFLPF(frame), cfg->settings.alex_manual   , "ALEX manual HPF/LPF", EP2_ALEXMANUALHPFLPF);
        chk_data(FR_EP2_VNA(frame)             , cfg->settings.vna           , "VNA mode"           , EP2_VNA);
        chk_data(FR_EP2_ALEXHPF(frame)         , cfg->settings.alex_hpf      , "ALEX HPF"           , EP2_ALEXHPF);
        chk_data(FR_EP2_ALEXBYPASS(frame)      , cfg->settings.alex_bypass   , "ALEX Bypass HPFs"   , EP2_ALEXBYPASS);
        chk_data(FR_EP2_LNA6M(frame)           , cfg->settings.lna6m         , "ALEX 6m LNA"        , EP2_LNA6M);
        chk_data(FR_EP2_ALEXTRDISABLE(frame)   , cfg->settings.alexTRdisable , "ALEX T/R disable"   , EP2_ALEXTRDISABLE);
        chk_data(FR_EP2_ALEXLPF(frame)         , cfg->settings.alex_lpf      , "ALEX LPF"           , EP2_ALEXLPF);
        // reset tx level. leave a little head-room for noise
        txdrv_dbl = (double) cfg->settings.txdrive * 0.00390625;  // div. by. 256
        break;

    case 20:
    case 21:
        chk_data(FR_EP2_ADC1PREAMP(frame), cfg->settings.rx_preamp[0], "ADC1 preamp"        , EP2_ADC1PREAMP);
        chk_data(FR_EP2_ADC2PREAMP(frame), cfg->settings.rx_preamp[1], "ADC2 preamp"        , EP2_ADC2PREAMP);
        chk_data(FR_EP2_ADC3PREAMP(frame), cfg->settings.rx_preamp[2], "ADC3 preamp"        , EP2_ADC3PREAMP);
        chk_data(FR_EP2_ADC4PREAMP(frame), cfg->settings.rx_preamp[3], "ADC4 preamp"        , EP2_ADC4PREAMP);
        chk_data(FR_EP2_TIPRING(frame)   , cfg->settings.tip_ring    , "TIP/Ring"           , EP2_TIPRING);
        chk_data(FR_EP2_MICBIAS(frame)   , cfg->settings.MicBias     , "MicBias"            , EP2_MICBIAS);
        chk_data(FR_EP2_MICPTT(frame)    , cfg->settings.MicPTT      , "MicPTT"             , EP2_MICPTT);
        chk_data(FR_EP2_LINEGAIN(frame)  , cfg->settings.LineGain    , "LineGain"           , EP2_LINEGAIN);
        chk_data(FR_EP2_MERTXATT0(frame) , cfg->settings.MerTxATT0   , "Mercury Att on TX/0", EP2_MERTXATT0);
        chk_data(FR_EP2_PURESIGNAL(frame), cfg->settings.PureSignal  , "PureSignal"         , EP2_PURESIGNAL);
        chk_data(FR_EP2_PENESEL(frame)   , cfg->settings.PeneSel     , "PenelopeSelect"     , EP2_PENESEL);
        chk_data(FR_EP2_METISDB9(frame)  , cfg->settings.MetisDB9    , "MetisDB9"           , EP2_METISDB9);
        chk_data(FR_EP2_MERTXATT1(frame) , cfg->settings.MerTxATT1   , "Mercury Att on TX/1", EP2_MERTXATT1);

        if (frame[4] & 0x40) {
            // some firmware/emulators use bit6 to indicate a 6-bit format
            // for a combined attenuator/preamplifier with the AD9866 chip.
            // the value is between 0 and 60 and formally corresponds to
            // to an rx gain of -12 to +48 dB. however, we set here that
            // a value of +16 (that is, 28 on the 0-60 scale) corresponds to
            // "zero attenuation"
            chk_data(FR_EP2_RX1HLATTGAIN(frame), cfg->settings.rx_att[0], "RX1 HL ATT/GAIN", EP2_RX1HLATTGAIN);
        } else {
            chk_data(FR_EP2_RX1ATT(frame)      , cfg->settings.rx_att[0], "RX1 ATT"        , EP2_RX1ATT);
            chk_data(FR_EP2_RX1ATTENABLE(frame), cfg->settings.rx1_attE , "RX1 ATT enable" , EP2_RX1ATTENABLE);
            //
            // some hardware emulates "switching off att and preamp" by setting ATT
            // to 20 dB, because the preamp cannot be switched.
            // if (!rx1_attE) rx_att[0]=20;
        }
        if (cfg->global.emulation != DEVICE_C25) {
            // set rx amplification factors. no switchable preamps available normally.
            rxatt_dbl[0] = pow(10.0, -0.05 * (10 * cfg->settings.AlexAtt + cfg->settings.rx_att[0]));
            rxatt_dbl[1] = pow(10.0, -0.05 * (cfg->settings.rx_att[1]));
            rxatt_dbl[2] = 1.0;
            rxatt_dbl[3] = 1.0;
        }
        break;

    case 22:
    case 23:
        chk_data(FR_EP2_RX2ATT(frame)   , cfg->settings.rx_att[1]  , "RX2 ATT"   , EP2_RX2ATT);
        chk_data(FR_EP2_CWREV(frame)    , cfg->settings.cw_reversed, "CW REV"    , EP2_CWREV);
        chk_data(FR_EP2_CWSPEED(frame)  , cfg->settings.cw_speed   , "CW SPEED"  , EP2_CWSPEED);
        chk_data(FR_EP2_CWMODE(frame)   , cfg->settings.cw_mode    , "CW MODE"   , EP2_CWMODE);
        chk_data(FR_EP2_CWWEIGHT(frame) , cfg->settings.cw_weight  , "CW WEIGHT" , EP2_CWWEIGHT);
        chk_data(FR_EP2_CWSPACING(frame), cfg->settings.cw_spacing , "CW SPACING", EP2_CWSPACING);

        // set rx amplification factors.
        rxatt_dbl[1] = pow(10.0, -0.05 * (cfg->settings.rx_att[1]));
        break;

    case 24:
    case 25:
        data = frame[1];
        data |= frame[2] << 8;
        chk_data(FR_EP2_C25EXTBOARDDATA(frame), cfg->settings.c25_ext_board_i2c_data, "C25 EXT BOARD DATA", EP2_C25EXTBOARDDATA);
        break;

    case 28:
    case 29:
        chk_data(FR_EP2_RX1ADC(frame), cfg->settings.rx_adc[0], "RX1 ADC", EP2_RX1ADC);
        chk_data(FR_EP2_RX2ADC(frame), cfg->settings.rx_adc[1], "RX2 ADC", EP2_RX2ADC);
        chk_data(FR_EP2_RX3ADC(frame), cfg->settings.rx_adc[2], "RX3 ADC", EP2_RX3ADC);
        chk_data(FR_EP2_RX4ADC(frame), cfg->settings.rx_adc[3], "RX4 ADC", EP2_RX4ADC);
        chk_data(FR_EP2_RX5ADC(frame), cfg->settings.rx_adc[4], "RX5 ADC", EP2_RX5ADC);
        chk_data(FR_EP2_RX6ADC(frame), cfg->settings.rx_adc[5], "RX6 ADC", EP2_RX6ADC);
        chk_data(FR_EP2_RX7ADC(frame), cfg->settings.rx_adc[6], "RX7 ADC", EP2_RX7ADC);
        chk_data(FR_EP2_TXATT(frame) , cfg->settings.txatt    , "TX ATT" , EP2_TXATT);
        txatt_dbl = pow(10.0, -0.05 * (double) cfg->settings.txatt);
        if (cfg->global.emulation == DEVICE_C25) {
            // redpitaya: hard-wired adc cfg->settings.
            cfg->settings.rx_adc[0] = 0;
            cfg->settings.rx_adc[1] = 1;
            cfg->settings.rx_adc[2] = 1;
        }
        break;

    case 30:
    case 31:
        chk_data(FR_EP2_CWINT(frame)         , cfg->settings.cw_internal    , "CW INT"          , EP2_CWINT);
        chk_data(FR_EP2_SIDETONEVOLUME(frame), cfg->settings.sidetone_volume, "SIDE TONE VOLUME", EP2_SIDETONEVOLUME);
        chk_data(FR_EP2_CWDELAY(frame)       , cfg->settings.cw_delay       , "CW DELAY"        , EP2_CWDELAY);
        cfg->settings.cw_delay = frame[3];
        break;

    case 32:
    case 33:
        chk_data(FR_EP2_CWHANG(frame)      , cfg->settings.cw_hang, "CW HANG"       , EP2_CWHANG);
        chk_data(FR_EP2_SIDETONEFREQ(frame), cfg->settings.freq   , "SIDE TONE FREQ", EP2_SIDETONEFREQ);
        break;
    }
}
