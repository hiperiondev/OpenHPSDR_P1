# OpenHPSDR_P1
The objective of this project is to create a complete infrastructure that supports the Open HPSDR version 1 protocol to implement the control of any generic software-defined radio hardware.

It is only necessary to implement the hardware control functions.

## Callbacks

### Functions for create and destroy hardware infrastructure.

This functions have not requirements and are called at start and end of the system.

> uint8_t (*hw_init)(void)
>
> uint8_t (*hw_deinit)(void)

### Function for manage changes in internal setting.

Every time an ep2 packet change an internal state this function is called

> void (*ep2_callback)(int func, char* name)

### Main thread for receive/transmit

This process push/poll data from IQ buffer and comunicate with hardware.

Data receive hpsdr_config_t *cfg

> void* (*hw_thread)(void *data)

## Buffers

RX/TX buffers are circular queues of float _Complex. All data write/read on an overflow/underflow conditions are discarded

### write functions

> int hpsdr_txbuffer_write(hpsdr_config_t **cfg, float _Complex *iq, const int size)
> 
> int hpsdr_rxbuffer_write(hpsdr_config_t **cfg, float _Complex *iq, const int size)

### read functions

> float _Complex* hpsdr_txbuffer_read(hpsdr_config_t **cfg, const int size)
>
> float _Complex* hpsdr_rxbuffer_read(hpsdr_config_t **cfg, const int size)

## TODO
* [x] Add documentation
* [x] Add Doxygen structure
* [ ] Complete protocol
   * [x] transmission
   * [ ] reception
