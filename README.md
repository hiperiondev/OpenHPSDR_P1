# OpenHPSDR_P1
The objective of this project is to create a complete infrastructure that supports the Open HPSDR version 1 protocol to implement the control of any generic software-defined radio hardware.

It is only necessary to implement the hardware control functions.

## Callbacks

### Functions for create and destroy hardware infrastructure.

This functions have not requirements and are called at start and end of the system.

> __uint8_t (*hw_init)(void)__.
>
> __uint8_t (*hw_deinit)(void)__.

### Function for manage changes in internal setting.

Every time an ep2 packet change an internal state this function is called

> __void (*ep2_callback)(int func, char* name)__

### Main thread for receive/transmit

This process push/poll data from IQ buffer and comunicate with hardware.

Data receive hpsdr_config_t *cfg

> __void* (*hw_thread)(void *data)__

## Buffers

RX/TX buffers are circular queues. All data write/read on an overflow/underflow conditions are discarded

### write functions

> __int hpsdr_txbuffer_write(hpsdr_config_t **cfg, float _Complex *iq, const int size)__
> 
> __int hpsdr_rxbuffer_write(hpsdr_config_t **cfg, float _Complex *iq, const int size)__

### read functions

> __float _Complex* hpsdr_txbuffer_read(hpsdr_config_t **cfg, const int size)__
> 
> __float _Complex* hpsdr_rxbuffer_read(hpsdr_config_t **cfg, const int size)__

## TODO
* [x] Add documentation
* [x] Add Doxygen structure
* [ ] Complete protocol
   * [x] transmission
   * [ ] reception
