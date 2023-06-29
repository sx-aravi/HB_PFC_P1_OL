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
       //write configurations
       //
       AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4

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


/*******************************************************************************
*
* ddi_InitializeADCs - Function to configure and power up all ADCs.
*
*
*******************************************************************************/

void ddi_InitializeADCs()
{
    //
    //Select the channels to convert and end of conversion flag
    //
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 5;  //SOC0 will convert pin A5
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 30; //sample window is 100 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0; // Software only
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 0; //end of SOC0 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;   //enable INT1 flag
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //make sure INT1 flag is cleared
    EDIS;
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




