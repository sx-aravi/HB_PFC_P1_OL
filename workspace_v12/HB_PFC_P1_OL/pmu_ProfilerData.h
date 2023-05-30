/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pmu_ProfilerData.h
* @brief profiler data objects definition module header
*
* This module defines and provides access to the PFC control and system data objects
*
* Created on: May 26, 2023
* @author Sarinx LLC
*******************************************************************************/

#ifndef PMU_PROFILERDATA_H_
#define PMU_PROFILERDATA_H_

#include "cam_VectorConversions.h"
#include "F28x_Project.h"
#include "cam_SVM.h"

//
/*******************************************************************************
* PFC control/performance/health data objects
*******************************************************************************/
//

typedef struct
{
    VECTOR pmu_SpaceVectorDQZ;
    VECTOR pmu_SpaceVectorRXZ;
    VECTOR pmu_SpaceVector3Phase;
    VECTOR pmu_SpaceVectorTransitionTime;
    uint16_t pmu_SpaceVectorRegion;
    uint16_t pmu_SpaceVectorSector;
    float pmu_AngleValue;
    float pmu_WrDQZ;
}SVM_OBJECT;


extern SVM_OBJECT SvmProfilerObject;


//
/*******************************************************************************
* Profiler data object functions
*******************************************************************************/
//


void pmu_SynchronizeProfilerObjects(void);
void pmu_UpdateProfilerObject(void);
void pmu_LogProfilerObject(SVM_OBJECT* spo);
void pmu_ReadProfilerObject(SVM_OBJECT* spo);
void pmu_WriteProfilerObject(SVM_OBJECT* spo);



#endif /* PMU_PROFILERDATA_H_ */
