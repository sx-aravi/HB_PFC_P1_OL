/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file siu_CLA.h
*
* This module define CLA tasks for all control algorithms
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef SIU_CLA_H_
#define SIU_CLA_H_

#include "F2837xD_Cla_defines.h"
#include "F2837xD_Cla_typedefs.h"
#include "F28x_Project.h"
//#include "driverlib.h"
#include "device.h"
#include <stdint.h>





//
//Task 2 (C) Variables
//

//
//Task 3 (C) Variables
//

//
//Task 4 (C) Variables
//

//
//Task 5 (C) Variables
//

//
//Task 6 (C) Variables
//

//
//Task 7 (C) Variables
//

//
//Task 8 (C) Variables
//



extern void siu_InitCla(void);

void CLA_initCpu1Cla1(void);


__interrupt void cla1Isr1();
__interrupt void cla1Isr2();
__interrupt void cla1Isr3();
__interrupt void cla1Isr4();
__interrupt void cla1Isr5();
__interrupt void cla1Isr6();
__interrupt void cla1Isr7();
__interrupt void cla1Isr8();

#endif /* SIU_CLA_H_ */
