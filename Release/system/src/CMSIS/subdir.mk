################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/CMSIS/system_sam3x.c \
../system/src/CMSIS/vectors_sam3x.c 

OBJS += \
./system/src/CMSIS/system_sam3x.o \
./system/src/CMSIS/vectors_sam3x.o 

C_DEPS += \
./system/src/CMSIS/system_sam3x.d \
./system/src/CMSIS/vectors_sam3x.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/CMSIS/%.o: ../system/src/CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DOS_USE_TRACE_SEMIHOSTING_STDOUT -I"../include" -I"../system/include" -I"../system/include/CMSIS" -I"../system/include/sam3x" -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


