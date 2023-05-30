/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_ADC.h
* @brief ADC device driver interface header
*
* This module is wrapper for ADC TI device driver library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/

#ifndef DDI_ADC_H_
#define DDI_ADC_H_

#include "F28x_Project.h"
#include "F2837xD_gpio.h"
#include "driverlib.h"
#include "device.h"


// PFC board adc input signals: DC LINK voltages, Input AC voltages, Input/DC currents, temperatures

#define ADC_VPHASE_A_FBK    ADCINA0
#define ADC_VPHASE_B_FBK    ADCINA1
#define ADC_VPHASE_C_FBK    ADCINA2
#define ADC_HV_VSNS         ADCINA3
#define ADC_HV_ISNS         ADCINA4
#define ADC_TMAX            ADCINA5

#define ADC_IPHASE_A_FBK    ADCINB0
#define ADC_IPHASE_B_FBK    ADCINB1
#define ADC_IPHASE_C_FBK    ADCINB2

#define ADC_VDC_LOWER       ADCINC3
#define ADC_VDC_UPPER       ADCINC4

#define ADC_SPARE4          ADCIND0
#define ADC_SPARE5          ADCIND1
#define ADC_HT1             ADCIND2
#define ADC_HT2             ADCIND3
#define ADC_ATEMP           ADCIND4



extern void ddi_ConfigureADCs();
extern void ddi_InitializeADCs();
void ddi_SetupADCContinuousMode();


#endif /* DDI_ADC_H_ */
