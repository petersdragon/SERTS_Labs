################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Thread.c \
../src/UART_driver.c \
../src/cs43l22.c \
../src/main.c \
../src/stm32f4_discovery.c \
../src/stm32f4_discovery_audio.c \
../src/stm32f4xx_it.c 

OBJS += \
./src/Thread.o \
./src/UART_driver.o \
./src/cs43l22.o \
./src/main.o \
./src/stm32f4_discovery.o \
./src/stm32f4_discovery_audio.o \
./src/stm32f4xx_it.o 

C_DEPS += \
./src/Thread.d \
./src/UART_driver.d \
./src/cs43l22.d \
./src/main.d \
./src/stm32f4_discovery.d \
./src/stm32f4_discovery_audio.d \
./src/stm32f4xx_it.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Arm C Compiler 5'
	armcc.exe --cpu=Cortex-M4.fp.sp --littleend -DHSE_VALUE="8000000" -DUSE_HAL_DRIVER -DSTM32F407xx -DUSE_STM32F4_DISCO -DARM_MATH_CM4 -D_RTE_ -DSTM32F407xx -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/Core/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/DSP/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/Driver/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/RTOS/RTX/INC" -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/RTOS/RTX/SRC/ARM" -I"C:/Users/peter/AppData/Local/Arm/Packs/ARM/CMSIS/5.7.0/CMSIS/RTOS/RTX/UserCodeTemplates" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.12.0/Board" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.12.0/FileSystem/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/MDK-Middleware/7.12.0/USB/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.15.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.15.0/Drivers/STM32F4xx_HAL_Driver/Inc" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.15.0/MDK/Device/Source/ARM" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.15.0/MDK/Templates/Inc" -I"C:/Users/peter/AppData/Local/Arm/Packs/Keil/STM32F4xx_DFP/2.15.0/MDK/Templates_LL/Inc" -I"C:\Users\peter\Development Studio Workspace\SERTS_Lab_11_ARM/RTE" -I"C:\Users\peter\Development Studio Workspace\SERTS_Lab_11_ARM/RTE/Device/STM32F407VGTx" -I"C:\Users\peter\Development Studio Workspace\SERTS_Lab_11_ARM/RTE/File_System" -I"C:\Users\peter\Development Studio Workspace\SERTS_Lab_11_ARM/RTE/USB" --c99 -O0 -g --split_sections --md --depend_format=unix_escaped --no_depend_system_headers --depend_dir="src" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


