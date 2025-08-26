################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/typec/usbc.c 

OBJS += \
./TinyUSB/typec/usbc.o 

C_DEPS += \
./TinyUSB/typec/usbc.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/typec/%.o TinyUSB/typec/%.su TinyUSB/typec/%.cyclo: ../TinyUSB/typec/%.c TinyUSB/typec/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.16/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-typec

clean-TinyUSB-2f-typec:
	-$(RM) ./TinyUSB/typec/usbc.cyclo ./TinyUSB/typec/usbc.d ./TinyUSB/typec/usbc.o ./TinyUSB/typec/usbc.su

.PHONY: clean-TinyUSB-2f-typec

