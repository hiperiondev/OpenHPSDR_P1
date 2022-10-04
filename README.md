# OpenHPSDR_P1
The objective of this project is to create a complete framework that supports the Open HPSDR version 1 protocol to implement the control of any generic software-defined radio hardware.

It is only necessary to implement the hardware control functions.

## Callback for manage changes in internal setting.

Every time an ep2 packet change an internal state this function is called
```c
void (*ep2_callback)(int func, char* name)
```

## Buffers

RX/TX buffers are circular queues of float _Complex.

### write functions
```c
bool hpsdr_txbuffer_write(hpsdr_config_t **cfg, float _Complex *iq);
bool hpsdr_rxbuffer_write(hpsdr_config_t **cfg, float _Complex *iq);
```

### read functions
```c
bool hpsdr_txbuffer_read(hpsdr_config_t **cfg, float _Complex *iq);
bool hpsdr_rxbuffer_read(hpsdr_config_t **cfg, float _Complex *iq);
```

## Build test
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
