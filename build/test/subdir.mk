# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../test/cargs.c \
../test/hpsdr_test.c 

C_DEPS += \
./test/cargs.d \
./test/hpsdr_test.d 

OBJS += \
./test/cargs.o \
./test/hpsdr_test.o 

# Each subdirectory must supply rules for building sources it contributes
test/%.o: ../test/%.c test/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"../hpsdr_p1/include" -I"../test" -I"../" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

clean: clean-test

clean-test:
	-$(RM) ./test/cargs.d ./test/cargs.o ./test/hpsdr_test.d ./test/hpsdr_test.o

.PHONY: clean-test
