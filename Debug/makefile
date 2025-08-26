################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include TinyUSB/typec/subdir.mk
-include TinyUSB/portable/synopsys/dwc2/subdir.mk
-include TinyUSB/portable/analog/max3421/subdir.mk
-include TinyUSB/host/subdir.mk
-include TinyUSB/device/subdir.mk
-include TinyUSB/common/subdir.mk
-include TinyUSB/class/video/subdir.mk
-include TinyUSB/class/vendor/subdir.mk
-include TinyUSB/class/usbtmc/subdir.mk
-include TinyUSB/class/net/subdir.mk
-include TinyUSB/class/msc/subdir.mk
-include TinyUSB/class/midi/subdir.mk
-include TinyUSB/class/hid/subdir.mk
-include TinyUSB/class/dfu/subdir.mk
-include TinyUSB/class/cdc/subdir.mk
-include TinyUSB/class/bth/subdir.mk
-include TinyUSB/class/audio/subdir.mk
-include TinyUSB/subdir.mk
-include FATFS/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := H723_MAX3421
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
H723_MAX3421.elf \

MAP_FILES += \
H723_MAX3421.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
H723_MAX3421.list \

OBJCOPY_BIN += \
H723_MAX3421.bin \


# All Target
all: main-build

# Main-build Target
main-build: H723_MAX3421.elf secondary-outputs

# Tool invocations
H723_MAX3421.elf H723_MAX3421.map: $(OBJS) $(USER_OBJS) D:\3D\CubeIDE\H723_MAX3421\ FAT_R0.15A\STM32H723VGTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "H723_MAX3421.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\3D\CubeIDE\H723_MAX3421 FAT_R0.15A\STM32H723VGTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="H723_MAX3421.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

H723_MAX3421.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "H723_MAX3421.list"
	@echo 'Finished building: $@'
	@echo ' '

H723_MAX3421.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "H723_MAX3421.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) H723_MAX3421.bin H723_MAX3421.elf H723_MAX3421.list H723_MAX3421.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
