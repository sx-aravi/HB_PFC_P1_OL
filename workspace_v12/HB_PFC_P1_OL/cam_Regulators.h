/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file cam_Regulators.h
* @brief perform PLL, current and voltage regulators algorithms
*
* This module define CLA tasks for all control algorithms
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef CAM_REGULATORS_H_
#define CAM_REGULATORS_H_

#include <stdint.h>
#include "F2837xD_device.h"

//
// Defines
//

#define M           3
#define N           3

//
// Globals
//

//
//Task 1 (C) Variables
//

extern float Cpu1ToCla1Transfer[M][N];
extern float Cla1ToCpu1Transfer[M][N];

//
// Function Prototypes
//
// The following are symbols defined in the CLA assembly code
// Including them in the shared header file makes them
// .global and the main CPU can make use of them.
//
extern __interrupt void Cla1Task1();
extern __interrupt void Cla1Task2();
extern __interrupt void Cla1Task3();
extern __interrupt void Cla1Task4();
extern __interrupt void Cla1Task5();
extern __interrupt void Cla1Task6();
extern __interrupt void Cla1Task7();
extern __interrupt void Cla1Task8();

#endif /* CAM_REGULATORS_H_ */
