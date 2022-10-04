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

#ifndef HPSDR_HARDWARE_FAKE_H_
#define HPSDR_HARDWARE_FAKE_H_

extern bool ifile;
extern bool ofile;
extern char ifilename[254];
extern char ofilename[254];

uint8_t iqtransmitter_init(void);
uint8_t iqtransmitter_deinit(void);
uint8_t iqreceiver_init(void);
uint8_t iqreceiver_deinit(void);
  void* iqtransmitter_thread(void *data);
  void* iqreceiver_thread(void *data);

#endif /* HPSDR_HARDWARE_FAKE_H_ */
