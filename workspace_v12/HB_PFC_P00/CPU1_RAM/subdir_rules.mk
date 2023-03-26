################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1725386159: ../cla_pfc.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs1200/ccs/utils/sysconfig_1.13.0/sysconfig_cli.bat" -s "C:/ti/C2000Ware_4_02_00_00/.metadata/sdk.json" -d "F2837xD" --script "C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/cla_pfc.syscfg" -o "syscfg" --package F2837xD_176PTP --part F2837xD_176PTP --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1725386159 ../cla_pfc.syscfg
syscfg/board.h: build-1725386159
syscfg/pinmux.csv: build-1725386159
syscfg/c2000ware_libraries.cmd.genlibs: build-1725386159
syscfg/c2000ware_libraries.opt: build-1725386159
syscfg/c2000ware_libraries.c: build-1725386159
syscfg/c2000ware_libraries.h: build-1725386159
syscfg/: build-1725386159

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00" --include_path="C:/ti/C2000Ware_4_02_00_00" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/device" --include_path="C:/ti/C2000Ware_4_02_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_suppress=10068 --diag_suppress=770 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.cla $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00" --include_path="C:/ti/C2000Ware_4_02_00_00" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/device" --include_path="C:/ti/C2000Ware_4_02_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_suppress=10068 --diag_suppress=770 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla1 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu2 -Ooff --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00" --include_path="C:/ti/C2000Ware_4_02_00_00" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/device" --include_path="C:/ti/C2000Ware_4_02_00_00/driverlib/f2837xd/driverlib" --include_path="C:/ti/ccs1200/ccs/tools/compiler/ti-cgt-c2000_22.6.0.LTS/include" --define=DEBUG --define=CPU1 --diag_suppress=10063 --diag_suppress=10068 --diag_suppress=770 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Anil Ravi/workspace_v12/HB_PFC_P00/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


