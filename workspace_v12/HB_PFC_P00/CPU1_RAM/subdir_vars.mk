################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../2837xD_RAM_CLA_lnk_cpu1.cmd 

SYSCFG_SRCS += \
../cla_pfc.syscfg 

CLA_SRCS += \
../pfc.cla 

LIB_SRCS += \
C:/ti/C2000Ware_4_02_00_00/driverlib/f2837xd/driverlib/ccs/Debug/driverlib.lib 

C_SRCS += \
./syscfg/board.c \
./syscfg/c2000ware_libraries.c \
../pfc_main.c 

GEN_FILES += \
./syscfg/board.c \
./syscfg/c2000ware_libraries.opt \
./syscfg/c2000ware_libraries.c 

CLA_DEPS += \
./pfc.d 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/board.d \
./syscfg/c2000ware_libraries.d \
./pfc_main.d 

GEN_OPTS += \
./syscfg/c2000ware_libraries.opt 

OBJS += \
./syscfg/board.obj \
./syscfg/c2000ware_libraries.obj \
./pfc.obj \
./pfc_main.obj 

GEN_MISC_FILES += \
./syscfg/board.h \
./syscfg/pinmux.csv \
./syscfg/c2000ware_libraries.cmd.genlibs \
./syscfg/c2000ware_libraries.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\board.obj" \
"syscfg\c2000ware_libraries.obj" \
"pfc.obj" \
"pfc_main.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\board.h" \
"syscfg\pinmux.csv" \
"syscfg\c2000ware_libraries.cmd.genlibs" \
"syscfg\c2000ware_libraries.h" 

C_DEPS__QUOTED += \
"syscfg\board.d" \
"syscfg\c2000ware_libraries.d" \
"pfc_main.d" 

GEN_FILES__QUOTED += \
"syscfg\board.c" \
"syscfg\c2000ware_libraries.opt" \
"syscfg\c2000ware_libraries.c" 

CLA_DEPS__QUOTED += \
"pfc.d" 

SYSCFG_SRCS__QUOTED += \
"../cla_pfc.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/board.c" \
"./syscfg/c2000ware_libraries.c" \
"../pfc_main.c" 


