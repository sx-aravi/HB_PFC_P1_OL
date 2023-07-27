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
#include <sdp_CurrentSensor.h>
#include <cam_PLL.h>
#include <cam_Regulators.h>
#include <siu_CLA.h>




// Start of CLA Specific Declaration

//Task 1 (C) Variables
// NOTE: Do not initialize the Message RAM variables globally, they will be
// reset during the message ram initialization phase in the CLA memory
// configuration routine
//

#define WAITSTEP     asm(" RPT #255 || NOP")

#pragma DATA_SECTION(Cpu1ToCla1Transfer,"CpuToCla1MsgRAM");
float Cpu1ToCla1Transfer[M][N];
#pragma DATA_SECTION(Cla1ToCpu1Transfer,"Cla1ToCpuMsgRAM");
float Cla1ToCpu1Transfer[M][N];


//
// Function Prototypes
//
void CLA_runTest(void);
void CLA_configClaMemory(void);
void CLA_initCpu1Cla1(void);





// End of CLA specific declaration





//
// EPWM12 10 microseconds interrupt service routine
//
__interrupt void epwm12ISR(void);


uint16_t MeasuredPhaseCurrentAValue =0;
uint16_t MeasuredTemperatureValue =0;
float PhACurrentPU = 0;



//
//Open the following for RAM version code to allow debugging
//

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
    //

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

    CLA_configClaMemory();
    siu_InitCla();

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


    //Fault Reset, DSPEN default values during the system initialization.
    GPIO_writePin(32,0);
    GPIO_writePin(29,0);


    CLA_runTest();

    //
    // take conversions indefinitely in loop
    //
        while(1)
        {

            phaselock();
            cdu_ProcessDiagnoticMsgs();
            pmu_SynchronizeProfilerObjects();

            //
            // Following code measures the Temperature & Phase A current for Open Loop Test only. This will be moved to CLA task for closed loop test
            //

/*            EALLOW;
            //AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
            //AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

            AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;
            AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            EDIS;
*/
            //AdcaRegs.ADCSOCFRC1.bit.SOC5 = 1;
            //AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;

            /*
            //Read Temperature Max value (Tmax)
            while(AdcaRegs.ADCINTFLG.bit.ADCINT1 == 0);
            AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
            MeasuredTemperatureValue = AdcaResultRegs.ADCRESULT5;
            */

            //Read 3-Phase AC: PhaseA Current value (IPhaseAFbk)
            //while(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0);
 /*           if(AdcbRegs.ADCINTFLG.bit.ADCINT1 == 0)
            {
                AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
                MeasuredPhaseCurrentAValue = AdcbResultRegs.ADCRESULT0;

                EALLOW;
                //AdcaRegs.ADCINTSEL1N2.bit.INT1E = 0;
                AdcbRegs.ADCINTSEL1N2.bit.INT1E = 0;
                EDIS;
            }*/
            //PhACurrentPU = sdp_ConvertCurrentToPU(MeasuredPhaseCurrentAValue);

            if(MeasuredPhaseCurrentAValue >= PHASE_CURRENT_FAULT_THRESHOLD_COUNT)
            {
                InverterState = OFF;
            }

            MeasuredPhaseCurrentAValue = Cla1ToCpu1Transfer[0][0];

        }

}



//
// EPWM12-ISR
//
__interrupt void epwm12ISR(void)
{

    // Call Space vector Modulation process every 10 microseconds

    GPIO_writePin(37,1);

    //AdcbRegs.ADCSOCFRC1.bit.SOC0 = 1;
    AdcbRegs.ADCSOCFRC1.all = 0xF;

    svm();

    if (InverterState == ON)
    {
        pdpu_UpdateCompareReg(SpaceVectorTransitionTime);
    }


    // CCS Debug Channel Graphs
    //DlogCh1 = DlogCh1_SVM;
    //DlogCh2 = DlogCh2_SVM;
    //DLOG_4CH_F_FUNC(&dlog_4ch1);


    //Read over temperature fault /TMPFT from inverter hardware and disable the inverter
    if(GPIO_readPin(33) == 0)
    {
        //InverterState = OFF;
    }


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



// CLA specific unction definitions, will be moved CLA files later



//
// CLA_runTest - Execute CLA1 Task1 Test
//
void CLA_runTest(void)
{


    Cpu1ToCla1Transfer[0][0] = SpaceVectorTransitionTime.Axis1;
    Cpu1ToCla1Transfer[0][1] = SpaceVectorTransitionTime.Axis1;
    Cpu1ToCla1Transfer[0][2] = SpaceVectorTransitionTime.Axis1;
    Cpu1ToCla1Transfer[1][0] = SpaceVectorTransitionTime.Axis2;
    Cpu1ToCla1Transfer[1][1] = SpaceVectorTransitionTime.Axis2;
    Cpu1ToCla1Transfer[1][2] = SpaceVectorTransitionTime.Axis2;
    Cpu1ToCla1Transfer[2][0] = SpaceVectorTransitionTime.Axis3;
    Cpu1ToCla1Transfer[2][1] = SpaceVectorTransitionTime.Axis3;
    Cpu1ToCla1Transfer[2][2] = SpaceVectorTransitionTime.Axis3;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_1);

#if 0
    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_2);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_3);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_4);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_5);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_6);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_7);
    WAITSTEP;

    CLA_forceTasks(CLA1_BASE,CLA_TASKFLAG_8);
    WAITSTEP;
#endif
}

//
// CLA_configClaMemory - Configure CLA memory sections
//
void CLA_configClaMemory(void)
{
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
    EALLOW;

#ifdef _FLASH
    //
    // Copy over code from FLASH to RAM
    //
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
           (uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Select LS4RAM and LS5RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS4 and LS5 and then
    // set the space to be a program block
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;
    //MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    //MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

    //
    // Next configure LS0RAM and LS1RAM as data spaces for the CLA
    // First configure the CLA to be the master for LS0(1) and then
    // set the spaces to be code blocks
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

    EDIS;
}



//
// End of File
//
