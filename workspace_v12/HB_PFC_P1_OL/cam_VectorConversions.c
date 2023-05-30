/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file clam_VectorCoversions.c
* @brief performs vector conversions
*
* This module computes vector conversion support functions
*
* @author Sarinx LLC
*******************************************************************************/

#include <cam_VectorConversions.h>


VECTOR VectDQZtoRXZ(VECTOR VectDQZ, float theta)
{
    float temp_sin, temp_cos;
    VECTOR VectRXZ;

    temp_sin = __sinpuf32(theta);
    temp_cos = __cospuf32(theta);

    VectRXZ.Axis1 = ( temp_cos * VectDQZ.Axis1) + (-temp_sin * VectDQZ.Axis2) + (0 * VectDQZ.Axis3);
    VectRXZ.Axis2 = ( temp_sin * VectDQZ.Axis1) + ( temp_cos * VectDQZ.Axis2) + (0 * VectDQZ.Axis3);
    VectRXZ.Axis3 = (     0 * VectDQZ.Axis1   ) + (    0 * VectDQZ.Axis2   ) +  (1 * VectDQZ.Axis3);

    return VectRXZ;
}

//(3/2)*[1, -1/2, -1/2;   0, sqrt(3)/2, -sqrt(3)/2; 1/sqrt(2), 1/sqrt(2), 1/sqrt(2);]^-1
VECTOR VectRXZtoABC(VECTOR VectRXZ)
{
    VECTOR VectABC;

    VectABC.Axis1 =  ( (       1.0 * VectRXZ.Axis1) + (       0.0 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;
    VectABC.Axis2 =  ( (      -0.5 * VectRXZ.Axis1) + (  SQRT3BY2 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;
    VectABC.Axis3 =  ( (      -0.5 * VectRXZ.Axis1) + ( -SQRT3BY2 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;

    return VectABC;
}





