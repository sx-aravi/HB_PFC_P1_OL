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


uint16_t ADCINA4_Val =0;
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

    //
    // take conversions indefinitely in loop
    //
        while(1)
        {

            cdu_ProcessDiagnoticMsgs();
            pmu_SynchronizeProfilerObjects();



            EALLOW;
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

            AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            EDIS;

            AdcaRegs.ADCSOCFRC1.bit.SOC0 = 1;
            AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;

            while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            ADCINA4_Val = AdcaResultRegs.ADCRESULT0;

            while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            ADCINA5_Val = AdcbResultRegs.ADCRESULT0;

            /*
            //Read 3-Phase AC: PhaseA Voltage value (VPhaseAFbk)
            ADCINA0_VPhaseAFbk_Val = AdcaResultRegs.ADCRESULT0;

            //Read 3-Phase AC: PhaseB Voltage value (VPhaseBFbk)
            ADCINA1_VPhaseAFbk_Val = AdcaResultRegs.ADCRESULT1;

            //Read 3-Phase AC: PhaseC Voltage value (VPhaseCFbk)
            ADCINA2_VPhaseAFbk_Val = AdcaResultRegs.ADCRESULT2;

            //Read DCLINK Voltage value (HV_Vsns)
            ADCINA3_DCLINKVoltage_Val = AdcaResultRegs.ADCRESULT3;

            //Read DCLINK Current value (HViSns)
            ADCINA4_DCLINKVoltage_Val = AdcaResultRegs.ADCRESULT4;

            //Read Temperature Max value (Tmax)
            ADCINA5_TempMax_Val = AdcaResultRegs.ADCRESULT5;

            //Read 3-Phase AC: PhaseA Current value (IPhaseAFbk)
            ADCINB0_TempMax_Val = AdcbResultRegs.ADCRESULT0;
            */



            EALLOW;
            AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0;
            AdcbRegs.ADCINTSEL1N2.bit.INT1E = 0;
            EDIS;

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


    // CCS Debug Channel Graphs
    //DlogCh1 = DlogCh1_SVM;
    //DlogCh2 = DlogCh2_SVM;
    //DLOG_4CH_F_FUNC(&dlog_4ch1);


    //Read /vDCFT, /iDCFT, /TMPFT, /vPHFT and /iFLT faults from inverter hardware and disable the inverter
    /*if((GPIO_readPin(33) == 1) | (GPIO_readPin(34) == 1) | (GPIO_readPin(35) == 1) |(GPIO_readPin(63) == 1))
    {
        InverterState = OFF;
    }*/

/*    EALLOW;
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 0x0001;
    EDIS;

    //
    //software force start SOC0 to SOC7
    //
    AdcaRegs.ADCSOCFRC1.all = 0x00FF;

    if (AdcaRegs.ADCINTFLG.bit.ADCINT1 == 1)
    {
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
        ADCINA5_Val = AdcaResultRegs.ADCRESULT4;
        EALLOW;
        AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0;
        EDIS;
    }
*/



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


// testing purposes only


//
// ConfigureADC - Write ADC configurations and power up the ADC for both
//                ADC A and ADC B
//
void ConfigureADC(void)
{
    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    //power up the ADC
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for 1ms to allow ADC time to power up
    //
    DELAY_US(1000);

    EDIS;
}

//
// SetupADCContinuous - setup the ADC to continuously convert on one channel
//
void SetupADCContinuous(Uint16 channel)
{
    Uint16 acqps;

    //
    // Determine minimum acquisition window (in SYSCLKS) based on resolution
    //
    if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
    {
        acqps = 14; //75ns
    }
    else //resolution is 16-bit
    {
        acqps = 63; //320ns
    }

    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC1CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC2CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC3CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC4CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC5CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC6CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC7CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC8CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC9CTL.bit.CHSEL  = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC10CTL.bit.CHSEL = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC11CTL.bit.CHSEL = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC12CTL.bit.CHSEL = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC13CTL.bit.CHSEL = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC14CTL.bit.CHSEL = channel;  //SOC will convert on channel
    AdcaRegs.ADCSOC15CTL.bit.CHSEL = channel;  //SOC will convert on channel

    AdcaRegs.ADCSOC0CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC3CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC4CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC5CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC6CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC7CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC8CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC9CTL.bit.ACQPS  = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC10CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC11CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC12CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC13CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC14CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles
    AdcaRegs.ADCSOC15CTL.bit.ACQPS = acqps;    //sample window is acqps +
                                               //1 SYSCLK cycles

    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0; //disable INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 0; //disable INT2 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT3E = 0; //disable INT3 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT4E = 0; //disable INT4 flag

    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;
    AdcaRegs.ADCINTSEL1N2.bit.INT2CONT = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT3CONT = 0;
    AdcaRegs.ADCINTSEL3N4.bit.INT4CONT = 0;

    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 6;  //end of SOC6 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 14; //end of SOC14 will set INT2 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT3SEL = 7;  //end of SOC7 will set INT3 flag
    AdcaRegs.ADCINTSEL3N4.bit.INT4SEL = 15; //end of SOC15 will set INT4 flag

    //
    //ADCINT2 will trigger first 8 SOCs
    //
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC2 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC3 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC4 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC5 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC6 = 2;
    AdcaRegs.ADCINTSOCSEL1.bit.SOC7 = 2;

    //
    //ADCINT1 will trigger second 8 SOCs
    //
    AdcaRegs.ADCINTSOCSEL2.bit.SOC8 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC9 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcaRegs.ADCINTSOCSEL2.bit.SOC15 = 1;
    EDIS;
}




//
// End of File
//
