/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pdpu_PwmUpdate.c
* @brief PWM threshold time updates source module
*
* This module receives the PWM on/off switching times from SVM module
* and updates the appropriate PWM modules accordingly
*
* Created on: Apr 14, 2023
*     Author: Sarinx LLC
*******************************************************************************/


#include <pdu_PwmUpdate.h>



/*******************************************************************************
* @brief This module shall Initialize the PWM module per the applciation needs
*
*******************************************************************************/

void pdpu_InitializePWM()
{

    //
    // Initialize all EPWM registers
    //

    /*pdpu_Initialize_PWM_7();
    pdpu_Initialize_PWM_8();
    pdpu_Initialize_PWM_9();
    pdpu_Initialize_PWM_10();
    pdpu_Initialize_PWM_11();
    pdpu_Initialize_PWM_12();*/

    pdpu_InitPWM(EPWM12_BASE);
    pdpu_InitPWM(EPWM11_BASE);
    pdpu_InitPWM(EPWM10_BASE);
    pdpu_InitPWM(EPWM9_BASE);
    pdpu_InitPWM(EPWM8_BASE);
    pdpu_InitPWM(EPWM7_BASE);

    //
    // Interrupt where we will change the Compare Values
    // Select INT on Time base counter zero event,
    // Enable INT, generate INT on 3rd event
    //
    EPWM_setInterruptSource(EPWM12_BASE, EPWM_INT_TBCTR_ZERO);
    EPWM_enableInterrupt(EPWM12_BASE);
    EPWM_setInterruptEventCount(EPWM12_BASE, 1U);

}

/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_InitPWM(uint32_t pwm_base)
{
    //
    // Set-up TBCLK
    //
    EPWM_setTimeBasePeriod(pwm_base, EPWM_TIMER_TBPRD);
    EPWM_setPhaseShift(pwm_base, 0U);
    EPWM_setTimeBaseCounter(pwm_base, 0U);

    //
    // Set Compare values
    //
    EPWM_setCounterCompareValue(pwm_base,EPWM_COUNTER_COMPARE_A,EPWM_CMP);
    EPWM_setCounterCompareValue(pwm_base,EPWM_COUNTER_COMPARE_B,EPWM_CMP);


    //
    // Set up counter mode
    //
    EPWM_setTimeBaseCounterMode(pwm_base, EPWM_COUNTER_MODE_UP_DOWN);
    EPWM_disablePhaseShiftLoad(pwm_base);
    EPWM_setClockPrescaler(pwm_base,EPWM_CLOCK_DIVIDER_1,EPWM_HSCLOCK_DIVIDER_1);

    //
    // Set up shadowing
    //
    EPWM_setCounterCompareShadowLoadMode(pwm_base,EPWM_COUNTER_COMPARE_A,EPWM_COMP_LOAD_ON_CNTR_ZERO);
    EPWM_setCounterCompareShadowLoadMode(pwm_base,EPWM_COUNTER_COMPARE_B,EPWM_COMP_LOAD_ON_CNTR_ZERO);


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_A, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);


    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_ZERO);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_PERIOD);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_HIGH, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPA);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_LOW, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPA);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_UP_CMPB);
    EPWM_setActionQualifierAction(pwm_base, EPWM_AQ_OUTPUT_B, EPWM_AQ_OUTPUT_NO_CHANGE, EPWM_AQ_OUTPUT_ON_TIMEBASE_DOWN_CMPB);
}


/*******************************************************************************
* @brief This module shall update the PWM module compare register values
* per the application needs
*
*******************************************************************************/

void pdpu_UpdateCompareReg(uint32_t pwm_base, VECTOR SVMTransitionTime, VECTOR SV3Phase)
{
    uint16_t compAValue;

    //ASSERT(EPWM_isBaseValid(pwm_base));

    if(SV3Phase.Axis1 >=0)
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM11_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM11_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EALLOW;
        EPwm11Regs.AQCSFRC.bit.CSFA = 0;
        EPwm11Regs.AQCSFRC.bit.CSFB = 0;
        EDIS;
    }
    else
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM12_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM12_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EALLOW;
        EPwm12Regs.AQCSFRC.bit.CSFA = 0;
        EPwm12Regs.AQCSFRC.bit.CSFB = 0;
        EDIS;
    }

    if(SV3Phase.Axis2 >=0)
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM9_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM9_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EPwm9Regs.AQCSFRC.bit.CSFA = 1;
        EPwm9Regs.AQCSFRC.bit.CSFB = 1;
    }
    else
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM10_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM10_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EPwm10Regs.AQCSFRC.bit.CSFA = 1;
        EPwm10Regs.AQCSFRC.bit.CSFB = 1;
    }

    if(SV3Phase.Axis3 >=0)
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM7_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM7_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EPwm7Regs.AQCSFRC.bit.CSFA = 1;
        EPwm7Regs.AQCSFRC.bit.CSFB = 1;
    }
    else
    {
        //EPWM_setActionQualifierContSWForceAction(EPWM8_BASE, EPWM_AQ_OUTPUT_A, EPWM_AQ_SW_DISABLED);
        //EPWM_setActionQualifierContSWForceAction(EPWM8_BASE, EPWM_AQ_OUTPUT_B, EPWM_AQ_SW_DISABLED);
        EPwm8Regs.AQCSFRC.bit.CSFA = 1;
        EPwm8Regs.AQCSFRC.bit.CSFB = 1;
    }

    compAValue = SVMTransitionTime.Axis1 * 998;

    EPwm12Regs.CMPA.bit.CMPA = compAValue;
    EPwm12Regs.CMPB.bit.CMPB = compAValue;
    //EPWM_setCounterCompareValue(pwm_base,EPWM_COUNTER_COMPARE_A,compAValue);
    //EPWM_setCounterCompareValue(pwm_base,EPWM_COUNTER_COMPARE_B,compAValue);

}


/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_6()
{
    //
    // Set-up TBCLK
    //
    EPwm6Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm6Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm6Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm6Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm6Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm6Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm6Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm6Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm6Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm6Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm6Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm6Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm6Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm6Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm6Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}


/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_7()
{
    //
    // Set-up TBCLK
    //
    EPwm7Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm7Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm7Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm7Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm7Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm7Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm7Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm7Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm7Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm7Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm7Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm7Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm7Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm7Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm7Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm7Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm7Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm7Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm7Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm7Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm7Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm7Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm7Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm7Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}

/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_8()
{
    //
    // Set-up TBCLK
    //
    EPwm8Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm8Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm8Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm8Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm8Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm8Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm8Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm8Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm8Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm8Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm8Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm8Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm8Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm8Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm8Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm8Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm8Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm8Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm8Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm8Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm8Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm8Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm8Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm8Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}

/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_9()
{
    //
    // Set-up TBCLK
    //
    EPwm9Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm9Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm9Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm9Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm9Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm9Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm9Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm9Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm9Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm9Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm9Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm9Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm9Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm9Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm9Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm9Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm9Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm9Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm9Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm9Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm9Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm9Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm9Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm9Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}

/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_10()
{
    //
    // Set-up TBCLK
    //
    EPwm10Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm10Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm10Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm10Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm10Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm10Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm10Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm10Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm10Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm10Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm10Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm10Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm10Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm10Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm10Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm10Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm10Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm10Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm10Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm10Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm10Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm10Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm10Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm10Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}


/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_11()
{
    //
    // Set-up TBCLK
    //
    EPwm11Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm11Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm11Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm11Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm11Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm11Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm11Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm11Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm11Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm11Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm11Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm11Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm11Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm11Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm11Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm11Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm11Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm11Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm11Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm11Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm11Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm11Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm11Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm11Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm11Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}


/*****************************************************************************************************
* @brief This module shall Initialize the individual PWM module per the custom configuration needs
* Up-Down-Count, Dual Edge Symmetric Waveform, With Independent Modulation on
* EPWMxA and EPWMxB — Complementary
******************************************************************************************************/

void pdpu_Initialize_PWM_12()
{
    //
    // Set-up TBCLK
    //
    EPwm12Regs.TBPRD = EPWM_TIMER_TBPRD;       // Set timer period 801 TBCLKs
    EPwm12Regs.TBPHS.bit.TBPHS = 0x0000;       // Phase is 0
    EPwm12Regs.TBCTR = 0x0000;                 // Clear counter

    //
    // Set Compare values
    //
    EPwm12Regs.CMPA.bit.CMPA = EPWM_CMPA;    // Set compare A value
    EPwm12Regs.CMPB.bit.CMPB = EPWM_CMPB;    // Set Compare B value


    //
    // Set up counter mode
    //
    EPwm12Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up and down
    EPwm12Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // Disable phase loading
    EPwm12Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // Clock ratio to SYSCLKOUT
    EPwm12Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Set up shadowing
    //
    EPwm12Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm12Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO; // Load on Zero
    EPwm12Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;


    // EPWM - ePWM Channel A - Action Qualifier Initialization

    EPwm12Regs.AQCTLA.bit.ZRO = 0;           // No change PWMA on time base reaches zero
    EPwm12Regs.AQCTLA.bit.PRD = 0;           // No change PWMA on time base reaches period
    EPwm12Regs.AQCTLA.bit.CAU = AQ_SET;      // Set PWM1A on event A, up count
    EPwm12Regs.AQCTLA.bit.CAD = AQ_CLEAR;    // Clear PWM1A on event A,down count
    EPwm12Regs.AQCTLA.bit.CBU = 0;           // No change PWMA on event B, up count
    EPwm12Regs.AQCTLB.bit.CBD = 0;           // No change PWMA on event B, down count

    // EPWM - ePWM Channel B - Action Qualifier Initialization

    EPwm12Regs.AQCTLA.bit.ZRO = 0;           // No change PWMB on time base reaches zero
    EPwm12Regs.AQCTLA.bit.PRD = 0;           // No change PWMB on time base reaches period
    EPwm12Regs.AQCTLA.bit.CBU = AQ_CLEAR;    // Clear PWMA on event A, up count
    EPwm12Regs.AQCTLA.bit.CBD = AQ_SET;     // Set PWMA on event A,down count
    EPwm12Regs.AQCTLA.bit.CAU = 0;           // No change PWMB on event B, up count
    EPwm12Regs.AQCTLB.bit.CAD = 0;          // No change PWMB on event B, down count

}

//END OF FILE
