################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/cores/WInterrupts.c \
../system/src/cores/dtostrf.c \
../system/src/cores/hooks.c \
../system/src/cores/itoa.c \
../system/src/cores/syscalls_sam3.c \
../system/src/cores/wiring.c \
../system/src/cores/wiring_analog.c \
../system/src/cores/wiring_digital.c 

CPP_SRCS += \
../system/src/cores/IPAddress.cpp \
../system/src/cores/Print.cpp \
../system/src/cores/RingBuffer.cpp \
../system/src/cores/Stream.cpp \
../system/src/cores/UARTClass.cpp \
../system/src/cores/USARTClass.cpp \
../system/src/cores/WMath.cpp \
../system/src/cores/WString.cpp \
../system/src/cores/variant.cpp 

OBJS += \
./system/src/cores/IPAddress.o \
./system/src/cores/Print.o \
./system/src/cores/RingBuffer.o \
./system/src/cores/Stream.o \
./system/src/cores/UARTClass.o \
./system/src/cores/USARTClass.o \
./system/src/cores/WInterrupts.o \
./system/src/cores/WMath.o \
./system/src/cores/WString.o \
./system/src/cores/dtostrf.o \
./system/src/cores/hooks.o \
./system/src/cores/itoa.o \
./system/src/cores/syscalls_sam3.o \
./system/src/cores/variant.o \
./system/src/cores/wiring.o \
./system/src/cores/wiring_analog.o \
./system/src/cores/wiring_digital.o 

C_DEPS += \
./system/src/cores/WInterrupts.d \
./system/src/cores/dtostrf.d \
./system/src/cores/hooks.d \
./system/src/cores/itoa.d \
./system/src/cores/syscalls_sam3.d \
./system/src/cores/wiring.d \
./system/src/cores/wiring_analog.d \
./system/src/cores/wiring_digital.d 

CPP_DEPS += \
./system/src/cores/IPAddress.d \
./system/src/cores/Print.d \
./system/src/cores/RingBuffer.d \
./system/src/cores/Stream.d \
./system/src/cores/UARTClass.d \
./system/src/cores/USARTClass.d \
./system/src/cores/WMath.d \
./system/src/cores/WString.d \
./system/src/cores/variant.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/cores/%.o: ../system/src/cores/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Wabi -Wctor-dtor-privacy -Wnoexcept -Wstrict-null-sentinel -Wsign-promo -Wno-conversion -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/cores/%.o: ../system/src/cores/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -Wno-conversion -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


