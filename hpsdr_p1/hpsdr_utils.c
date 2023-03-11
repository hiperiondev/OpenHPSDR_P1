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

#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

uint64_t get_nanos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint64_t ns = (((uint64_t) ts.tv_sec)* 1000000000L) + (uint64_t) ts.tv_nsec;
    return ns;
}

int32_t convert_s16_to_s24(int16_t s16) {
    uint32_t output = 0;
    int temp = s16;// * (INT_MAX >> 8);
    unsigned char *ptemp = (unsigned char*) &temp;

    output |= *(ptemp + 2);
    output <<= 8;
    output |= *(ptemp + 1);
    output <<= 8;
    output |= *ptemp;

    return output;
}

int32_t convert_s24_to_s32(int32_t s24) {
    unsigned char *byteArray = (unsigned char*) &s24;

    int result = (((0xFF & byteArray[0]) << 16) | ((0xFF & byteArray[1]) << 8) | (0xFF & byteArray[2]));
    if ((result & 0x00800000) > 0) {
        result = (int) ((unsigned int) result | (unsigned int) 0xFF000000);
    } else {
        result = (int) ((unsigned int) result & (unsigned int) 0x00FFFFFF);
    }
    return result;
}

int32_t convert_s16_to_s32(int16_t s16) {
    unsigned char *byteArray = (unsigned char*) &s16;

    int result = (((0xFF & byteArray[0]) << 8) | (0xFF & byteArray[1]));
    if ((result & 0x00008000) > 0) {
        result = (int) ((unsigned int) result | (unsigned int) 0xFFFF0000);

    } else {
        result = (int) ((unsigned int) result & (unsigned int) 0x0000FFFF);
    }
    return result;
}

