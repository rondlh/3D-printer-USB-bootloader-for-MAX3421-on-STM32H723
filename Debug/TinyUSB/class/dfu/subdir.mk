################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/class/dfu/dfu_device.c \
../TinyUSB/class/dfu/dfu_rt_device.c 

OBJS += \
./TinyUSB/class/dfu/dfu_device.o \
./TinyUSB/class/dfu/dfu_rt_device.o 

C_DEPS += \
./TinyUSB/class/dfu/dfu_device.d \
./TinyUSB/class/dfu/dfu_rt_device.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/class/dfu/%.o TinyUSB/class/dfu/%.su TinyUSB/class/dfu/%.cyclo: ../TinyUSB/class/dfu/%.c TinyUSB/class/dfu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-class-2f-dfu

clean-TinyUSB-2f-class-2f-dfu:
	-$(RM) ./TinyUSB/class/dfu/dfu_device.cyclo ./TinyUSB/class/dfu/dfu_device.d ./TinyUSB/class/dfu/dfu_device.o ./TinyUSB/class/dfu/dfu_device.su ./TinyUSB/class/dfu/dfu_rt_device.cyclo ./TinyUSB/class/dfu/dfu_rt_device.d ./TinyUSB/class/dfu/dfu_rt_device.o ./TinyUSB/class/dfu/dfu_rt_device.su

.PHONY: clean-TinyUSB-2f-class-2f-dfu

