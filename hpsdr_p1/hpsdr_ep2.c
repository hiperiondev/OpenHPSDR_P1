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
#include <math.h>

#include "hpsdr_debug.h"
#include "hpsdr_definitions.h"
#include "hpsdr_p1.h"
#include "hpsdr_ep2.h"

// floating-point represents of tx att, rx att, and rx preamp settings
double txatt_dbl = 1.0;
double rxatt_dbl[4] = { 1.0, 1.0, 1.0, 1.0 };  // this reflects both att and preamp
double txdrv_dbl = 0.99;

#define chk_data(fun,str) \
		if (FR_##fun(frame) != cfg->ep2_value[fun]) { \
		    cfg->ep2_value[fun] = FR_##fun(frame);    \
		    cfg->ep2_cb(cfg->ep2_value, fun, str);    \
		}

void ep2_handler(hpsdr_config_t *cfg, uint8_t *frame) {
    uint16_t data;

    chk_data(EP2_PTT, "PTT");
    switch (frame[0]) {
    case 0:
    case 1:
        chk_data(EP2_RATE                , "SampleRate"             );
        chk_data(EP2_REF10               , "Ref10MHz"               );
        chk_data(EP2_SRC122              , "Source122MHz"           );
        chk_data(EP2_PMCONFIG            , "Penelope/Mercury config");
        chk_data(EP2_MICSRC              , "Mic Source"             );
        chk_data(EP2_TXCLASSC            , "TX Class E"             );
        chk_data(EP2_OPENCOLLECTOROUTPUTS, "OpenCollector"          );
        chk_data(EP2_RECEIVERS           , "Receivers"              );
        chk_data(EP2_MICTS               , "TimeStampMic"           );
        chk_data(EP2_COMMONMERCURYFREQ   , "Common Mercury Freq"    );

        chk_data(EP2_ALEXATT             , "AlexAtt"                );
        chk_data(EP2_PREAMP              , "Preamp"                 );
        chk_data(EP2_LTDITHER            , "LTdither"               );
        chk_data(EP2_LTRANDOM            , "LTrandom"               );

        chk_data(EP2_ALEXRXANT           , "AlexRXant"              );
        chk_data(EP2_ALEXXRXOUT          , "AlexRXout"              );
        chk_data(EP2_ALETXREL            , "AlexTXrel"              );
        chk_data(EP2_DUPLEX              , "Duplex"                 );

        if (cfg->global.emulation == DEVICE_C25) {
            // charly25: has two 18-dB preamps that are switched with "preamp" and "dither"
            //           and two attenuators encoded in alex-att
            //           both only applies to rx1!
            rxatt_dbl[0] = pow(10.0, -0.05 * (12 * cfg->ep2_value[EP2_ALEXATT] - 18 * cfg->ep2_value[EP2_LTDITHER] - 18 * cfg->ep2_value[EP2_PREAMP]));
            rxatt_dbl[1] = 1.0;
        } else {
            // assume that it has alex attenuators in addition to the step attenuators
            rxatt_dbl[0] = pow(10.0, -0.05 * (10 * cfg->ep2_value[EP2_ALEXATT] + cfg->ep2_value[EP2_RX1ATT]));
            rxatt_dbl[1] = 1.0;
        }
        break;

    case 2:
    case 3:
        chk_data(EP2_TXFREQ,   "TX Frequency");
        break;

    case 4:
    case 5:
        chk_data(EP2_RXFREQ1, "RX Frequency 1");
        break;

    case 6:
    case 7:
        chk_data(EP2_RXFREQ2, "RX Frequency 2");
        break;

    case 8:
    case 9:
        chk_data(EP2_RXFREQ3, "RX Frequency 3");
        break;

    case 10:
    case 11:
        chk_data(EP2_RXFREQ4, "RX Frequency 4");
        break;

    case 12:
    case 13:
        chk_data(EP2_RXFREQ5, "RX Frequency 5");
        break;

    case 14:
    case 15:
        chk_data(EP2_RXFREQ6, "RX Frequency 6");
        break;

    case 16:
    case 17:
        chk_data(EP2_RXFREQ7, "RX Frequency 7");
        break;

    case 18:
    case 19:
        chk_data(EP2_TXDRIVE         , "TX Drive"           );
        chk_data(EP2_HERMESCONFIG    , "HERMES Config"      );
        chk_data(EP2_ALEXMANUALHPFLPF, "ALEX manual HPF/LPF");
        chk_data(EP2_VNA             , "VNA mode"           );
        chk_data(EP2_ALEXHPF         , "ALEX HPF"           );
        chk_data(EP2_ALEXBYPASS      , "ALEX Bypass HPFs"   );
        chk_data(EP2_LNA6M           , "ALEX 6m LNA"        );
        chk_data(EP2_ALEXTRDISABLE   , "ALEX T/R disable"   );
        chk_data(EP2_ALEXLPF         , "ALEX LPF"           );
        // reset tx level. leave a little head-room for noise
        txdrv_dbl = (double) cfg->ep2_value[EP2_TXDRIVE] * 0.00390625;  // div. by. 256
        break;

    case 20:
    case 21:
        chk_data(EP2_ADC1PREAMP, "ADC1 preamp"         );
        chk_data(EP2_ADC2PREAMP, "ADC2 preamp"         );
        chk_data(EP2_ADC3PREAMP, "ADC3 preamp"         );
        chk_data(EP2_ADC4PREAMP, "ADC4 preamp"         );
        chk_data(EP2_TIPRING   , "TIP/Ring"            );
        chk_data(EP2_MICBIAS   , "Mic Bias"            );
        chk_data(EP2_MICPTT    , "Mic PTT"             );
        chk_data(EP2_LINEGAIN  , "Line Gain"           );
        chk_data(EP2_MERTXATT0 , "Mercury Att on TX/0" );
        chk_data(EP2_PURESIGNAL, "Pure Signal"         );
        chk_data(EP2_PENESEL   , "Penelope Select"     );
        chk_data(EP2_METISDB9  , "Metis DB9"           );
        chk_data(EP2_MERTXATT1 , "Mercury Att on TX/1" );

        if (frame[4] & 0x40) {
            // some firmware/emulators use bit6 to indicate a 6-bit format
            // for a combined attenuator/preamplifier with the AD9866 chip.
            // the value is between 0 and 60 and formally corresponds to
            // to an rx gain of -12 to +48 dB. however, we set here that
            // a value of +16 (that is, 28 on the 0-60 scale) corresponds to
            // "zero attenuation"
            chk_data(EP2_RX1HLATTGAIN, "RX1 HL ATT/Gain");
        } else {
            chk_data(EP2_RX1ATT      , "RX1 ATT"        );
            chk_data(EP2_RX1ATTENABLE, "RX1 ATT enable" );
            //
            // some hardware emulates "switching off att and preamp" by setting ATT
            // to 20 dB, because the preamp cannot be switched.
            // if (!rx1_attE) rx_att[0]=20;
        }
        if (cfg->global.emulation != DEVICE_C25) {
            // set rx amplification factors. no switchable preamps available normally.
            rxatt_dbl[0] = pow(10.0, -0.05 * (10 * cfg->ep2_value[EP2_ALEXATT] + cfg->ep2_value[EP2_RX1ATT]));
            rxatt_dbl[1] = pow(10.0, -0.05 * (cfg->ep2_value[EP2_RX2ATT]));
            rxatt_dbl[2] = 1.0;
            rxatt_dbl[3] = 1.0;
        }
        break;

    case 22:
    case 23:
        chk_data(EP2_RX2ATT   , "RX2 ATT"   );
        chk_data(EP2_CWREV    , "CW REV"    );
        chk_data(EP2_CWSPEED  , "CW Speed"  );
        chk_data(EP2_CWMODE   , "CW Mode"   );
        chk_data(EP2_CWWEIGHT , "CW Weight" );
        chk_data(EP2_CWSPACING, "CW Spacing");

        // set rx amplification factors.
        rxatt_dbl[1] = pow(10.0, -0.05 * (cfg->ep2_value[EP2_RX2ATT]));
        break;

    case 24:
    case 25:
        data = frame[1];
        data |= frame[2] << 8;
        chk_data(EP2_C25EXTBOARDDATA, "C25 Ext Board Data");
        break;

    case 28:
    case 29:
        chk_data(EP2_RX1ADC, "RX1 ADC");
        chk_data(EP2_RX2ADC, "RX2 ADC");
        chk_data(EP2_RX3ADC, "RX3 ADC");
        chk_data(EP2_RX4ADC, "RX4 ADC");
        chk_data(EP2_RX5ADC, "RX5 ADC");
        chk_data(EP2_RX6ADC, "RX6 ADC");
        chk_data(EP2_RX7ADC, "RX7 ADC");
        chk_data(EP2_TXATT , "TX ATT" );
        txatt_dbl = pow(10.0, -0.05 * (double) cfg->ep2_value[EP2_TXATT]);
        if (cfg->global.emulation == DEVICE_C25) {
            // redpitaya: hard-wired adc cfg->settings.
            cfg->ep2_value[EP2_RX1ADC] = 0;
            cfg->ep2_value[EP2_RX2ADC] = 1;
            cfg->ep2_value[EP2_RX3ADC] = 1;
        }
        break;

    case 30:
    case 31:
        chk_data(EP2_CWINT         , "CW INT"          );
        chk_data(EP2_SIDETONEVOLUME, "Side Tone Volume");
        chk_data(EP2_CWDELAY       , "CW Delay"        );
        cfg->ep2_value[EP2_CWDELAY] = frame[3];
        break;

    case 32:
    case 33:
        chk_data(EP2_CWHANG      , "CW Hang"       );
        chk_data(EP2_SIDETONEFREQ, "Side Tone Frequency");
        break;
    }
}
