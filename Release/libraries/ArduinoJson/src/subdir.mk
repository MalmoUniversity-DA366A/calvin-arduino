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
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DOS_USE_TRACE_SEMIHOSTING_STDOUT -I"../include" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Wabi -Wctor-dtor-privacy -Wnoexcept -Wnon-virtual-dtor -Wstrict-null-sentinel -Wsign-promo -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


