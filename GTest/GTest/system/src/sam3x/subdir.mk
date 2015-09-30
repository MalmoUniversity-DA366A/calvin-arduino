################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/sam3x/adc.c \
../system/src/sam3x/dacc.c \
../system/src/sam3x/interrupt_sam_nvic.c \
../system/src/sam3x/pio.c \
../system/src/sam3x/pmc.c \
../system/src/sam3x/pwmc.c \
../system/src/sam3x/spi.c \
../system/src/sam3x/tc.c \
../system/src/sam3x/timetick.c \
../system/src/sam3x/uart.c 

OBJS += \
./system/src/sam3x/adc.o \
./system/src/sam3x/dacc.o \
./system/src/sam3x/interrupt_sam_nvic.o \
./system/src/sam3x/pio.o \
./system/src/sam3x/pmc.o \
./system/src/sam3x/pwmc.o \
./system/src/sam3x/spi.o \
./system/src/sam3x/tc.o \
./system/src/sam3x/timetick.o \
./system/src/sam3x/uart.o 

C_DEPS += \
./system/src/sam3x/adc.d \
./system/src/sam3x/dacc.d \
./system/src/sam3x/interrupt_sam_nvic.d \
./system/src/sam3x/pio.d \
./system/src/sam3x/pmc.d \
./system/src/sam3x/pwmc.d \
./system/src/sam3x/spi.d \
./system/src/sam3x/tc.d \
./system/src/sam3x/timetick.d \
./system/src/sam3x/uart.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/sam3x/%.o: ../system/src/sam3x/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -Wno-conversion -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


