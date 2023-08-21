/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file CurrentControl.c
* @brief Current Control loop
*
* This module controls currents based on current command
*
* @author Sarinx LLC
*******************************************************************************/

//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<<<<<<<<<< HEADER FILES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|

#include <cam_CurrentControl.h>

/*
#define COMPUTE_PI_ERROR(CMD, FBK, ERR) \
    do { ERR.Axis1 = CMD.Axis1 - FBK.Axis1; \
         ERR.Axis2 = CMD.Axis2 - FBK.Axis2; \
         ERR.Axis3 = CMD.Axis3 - FBK.Axis3; } while(0);
*/

#define Knorm       (1.0)
#define Ki2         (1.0)
#define Wr          (6283.2)
#define Ki2_BY_Wr   (Ki2/Wr)
#define Tdsp        (0.00004)
#define Tdsp_half   (0.00004/2.0)
#define Kp          (4.9299)

#define Vr_grid     (391.92)
#define Vr_dclink   (400.0)
#define Trti_by_3   ((Vr_grid/Vr_dclink)*SQRT3BY2)

VECTOR Ifbk;
VECTOR Icmd;
unsigned int CurrentControlEnable;

//----------------------------------------------------------------------
//  CurrentControl
//----------------------------------------------------------------------
void CurrentControl()
{
    VECTOR Ierr, IerrTemp;
    static VECTOR Ierr_ki, Ierr_kp;
    VECTOR GridVoltageDQZ_Trti;
    static VECTOR Ierr_ki_fwd, Ierr_ki_bck;

    if(CurrentControlEnable)
    {
        Ierr = ComputeCurrentError(Icmd, Ifbk);

        Ierr = VectorGainCompute(Ierr, Knorm);

        Ierr = VectorGainCompute(Ierr, Ki2_BY_Wr);

        // Calculate Integral
        IerrTemp = VectorGainCompute(Ierr, Wr);

        Ierr_ki = VectorSum(IerrTemp, Ierr_ki_fwd);

        Ierr_ki_fwd.Axis1 = IerrTemp.Axis1;
        Ierr_ki_fwd.Axis2 = IerrTemp.Axis2;
        Ierr_ki_fwd.Axis3 = IerrTemp.Axis3;

        IerrTemp = VectorGainCompute(Ierr_ki, Tdsp_half);

        Ierr_ki = VectorSum(IerrTemp, Ierr_ki_bck);

        Ierr_ki = VectorSaturate(Ierr_ki, 1.0, -1.0);

        Ierr_ki_bck.Axis1 = Ierr_ki.Axis1;
        Ierr_ki_bck.Axis2 = Ierr_ki.Axis2;
        Ierr_ki_bck.Axis3 = Ierr_ki.Axis3;

        // Calculate Proportional
        Ierr_kp = VectorCrossProduct(GridVelocityVectDQZ, Ierr_ki);

        Ierr_kp = VectorSum(Ierr_kp, Ierr);

        Ierr_kp = VectorGainCompute(Ierr_kp, Kp);

        // Calculate SVMcmd
        SpaceVectorDQZ = VectorSum(Ierr_kp, Ierr_ki);

        SpaceVectorDQZ = VectorSaturate(SpaceVectorDQZ, 1.0, -1.0);

        SpaceVectorDQZ.Axis1 = 0.0 - SpaceVectorDQZ.Axis1;
        SpaceVectorDQZ.Axis2 = 0.0 - SpaceVectorDQZ.Axis2;
        SpaceVectorDQZ.Axis3 = 0.0 - SpaceVectorDQZ.Axis3;

        GridVoltageDQZ_Trti = VectorGainCompute(GridVoltageDQZ, Trti_by_3);

        SpaceVectorDQZ = VectorSum(GridVoltageDQZ_Trti, SpaceVectorDQZ);

        SpaceVectorDQZ = VectorSaturate(SpaceVectorDQZ, 1.0, -1.0);
    }
    else
    {
        Ierr_ki.Axis1 = Ierr_ki.Axis2 = Ierr_ki.Axis3 = 0;
        Ierr_ki_fwd.Axis1 = Ierr_ki_fwd.Axis2 = Ierr_ki_fwd.Axis3 = 0;
        Ierr_ki_bck.Axis1 = Ierr_ki_bck.Axis2 = Ierr_ki_bck.Axis3 = 0;
        SpaceVectorDQZ.Axis1 = SpaceVectorDQZ.Axis2 = SpaceVectorDQZ.Axis3 = 0;
    }
}

// end of file
