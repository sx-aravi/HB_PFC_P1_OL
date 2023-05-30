/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_CAN.c
* @brief CAN bus devicew driver interface functions
*
* This module is wrapper for CAN bus TI device driver library for specific functions
*
* Created on: May 16, 2023
* @author Sarinx LLC
*******************************************************************************/

#include "ddi_CAN.h"




void ddi_ConfigureCAN(void)
{

    EALLOW;
    //CAN bus-B pin configuration
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANRXB);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANTXB);
    EDIS;

}


void ddi_InitializeCAN(void)
{

    //
    // Initialize and Set up the CAN bus bit rate to 500kHz for each module
    //

    CAN_initModule(CANB_BASE);

    //CAN_setBitRate(CANB_BASE, DEVICE_SYSCLK_FREQ, 500000, 16);
    CAN_setBitRate(CANB_BASE, DEVICE_SYSCLK_FREQ, 125000, 16);

    CAN_setupMessageObject(CANB_BASE, TX_MSG_OBJ_ID, 0x95555556,
                                   CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0,
                                   CAN_MSG_OBJ_NO_FLAGS, TX_MSG_DATA_LENGTH);

    CAN_setupMessageObject(CANB_BASE, RX_MSG_OBJ_ID, 0x9555555,
                                     CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_RX, 0,
                                     CAN_MSG_OBJ_NO_FLAGS, RX_MSG_DATA_LENGTH);

}


void ddi_ReadCanMessage(void)
{
    if(((HWREGH(CANB_BASE + CAN_O_ES) & CAN_ES_RXOK)) == CAN_ES_RXOK)
       {
           //
           // Get the received message
           //
           CAN_readMessage(CANB_BASE, RX_MSG_OBJ_ID, CanRxMsgData);

           //
           // Poll TxOk bit in CAN_ES register to check completion of transmission
           //

           CAN_sendMessage(CANB_BASE, TX_MSG_OBJ_ID, TX_MSG_DATA_LENGTH, CanRxMsgData);
           while(((HWREGH(CANB_BASE + CAN_O_ES) & CAN_ES_TXOK)) !=  CAN_ES_TXOK)
           {
               NOP;
           }
       }

}

//End of File
