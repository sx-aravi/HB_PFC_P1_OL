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

float sdp_BiasAndScaleCurrentSensorValue(uint16_t value)
{
    float scaled_val;

    // Need to be implmenetd based on the requried bias and scaling.

    scaled_val = value;
    return scaled_val;
}


float sdp_ConvertCurrentToPU(uint16_t value)
{
    float pu_val;

    /* Phase Current sensing */
    /* 4095 counts for 3V.
     * 300 A ==> 3 V i.e. 0.01V --> 1A or 13.65 counts to 1A
     */

    pu_val = (value / 4095) * 0.01;
    return pu_val;
}



// End of file


