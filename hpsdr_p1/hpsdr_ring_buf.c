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
#include <stdbool.h>
#include <math.h>

#include "hpsdr_ring_buf.h"

////////////////////////////////////////////////////////////////////////////////
void RingBuf_ctor(RingBuf *const me, double _Complex sto[], RingBufCtr sto_len) {
    me->buf = &sto[0];
    me->end = sto_len;
    me->head = 0U;
    me->tail = 0U;
}

////////////////////////////////////////////////////////////////////////////////
bool RingBuf_put(RingBuf *const me, double _Complex const el) {
    RingBufCtr head = me->head + 1U;
    if (head == me->end) {
        head = 0U;
    }
    if (head != me->tail) {     // buffer NOT full?
        me->buf[me->head] = el; // copy the element into the buffer
        me->head = head;        // update the head to a *valid* index
        return true;            // element placed in the buffer
    } else {
        return false;           // element NOT placed in the buffer
    }
}

////////////////////////////////////////////////////////////////////////////////
bool RingBuf_get(RingBuf *const me, double _Complex *pel) {
    RingBufCtr tail = me->tail;
    if (me->head != tail) {     // ring buffer NOT empty?
        *pel = me->buf[tail];
        ++tail;
        if (tail == me->end) {
            tail = 0U;
        }
        me->tail = tail;        // update the tail to a *valid* index
        return true;
    } else {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////
RingBufCtr RingBuf_num_free(RingBuf *const me) {
    RingBufCtr head = me->head;
    RingBufCtr tail = me->tail;
    if (head == tail) {         // buffer empty?
        return (RingBufCtr) (me->end - 1U);
    } else
        if (head < tail) {
            return (RingBufCtr) (tail - head - 1U);
        } else {
            return (RingBufCtr) (me->end + tail - head - 1U);
        }
}

////////////////////////////////////////////////////////////////////////////////
void RingBuf_process_all(RingBuf *const me, RingBufHandler handler) {
    RingBufCtr tail = me->tail;
    while (me->head != tail) {  // ring buffer NOT empty?
        (*handler)(me->buf[tail]);
        ++tail;
        if (tail == me->end) {
            tail = 0U;
        }
        me->tail = tail;        // update the tail to a *valid* index
    }
}
