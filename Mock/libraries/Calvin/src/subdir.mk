################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/Calvin/src/Calvin.cpp 

OBJS += \
./libraries/Calvin/src/Calvin.o 

CPP_DEPS += \
./libraries/Calvin/src/Calvin.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/Calvin/src/%.o: ../libraries/Calvin/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


