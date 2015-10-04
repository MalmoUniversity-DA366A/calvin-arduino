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
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DOS_USE_TRACE_SEMIHOSTING_STDOUT -I"../include" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Wabi -Wctor-dtor-privacy -Wnoexcept -Wnon-virtual-dtor -Wstrict-null-sentinel -Wsign-promo -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


