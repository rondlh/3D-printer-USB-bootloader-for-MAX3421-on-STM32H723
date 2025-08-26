################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyFATFS/ff.c \
../TinyFATFS/ffsystem.c \
../TinyFATFS/ffunicode.c 

OBJS += \
./TinyFATFS/ff.o \
./TinyFATFS/ffsystem.o \
./TinyFATFS/ffunicode.o 

C_DEPS += \
./TinyFATFS/ff.d \
./TinyFATFS/ffsystem.d \
./TinyFATFS/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
TinyFATFS/%.o TinyFATFS/%.su TinyFATFS/%.cyclo: ../TinyFATFS/%.c TinyFATFS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 TinyFAT FS/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 TinyFAT FS/TinyFATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyFATFS

clean-TinyFATFS:
	-$(RM) ./TinyFATFS/ff.cyclo ./TinyFATFS/ff.d ./TinyFATFS/ff.o ./TinyFATFS/ff.su ./TinyFATFS/ffsystem.cyclo ./TinyFATFS/ffsystem.d ./TinyFATFS/ffsystem.o ./TinyFATFS/ffsystem.su ./TinyFATFS/ffunicode.cyclo ./TinyFATFS/ffunicode.d ./TinyFATFS/ffunicode.o ./TinyFATFS/ffunicode.su

.PHONY: clean-TinyFATFS

