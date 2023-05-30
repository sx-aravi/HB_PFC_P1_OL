/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file device.h
* @brief PFC control board DSP pin configuration
*
*
* @author Sarinx LLC
*******************************************************************************/

//
// Included Files
//
#include "driverlib.h"

#if (!defined(CPU1) && !defined(CPU2))
#error "You must define CPU1 or CPU2 in your project properties.  Otherwise, \
the offsets in your header files will be inaccurate."
#endif

#if (defined(CPU1) && defined(CPU2))
#error "You have defined both CPU1 and CPU2 in your project properties.  Only \
a single CPU should be defined."
#endif

//*****************************************************************************
//
// Defines for pin numbers and other GPIO configuration
//
//*****************************************************************************

//
// GPIO assignment for Relay Drive Command I/Os
//

#define DEVICE_GPIO_PIN_K2_FB           83U  // GPIO number for K2_FB
#define DEVICE_GPIO_PIN_K3_FB           80U  // GPIO number for K3_FB
#define DEVICE_GPIO_PIN_IO1             79U  // GPIO number for IO1
#define DEVICE_GPIO_PIN_IO2             77U  // GPIO number for IO2
#define DEVICE_GPIO_PIN_IO3             76U  // GPIO number for IO3
#define DEVICE_GPIO_PIN_IO4             75U  // GPIO number for IO4
#define DEVICE_GPIO_PIN_K1_CTRL_DSP     68U  // GPIO number for K1_CONTROL_DSP
#define DEVICE_GPIO_PIN_K2_CTRL_DSP     69U  // GPIO number for K2_CONTROL_DSP
#define DEVICE_GPIO_PIN_K3_CTRL_DSP     74U  // GPIO number for K3_CONTROL_DSP
#define DEVICE_GPIO_PIN_ACT_DSB_DSP     47U  // GPIO number for ACT_DSB_DSP

#define DEVICE_GPIO_CFG_K2_FB           GPIO_83_GPIO83  // "pinConfig" for K2_FB
#define DEVICE_GPIO_CFG_K3_FB           GPIO_80_GPIO80  // "pinConfig" for K3_FB
#define DEVICE_GPIO_CFG_IO1             GPIO_79_GPIO79  // "pinConfig" for IO1
#define DEVICE_GPIO_CFG_IO2             GPIO_77_GPIO77  // "pinConfig" for IO2
#define DEVICE_GPIO_CFG_IO3             GPIO_76_GPIO76  // "pinConfig" for IO3
#define DEVICE_GPIO_CFG_IO4             GPIO_75_GPIO75  // "pinConfig" for IO4
#define DEVICE_GPIO_CFG_K1_CTRL_DSP     GPIO_68_GPIO68  // GPIO number for K1_CONTROL_DSP
#define DEVICE_GPIO_CFG_K2_CTRL_DSP     GPIO_69_GPIO69  // GPIO number for K2_CONTROL_DSP
#define DEVICE_GPIO_CFG_K3_CTRL_DSP     GPIO_74_GPIO74  // GPIO number for K3_CONTROL_DSP
#define DEVICE_GPIO_CFG_ACT_DSB_DSP     GPIO_47_GPIO47  // GPIO number for ACT_DSB_DSP


//
// GPIO assignment for SCI-A
//

#define DEVICE_GPIO_PIN_SCIRXDA         43U             // GPIO number for SCI RX
#define DEVICE_GPIO_PIN_SCITXDA         42U             // GPIO number for SCI TX
#define DEVICE_GPIO_CFG_SCIRXDA         GPIO_43_SCIRXDA // "pinConfig" for SCI RX
#define DEVICE_GPIO_CFG_SCITXDA         GPIO_42_SCITXDA // "pinConfig" for SCI TX


//
// GPIO assignment for CAN-A and CAN-B
//

#define DEVICE_GPIO_CFG_CANRXA          GPIO_70_CANRXA  // "pinConfig" for CANA RX
#define DEVICE_GPIO_CFG_CANTXA          GPIO_71_CANTXA  // "pinConfig" for CANA TX
#define DEVICE_GPIO_CFG_CANRXB          GPIO_39_CANRXB  // "pinConfig" for CANB RX
#define DEVICE_GPIO_CFG_CANTXB          GPIO_38_CANTXB   // "pinConfig" for CANB TX

//
// GPIO assignment for SPI-B --> EEPROM Port
//

#define DEVICE_GPIO_PIN_SPIB_STE        27U
#define DEVICE_GPIO_PIN_SPIB_CLK        26U
#define DEVICE_GPIO_PIN_SPIB_SIMO       24U
#define DEVICE_GPIO_PIN_SPIB_SOMI       25U
#define DEVICE_GPIO_CFG_SPIB_STE        GPIO_27_SPISTEB
#define DEVICE_GPIO_CFG_SPIB_CLK        GPIO_26_SPICLKB
#define DEVICE_GPIO_CFG_SPIB_SIMO       GPIO_24_SPISIMOB
#define DEVICE_GPIO_CFG_SPIB_SOMI       GPIO_25_SPISOMIB

//
// GPIO assignment for SPI-C --> DAC Port
//

#define DEVICE_GPIO_PIN_SPIC_STE        53U
#define DEVICE_GPIO_PIN_SPIC_CLK        52U
#define DEVICE_GPIO_PIN_SPIC_SIMO       50U
#define DEVICE_GPIO_PIN_SPIC_SOMI       51U
#define DEVICE_GPIO_CFG_SPIC_STE        GPIO_53_SPISTEC
#define DEVICE_GPIO_CFG_SPIC_CLK        GPIO_52_SPICLKC
#define DEVICE_GPIO_CFG_SPIC_SIMO       GPIO_50_SPISIMOC
#define DEVICE_GPIO_CFG_SPIC_SOMI       GPIO_51_SPISOMIC

//
// GPIO assignment for SCI-A --> RS232 Port
//

#define DEVICE_GPIO_PIN_SCIA_RX         43U
#define DEVICE_GPIO_PIN_SCIA_TX         42U
#define DEVICE_GPIO_CFG_SCIA_RX         GPIO_43_SCIRXDA
#define DEVICE_GPIO_CFG_SCIA_TX         GPIO_42_SCITXDA

//
// GPIO assignment for PWM
//

#define DEVICE_GPIO_PIN_EPWM1A          23U
#define DEVICE_GPIO_PIN_EPWM1B          22U
#define DEVICE_GPIO_PIN_EPWM2A          21U
#define DEVICE_GPIO_PIN_EPWM2B          20U
#define DEVICE_GPIO_PIN_EPWM3A          19U
#define DEVICE_GPIO_PIN_EPWM3B          18U
#define DEVICE_GPIO_PIN_EPWM4A          17U
#define DEVICE_GPIO_PIN_EPWM4B          16U
#define DEVICE_GPIO_PIN_EPWM5A          15U
#define DEVICE_GPIO_PIN_EPWM5B          14U
#define DEVICE_GPIO_PIN_EPWM6A          13U
#define DEVICE_GPIO_PIN_EPWM6B          12U

#define DEVICE_GPIO_CFG_EPWM1A          GPIO_23_EPWM12B
#define DEVICE_GPIO_CFG_EPWM1B          GPIO_22_EPWM12A
#define DEVICE_GPIO_CFG_EPWM2A          GPIO_21_EPWM11B
#define DEVICE_GPIO_CFG_EPWM2B          GPIO_20_EPWM11A
#define DEVICE_GPIO_CFG_EPWM3A          GPIO_19_EPWM10B
#define DEVICE_GPIO_CFG_EPWM3B          GPIO_18_EPWM10A
#define DEVICE_GPIO_CFG_EPWM4A          GPIO_17_EPWM9B
#define DEVICE_GPIO_CFG_EPWM4B          GPIO_16_EPWM9A
#define DEVICE_GPIO_CFG_EPWM5A          GPIO_15_EPWM8B
#define DEVICE_GPIO_CFG_EPWM5B          GPIO_14_EPWM8A
#define DEVICE_GPIO_CFG_EPWM6A          GPIO_13_EPWM7B
#define DEVICE_GPIO_CFG_EPWM6B          GPIO_12_EPWM7A


//
// GPIO assignment for Boot
//

#define DEVICE_GPIO_PIN_BOOT1           72U
#define DEVICE_GPIO_PIN_BOOT0           84U
#define DEVICE_GPIO_CFG_BOOT1           GPIO_72_GPIO72
#define DEVICE_GPIO_CFG_BOOT0           GPIO_84_GPIO84


//
// GPIO assignment for RS485
//

#define DEVICE_GPIO_PIN_CTL485          67U
#define DEVICE_GPIO_CFG_CTL485          GPIO_67_GPIO67



//
// GPIO assignment for I/Os and Faults
//

#define DEVICE_GPIO_PIN_IO41            41U
#define DEVICE_GPIO_PIN_IO48            48U
#define DEVICE_GPIO_PIN_FLT_RST         99U
#define DEVICE_GPIO_PIN_VPHASE_FLT      63U
#define DEVICE_GPIO_PIN_I_FLT           54U
#define DEVICE_GPIO_PIN_FLT_1N          28U

#define DEVICE_GPIO_CFG_IO41            GPIO_41_GPIO41
#define DEVICE_GPIO_CFG_IO48            GPIO_48_GPIO48
#define DEVICE_GPIO_CFG_PIN_FLT_RST     GPIO_99_GPIO99
#define DEVICE_GPIO_CFG_VPHASE_FLT      GPIO_63_GPIO63
#define DEVICE_GPIO_CFG_I_FLT           GPIO_54_GPIO54
#define DEVICE_GPIO_CFG_FLT_1N          GPIO_28_GPIO28




//
//*****************************************************************************
//
// Defines related to clock configuration
//
//*****************************************************************************
//

//
// 25MHz XTAL on PFC Control Board. For use with SysCtl_getClock().
//
#define DEVICE_OSCSRC_FREQ          25000000U

//
// Define to pass to SysCtl_setClock(). Will configure the clock as follows:
// PLLSYSCLK = 25MHz (XTAL_OSC) * 16 (IMULT) * 1 (FMULT) / 2 (PLLCLK_BY_2)
//
#define DEVICE_SETCLOCK_CFG         (SYSCTL_OSCSRC_XTAL | SYSCTL_IMULT(16) |  \
                                     SYSCTL_FMULT_NONE | SYSCTL_SYSDIV(2) |   \
                                     SYSCTL_PLL_ENABLE)

//
// 200MHz SYSCLK frequency based on the above DEVICE_SETCLOCK_CFG. Update the
// code below if a different clock configuration is used!
//
#define DEVICE_SYSCLK_FREQ          ((DEVICE_OSCSRC_FREQ * 16 * 1) / 2)



//
// 50MHz LSPCLK frequency based on the above DEVICE_SYSCLK_FREQ and a default
// low speed peripheral clock divider of 4. Update the code below if a
// different LSPCLK divider is used!
//
#define DEVICE_LSPCLK_FREQ          (DEVICE_SYSCLK_FREQ / 4)

//*****************************************************************************
//
// Macro to call SysCtl_delay() to achieve a delay in microseconds. The macro
// will convert the desired delay in microseconds to the count value expected
// by the function. \b x is the number of microseconds to delay.
//
//*****************************************************************************
#define DEVICE_DELAY_US(x) SysCtl_delay(((((long double)(x)) / (1000000.0L /  \
                              (long double)DEVICE_SYSCLK_FREQ)) - 9.0L) / 5.0L)

//
// The macros that can be used as parameter to the function Device_bootCPU2
//
#define C1C2_BROM_BOOTMODE_BOOT_FROM_PARALLEL                        0x00000000U
#define C1C2_BROM_BOOTMODE_BOOT_FROM_SCI                             0x00000001U
#define C1C2_BROM_BOOTMODE_BOOT_FROM_SPI                             0x00000004U
#define C1C2_BROM_BOOTMODE_BOOT_FROM_I2C                             0x00000005U
#define C1C2_BROM_BOOTMODE_BOOT_FROM_CAN                             0x00000007U
#define C1C2_BROM_BOOTMODE_BOOT_FROM_RAM                             0x0000000AU
#define C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH                           0x0000000BU

//
// Other macros that are needed for the Device_bootCPU2 function
//
#define BROM_IPC_EXECUTE_BOOTMODE_CMD                                0x00000013U
#define C1C2_BROM_BOOTMODE_BOOT_COMMAND_MAX_SUPPORT_VALUE            0x0000000CU
#define C2_BOOTROM_BOOTSTS_C2TOC1_IGNORE                             0x00000000U
#define C2_BOOTROM_BOOTSTS_SYSTEM_START_BOOT                         0x00000001U
#define C2_BOOTROM_BOOTSTS_SYSTEM_READY                              0x00000002U
#define C2_BOOTROM_BOOTSTS_C2TOC1_BOOT_CMD_ACK                       0x00000003U
#define C2_BOOTROM_BOOTSTS_C2TOC1_BOOT_CMD_NAK_STATUS_NOT_SUPPORTED  0x00000004U
#define C2_BOOTROM_BOOTSTS_C2TOC1_BOOT_CMD_NAK_STATUS_BUSY_WITH_BOOT 0x00000005U

//
// Macros used as return value by the Device_bootCPU2 function
//
#define STATUS_FAIL                 0x0001
#define STATUS_PASS                 0x0000

//*****************************************************************************
//
// Defines, Globals, and Header Includes related to Flash Support
//
//*****************************************************************************
#ifdef _FLASH
#include <stddef.h>

extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadEnd;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
extern uint16_t RamfuncsRunEnd;
extern uint16_t RamfuncsRunSize;

#define DEVICE_FLASH_WAITSTATES 3

#endif

extern uint32_t Example_PassCount;
extern uint32_t Example_Fail;

//*****************************************************************************
//
// Function Prototypes
//
//*****************************************************************************
//*****************************************************************************
//
//! \addtogroup device_api
//! @{
//
//*****************************************************************************
//*****************************************************************************
//
//! @brief Function to initialize the device. Primarily initializes system control to a
//! known state by disabling the watchdog, setting up the SYSCLKOUT frequency,
//! and enabling the clocks to the peripherals.
//!
//! \param None.
//! \return None.
//
//*****************************************************************************
extern void Device_init(void);
//*****************************************************************************
//!
//!
//! @brief Function to turn on all peripherals, enabling reads and writes to the
//! peripherals' registers.
//!
//! Note that to reduce power, unused peripherals should be disabled.
//!
//! @param None
//! @return None
//
//*****************************************************************************
extern void Device_enableAllPeripherals(void);
//*****************************************************************************
//!
//!
//! @brief Function to disable pin locks on GPIOs.
//!
//! @param None
//! @return None
//
//*****************************************************************************
extern void Device_initGPIO(void);
//*****************************************************************************
//!
//! @brief Function to enable pullups for the unbonded GPIOs on the 176PTP package:
//! GPIOs     Grp Bits
//! 95-132    C   31
//!           D   31:0
//!           E   4:0
//! 134-168   E   31:6
//!           F   8:0
//!
//! @param None
//! @return None
//
//*****************************************************************************
extern void Device_enableUnbondedGPIOPullupsFor176Pin(void);
//*****************************************************************************
//!
//! @brief Function to enable pullups for the unbonded GPIOs on the 100PZ package:
//! GPIOs     Grp Bits
//! 0-1       A   1:0
//! 5-9       A   9:5
//! 22-40     A   31:22
//!           B   8:0
//! 44-57     B   25:12
//! 67-68     C   4:3
//! 74-77     C   13:10
//! 79-83     C   19:15
//! 93-168    C   31:29
//!           D   31:0
//!           E   31:0
//!           F   8:0
//! @param None
//! @return None
//
//
//*****************************************************************************
extern void Device_enableUnbondedGPIOPullupsFor100Pin(void);
//*****************************************************************************
//!
//! @brief Function to enable pullups for the unbonded GPIOs on the
//! 176PTP package.
//!
//! @param None
//! @return None
//
//*****************************************************************************
extern void Device_enableUnbondedGPIOPullups(void);
#ifdef CPU1
//*****************************************************************************
//!
//! @brief Function to implement Analog trim of TMX devices
//!
//! @param None
//! @return None
//
//*****************************************************************************
extern void Device_configureTMXAnalogTrim(void);
//*****************************************************************************
//! @brief Executes a CPU02 control system bootloader.
//!
//! \param bootMode specifies which CPU02 control system boot mode to execute.
//!
//! This function will allow the CPU01 master system to boot the CPU02 control
//! system via the following modes: Boot to RAM, Boot to Flash, Boot via SPI,
//! SCI, I2C, or parallel I/O. This function blocks and waits until the
//! control system boot ROM is configured and ready to receive CPU01 to CPU02
//! IPC INT0 interrupts. It then blocks and waits until IPC INT0 and
//! IPC FLAG31 are available in the CPU02 boot ROM prior to sending the
//! command to execute the selected bootloader.
//!
//! The \e bootMode parameter accepts one of the following values:
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_PARALLEL
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_SCI
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_SPI
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_I2C
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_CAN
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_RAM
//!  - \b C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH
//!
//! \return 0 (success) if command is sent, or 1 (failure) if boot mode is
//! invalid and command was not sent.
//
//*****************************************************************************
extern uint16_t Device_bootCPU2(uint32_t ulBootMode);
#endif
//*****************************************************************************
//!
//! @brief Error handling function to be called when an ASSERT is violated
//!
//! @param *filename File name in which the error has occurred
//! @param line Line number within the file
//! @return None
//
//*****************************************************************************
extern void __error__(const char *filename, uint32_t line);


//
// End of file
//
