################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libraries/Ethernet/src/Dhcp.cpp \
../libraries/Ethernet/src/Dns.cpp \
../libraries/Ethernet/src/Ethernet.cpp \
../libraries/Ethernet/src/EthernetClient.cpp \
../libraries/Ethernet/src/EthernetServer.cpp \
../libraries/Ethernet/src/EthernetUdp.cpp 

OBJS += \
./libraries/Ethernet/src/Dhcp.o \
./libraries/Ethernet/src/Dns.o \
./libraries/Ethernet/src/Ethernet.o \
./libraries/Ethernet/src/EthernetClient.o \
./libraries/Ethernet/src/EthernetServer.o \
./libraries/Ethernet/src/EthernetUdp.o 

CPP_DEPS += \
./libraries/Ethernet/src/Dhcp.d \
./libraries/Ethernet/src/Dns.d \
./libraries/Ethernet/src/Ethernet.d \
./libraries/Ethernet/src/EthernetClient.d \
./libraries/Ethernet/src/EthernetServer.d \
./libraries/Ethernet/src/EthernetUdp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/Ethernet/src/%.o: ../libraries/Ethernet/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DOS_USE_TRACE_SEMIHOSTING_STDOUT -I"../include" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -Wabi -Wctor-dtor-privacy -Wnoexcept -Wnon-virtual-dtor -Wstrict-null-sentinel -Wsign-promo -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


