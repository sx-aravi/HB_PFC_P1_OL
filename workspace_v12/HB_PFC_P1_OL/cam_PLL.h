/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file cam_PLL.h
* @brief process the input voltage and phase to perform phase locked loop functions
*
* This module is part of control law application modules to perform PLL specific functions
*
* Created on: June 10, 2023
* @author Sarinx LLC
*******************************************************************************/

#ifndef CAM_PLL_H_
#define CAM_PLL_H_


#include <cam_VectorConversions.h>
#include <cam_SVM.h>

#define Z_PLL                       (0.707)
#define W_PLL                       (2 * 3.14 * 180)
#define WR                          (2 * 3.14 * 1000)
#define INV_WR                      (1 / WR)
#define W_PLL_BY_WR                 (W_PLL / WR)
#define W_PLL_TIMES_W_PLL_BY_WR     (W_PLL * W_PLL / WR)
#define Z_PLL_BY_W_PLL              (Z_PLL / W_PLL)
#define W_PLL_BY_2Z_PLL             (W_PLL / (2.0 * Z_PLL))

//extern float AngleValue;
extern float theta_pll;
extern float VelocityDQZ;
extern VECTOR VoltageCap3Phase;
extern VECTOR VoltageCapRXZ;
extern VECTOR VoltageCapDQZ;
extern VECTOR GridVelocityVectDQZ;

void phaselock(void);
VECTOR NormalizeVoltageCapDQZ(VECTOR VectDQZ, float VectDQZ_magnitude);
float UpdatePLLRegulator(float input);
float GenerateGridVelocityDQZ(float inputVelocity);


#endif /* CAM_PLL_H_ */
