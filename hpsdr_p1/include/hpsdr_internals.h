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

#ifndef HPSDR_INTERNALS_H_
#define HPSDR_INTERNALS_H_

#include <stdint.h>

extern int enable_thread; // network thread
extern int active_thread; // ep6 thread

// RTXLEN must be an sixteen-fold multiple of 63
// because we have 63 samples per 512-byte METIS packet,
// and two METIS packets per TCP/UDP packet,
// and two/four/eight-fold up-sampling if the TX sample
// rate is 96000/192000/384000
#define BUFFLEN 63 // TODO: check

extern uint32_t last_seqnum; // sequence number of received packet
extern uint32_t seqnum;      // sequence number of received packet

// Constants for conversion of TX power
extern double c1, c2;

#endif /* HPSDR_INTERNALS_H_ */
