################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/common/tusb_fifo.c 

OBJS += \
./TinyUSB/common/tusb_fifo.o 

C_DEPS += \
./TinyUSB/common/tusb_fifo.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/common/%.o TinyUSB/common/%.su TinyUSB/common/%.cyclo: ../TinyUSB/common/%.c TinyUSB/common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-common

clean-TinyUSB-2f-common:
	-$(RM) ./TinyUSB/common/tusb_fifo.cyclo ./TinyUSB/common/tusb_fifo.d ./TinyUSB/common/tusb_fifo.o ./TinyUSB/common/tusb_fifo.su

.PHONY: clean-TinyUSB-2f-common

