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
	@echo 'Invoking: GCC C++ Compiler'
	g++ -D_MOCK_ -DDEBUG -DF_CPU=84000000UL -DSAM3XA_SERIES -DTRACE -DOS_USE_TRACE_SEMIHOSTING_STDOUT -D__SAM3X8E__ -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/LiquidCrystal/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/SPI" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/Ethernet/src/utility" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/libraries/ArduinoJson/src" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/cores" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/calvinlib" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/unity" -I"/Users/peterjohansson/Desktop/Github/calvin-arduino/system/include/test" -I"../system/include" -I”/usr/local/include” -I"../system/include/CMSIS" -I"../system/include/sam3x" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


