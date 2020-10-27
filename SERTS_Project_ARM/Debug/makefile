################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include RTE/USB/subdir.mk
-include RTE/File_System/subdir.mk
-include RTE/Device/STM32F407VGTx/subdir.mk
-include RTE/Compiler/subdir.mk
-include RTE/CMSIS_Driver/subdir.mk
-include RTE/CMSIS/subdir.mk
-include RTE/Board_Support/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SERTS_Project_ARM.axf

# Tool invocations
SERTS_Project_ARM.axf: $(OBJS) D:\Users\peter\Source\Repos\SERTS_Labs\SERTS_Project_ARM/STM32F407VGTx.sct $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: Arm Linker 5'
	armlink.exe --cpu=Cortex-M4.fp.sp --entry=Reset_Handler --scatter="D:\Users\peter\Source\Repos\SERTS_Labs\SERTS_Project_ARM/STM32F407VGTx.sct" --info=sizes --strict -o "SERTS_Project_ARM.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SX_DEPS) $(ASM_UPPER_DEPS) $(EXECUTABLES) $(OBJS) $(SX_UPPER_DEPS) $(ASM_DEPS) $(S_DEPS) $(S_UPPER_DEPS) $(C_DEPS) SERTS_Project_ARM.axf 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
