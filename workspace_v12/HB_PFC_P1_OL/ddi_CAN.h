/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_CAN.h
* @brief CAN bus devicew driver interface header
*
* This module is wrapper for CAN bus TI device driver library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/
#ifndef DDI_CAN_H_
#define DDI_CAN_H_

#include "driverlib.h"
#include "device.h"


#define RX_MSG_DATA_LENGTH    0   // "Don't care" for a Receive mailbox
#define RX_MSG_OBJ_ID      1   // Use mailbox 1
#define TXCOUNT  100000
#define TX_MSG_DATA_LENGTH    8
#define MSG_DATA_LENGTH     8
#define TX_MSG_OBJ_ID      2

extern uint16_t CanRxMsgData[8];

void ddi_ConfigureCAN(void);
void ddi_InitializeCAN(void);
void ddi_ReadCanMessage(void);



#endif /* DDI_CAN_H_ */
