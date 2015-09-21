################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/ArduinoJson/src/Internals/Comments.cpp \
../libraries/ArduinoJson/src/Internals/Encoding.cpp \
../libraries/ArduinoJson/src/Internals/IndentedPrint.cpp \
../libraries/ArduinoJson/src/Internals/JsonParser.cpp \
../libraries/ArduinoJson/src/Internals/List.cpp \
../libraries/ArduinoJson/src/Internals/Prettyfier.cpp \
../libraries/ArduinoJson/src/Internals/StaticStringBuilder.cpp 

OBJS += \
./libraries/ArduinoJson/src/Internals/Comments.o \
./libraries/ArduinoJson/src/Internals/Encoding.o \
./libraries/ArduinoJson/src/Internals/IndentedPrint.o \
./libraries/ArduinoJson/src/Internals/JsonParser.o \
./libraries/ArduinoJson/src/Internals/List.o \
./libraries/ArduinoJson/src/Internals/Prettyfier.o \
./libraries/ArduinoJson/src/Internals/StaticStringBuilder.o 

CPP_DEPS += \
./libraries/ArduinoJson/src/Internals/Comments.d \
./libraries/ArduinoJson/src/Internals/Encoding.d \
./libraries/ArduinoJson/src/Internals/IndentedPrint.d \
./libraries/ArduinoJson/src/Internals/JsonParser.d \
./libraries/ArduinoJson/src/Internals/List.d \
./libraries/ArduinoJson/src/Internals/Prettyfier.d \
./libraries/ArduinoJson/src/Internals/StaticStringBuilder.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/ArduinoJson/src/Internals/%.o: ../libraries/ArduinoJson/src/Internals/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/CMSIS" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/sam3x" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


