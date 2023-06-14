//#############################################################################
//
// FILE:   empty_bitfield_driverlib_main.c
//
// TITLE:  Empty Example
//
// Empty Bit-Field & Driverlib Example
//
// This example is an empty project setup for Bit-Field and Driverlib 
// development.
//
//#############################################################################
//
// $Release Date: $
// $Copyright:
// Copyright (C) 2013-2023 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Included Files
//
#include <cdu_MsgProcessor.h>
#include "F28x_Project.h"
#include "driverlib.h"
#include "device.h"
#include "cam_SVM.h"
#include "siu_PfcBoard.h"
#include "pdu_PwmUpdate.h"
#include "ddi_CAN.h"
#include "pmu_ProfilerData.h"



//
// EPWM12 10 microseconds interrupt service routine
//
__interrupt void epwm12ISR(void);


uint16_t ADCINA5_Val =0;
uint16_t _100usecTimer =0;

//
// Main
//
void main(void)

{


    //
    // Initialize device clock and peripherals
    //

    Device_init();

    //
    // Disable pin locks and enable internal pull ups.
    //
    Device_initGPIO();

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // Assign the interrupt service routines to ePWM interrupts
    //
    Interrupt_register(INT_EPWM12, &epwm12ISR);

    //
    // Configure all GPIOs for PWM, SPI, CAN, ADC, and discrete I/Os
    //

    siu_ConfigurePfcBoard();

    //
    // Disable sync(Freeze clock to PWM as well)
    //

    SysCtl_disablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    ddi_InitializeCAN();
    pdpu_InitializePWM();
    ddi_InitializeSpi();
    ddi_InitializeADCs();

    //
    // Enable sync and clock to PWM
    //

    SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_TBCLKSYNC);

    //
    // Enable ePWM interrupts
    //
    Interrupt_enable(INT_EPWM12);


    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;


    //
    // Start CAN module B operations
    //
    CAN_startModule(CANB_BASE);

    //ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
    //AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1; //SOC0

    for(;;)
    {
        cdu_ProcessDiagnoticMsgs();
        pmu_SynchronizeProfilerObjects();

        //
        // Convert, wait for completion, and store results
        //

        //
        // Convert, wait for completion, and store results
        //
        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);

        if (_100usecTimer == 10)
        {
            _100usecTimer = 0;

            if (ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == true)
            {
                //AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                //ADCINA5_Val = AdcaResultRegs.ADCRESULT5;
                ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
                ADCINA5_Val  = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
            }
        }
    }

}



//
// EPWM12-ISR
//
__interrupt void epwm12ISR(void)
{

    //GPIO_togglePin(37);
    _100usecTimer += 1;

    // Call Space vector Modulation process every 10 microseconds

    GPIO_writePin(37,1);

    svm();

    pdpu_UpdateCompareReg(SpaceVectorTransitionTime);

    GPIO_writePin(37,0);

    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(EPWM12_BASE);

    //
    // Acknowledge interrupt group
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);


}



//
// End of File
//
