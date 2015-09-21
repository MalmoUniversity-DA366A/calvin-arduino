################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/calvinlib/blinkLED.c 

OBJS += \
./system/src/calvinlib/blinkLED.o 

C_DEPS += \
./system/src/calvinlib/blinkLED.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/calvinlib/%.o: ../system/src/calvinlib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


