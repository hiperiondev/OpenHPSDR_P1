# OpenHPSDR_P1
The objective of this project is to create a complete infrastructure that supports the Open HPSDR version 1 protocol to implement the control of any generic software-defined radio hardware.

It is only necessary to implement the hardware control functions.

## Callbacks

Functions for create and destroy hardware infrastructure.

This functions have not requirements and are called at start and end of the system
```
uint8_t (*hw_init)(void);
uint8_t (*hw_deinit)(void);
```

Function for set frequency of receiver/transmitter.

This function are called any time an ep2 packet change frequency of receiver/transmitter 
```
uint8_t (*hw_set_freq)(uint8_t id, uint64_t freq_hz);
```

Function for manage changes in internal setting.

Every time an ep2 packet change an internal state this function is called
```
void (*ep2_callback)(int func, char* name);
```

Main thread for receive/transmit

This process push/poll data from IQ buffer and comunicate with hardware
```
void* (*hw_thread)(void *data);
```

## Buffers
...



## TODO
* [ ] Add documentation
* [x] Add Doxygen structure
* [ ] Complete protocol
   * [x] transmission
   * [ ] reception
