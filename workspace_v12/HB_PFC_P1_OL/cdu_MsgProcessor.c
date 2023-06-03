
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

uint16_t FunctionCodes[256];
uint16_t CanRxMsgData[8];
uint16_t CanTxMsgData[8];

uint16_t SOM = 0;
uint16_t EOM = 0;

void cdu_ProcessDiagnoticMsgs()
{
    uint16_t i;

    ddi_ReadCanMessage();

    if(CanRxMsgData[0] == 0x23)
    {
        SOM = 1;
        EOM = 0;
    }
    if(CanRxMsgData[7] == 0x24)
    {
            SOM = 0;
            EOM = 1;
    }


    for (i=1; i< 7; i++)
    {

        switch (CanRxMsgData[i])
        {
            // OBJECT OVERWRITE: SVM Object Data
            case 0x21:
                if(SOM == 1)
                {
                    // Overwrite space vector amplitude and velocity vector values.
                    SpaceVectorDQZ.Axis1 = 0;
                    SpaceVectorDQZ.Axis2 = 0.75;
                    SpaceVectorDQZ.Axis3 = 0;
                    WrDQZ = 60 ;
                }
                break;

            // OBJECT OVERWRITE: SVM Object Data
            case 0x22:
                if(SOM == 1)
                {
                    // Overwrite space vector amplitude and velocity vector values.
                    SpaceVectorDQZ.Axis1 = 0;
                    SpaceVectorDQZ.Axis2 = 0.50;
                    SpaceVectorDQZ.Axis3 = 0;
                    WrDQZ = 60 ;
                }
                break;

            // OBJECT OVERWRITE: SVM Object Data
            case 0x25:
                if(SOM == 1)
                {
                    // Overwrite space vector amplitude and velocity vector values.
                    SpaceVectorDQZ.Axis1 = 0;
                    SpaceVectorDQZ.Axis2 = 0.25;
                    SpaceVectorDQZ.Axis3 = 0;
                    WrDQZ = 60 ;
                }
                break;

            // OBJECT READ: SVM Object Data
            case 0x02:
                if(SOM == 1)
                {
                    // Prepare TxMsg data with SVM Object data
                    CanTxMsgData[0] = 0x23;
                    CanTxMsgData[1] = 0x23;
                    CanTxMsgData[2] = SvmProfilerObject.pmu_AngleValue;
                    CanTxMsgData[3] = SvmProfilerObject.pmu_WrDQZ;
                    CanTxMsgData[4] = SvmProfilerObject.pmu_SpaceVectorDQZ.Axis1;
                    CanTxMsgData[5] = SvmProfilerObject.pmu_SpaceVectorDQZ.Axis2;
                    CanTxMsgData[6] = SvmProfilerObject.pmu_SpaceVectorDQZ.Axis3;
                    CanTxMsgData[7] = 0x24;
                }
                break;

            // default condition to exit
            default:
                break;
        }
    }

}



