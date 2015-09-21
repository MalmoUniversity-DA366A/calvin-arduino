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
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


