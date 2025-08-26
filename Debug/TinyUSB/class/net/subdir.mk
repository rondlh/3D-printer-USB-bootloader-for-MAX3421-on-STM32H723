################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/class/net/ecm_rndis_device.c \
../TinyUSB/class/net/ncm_device.c 

OBJS += \
./TinyUSB/class/net/ecm_rndis_device.o \
./TinyUSB/class/net/ncm_device.o 

C_DEPS += \
./TinyUSB/class/net/ecm_rndis_device.d \
./TinyUSB/class/net/ncm_device.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/class/net/%.o TinyUSB/class/net/%.su TinyUSB/class/net/%.cyclo: ../TinyUSB/class/net/%.c TinyUSB/class/net/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-class-2f-net

clean-TinyUSB-2f-class-2f-net:
	-$(RM) ./TinyUSB/class/net/ecm_rndis_device.cyclo ./TinyUSB/class/net/ecm_rndis_device.d ./TinyUSB/class/net/ecm_rndis_device.o ./TinyUSB/class/net/ecm_rndis_device.su ./TinyUSB/class/net/ncm_device.cyclo ./TinyUSB/class/net/ncm_device.d ./TinyUSB/class/net/ncm_device.o ./TinyUSB/class/net/ncm_device.su

.PHONY: clean-TinyUSB-2f-class-2f-net

