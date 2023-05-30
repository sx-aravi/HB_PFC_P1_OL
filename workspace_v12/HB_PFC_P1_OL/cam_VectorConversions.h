/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file clam_VectorCoversions.h
* @brief performs vector conversions
*
* This module computes vector conversion support functions
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef CAM_VECTORCONVERSIONS_H_
#define CAM_VECTORCONVERSIONS_H_

typedef struct
{
    float Axis1;    /**< Phase A or Axis 1 */
    float Axis2;    /**< Phase B or Axis 2 */
    float Axis3;    /**< Phase C or Axis 3 */
}VECTOR;

#define ONEBYSQRT2      ( 0.70721 )
#define SQRT3BY2        ( 0.866602)
#define PMW_PERIOD      ( 50e-6   )



extern VECTOR VectDQZtoRXZ(VECTOR VectDQZ, float theta);
extern VECTOR VectRXZtoABC(VECTOR VectRXZ);




#endif /* CAM_VECTORCONVERSIONS_H_ */
