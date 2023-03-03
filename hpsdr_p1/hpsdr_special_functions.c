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

#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "hpsdr_debug.h"
#include "hpsdr_p1.h"

// special functions
void hpsdr_erase_packet(uint8_t *buffer) {
    sleep(1);
    memset(buffer, 0, 60);
    buffer[0] = 0xEF;
    buffer[1] = 0xFE;
    buffer[2] = 0x03;
    // mac: aa:bb:cc:dd:ee:ff
    buffer[3] = 0xAA; // mac MSB
    buffer[4] = 0xBB;
    buffer[5] = 0xCC;
    buffer[6] = 0xDD;
    buffer[7] = 0xEE;
    buffer[8] = 0xFF; // mac LSB
}

void hpsdr_program(uint8_t *buffer) {
    usleep(1000);
    memset(buffer, 0, 60);
    buffer[0] = 0xEF;
    buffer[1] = 0xFE;
    buffer[2] = 0x04;
    buffer[3] = 0xAA;
    buffer[4] = 0xBB;
    buffer[5] = 0xCC;
    buffer[6] = 0xDD;
    buffer[7] = 0xEE;
    buffer[8] = 0xFF;
}

void hpsdr_set_ip(uint8_t *buffer) {
    buffer[2] = 0x02;
    memset(buffer + 9, 0, 54);
}
