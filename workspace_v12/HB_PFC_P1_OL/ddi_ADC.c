/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_CAN.c
* @brief CAN bus devicew driver interface header
*
* This module is wrapper for CAN bus TI device driver library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/

#include "ddi_ADC.h"



/*******************************************************************************
*
* ddi_ConfigireADCs - Function to configure all ADCs.
*
*
*******************************************************************************/

void ddi_ConfigureADCs()
{

       EALLOW;

       //
       // Write configurations for ADC A Registers
       //

       // Reset ADC
       SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_ADCA);

       // Configure the ADC-A base registers
       ADC_disableConverter(ADCA_BASE);

       // Enable ADC-A clock
       SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCA);

       //set ADCCLK divider to /4
       AdcaRegs.ADCCTL2.bit.PRESCALE = 6;

       //Configure the resolution
       ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

       //Set pulse positions to late
       AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

       // Power up the ADC
       ADC_enableConverter(ADCA_BASE);

       EDIS;

       //
       //delay for 1ms to allow ADC time to power up
       //
       DELAY_US(1000);

       //
       // Write configurations for ADC B Registers
       //

       // Reset ADC
       SysCtl_resetPeripheral(SYSCTL_PERIPH_RES_ADCB);

       // Configure the ADC-B base registers
       ADC_disableConverter(ADCB_BASE);

       // Enable ADC-B clock
       SysCtl_enablePeripheral(SYSCTL_PERIPH_CLK_ADCB);

       //set ADCCLK divider to /4
       AdcbRegs.ADCCTL2.bit.PRESCALE = 6;

       //Configure the resolution
       ADC_setMode(ADCB_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);

       //Set pulse positions to late
       AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;

       // Power up the ADC
       ADC_enableConverter(ADCB_BASE);

       EDIS;

       //
       //delay for 1ms to allow ADC time to power up
       //
       DELAY_US(1000);

}


/*******************************************************************************
*
* ddi_InitializeADCs - Function to configure and power up all ADCs.
*
*
*******************************************************************************/

void ddi_InitializeADCs()
{
/*
    //
    //Select the channels to convert and end of conversion flag for ADC A
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 5;  //SOC0 will convert pin A5
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0; // Software only
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 1;
    EDIS;
*/
    ddi_InitializeSOC0_ADCA(5);
    ddi_InitializeSOC0_ADCB(0);

    //void ddi_InitializeADCA();


 }



/*******************************************************************************
*
* ddi_InitializeSOC0_ADCA - Function to initialize ADCA channel.
* Select the channels to convert and end of conversion flag for ADC A
*
*******************************************************************************/

void ddi_InitializeSOC0_ADCA(short channel)
{
    //
    //Select the channels to convert and end of conversion flag for ADC A
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin A5
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0; // Software only
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcaRegs.ADCINTSOCSEL1.bit.SOC0 = 1;
    EDIS;
 }


/*******************************************************************************
*
* ddi_InitializeSOC1_ADCA - Function to initialize ADCA channel.
* Select the channels to convert and end of conversion flag for ADC A
*
*******************************************************************************/

void ddi_InitializeSOC1_ADCA(short channel)
{
    //
    //Select the channels to convert and end of conversion flag for ADC A
    //
    EALLOW;
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = channel;  //SOC1 will convert pin/channel
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0; // Software only
    AdcaRegs.ADCINTSEL1N2.bit.INT2SEL = 1; //end of SOC1 will set INT2 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT2E = 1;   //enable INT2 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT2 = 1; //make sure INT2 flag is cleared
    AdcaRegs.ADCINTSOCSEL1.bit.SOC1 = 1;
    EDIS;
 }



/*******************************************************************************
*
* ddi_InitializeSOC0_ADCB - Function to initialize ADCB channel.
* Select the channels to convert and end of conversion flag for ADC B
*
*******************************************************************************/

void ddi_InitializeSOC0_ADCB(short channel)
{
    //
    //Select the channels to convert and end of conversion flag for ADC B
    //
    EALLOW;
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = channel;  //SOC0 will convert pin A5
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = 14; //sample window is 100 SYSCLK cycles
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = 0; // Software only
    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcbRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    AdcbRegs.ADCINTSOCSEL1.bit.SOC0 = 1;
    EDIS;
 }



/*******************************************************************************
*
* ddi_InitializeADCA - Function to initialize ADCA channel.
* Select the channels to convert and end of conversion flag for ADC A
*
*******************************************************************************/

void ddi_InitializeADCA()
{

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 15);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN5, 15);


    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 15);
    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN1, 15);
    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 15);
    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, 15);
    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER4, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 15);
    //ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER5, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN5, 15);

    //
    // Set SOC1 to set the interrupt 1 flag. Enable the interrupt and make
    // sure its flag is cleared.
    //
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
 }



/*******************************************************************************
*
* ddi_SetupADCContinuousMode - setup the ADC to continuously convert on one channel
*
*******************************************************************************/

void ddi_SetupADCContinuousMode()
{
    uint16_t acqps;

    //
    // Determine acquisition window (in SYSCLKS) based on resolution i.e., 12-bit
    //

    acqps = 30; // 150ns

    //
    // - NOTE: A longer sampling window will be required if the ADC driving
    //   source is less than ideal (an ideal source would be a high bandwidth
    //   op-amp with a small series resistance). See TI application report
    //   SPRACT6 for guidance on ADC driver design.
    // - NOTE: A slightly longer S+H window is used with 12-bit resolution to
    //   ensure the data collection loop can keep up with the ADC
    //

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY,
                     ADC_CH_ADCIN5, acqps);


}


//
// End of file
//




