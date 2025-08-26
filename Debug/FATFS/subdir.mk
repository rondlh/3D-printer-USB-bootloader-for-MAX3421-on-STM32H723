################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/ff.c \
../FATFS/ffsystem.c \
../FATFS/ffunicode.c 

OBJS += \
./FATFS/ff.o \
./FATFS/ffsystem.o \
./FATFS/ffunicode.o 

C_DEPS += \
./FATFS/ff.d \
./FATFS/ffsystem.d \
./FATFS/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/%.o FATFS/%.su FATFS/%.cyclo: ../FATFS/%.c FATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.15A/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.15A/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-FATFS

clean-FATFS:
	-$(RM) ./FATFS/ff.cyclo ./FATFS/ff.d ./FATFS/ff.o ./FATFS/ff.su ./FATFS/ffsystem.cyclo ./FATFS/ffsystem.d ./FATFS/ffsystem.o ./FATFS/ffsystem.su ./FATFS/ffunicode.cyclo ./FATFS/ffunicode.d ./FATFS/ffunicode.o ./FATFS/ffunicode.su

.PHONY: clean-FATFS

