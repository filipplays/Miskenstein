################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../System/LED.c \
../System/SCI.c \
../System/XPT2046_touch.c \
../System/buf.c \
../System/joystick.c \
../System/kbd.c \
../System/lcd.c \
../System/lcd_backlight.c \
../System/lcd_ili9341.c \
../System/periodic_services.c \
../System/podatki.c \
../System/pomozne_funkcije.c \
../System/ray_funkcije.c \
../System/timing_utils.c \
../System/ugui.c 

OBJS += \
./System/LED.o \
./System/SCI.o \
./System/XPT2046_touch.o \
./System/buf.o \
./System/joystick.o \
./System/kbd.o \
./System/lcd.o \
./System/lcd_backlight.o \
./System/lcd_ili9341.o \
./System/periodic_services.o \
./System/podatki.o \
./System/pomozne_funkcije.o \
./System/ray_funkcije.o \
./System/timing_utils.o \
./System/ugui.o 

C_DEPS += \
./System/LED.d \
./System/SCI.d \
./System/XPT2046_touch.d \
./System/buf.d \
./System/joystick.d \
./System/kbd.d \
./System/lcd.d \
./System/lcd_backlight.d \
./System/lcd_ili9341.d \
./System/periodic_services.d \
./System/podatki.d \
./System/pomozne_funkcije.d \
./System/ray_funkcije.d \
./System/timing_utils.d \
./System/ugui.d 


# Each subdirectory must supply rules for building sources it contributes
System/%.o: ../System/%.c System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/zevni/Desktop/PROG/FAKS/tekmovanje_isystem/OME_PROJEKT_Filip_Zevnik/OME_projekt_Filip_Zevnik/System/Include" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-System

clean-System:
	-$(RM) ./System/LED.d ./System/LED.o ./System/SCI.d ./System/SCI.o ./System/XPT2046_touch.d ./System/XPT2046_touch.o ./System/buf.d ./System/buf.o ./System/joystick.d ./System/joystick.o ./System/kbd.d ./System/kbd.o ./System/lcd.d ./System/lcd.o ./System/lcd_backlight.d ./System/lcd_backlight.o ./System/lcd_ili9341.d ./System/lcd_ili9341.o ./System/periodic_services.d ./System/periodic_services.o ./System/podatki.d ./System/podatki.o ./System/pomozne_funkcije.d ./System/pomozne_funkcije.o ./System/ray_funkcije.d ./System/ray_funkcije.o ./System/timing_utils.d ./System/timing_utils.o ./System/ugui.d ./System/ugui.o

.PHONY: clean-System

