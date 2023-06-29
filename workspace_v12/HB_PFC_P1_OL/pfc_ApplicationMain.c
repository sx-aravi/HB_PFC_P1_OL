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
#include "dmu_CcsDataLog.h"

//
// EPWM12 10 microseconds interrupt service routine
//
__interrupt void epwm12ISR(void);


uint16_t ADCINA5_Val =0;


//
//OPen following code for RAM version code to allow debugging
/*float DBUFF_4CH1[200],
      DBUFF_4CH2[200],
      DBUFF_4CH3[200],
      DBUFF_4CH4[200],
      DlogCh1,
      DlogCh2,
      DlogCh3,
      DlogCh4;
//
// Create an instance of DATALOG Module
//
DLOG_4CH_F dlog_4ch1;

*/

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

    // Initialize DLOG
    //Disabled for FLASH version code; open for RAM version code
	/*DLOG_4CH_F_init(&dlog_4ch1);
	dlog_4ch1.input_ptr1 = &DlogCh1;
	dlog_4ch1.input_ptr2 = &DlogCh2;
	dlog_4ch1.input_ptr3 = &DlogCh3;
	dlog_4ch1.input_ptr4 = &DlogCh4;
	dlog_4ch1.output_ptr1 = &DBUFF_4CH1[0];
	dlog_4ch1.output_ptr2 = &DBUFF_4CH2[0];
	dlog_4ch1.output_ptr3 = &DBUFF_4CH3[0];
	dlog_4ch1.output_ptr4 = &DBUFF_4CH4[0];
	dlog_4ch1.size = 200;
	dlog_4ch1.pre_scalar = 5; //1
	dlog_4ch1.trig_value = 0.01;
	dlog_4ch1.status = 2;*/

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

    //Fault Reset, DSPEN default values during the system initialization.
    GPIO_writePin(32,0);
    GPIO_writePin(29,0);


    for(;;)
    {
        cdu_ProcessDiagnoticMsgs();
        pmu_SynchronizeProfilerObjects();

        //Read /vDCFT, /iDCFT, /TMPFT, /vPHFT and /iFLT faults from inverter hardware and disable the inverter
        /*if((GPIO_readPin(33) == 1) | (GPIO_readPin(34) == 1) | (GPIO_readPin(35) == 1) |(GPIO_readPin(63) == 1))
        {
            InverterState = OFF;
        }*/



        //
        // Convert, wait for completion, and store results
        //
        /*
        if (ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1) == true)
        {
            ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
            //AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            //ADCINA5_Val = AdcaResultRegs.ADCRESULT5;
            ADCINA5_Val  = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);
            ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
        }
        */

    }

}



//
// EPWM12-ISR
//
__interrupt void epwm12ISR(void)
{

    // Call Space vector Modulation process every 10 microseconds

    GPIO_writePin(37,1);

    svm();

    if (InverterState == ON)
    {
        pdpu_UpdateCompareReg(SpaceVectorTransitionTime);
    }

    //DlogCh1 = DlogCh1_SVM;
    //DlogCh2 = DlogCh2_SVM;


    // Debug
    //DLOG_4CH_F_FUNC(&dlog_4ch1);



    //Read /vDCFT, /iDCFT, /TMPFT, /vPHFT and /iFLT faults from inverter hardware and disable the inverter
    /*if((GPIO_readPin(33) == 1) | (GPIO_readPin(34) == 1) | (GPIO_readPin(35) == 1) |(GPIO_readPin(63) == 1))
    {
        InverterState = OFF;
    }*/


    GPIO_writePin(37,0);

    //
    // Clear INT flag for this timer
    //
    EPWM_clearEventTriggerInterruptFlag(EPWM12_BASE);

    //
    // Acknowledge interrupt group
    //
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP3);

    //GPIO_writePin(37,0);
}



//
// End of File
//
