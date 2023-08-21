/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file CurrentControl.h
* @brief Current Control loop
*
* This module controls currents based on current command
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef CURRENTCONTROL_H_
#define CURRENTCONTROL_H_

#include <cam_VectorConversions.h>
#include <cam_PLL.h>
#include <cam_SVM.h>

extern unsigned int CurrentControlEnable;
extern VECTOR Ifbk;
extern VECTOR Icmd;

inline VECTOR ComputeCurrentError(VECTOR Cmd, VECTOR Fbk)
{
    VECTOR Err;
    Err.Axis1 = Cmd.Axis1 - Fbk.Axis1;
    Err.Axis2 = Cmd.Axis2 - Fbk.Axis2;
    Err.Axis3 = Cmd.Axis3 - Fbk.Axis3;

    return Err;
}

void CurrentControl();

#endif /* CURRENTCONTROL_H_ */
