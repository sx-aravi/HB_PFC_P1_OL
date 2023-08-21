/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file sdp_CurrentSensor.c
* @brief process the raw current values to scale, bias, filter and convert in to per unit
*
* This module is part of sensor data processing unit to perform current sensor specific functions
*
* Created on: July 02, 2023
* @author Sarinx LLC
*******************************************************************************/

#include <sdp_CurrentSensor.h>


// default fault threshold is @10A
uint16_t PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2183;
float PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 1.74;

extern VECTOR PercentCurrentValue;

float sdp_BiasAndScaleCurrentSensorValue(uint16_t value)
{
    float scaled_val;

   // Convert counts in to measured current value from 0-250A range;
    scaled_val = value * 8.19;


   // Normalize to 0-1; norm_val = ( val – MIN) / (MAX – MIN)
   // minimum current is 0, and maximum current is 250A.

   //PercentCurrentValue.Axis1 = scaled_val/250;
    PercentCurrentValue.Axis1 = scaled_val * 0.004;

   return scaled_val;
}


float sdp_ConvertCurrentToPU(uint16_t value)
{
    float pu_val;

    /* Phase Current sensing */
    /* 4095 counts for 3.3V.
     * 250 A ==> 3.3 V i.e. 0.0132V --> 1A or 16.38 counts to 1A
     */

    //pu_val = (value*16.38 / 4095) * 0.0132;
    pu_val = value * 16.38 * 2.442002442002442e-4 * 0.0132;
    return pu_val;
}



// End of file


