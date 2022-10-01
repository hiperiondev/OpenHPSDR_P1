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

#ifndef RING_BUF_H
#define RING_BUF_H

#include <math.h>
#include <stdbool.h>

/*! Ring buffer counter/index
 *
 * @attention
 * The following RingBufCtr type is assumed to be "atomic" in a target CPU,
 * meaning that the CPU needs to write the whole RingBufCtr in one machine
 * instruction. An example of violating this assumption would be an 8-bit
 * CPU, which writes uint16_t (2-bytes) in 2 machine instructions. For such
 * 8-bit CPU, the maximum size of RingBufCtr would be uint8_t (1-byte).
 *
 * Another case of violating the "atomic" writes to RingBufCtr type would
 * be misalignment of a RingBufCtr variable in memory, which could cause
 * the compiler to generate multiple instructions to write a RingBufCtr value.
 * Therefore, it is further assumed that all RingBufCtr variables in the
 * following RingBuf struct *are* correctly aligned for "atomic" access.
 * In practice, most C compilers should provide such natural alignment
 * (by inserting some padding into the struct, if necessary).
 */
typedef uint16_t RingBufCtr;

/*! Ring buffer element type
 *
 * @details
 * The type of the ring buffer elements is not critical for the lock-free
 * operation and does not need to be "atomic". For example, it can be
 * an integer type (uint16_t, uint32_t, uint64_t), a pointer type,
 * or even a struct type. However, the bigger the type the more RAM is
 * needed for the ring buffer and more CPU cycles are needed to copy it
 * into and out of the buffer memory.
 */
//typedef float _Complex RingBufElement;

/*! Ring buffer struct */
typedef struct {
        float _Complex *buf;  // pointer to the start of the ring buffer
             RingBufCtr end;  // offset of the end of the ring buffer
    RingBufCtr volatile head; // offset to where next el. will be inserted
    RingBufCtr volatile tail; // offset of where next el. will be removed
} RingBuf;

      void RingBuf_ctor(RingBuf *const me, float _Complex sto[], RingBufCtr sto_len);
RingBufCtr RingBuf_num_free(RingBuf *const me);
      bool RingBuf_put(RingBuf *const me, float _Complex const el);
      bool RingBuf_get(RingBuf *const me, float _Complex *pel);

/*! Ring buffer callback function for RingBuf_process_all()
 *
 * @details
 * The callback processes one element and runs in the context of
 * RingBuf_process_all().
 */
typedef void (*RingBufHandler)(float _Complex const el);

void RingBuf_process_all(RingBuf *const me, RingBufHandler handler);

#endif /* RING_BUF_H */
