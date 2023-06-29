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

    //Multiply TI Sine/Cosine outputs by 2/3 or 0.666667

    //temp_sin = 0.6667 * __sinpuf32(theta);
    //temp_cos = 0.6667 * __cospuf32(theta);

    temp_sin =  __sinpuf32(theta);
    temp_cos =  __cospuf32(theta);


    VectRXZ.Axis1 = ( temp_cos * VectDQZ.Axis1) + (-temp_sin * VectDQZ.Axis2) + (0 * VectDQZ.Axis3);
    VectRXZ.Axis2 = ( temp_sin * VectDQZ.Axis1) + ( temp_cos * VectDQZ.Axis2) + (0 * VectDQZ.Axis3);
    VectRXZ.Axis3 = (     0 * VectDQZ.Axis1   ) + (    0 * VectDQZ.Axis2   ) +  (1 * VectDQZ.Axis3);

    return VectRXZ;
}

/*(3/2)*[1, -1/2, -1/2;
        0, sqrt(3)/2, -sqrt(3)/2;
        1/sqrt(2), 1/sqrt(2), 1/sqrt(2);]
        */
VECTOR VectRXZtoABC(VECTOR VectRXZ)
{
    VECTOR VectABC;

    VectABC.Axis1 =  ( (       1.0 * VectRXZ.Axis1) + (       0.0 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;
    VectABC.Axis2 =  ( (      -0.5 * VectRXZ.Axis1) + (  SQRT3BY2 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;
    VectABC.Axis3 =  ( (      -0.5 * VectRXZ.Axis1) + ( -SQRT3BY2 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * 1.5;

    return VectABC;
}

/*
|        0,         1,   1/sqrt(2); |
| -sqrt(3)/2,   -1/2,    1/sqrt(2); |
| sqrt(3)/2,     -1/2    1/sqrt(2); |
*/

VECTOR VectRXZtoABCSin(VECTOR VectRXZ)
{
    VECTOR VectABC;

    VectABC.Axis1 =  ( (         0 * VectRXZ.Axis1) + (     1 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * TWOBYSQRT3;
    VectABC.Axis2 =  ( ( -SQRT3BY2 * VectRXZ.Axis1) + (  -0.5 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * TWOBYSQRT3;
    VectABC.Axis3 =  ( (  SQRT3BY2 * VectRXZ.Axis1) + (  -0.5 * VectRXZ.Axis2) + ( ONEBYSQRT2 * VectRXZ.Axis3)) * TWOBYSQRT3;

    return VectABC;
}

//
/*******************************************************************************
*//**
* @brief Utility function to rotate a space vector by 30 degrees
* @param SpaceVectorRXZ             - space vector in RXZ to be rotated by 30 deg
* @retun SpaceVectorRXZ             - Space Vector with 30 deg rotation
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
// [ sqrt(3)/2, -1/2,  0;       1/2, sqrt(3)/2, 0;      0, 0, 1;]
//
VECTOR RotateRXZby30Degree(VECTOR VectorRXZ)
{
    VECTOR tempVect;

    tempVect.Axis1 =  ( ( SQRT3BY2 * VectorRXZ.Axis1) + (    -0.5 * VectorRXZ.Axis2) + (   0  * VectorRXZ.Axis3));
    tempVect.Axis2 =  ( (      0.5 * VectorRXZ.Axis1) + (SQRT3BY2 * VectorRXZ.Axis2) + (   0  * VectorRXZ.Axis3));
    tempVect.Axis3 =  ( (        0 * VectorRXZ.Axis1) + (       0 * VectorRXZ.Axis2) + (   1  * VectorRXZ.Axis3));

    return tempVect;
}

