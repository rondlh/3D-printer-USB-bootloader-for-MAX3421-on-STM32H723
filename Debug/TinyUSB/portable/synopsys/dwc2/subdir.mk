################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TinyUSB/portable/synopsys/dwc2/dcd_dwc2.c \
../TinyUSB/portable/synopsys/dwc2/dwc2_common.c \
../TinyUSB/portable/synopsys/dwc2/hcd_dwc2.c 

OBJS += \
./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.o \
./TinyUSB/portable/synopsys/dwc2/dwc2_common.o \
./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.o 

C_DEPS += \
./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.d \
./TinyUSB/portable/synopsys/dwc2/dwc2_common.d \
./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.d 


# Each subdirectory must supply rules for building sources it contributes
TinyUSB/portable/synopsys/dwc2/%.o TinyUSB/portable/synopsys/dwc2/%.su TinyUSB/portable/synopsys/dwc2/%.cyclo: ../TinyUSB/portable/synopsys/dwc2/%.c TinyUSB/portable/synopsys/dwc2/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_PWR_LDO_SUPPLY -DUSE_HAL_DRIVER -DSTM32H723xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.15A/TinyUSB" -I"D:/3D/CubeIDE/H723_MAX3421 FAT_R0.15A/FATFS" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-TinyUSB-2f-portable-2f-synopsys-2f-dwc2

clean-TinyUSB-2f-portable-2f-synopsys-2f-dwc2:
	-$(RM) ./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.cyclo ./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.d ./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.o ./TinyUSB/portable/synopsys/dwc2/dcd_dwc2.su ./TinyUSB/portable/synopsys/dwc2/dwc2_common.cyclo ./TinyUSB/portable/synopsys/dwc2/dwc2_common.d ./TinyUSB/portable/synopsys/dwc2/dwc2_common.o ./TinyUSB/portable/synopsys/dwc2/dwc2_common.su ./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.cyclo ./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.d ./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.o ./TinyUSB/portable/synopsys/dwc2/hcd_dwc2.su

.PHONY: clean-TinyUSB-2f-portable-2f-synopsys-2f-dwc2

