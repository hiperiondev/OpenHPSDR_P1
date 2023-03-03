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
#include "hpsdr_ring_buf.h"

#define MACADDR1 0xAA
#define MACADDR2 0xBB
#define MACADDR3 0xCC
#define MACADDR4 0xDD
#define MACADDR5 0xEE
#define MACADDR6 0xFF

/**
 * @brief Max buffers
 *
 * Max buffers
 */
#define RX_BUFFERS 8
#define TX_BUFFERS 8
/**
 * @brief Callbacks prototypes
 *
 * Callbacks prototypes
 */
typedef    void (*ep2_callback)(uint32_t *ep2_value, int func, char* name);

/**
 * @brief Struct of Global configuration
 *
 * Struct of Global configurations
 */
typedef struct global {
                bool debug;     /*!< debug enabled */
                 int iqburst;   /*!< burst for transmitter */
    emulation_type_t emulation; /*!< emulation type */
} global_t;

/**
 * @brief Struct of Internal configuration
 *
 * Struct of internal configuration.
 */
typedef struct hpsdr_config {
       global_t global;        /*!< global settings*/
       uint32_t ep2_value[72]; /*!< ep2 protol values */
        RingBuf rxbuff;        /*!< reception buffer */
        RingBuf txbuff;        /*!< transmission buffer */
           void *user;         /*!< user data */
   ep2_callback ep2_cb;        /*!< ep2 packet callback */
} hpsdr_config_t;

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

/**
 * @brief HPSDR Start
 *
 * HPSDR Start
 */
void hpsdr_start(hpsdr_config_t **cfg);

/**
 * @brief HPSDR Start
 *
 * HPSDR Start
 */
void hpsdr_stop(void);

/**
 * @brief Clear/Initialize configuration
 *
 * Clear/Initialize configuration
 */
void hpsdr_clear_config(hpsdr_config_t **cfg);

/**
 * @brief Write to tx buffer
 *
 * Push to tx buffer
 *
 * @param cfg configuration
 * @param iq  sample
 * @return result of write
 */
bool hpsdr_txbuffer_write(hpsdr_config_t **cfg, double _Complex *iq);

/**
 * @brief Write to rx buffer
 *
 * Push to rx buffer
 *
 * @param cfg configuration
 * @param iq  sample
 * @return result of write
 */
bool hpsdr_rxbuffer_write(hpsdr_config_t **cfg, double _Complex *iq);

/**
 * @brief Read from tx buffer
 *
 * Push to tx buffer
 *
 * @param cfg configuration
 * @param iq  buffer data
 * @return result of read
 */
bool hpsdr_txbuffer_read(hpsdr_config_t **cfg, double _Complex *iq);

/**
 * @brief Read from rx buffer
 *
 * Push to rx buffer
 *
 * @param cfg configuration
 * @param iq  buffer data
 * @return result of read
 */
bool hpsdr_rxbuffer_read(hpsdr_config_t **cfg, double _Complex *iq);

#endif /* HPSDR_P1_H_ */
