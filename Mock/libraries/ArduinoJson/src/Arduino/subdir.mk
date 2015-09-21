################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/ArduinoJson/src/Arduino/Print.cpp \
../libraries/ArduinoJson/src/Arduino/String.cpp 

OBJS += \
./libraries/ArduinoJson/src/Arduino/Print.o \
./libraries/ArduinoJson/src/Arduino/String.o 

CPP_DEPS += \
./libraries/ArduinoJson/src/Arduino/Print.d \
./libraries/ArduinoJson/src/Arduino/String.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/ArduinoJson/src/Arduino/%.o: ../libraries/ArduinoJson/src/Arduino/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


