/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pmu_ProfilerData.c
* @brief profiler data objects definition module
*
* This module defines and provides access to the PFC control and system data objects
*
* Created on: May 26, 2023
* @author Sarinx LLC
*******************************************************************************/

#include"pmu_ProfilerData.h"

SVM_OBJECT SvmProfilerObject;


extern float AngleValue;
extern float WrDQZ;
extern float DlogCh1_SVM;
extern float DlogCh2_SVM;

extern VECTOR SpaceVectorRXZ;
extern VECTOR SpaceVectorDQZ;
extern VECTOR SpaceVector3Phase;
extern VECTOR SpaceVectorTransitionTime;
extern VECTOR SVMOutputTimes;

void pmu_SynchronizeProfilerObjects(void)
{
    // Synchronizing SVM profiler object data with real-time SVM object

    SvmProfilerObject.pmu_AngleValue = AngleValue;
    SvmProfilerObject.pmu_WrDQZ = WrDQZ;
    SvmProfilerObject.pmu_SpaceVectorRegion = Region;
    SvmProfilerObject.pmu_SpaceVectorSector = Segment;

    SvmProfilerObject.pmu_SpaceVectorDQZ.Axis1 = SpaceVectorDQZ.Axis1;
    SvmProfilerObject.pmu_SpaceVectorDQZ.Axis2 = SpaceVectorDQZ.Axis2;
    SvmProfilerObject.pmu_SpaceVectorDQZ.Axis3 = SpaceVectorDQZ.Axis3;

    SvmProfilerObject.pmu_SpaceVectorRXZ.Axis1 = SpaceVectorRXZ.Axis1;
    SvmProfilerObject.pmu_SpaceVectorRXZ.Axis2 = SpaceVectorRXZ.Axis2;
    SvmProfilerObject.pmu_SpaceVectorRXZ.Axis3 = SpaceVectorRXZ.Axis3;

    SvmProfilerObject.pmu_SpaceVector3Phase.Axis1 = SpaceVector3Phase.Axis1;
    SvmProfilerObject.pmu_SpaceVector3Phase.Axis2 = SpaceVector3Phase.Axis2;
    SvmProfilerObject.pmu_SpaceVector3Phase.Axis3 = SpaceVector3Phase.Axis3;

    SvmProfilerObject.pmu_SpaceVectorTransitionTime.Axis1 = SpaceVectorTransitionTime.Axis1;
    SvmProfilerObject.pmu_SpaceVectorTransitionTime.Axis2 = SpaceVectorTransitionTime.Axis2;
    SvmProfilerObject.pmu_SpaceVectorTransitionTime.Axis3 = SpaceVectorTransitionTime.Axis3;


}

//
//End of file
//
