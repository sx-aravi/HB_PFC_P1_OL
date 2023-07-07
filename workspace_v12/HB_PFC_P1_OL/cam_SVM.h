/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file clam_SVM.c
* @brief space vector modulation
*
* This module computes space vector transition times based on regions & sectors
*
* @author Sarinx LLC
*******************************************************************************/


#ifndef CAM_SVM_H_
#define CAM_SVM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "cam_VectorConversions.h"
#include "gpio.h"
#include "pdu_PwmUpdate.h"


//
// Defines
//
#define BUFFER_SIZE       64
#define TABLE_SIZE        64
#define TABLE_SIZE_M_1    TABLE_SIZE-1
#define PIBYTWO           1.570796327
#define PI                3.141592653589
#define INV2PI            0.159154943
#define TWOPI             6.283


// For 50 KHz
#define TBASE             0.00002

//For 100 Khz
//#define TBASE           0.00001

//
// Globals
//


extern uint16_t ClaTaskCounter;

extern float AngleValue;
extern float WrDQZ;
extern short Segment;
extern short Region;

extern float DlogCh1_SVM;
extern float DlogCh2_SVM;

extern VECTOR SpaceVectorRXZ;
extern VECTOR SpaceVectorDQZ;
extern VECTOR SpaceVector3Phase;
extern VECTOR SpaceVectorTransitionTime;
extern VECTOR SVMOutputTimes;

// CCS Scope Buffers
//extern float GraphFloatBuffer[50];
//extern uint16_t GraphIntBuffer[3][50];

//
// Function Prototypes
//
void svm(void);
void GenerateSpaceVectorRXZ();
short DetectSegment(VECTOR);
void DetectSegmentnSegmentID(VECTOR);
void DetectRegion();
void CounterRotateSpaceVectorRXZ();
void CalculateTransitions();
void RotateSpaceVectorRXZ();
VECTOR RotateRXZby30Degree(VECTOR);
void ConvertRXZtoThreePhase();
void ConvertRotatedRXZtoThreePhase(VECTOR);

// Variables for external control via CAN
extern short InverterState;
extern short GpioFaultResetBit;
extern short GpioEnableBit;

typedef enum
{
    OFF,
    ON,
    UNKNOWN
}INVERTER_STATE;



#ifdef __cplusplus
}
#endif // extern "C"

#endif /* CAM_SVM_H_ */
