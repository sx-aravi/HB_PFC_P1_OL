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

#define TWOBYTHREE      (0.66666)
#define ONEBYSQRT2      ( 0.70721 )
#define SQRT3BY2        ( 0.866602)
#define PMW_PERIOD      ( 50e-6   )
#define TWOBYSQRT3      (1.1547005383)
#define ONEBYSQRT3      (0.5773502691896258)



extern VECTOR VectDQZtoRXZ(VECTOR VectDQZ, float theta);
extern VECTOR VectRXZtoABC(VECTOR VectRXZ);
extern VECTOR VectRXZtoABCSin(VECTOR VectRXZ);
extern VECTOR RotateRXZby30Degree(VECTOR VectorRXZ);

extern VECTOR VectABCtoRXZ(VECTOR VectABC);
extern VECTOR VectRXZtoDQZ(VECTOR VectRXZ, float theta);

extern float ComputeVectorMagnitude(VECTOR VectDQZ);
extern VECTOR VectorCrossProduct(VECTOR vect1, VECTOR vect2);




#endif /* CAM_VECTORCONVERSIONS_H_ */
