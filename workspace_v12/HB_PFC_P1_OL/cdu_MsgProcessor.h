/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file cdu_MsgDataProcessor.h
* @brief CAN message data processing module header
*
* This module is process the CAN messages for proprietary diagnostics data management
*
* Created on: May 26, 2023
* @author Sarinx LLC
*******************************************************************************/
#ifndef CDU_MSGPROCESSOR_H_
#define CDU_MSGPROCESSOR_H_

#include "driverlib.h"
#include "device.h"


extern uint16_t FunctionCodes[256];

extern uint16_t SOM;
extern uint16_t EOM;


void cdu_ProcessDiagnoticMsgs(void);

#endif /* CDU_MSGPROCESSOR_H_ */
