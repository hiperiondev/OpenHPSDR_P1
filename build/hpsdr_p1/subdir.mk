# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hpsdr_p1/hpsdr_debug.c \
../hpsdr_p1/hpsdr_ep2.c \
../hpsdr_p1/hpsdr_ep6.c \
../hpsdr_p1/hpsdr_network.c \
../hpsdr_p1/hpsdr_p1.c \
../hpsdr_p1/hpsdr_ring_buf.c \
../hpsdr_p1/hpsdr_rx_samples.c \
../hpsdr_p1/hpsdr_special_functions.c \
../hpsdr_p1/hpsdr_tx_samples.c 

C_DEPS += \
./hpsdr_p1/hpsdr_debug.d \
./hpsdr_p1/hpsdr_ep2.d \
./hpsdr_p1/hpsdr_ep6.d \
./hpsdr_p1/hpsdr_network.d \
./hpsdr_p1/hpsdr_p1.d \
./hpsdr_p1/hpsdr_ring_buf.d \
./hpsdr_p1/hpsdr_rx_samples.d \
./hpsdr_p1/hpsdr_special_functions.d \
./hpsdr_p1/hpsdr_tx_samples.d 

OBJS += \
./hpsdr_p1/hpsdr_debug.o \
./hpsdr_p1/hpsdr_ep2.o \
./hpsdr_p1/hpsdr_ep6.o \
./hpsdr_p1/hpsdr_network.o \
./hpsdr_p1/hpsdr_p1.o \
./hpsdr_p1/hpsdr_ring_buf.o \
./hpsdr_p1/hpsdr_rx_samples.o \
./hpsdr_p1/hpsdr_special_functions.o \
./hpsdr_p1/hpsdr_tx_samples.o 

# Each subdirectory must supply rules for building sources it contributes
hpsdr_p1/%.o: ../hpsdr_p1/%.c hpsdr_p1/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../hpsdr_p1/include" -I"../test" -I".." -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean: clean-hpsdr_p1

clean-hpsdr_p1:
	-$(RM) ./hpsdr_p1/hpsdr_debug.d ./hpsdr_p1/hpsdr_debug.o ./hpsdr_p1/hpsdr_ep2.d ./hpsdr_p1/hpsdr_ep2.o ./hpsdr_p1/hpsdr_ep6.d ./hpsdr_p1/hpsdr_ep6.o ./hpsdr_p1/hpsdr_network.d ./hpsdr_p1/hpsdr_network.o ./hpsdr_p1/hpsdr_p1.d ./hpsdr_p1/hpsdr_p1.o ./hpsdr_p1/hpsdr_ring_buf.d ./hpsdr_p1/hpsdr_ring_buf.o ./hpsdr_p1/hpsdr_rx_samples.d ./hpsdr_p1/hpsdr_rx_samples.o ./hpsdr_p1/hpsdr_special_functions.d ./hpsdr_p1/hpsdr_special_functions.o ./hpsdr_p1/hpsdr_tx_samples.d ./hpsdr_p1/hpsdr_tx_samples.o

.PHONY: clean-hpsdr_p1
