/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pdpu_PwmUpdate.h
* @brief PWM threshold time updates header
*
* This module receives the PWM on/off switching times from SVM module
* and updates the appropriate PWM modules accordingly
*
* Created on: Apr 14, 2023
*     Author: Sarinx LLC
*******************************************************************************/

#ifndef PDU_PWMUPDATE_H_
#define PDU_PWMUPDATE_H_


#include "cam_VectorConversions.h"
#include "F28x_Project.h"
#include "F2837xD_epwm.h"
#include "epwm.h"
#include "cam_VectorConversions.h"
#include "gpio.h"

//
// Defines
//

//50 KHz --> 1000 count
//#define EPWM_TIMER_TBPRD   1000U
//#define EPWM_CMP           500U

//100 KHz --> 500 count
#define EPWM_TIMER_TBPRD   500U
#define EPWM_CMP           250U

#define EPWM_CMPA          500U
#define EPWM_CMPB          500U
#define EPWM_CMP_UP        1U
#define EPWM_CMP_DOWN      0U
#define MAX_PWM_MODULES    6U

//
// Globals
//
typedef struct
{
    uint32_t epwmModule;
    uint16_t epwmCompADirection;
    uint16_t epwmCompBDirection;
    uint16_t epwmTimerIntCount;
    uint16_t epwmMaxCompA;
    uint16_t epwmMinCompA;
    uint16_t epwmMaxCompB;
    uint16_t epwmMinCompB;
    uint16_t prevCompAValue;
    uint16_t prevCompBValue;
}epwmConfiguration;


//
// Globals to hold the ePWM information used in this example
//



/*******************************************************************************
* @brief This module shall Initialize the PWM module per the application needs
*
*******************************************************************************/

extern void pdpu_InitializePWM();

void pdpu_Initialize_PWM_6(void);
void pdpu_Initialize_PWM_7(void);
void pdpu_Initialize_PWM_8(void);
void pdpu_Initialize_PWM_9(void);
void pdpu_Initialize_PWM_10(void);
void pdpu_Initialize_PWM_11(void);
void pdpu_Initialize_PWM_12(void);

extern void pdpu_UpdateCompareReg(VECTOR SVMTransitionTime);
extern void pdpu_InitPWM(uint32_t pwm_base);
extern void pdpu_Disable_All_Phases();


void pdpu_Enable_Phase_A_UpperChannel_B();
void pdpu_Enable_Phase_A_LowerChannel_B();

void pdpu_Enable_Phase_B_UpperChannel_B();
void pdpu_Enable_Phase_B_LowerChannel_B();

void pdpu_Enable_Phase_C_UpperChannel_B();
void pdpu_Enable_Phase_C_LowerChannel_B();




#endif /* PDU_PWMUPDATE_H_ */
