/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pfc_board.h
* @brief PFC control board configuration & initialization
*
* This module provides the functions requried for configuring & initializing
* the PFC control board I/O
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef SIU_PFCBOARD_H
#define SIU_PFCBOARD_H

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//
// Included Files
//

#include "ddi_GPIO.h"
#include "ddi_CAN.h"
#include "ddi_SPI.h"
#include "ddi_ADC.h"


#define PWM_HALF_PERIOD     25000           // period/2 for 2 kHz symmetric PWM w/ 100 MHz ePWM clock
#define PWM_DUTY_CYCLE      18750           // 25% duty cycle
#define PWM_MIN_DUTY        22500           // 10% duty cycle for PWM modulation
#define PWM_MAX_DUTY        2500            // 90% duty cycle for PWM modulation
#define PWM_STEP            10              // Step size change for PWM modulation

extern uint32_t PwmDuty;
extern uint32_t PwmPeriod;
extern uint16_t PWM_MODULATE;

//*****************************************************************************
//
// PinMux Configurations for PFC Control Board
//
//*****************************************************************************

#define TOTAL_USED_PWM_MODULES     6U


//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************

//*****************************************************************************
//
// Board Configurations
//
//*****************************************************************************

void siu_ConfigurePfcBoard();
void SYNC_init();

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif  // end of BOARD_H definition
