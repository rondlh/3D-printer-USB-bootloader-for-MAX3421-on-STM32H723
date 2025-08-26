################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/host/hub.c \
../TinyUSB/host/usbh.c 

OBJS += \
./TinyUSB/host/hub.o \
./TinyUSB/host/usbh.o 

C_DEPS += \
./TinyUSB/host/hub.d \
./TinyUSB/host/usbh.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/host/%.o TinyUSB/host/%.su TinyUSB/host/%.cyclo: ../TinyUSB/host/%.c TinyUSB/host/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-host

clean-TinyUSB-2f-host:
	-$(RM) ./TinyUSB/host/hub.cyclo ./TinyUSB/host/hub.d ./TinyUSB/host/hub.o ./TinyUSB/host/hub.su ./TinyUSB/host/usbh.cyclo ./TinyUSB/host/usbh.d ./TinyUSB/host/usbh.o ./TinyUSB/host/usbh.su

.PHONY: clean-TinyUSB-2f-host

