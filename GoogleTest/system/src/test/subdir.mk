################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/test/test_exampel.c 

OBJS += \
./system/src/test/test_exampel.o 

C_DEPS += \
./system/src/test/test_exampel.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/test/%.o: ../system/src/test/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDEBUG -DARDUINO -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


