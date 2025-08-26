################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/device/usbd.c \
../TinyUSB/device/usbd_control.c 

OBJS += \
./TinyUSB/device/usbd.o \
./TinyUSB/device/usbd_control.o 

C_DEPS += \
./TinyUSB/device/usbd.d \
./TinyUSB/device/usbd_control.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/device/%.o TinyUSB/device/%.su TinyUSB/device/%.cyclo: ../TinyUSB/device/%.c TinyUSB/device/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-device

clean-TinyUSB-2f-device:
	-$(RM) ./TinyUSB/device/usbd.cyclo ./TinyUSB/device/usbd.d ./TinyUSB/device/usbd.o ./TinyUSB/device/usbd.su ./TinyUSB/device/usbd_control.cyclo ./TinyUSB/device/usbd_control.d ./TinyUSB/device/usbd_control.o ./TinyUSB/device/usbd_control.su

.PHONY: clean-TinyUSB-2f-device

