################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/newlib/_exit.c \
../system/src/newlib/_sbrk.c \
../system/src/newlib/_startup.c \
../system/src/newlib/_syscalls.c \
../system/src/newlib/_write.c \
../system/src/newlib/assert.c 

CPP_SRCS += \
../system/src/newlib/_cxx.cpp 

OBJS += \
./system/src/newlib/_cxx.o \
./system/src/newlib/_exit.o \
./system/src/newlib/_sbrk.o \
./system/src/newlib/_startup.o \
./system/src/newlib/_syscalls.o \
./system/src/newlib/_write.o \
./system/src/newlib/assert.o 

C_DEPS += \
./system/src/newlib/_exit.d \
./system/src/newlib/_sbrk.d \
./system/src/newlib/_startup.d \
./system/src/newlib/_syscalls.d \
./system/src/newlib/_write.d \
./system/src/newlib/assert.d 

CPP_DEPS += \
./system/src/newlib/_cxx.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/newlib/%.o: ../system/src/newlib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Wabi -Wctor-dtor-privacy -Wnoexcept -Wstrict-null-sentinel -Wsign-promo -Wno-conversion -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/src/newlib/%.o: ../system/src/newlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -Wno-conversion -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


