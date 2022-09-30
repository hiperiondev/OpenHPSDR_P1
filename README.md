# OpenHPSDR_P1
The objective of this project is to create a complete infrastructure that supports the Open HPSDR version 1 protocol to implement the control of any generic software-defined radio hardware.

It is only necessary to implement the hardware control functions.

## Callbacks

### Functions for create and destroy hardware infrastructure.

This functions have not requirements and are called at start and end of the system.
```c
uint8_t (*hw_init)(void)
uint8_t (*hw_deinit)(void)
```

### Function for manage changes in internal setting.

Every time an ep2 packet change an internal state this function is called
```c
void (*ep2_callback)(int func, char* name)
```

### Main thread for receive/transmit

This process push/poll data from IQ buffer and comunicate with hardware.

Data receive hpsdr_config_t *cfg
```c
void* (*hw_thread)(void *data)
```

## Buffers

RX/TX buffers are circular queues of float _Complex. All data write/read on an overflow/underflow conditions are discarded

### write functions
```c
void hpsdr_txbuffer_write(hpsdr_config_t **cfg, float _Complex *iq);
void hpsdr_rxbuffer_write(hpsdr_config_t **cfg, float _Complex *iq);
```

### read functions
```c
void hpsdr_txbuffer_read(hpsdr_config_t **cfg, float _Complex *data);
void hpsdr_rxbuffer_read(hpsdr_config_t **cfg, float _Complex *data);
```

## BUILD TEST
```bash
$ git clone https://github.com/hiperiondev/OpenHPSDR_P1.git
$ cd OpenHPSDR_P1
$ cd build
$ make all
$ ./OpenHPSDR_P1 -d
```

## TODO
* [x] Add documentation
* [x] Add Doxygen structure
* [ ] Complete protocol
   * [x] transmission
   * [ ] reception
