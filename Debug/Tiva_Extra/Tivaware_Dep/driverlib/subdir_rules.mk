################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Tiva_Extra/Tivaware_Dep/driverlib/%.obj: ../Tiva_Extra/Tivaware_Dep/driverlib/%.s $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"/home/kd/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/kd/A4988_Driver_Tiva" --include_path="/home/kd/A4988_Driver_Tiva/Tiva_Extra/Tivaware_Dep" --include_path="/home/kd/A4988_Driver_Tiva/Tiva_Extra" --include_path="/home/kd/ti/ccsv8/tools/compiler/ti-cgt-arm_18.1.3.LTS/include" --define=ccs="ccs" --define=TARGET_IS_TM4C123_RB1 --define=PART_TM4C123GH6PM -g --c99 --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="Tiva_Extra/Tivaware_Dep/driverlib/$(basename $(<F)).d_raw" --obj_directory="Tiva_Extra/Tivaware_Dep/driverlib" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


