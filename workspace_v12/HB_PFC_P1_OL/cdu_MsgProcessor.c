
/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file cdu_MsgDataProcessor.c
* @brief CAN message data processing module functions
*
* This module is process the CAN messages for proprietary diagnostics data management
*
* Created on: May 26, 2023
* @author Sarinx LLC
*******************************************************************************/

#include "cdu_MsgProcessor.h"
#include "ddi_CAN.h"
#include "cam_SVM.h"
#include "pmu_ProfilerData.h"
#include <sdp_CurrentSensor.h>

uint16_t FunctionCodes[256];
uint16_t CanRxMsgData[8];
uint16_t CanTxMsgData[8];

uint16_t SOM = 0;
uint16_t EOM = 0;

void cdu_ProcessDiagnoticMsgs()
{

    ddi_ReadCanMessage();

    // Inverter ON/OFF command
    if (CanRxMsgData[0] == 0x1)
    {
        InverterState = ON;
        GpioFaultResetBit = 1;
        GpioEnableBit= 1;
        GPIO_writePin(32,1);
        GPIO_writePin(29,1);

    }
    else if(CanRxMsgData[0] == 0x0)
    {
        InverterState = OFF;
        GpioFaultResetBit = 0;
        GpioEnableBit= 0;
        GPIO_writePin(32,0);
        GPIO_writePin(29,0);

    }

    switch(CanRxMsgData[1])
    {
        case 0x0:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.866;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x1:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.75;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x2:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.6;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x3:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.50;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x4:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.433;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x5:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.25;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        case 0x6:
            // Overwrite space vector amplitude values
            SpaceVectorDQZ.Axis1 = 0;
            SpaceVectorDQZ.Axis2 = 0.217;
            SpaceVectorDQZ.Axis3 = 0;
            break;

        default:
            break;
     }

    switch(CanRxMsgData[2])
        {
            case 0x0:
                // Overwrite velocity vector value
                WrDQZ = 60;
                break;

            case 0x1:
                // Overwrite velocity vector value
                WrDQZ = 100;
                break;

            case 0x2:
                // Overwrite velocity vector value
                WrDQZ = 200;
                break;

            case 0x3:
                // Overwrite velocity vector value
                WrDQZ = 500;
                break;

            case 0x4:
                // Overwrite velocity vector value
                WrDQZ = 1000;
                break;

            default:
                break;
         }



    switch(CanRxMsgData[3])
    {
        case 0x0:

            // Fault Limit @10 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2183;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 1.74;

            break;

        case 0x1:
            // Fault Limit @20 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2320;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 1.83;
            break;

        case 0x2:
            // Fault Limit @30 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2456;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 1.92;
            break;

        case 0x3:
            // Fault Limit @40 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2593;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.01;
            break;

        case 0x4:
            // Fault Limit @50 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2729;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.1;
            break;

        case 0x5:
            // Fault Limit @60 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 2866;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.19;
            break;

        case 0x6:
            // Fault Limit @70 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 3002;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.28;
            break;

        case 0x7:
            // Fault Limit @80 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 3139;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.37;
            break;

        case 0x8:
            // Fault Limit @90 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 3275;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.46;
            break;

        case 0x9:
            // Fault Limit @100 A
            PHASE_CURRENT_FAULT_THRESHOLD_COUNT = 3412;
            PHASE_CURRENT_FAULT_THRESHOLD_LIMIT = 2.55;

            break;

        default:
            break;
     }
}

// end of file
