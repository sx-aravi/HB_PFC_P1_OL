/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_GPIO.h
* @brief GPIO devic driver interface header
*
* This module is wrapper for GPIO TI BItmap registers library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/
#ifndef DDI_GPIO_H_
#define DDI_GPIO_H_

#include "F28x_Project.h"
#include "F2837xD_gpio.h"
#include "driverlib.h"
#include "device.h"


extern void ddi_ConfigureGPIOs(void);
void ddi_InitEPwm7Gpio(void);
void ddi_InitEPwm8Gpio(void);
void ddi_InitEPwm9Gpio(void);
void ddi_InitEPwm10Gpio(void);
void ddi_InitEPwm11Gpio(void);
void ddi_InitEPwm12Gpio(void);

void InitializePinMux(uint32_t, uint32_t, uint32_t, uint32_t);


#endif /* DDI_GPIO_H_ */
