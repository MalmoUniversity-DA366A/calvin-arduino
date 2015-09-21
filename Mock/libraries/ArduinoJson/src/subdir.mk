################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/ArduinoJson/src/JsonArray.cpp \
../libraries/ArduinoJson/src/JsonBuffer.cpp \
../libraries/ArduinoJson/src/JsonObject.cpp \
../libraries/ArduinoJson/src/JsonVariant.cpp 

OBJS += \
./libraries/ArduinoJson/src/JsonArray.o \
./libraries/ArduinoJson/src/JsonBuffer.o \
./libraries/ArduinoJson/src/JsonObject.o \
./libraries/ArduinoJson/src/JsonVariant.o 

CPP_DEPS += \
./libraries/ArduinoJson/src/JsonArray.d \
./libraries/ArduinoJson/src/JsonBuffer.d \
./libraries/ArduinoJson/src/JsonObject.d \
./libraries/ArduinoJson/src/JsonVariant.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/ArduinoJson/src/%.o: ../libraries/ArduinoJson/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


