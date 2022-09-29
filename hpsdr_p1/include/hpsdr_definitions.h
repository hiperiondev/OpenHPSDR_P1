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

#ifndef HPSDR_DEFINITIONS_H_
#define HPSDR_DEFINITIONS_H_

/**
 * @brief Devices type
 *
 * Devices type.
 */
typedef enum {
    DEVICE_METIS        = 0,    //
    DEVICE_HERMES       = 1,    //
    DEVICE_GRIFFIN      = 2,    //
    DEVICE_ANGELIA      = 4,    //
    DEVICE_ORION        = 5,    //
    DEVICE_HERMES_LITE  = 6,    //
    DEVICE_HERMES_LITE2 = 1006, //
    DEVICE_ORION2       = 10,   //
    DEVICE_C25          = 100   //
} emulation_type_t;

/**
 * @brief EP2 functions
 *
 * EP2 functions.
 */
typedef enum ep2_functions {
    EP2_PTT,                  //
    EP2_RATE,                 //
    EP2_REF10,                //
    EP2_SRC122,               //
    EP2_PMCONFIG,             //
    EP2_MICSRC,               //
    EP2_TXCLASSC,             //
    EP2_OPENCOLLECTOROUTPUTS, //
    EP2_RECEIVERS,            //
    EP2_MICTS,                //
    EP2_COMMONMERCURYFREQ,    //
    EP2_TXFREQ,               //
    EP2_RXFREQ,               //
    EP2_RXFREQ2,              //
    EP2_RXFREQ3,              //
    EP2_RXFREQ4,              //
    EP2_RXFREQ5,              //
    EP2_RXFREQ6,              //
    EP2_RXFREQ7,              //
    EP2_TXDRIVE,              //
    EP2_HERMESCONFIG,         //
    EP2_ALEXMANUALHPFLPF,     //
    EP2_VNA,                  //
    EP2_ALEXHPF,              //
    EP2_ALEXBYPASS,           //
    EP2_LNA6M,                //
    EP2_ALEXTRDISABLE,        //
    EP2_ALEXLPF,              //
    EP2_ADC1PREAMP,           //
    EP2_ADC2PREAMP,           //
    EP2_ADC3PREAMP,           //
    EP2_ADC4PREAMP,           //
    EP2_TIPRING,              //
    EP2_MICBIAS,              //
    EP2_MICPTT,               //
    EP2_LINEGAIN,             //
    EP2_MERTXATT0,            //
    EP2_PURESIGNAL,           //
    EP2_PENESEL,              //
    EP2_METISDB9,             //
    EP2_MERTXATT1,            //
    EP2_RX1HLATTGAIN,         //
    EP2_RX1ATT,               //
    EP2_RX1ATTENABLE,         //
    EP2_RX2ATT,               //
    EP2_CWREV,                //
    EP2_CWSPEED,              //
    EP2_CWMODE,               //
    EP2_CWWEIGHT,             //
    EP2_CWSPACING,            //
    EP2_C25EXTBOARDDATA,      //
    EP2_RX1ADC,               //
    EP2_RX2ADC,               //
    EP2_RX3ADC,               //
    EP2_RX4ADC,               //
    EP2_RX5ADC,               //
    EP2_RX6ADC,               //
    EP2_RX7ADC,               //
    EP2_TXATT,                //
    EP2_CWINT,                //
    EP2_SIDETONEVOLUME,       //
    EP2_CWDELAY,              //
    EP2_CWHANG,               //
    EP2_SIDETONEFREQ,         //
    EP2_ALEXATT,              //
    EP2_LTDITHER,             //
    EP2_PREAMP,               //
    EP2_LTRANDOM,             //
    EP2_ALEXRXANT,            //
    EP2_ALEXXRXOUT,           //
    EP2_ALETXREL,             //
    EP2_DUPLEX,               //
} ep2_functions_t;

#endif /* HPSDR_DEFINITIONS_H_ */
