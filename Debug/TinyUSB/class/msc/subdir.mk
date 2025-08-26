################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/class/msc/msc_device.c \
../TinyUSB/class/msc/msc_host.c 

OBJS += \
./TinyUSB/class/msc/msc_device.o \
./TinyUSB/class/msc/msc_host.o 

C_DEPS += \
./TinyUSB/class/msc/msc_device.d \
./TinyUSB/class/msc/msc_host.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/class/msc/%.o TinyUSB/class/msc/%.su TinyUSB/class/msc/%.cyclo: ../TinyUSB/class/msc/%.c TinyUSB/class/msc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-class-2f-msc

clean-TinyUSB-2f-class-2f-msc:
	-$(RM) ./TinyUSB/class/msc/msc_device.cyclo ./TinyUSB/class/msc/msc_device.d ./TinyUSB/class/msc/msc_device.o ./TinyUSB/class/msc/msc_device.su ./TinyUSB/class/msc/msc_host.cyclo ./TinyUSB/class/msc/msc_host.d ./TinyUSB/class/msc/msc_host.o ./TinyUSB/class/msc/msc_host.su

.PHONY: clean-TinyUSB-2f-class-2f-msc

