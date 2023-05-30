/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_GPIO.c
* @brief GPIO devic driver interface functions
*
* This module is wrapper for GPIO TI BItmap registers library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/
#include "ddi_GPIO.h"
#include "gpio.h"


//*****************************************************************************
//
// GPIO PinMux Configurations
//
//! This function configures the pin mux that selects the peripheral function
//! associated with a particular GPIO pin.  Only one peripheral function at a
//! time can be associated with a GPIO pin, and each peripheral function should
//! only be associated with a single GPIO pin at a time (despite the fact that
//! many of them can be associated with more than one GPIO pin).
//
//*****************************************************************************

void ddi_ConfigureGPIOs()
{

    //InitGpio();

    /*ddi_InitEPwm7Gpio();
    ddi_InitEPwm8Gpio();
    ddi_InitEPwm9Gpio();
    ddi_InitEPwm10Gpio();
    ddi_InitEPwm11Gpio();
    ddi_InitEPwm12Gpio();*/

    InitializePinMux(GPIO_23_EPWM12B,23,GPIO_22_EPWM12A,22);
    InitializePinMux(GPIO_21_EPWM11B,21,GPIO_20_EPWM11A,20);
    InitializePinMux(GPIO_19_EPWM10B,19,GPIO_18_EPWM10A,18);
    InitializePinMux(GPIO_17_EPWM9B,17,GPIO_16_EPWM9A,16);
    InitializePinMux(GPIO_15_EPWM8B,15,GPIO_14_EPWM8A,14);
    InitializePinMux(GPIO_13_EPWM7B,13,GPIO_12_EPWM7A,12);



    // GPIO 73 configuration for testing/debugging the CPU clock outputs
    /*GPIO_setPinConfig(GPIO_73_XCLKOUT);
    GPIO_setPadConfig(73, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(73, GPIO_QUAL_SYNC);
    GPIO_setDirectionMode(73, GPIO_DIR_MODE_OUT);
    GPIO_setControllerCore(73, GPIO_CORE_CPU1);
    SysCtl_selectClockOutSource(SYSCTL_CLOCKOUT_SYSCLK);*/

    //
    // GPIO 37 configuration for various execution timings check
    //

    GPIO_setPinConfig(GPIO_37_GPIO37);
    GPIO_setPadConfig(37, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(37, GPIO_QUAL_SYNC);
    GPIO_setDirectionMode(37, GPIO_DIR_MODE_OUT);
    GPIO_setControllerCore(37, GPIO_CORE_CPU1);

}



//*****************************************************************************
//
// GPIO PinMux Configurations
//
//*****************************************************************************
void InitializePinMux(uint32_t pinConfigA, uint32_t pinA, uint32_t pinConfigB, uint32_t pinB)
{
    //
    // PinMux for modules assigned to CPU1
    //

    GPIO_setPinConfig(pinConfigA);
    GPIO_setPadConfig(pinA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(pinA, GPIO_QUAL_SYNC);

    GPIO_setPinConfig(pinConfigB);
    GPIO_setPadConfig(pinB, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(pinB, GPIO_QUAL_SYNC);
}



//
// ddi_InitEPwm7Gpio - Initialize EPWM7 GPIOs
//
void ddi_InitEPwm7Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;    // Disable pull-up on GPIO12 (EPWM7A)
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 1;    // Disable pull-up on GPIO13 (EPWM7B)

    //
    // Configure EPWM-7 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM7 functional pins.
    //

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 1;   // Configure GPIO12 as EPWM7A
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 1;   // Configure GPIO13 as EPWM7B

    EDIS;
}




//
// ddi_InitEPwm8Gpio - Initialize EPWM8 GPIOs
//
void ddi_InitEPwm8Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 1;    // Disable pull-up on GPIO14 (EPWM8A)
    GpioCtrlRegs.GPAPUD.bit.GPIO15 = 1;    // Disable pull-up on GPIO15 (EPWM8B)

    //
    // Configure EPWM-8 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM8 functional pins.
    //

    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 1;   // Configure GPIO14 as EPWM8A
    GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 1;   // Configure GPIO15 as EPWM8B

    EDIS;
}



//
// ddi_InitEPwm9Gpio - Initialize EPWM9 GPIOs
//
void ddi_InitEPwm9Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 1;    // Disable pull-up on GPIO16 (EPWM9A)
    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 1;    // Disable pull-up on GPIO17 (EPWM9B)

    //
    // Configure EPWM-9 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM9 functional pins.
    //

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;   // Configure GPIO16 as EPWM9A
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;   // Configure GPIO17 as EPWM9B

    EDIS;
}



//
// ddi_InitEPwm10Gpio - Initialize EPWM10 GPIOs
//
void ddi_InitEPwm10Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 1;    // Disable pull-up on GPIO18 (EPWM10A)
    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 1;    // Disable pull-up on GPIO19 (EPWM10B)

    //
    // Configure EPWM-10 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM10 functional pins.
    //

    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;   // Configure GPIO18 as EPWM10A
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;   // Configure GPIO19 as EPWM10B

    EDIS;
}



//
// ddi_InitEPwm11Gpio - Initialize EPWM11 GPIOs
//
void ddi_InitEPwm11Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO20 = 1;    // Disable pull-up on GPIO20 (EPWM11A)
    GpioCtrlRegs.GPAPUD.bit.GPIO21 = 1;    // Disable pull-up on GPIO21 (EPWM11B)

    //
    // Configure EPWM-11 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM11 functional pins.
    //

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 1;   // Configure GPIO20 as EPWM11A
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 1;   // Configure GPIO21 as EPWM11B

    EDIS;
}


//
// InitEPwm12Gpio - Initialize EPWM12 GPIOs
//
void ddi_InitEPwm12Gpio()
{
    EALLOW;
    //
    // Disable internal pull-up for the selected output pins for reduced power consumption
    // Pull-ups can be enabled or disabled by the user.
    // This will enable the pullups for the specified pins.
    //
    GpioCtrlRegs.GPAPUD.bit.GPIO22 = 1;    // Disable pull-up on GPIO22 (EPWM12A)
    GpioCtrlRegs.GPAPUD.bit.GPIO23 = 1;    // Disable pull-up on GPIO23 (EPWM12B)

    //
    // Configure EPWM-12 pins using GPIO regs
    // This specifies which of the possible GPIO pins will be EPWM12 functional pins.
    //

    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 1;   // Configure GPIO22 as EPWM12A
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 1;   // Configure GPIO23 as EPWM12B

    EDIS;
}


//
// End of file
//


