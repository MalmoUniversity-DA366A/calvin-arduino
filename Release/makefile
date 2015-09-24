################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/src/unity/subdir.mk
-include system/src/test/subdir.mk
-include system/src/sam3x/subdir.mk
-include system/src/newlib/subdir.mk
-include system/src/diag/subdir.mk
-include system/src/cortexm/subdir.mk
-include system/src/cores/subdir.mk
-include system/src/calvinlib/subdir.mk
-include system/src/CMSIS/subdir.mk
-include src/subdir.mk
-include libraries/SPI/subdir.mk
-include libraries/LiquidCrystal/src/subdir.mk
-include libraries/Ethernet/src/utility/subdir.mk
-include libraries/Ethernet/src/subdir.mk
-include libraries/Calvin/src/subdir.mk
-include libraries/ArduinoJson/src/Internals/subdir.mk
-include libraries/ArduinoJson/src/Arduino/subdir.mk
-include libraries/ArduinoJson/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Calvin.hex \

SECONDARY_SIZE += \
Calvin.siz \


# All Target
all: Calvin.elf secondary-outputs

# Tool invocations
Calvin.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"Calvin.map" --specs=nano.specs -o "Calvin.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Calvin.hex: Calvin.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "Calvin.elf"  "Calvin.hex"
	@echo 'Finished building: $@'
	@echo ' '

Calvin.siz: Calvin.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "Calvin.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) Calvin.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
