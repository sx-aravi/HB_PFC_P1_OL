/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file sdp_CurrentSensor.h
* @brief process the raw current values to scale, bias, filter and convert in to per unit
*
* This module is part of sensor data processing unit to perform current sensor specific functions
*
* Created on: July 02, 2023
* @author Sarinx LLC
*******************************************************************************/

#ifndef SDP_CURRENTSENSOR_H_
#define SDP_CURRENTSENSOR_H_

#include "F28x_Project.h"
#include "driverlib.h"
#include "device.h"


extern uint16_t PHASE_CURRENT_FAULT_THRESHOLD_COUNT;
extern float PHASE_CURRENT_FAULT_THRESHOLD_LIMIT;



float sdp_BiasAndScaleCurrentSensorValue(uint16_t value);
float sdp_ConvertCurrentToPU(uint16_t value);


#endif /* SDP_CURRENTSENSOR_H_ */
