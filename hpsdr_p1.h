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

#ifndef HPSDR_P1_H_
#define HPSDR_P1_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <complex.h>

#include "hpsdr_definitions.h"

typedef uint8_t (*hw_init)(void);
typedef uint8_t (*hw_deinit)(void);
typedef uint8_t (*hw_set_freq)(uint64_t freq_hz);
typedef   void* (*hw_thread)(void *data);
typedef    void (*ep2_callback)(int func, char* name);

/**
 * @brief Struct of Global configuration
 *
 * Struct of Global configurations
 */
typedef struct global {
                bool debug;     // debug enabled
                 int iqburst;   // burst for transmitter
    emulation_type_t emulation; // emulation type
} global_t;

/**
 * @brief Struct of Callbacks
 *
 * Struct of Internal Callbacks
 */
typedef struct hpsdr_callbacks {
         hw_init tx_init;     // transmitter initialize callback
       hw_deinit tx_deinit;   // transmitter deinitialize callback
     hw_set_freq tx_set_freq; // transmitter set frequency callback
       hw_thread tx_thread;   // transmitter thread callback

         hw_init rx_init;     // receiver initialize callback
       hw_deinit rx_deinit;   // receiver deinitialize callback
     hw_set_freq rx_set_freq; // receiver set frequency callback
       hw_thread rx_thread;   // receiver thread callback

    ep2_callback ep2;         // ep2 packet callback
} hpsdr_callbacks_t;

/**
 * @brief Struct of Internal configuration
 *
 * Struct of internal configuration.
 */
typedef struct hpsdr_config {
             global_t global;
    hpsdr_callbacks_t cb;
                 void *rxbuff;
                 void *txbuff;
} hpsdr_config_t;

/**
 * @brief Configuration
 *
 * All HPSDR configurations
 */
extern hpsdr_config_t *cfg;

/**
 * @brief HPSDR Initialization
 *
 * HPSDR Initialization
 *
 * @param cfg HPSDR configuration
 */
void hpsdr_init(hpsdr_config_t **cfg);

/**
 * @brief HPSDR Deinitialization
 *
 * HPSDR Deinitialization
 *
 * @param cfg HPSDR configuration
 */
void hpsdr_deinit(hpsdr_config_t **cfg);

#endif /* HPSDR_P1_H_ */
