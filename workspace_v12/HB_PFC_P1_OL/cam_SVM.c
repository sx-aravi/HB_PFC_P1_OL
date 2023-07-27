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
#include <cam_SVM.h>
#include <cam_VectorConversions.h>
#include <math.h>
#include <stdlib.h>

//
//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<<<< PRE-PROCESSOR DIRECTIVES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|
//

#define SQRT2       1.41421
#define SQRT3       1.73205
//#define ONEBYSQRT3  0.57735
#define THREEBYSQRT2 2.1213203
//#define ONEBYSQRT2  0.70721
//#define SQRT3BY2    0.866602
//
// Globals
//
// Note that the globals defined in the .cla source are global to the cla source
// file.  i.e. they may be shared across tasks. All of the data shared between
// the CLA and the C28x CPU must be defined in the C (or C++) code, and not the
// CLA code.
//

float BiasOut;
float BiasIn;
float AngleValue;
short Segment;
short Region;
//float WrDQZ = 0.06;//0.4;
float WrDQZ = 60;
//float WrDQZ = -60;
short SegmentID;
//short RegionID;

short region_index;

float DlogCh1_SVM = 0;
float DlogCh2_SVM = 0;

VECTOR SpaceVectorDQZ = {0.866, 0, 0};
//VECTOR SpaceVectorDQZ = {0.65, 0, 0};
//VECTOR SpaceVectorDQZ = {0.5, 0, 0};
//VECTOR SpaceVectorDQZ = {0.433, 0, 0};
//VECTOR SpaceVectorDQZ = {0.217, 0, 0};

VECTOR TempSpaceVectorRXZ;

VECTOR SpaceVectorRXZ;
VECTOR CounterRotatedRXZ;
VECTOR TransitionTimes;
VECTOR SpaceVectorTransitionTime;
VECTOR SpaceVector3Phase;
VECTOR RotatedSpaceVector3Phase;
VECTOR CounterRotatedRXZ1;


VECTOR SpaceVector3FSin;
VECTOR TempDebugVector;
short TempDebugVar;

// Variables for external control via CAN
short InverterState = OFF;
short GpioFaultResetBit = 0;
short GpioEnableBit = 0;

VECTOR TransitionVector;


/** Vector to store the three axis of the three phase current / voltage
*   for control algorithm */

/*typedef enum
{
    FAULT_S,
    _0TO60,
    _60TO120,
    _120TO180,
    _180TO240,
    _240TO300,
    _300TO360
}SEGMENT_ID;*/


typedef enum
{
    FAULT_S,
    _330to030,
    _030to090,
    _090to150,
    _150to210,
    _210to270,
    _270to330
}SEGMENT_ID;


typedef enum
{
    FAULT_R,
    _30TO90,
    _90TO150,
    _150TO210,
    _210TO270,
    _270TO330,
    _330TO360
}REGION_ID;


typedef enum
{
    DEFAULT,
    REGION_1A,
    REGION_1B,
    REGION_2A,
    REGION_2B,
    REGION_3A,
    REGION_3B,
    REGION_4A,
    REGION_4B,
    REGION_5A,
    REGION_5B, //REGION_6A,
    REGION_6A, //REGION_5B,
    REGION_6B
}REGION_INDEX;  //RegionIndex

// zxcvb
typedef enum
{
    DEFAULT1,
    ABCtoACB,
    ABCtoABC,
    ABCtoBAC,
    ABCtoCAB,
    ABCtoCBA,
    ABCtoBCA
}ROTATE_INDEX;

typedef enum
{
    STATE_OFF,
    STATE_0TO30,
    STATE_30TO60,
    STATE_60TO90,
    STATE_90TO120,
    STATE_120TO150,
    STATE_150TO180,
    STATE_180TO210,
    STATE_210TO240,
    STATE_240TO270,
    STATE_270TO300,
    STATE_300TO330,
    STATE_330TO360
}STATE_NUM;

short Segment_s;
short SegmentID_s;
//short Polarity_ph[3];
short StateNum = 0;

struct PWMEnableTag
{
    short Upper_Phase_A_PWM_Enable;
    short Lower_Phase_A_PWM_Enable;
    short Upper_Phase_B_PWM_Enable;
    short Lower_Phase_B_PWM_Enable;
    short Upper_Phase_C_PWM_Enable;
    short Lower_Phase_C_PWM_Enable;
}PWMEnable;

//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<< GLOBAL FUNCTION DEFINITIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|

void svm(void)
{
    GenerateSpaceVectorRXZ();
    ConvertRXZtoThreePhase();
    DetectSegmentnSegmentID(SpaceVector3FSin);
    Segment=Segment_s;
    SegmentID=SegmentID_s;

    //DetectRegion();
    CounterRotateSpaceVectorRXZ();
    DetectRegion();
    CalculateTransitions();
    RotateSpaceVectorRXZ();

    DlogCh1_SVM = SpaceVectorTransitionTime.Axis1;//AngleValue;//SegmentID;//SpaceVectorTransitionTime.Axis2;
    DlogCh2_SVM = SpaceVectorTransitionTime.Axis2;//Region; //TransitionVector.Axis1;


  }

//local functions
//

/*******************************************************************************
*//**
* @brief Counter Rotation of Spacevector RXZ
* @param SpaceVectorRXZ     - Three phase input
* @param Segment            - space vector segment
* @retun CounterRotatedRXZ  - Space Vector RXZ with counter rotation
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//

void CounterRotateSpaceVectorRXZ()
{

    //SpaceVector3FSin = VectRXZtoABCSin(SpaceVectorRXZ);

    switch (SegmentID)
    {

        case _330to030: // [ 1, 0,  0;      0, 1, 0;      0, 0, 1;]
            {
                CounterRotatedRXZ.Axis1 = SpaceVector3FSin.Axis1;
                CounterRotatedRXZ.Axis2 = SpaceVector3FSin.Axis2;
                CounterRotatedRXZ.Axis3 = SpaceVector3FSin.Axis3;

                break;
            }

        case _030to090: // [ 0, 0,  -1;      -1, 0, 0;      0, -1, 0;]
            {

                CounterRotatedRXZ.Axis1 = -SpaceVector3FSin.Axis3;
                CounterRotatedRXZ.Axis2 = -SpaceVector3FSin.Axis1;
                CounterRotatedRXZ.Axis3 = -SpaceVector3FSin.Axis2;

                break;
            }

        case _090to150: // [ 0, 1,  0;      0, 0, 1;      1, 0, 0;]
            {

                CounterRotatedRXZ.Axis1 = SpaceVector3FSin.Axis2;
                CounterRotatedRXZ.Axis2 = SpaceVector3FSin.Axis3;
                CounterRotatedRXZ.Axis3 = SpaceVector3FSin.Axis1;


                break;
            }

        case _150to210: // [-1, 0, 0;      0, -1, 0;        0, 0, -1;]
            {

                CounterRotatedRXZ.Axis1 = -SpaceVector3FSin.Axis1;
                CounterRotatedRXZ.Axis2 = -SpaceVector3FSin.Axis2;
                CounterRotatedRXZ.Axis3 = -SpaceVector3FSin.Axis3;

                //BiasOut = BiasIn;
                break;
            }

        case _210to270: // [ 0, 0,  1;      1, 0, 0;      0, 1, 0;]
            {
                CounterRotatedRXZ.Axis1 = SpaceVector3FSin.Axis3;
                CounterRotatedRXZ.Axis2 = SpaceVector3FSin.Axis1;
                CounterRotatedRXZ.Axis3 = SpaceVector3FSin.Axis2;

                break;
            }

        case _270to330: // [ 0, -1, 0;      0, 0,  -1;    -1, 0, 0;]

            {

                CounterRotatedRXZ.Axis1 = -SpaceVector3FSin.Axis2;
                CounterRotatedRXZ.Axis2 = -SpaceVector3FSin.Axis3;
                CounterRotatedRXZ.Axis3 = -SpaceVector3FSin.Axis1;

                break;
            }

        default:
            {
                CounterRotatedRXZ.Axis1 = 0;
                CounterRotatedRXZ.Axis2 = 0;
                CounterRotatedRXZ.Axis3 = 0;

                 break;
            }
    }
}

//
/*******************************************************************************
*//**
* @brief Calculates space vector transitions
* @param CounterRotatedRXZ      - Counter rotated space vector RXZ
* @param BiasIn                 - Bias
* @param SegmentID              - space vector segment ID
* @param RegionID               - space vector region ID
* @retun TransitionTimes        - Space Vector transition times
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//  reshape([0,0,0,0,0,0,0,0,1,3,1,3,1,3,0,2,4,2,4,2,4,0,5,7,5,7,5,7,0,8,6,8,6,8,6,0,9,11,9,11,9,11,0,12,10,12,10,12,10],7,7)

void CalculateTransitions()
{

    float TransitionBias;



    short TransitionLookUpTable [7][7] = {{0,0,0,0,0,0,0},
                                          {0,1,3,5,8,9,11},
                                          {0,2,4,6,7,10,12},
                                          {0,1,3,5,8,9,11},
                                          {0,2,4,6,7,10,12},
                                          {0,1,3,5,8,9,11},
                                          {0,2,4,6,7,10,12}
                                         };

    region_index = TransitionLookUpTable[SegmentID][Region];


    TempDebugVar = region_index;

    CounterRotatedRXZ1.Axis1 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (0 * CounterRotatedRXZ.Axis3));
    CounterRotatedRXZ1.Axis2 = ((     0 * CounterRotatedRXZ.Axis1) + (1 * CounterRotatedRXZ.Axis2)  + (0 * CounterRotatedRXZ.Axis3));
    CounterRotatedRXZ1.Axis3 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (1 * CounterRotatedRXZ.Axis3));


    switch (region_index)
    {

        case REGION_1A: //case 1 -  [ 0, 0, -1;   2, 0, 1;   0, 0, 1;], + [1; 0 ; 0;]
            {
                TransitionVector.Axis1 = ((     0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis2 = ((    -2 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;

                break;
            }
        case REGION_1B: //case 3 - [ 0, 0, -1;   -2, 0, -1;   0, 0, 1;], + [1; 1 ; 0;]
            {
                TransitionVector.Axis1 = ((     0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis2 = ((     2 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;

                break;
            }

        case REGION_2A: //case 2 - [ -1, 0, 0;   1, 0, 2;   1, 0,  0;], + [1; 0 ; 0;]
            {
                TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ1.Axis1) + (-1 * CounterRotatedRXZ1.Axis2)  + (0 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis2 = ((    0 * CounterRotatedRXZ1.Axis1) + ( 1 * CounterRotatedRXZ1.Axis2)  + (0 * CounterRotatedRXZ1.Axis3)) + 1;
                TransitionVector.Axis3 = ((    2 * CounterRotatedRXZ1.Axis1) + ( 1 * CounterRotatedRXZ1.Axis2)  + (0 * CounterRotatedRXZ1.Axis3)) + 1;

                break;
            }

        case REGION_2B: //case 4 - [ -1, 0, 0;   -1, 0, -2;   1, 0,  0;], + [1; 1 ; 0;]
            {
                TransitionVector.Axis1 = ((    -2 * CounterRotatedRXZ1.Axis1) + (-1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis2 = ((     0 * CounterRotatedRXZ1.Axis1) + (-1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ1.Axis1) + ( 1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 1;

                break;
            }
        case REGION_3A: //case 5 - [ 1, 0, 0;   -1, 0, -2;   -1, 0, 0;], + [1; 3 ; 1;]/2
            {
                TransitionVector.Axis1 = ((    -1 * CounterRotatedRXZ1.Axis1) + (-2 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) - 0.5;
                TransitionVector.Axis2 = ((    -1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
                TransitionVector.Axis3 = ((     1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;

                break;
            }

        case REGION_3B: //case 7 - [ 1, 0, 0;   1, 0, 2;   -1, 0, 0;], + [1; -1 ; 1;]/2
            {
                TransitionVector.Axis1 = ((    -1 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
                TransitionVector.Axis2 = ((     1 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
                TransitionVector.Axis3 = ((     1 * CounterRotatedRXZ1.Axis1) + (2 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 1.5;

                break;
            }

       case REGION_4A: //case 6 - [ 0, 0, 1;   2, 0, 1;   0, 0, -1;], + [1; -1 ; 1;]/2
           {
                TransitionVector.Axis1 = ((    -1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
                TransitionVector.Axis2 = ((     1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
                TransitionVector.Axis3 = ((     1 * CounterRotatedRXZ1.Axis1) + ( 2 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 1.5;

                break;
           }

       case REGION_4B: //case 8 - [ 0, 0, 1;   -2, 0, -1;   0, 0, -1;], + [1; 3 ; 1;]/2
           {
               TransitionVector.Axis1 = ((   -1 * CounterRotatedRXZ1.Axis1) + (-2 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) - 0.5;
               TransitionVector.Axis2 = ((   -1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;
               TransitionVector.Axis3 = ((    1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0.5;

               break;
           }

       case REGION_5A: //case 9 - [ 0, 2, 0;   1, 1, -1;   0,  0, 0;], + [0; 0 ; 0;]
           {
               TransitionVector.Axis1 = ((       0 * CounterRotatedRXZ1.Axis1) + (-1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis2 = ((       0 * CounterRotatedRXZ1.Axis1) + ( 1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 1;
               TransitionVector.Axis3 = ((       1 * CounterRotatedRXZ1.Axis1) + ( 1 * CounterRotatedRXZ1.Axis2)  + ( 0 * CounterRotatedRXZ1.Axis3)) + 1;

               break;
           }

       case REGION_5B: //case 11 - [ 0, -1, 0;   0, -1, -2;   0,  1, 0;], + [0; 1 ; 1;]
           {
               TransitionVector.Axis1 = ((     -1 * CounterRotatedRXZ1.Axis1)   + (-1 * CounterRotatedRXZ1.Axis2)   + ( 0 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis2 = ((      0 * CounterRotatedRXZ1.Axis1)   + (-1 * CounterRotatedRXZ1.Axis2)   + ( 0 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis3 = ((      0 * CounterRotatedRXZ1.Axis1)   + ( 1 * CounterRotatedRXZ1.Axis2)   + ( 0 * CounterRotatedRXZ1.Axis3)) + 1;

               break;
           }

       case REGION_6A: //case 10 - [ 0, 0, 0;   0.5, 0.5, -0.5;   0,  1, 0;], + [0.5; 0.5 ; 0.5;]
           {
               TransitionVector.Axis1 = ((        0 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis2 = ((       -1 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;
               TransitionVector.Axis3 = ((        0 * CounterRotatedRXZ1.Axis1) + ( 0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;

               break;
           }

       case REGION_6B: //case 12 - [ 0, -1, 0;   0, -1, -2;   0,  1, 0;], + [0; 0 ; 1;]
           {
               TransitionVector.Axis1 = ((       0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis2 = ((       1 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + ( 1 * CounterRotatedRXZ1.Axis3)) + 0;
               TransitionVector.Axis3 = ((       0 * CounterRotatedRXZ1.Axis1) + (0 * CounterRotatedRXZ1.Axis2)  + (-1 * CounterRotatedRXZ1.Axis3)) + 1;

               break;
           }
    }


// test -1, otherwise 0
#if 1
    BiasIn = 0;
#endif

    TransitionBias = TransitionVector.Axis3 * BiasIn;
    TransitionTimes.Axis1 = TransitionVector.Axis1 + TransitionBias;
    TransitionTimes.Axis2 = TransitionVector.Axis2 + TransitionBias;
    TransitionTimes.Axis3 = TransitionVector.Axis3 + TransitionBias;
}

//
/*******************************************************************************
*//**
* @brief Calculates space vector transition time
* @param TransitionTimes            - Space vecdtor transition times
* @param SegmentID                  - space vector segment ID
* @param RegionID                   - space vector region ID
* @retun SpaceVectorTransitionTime  - Space Vector transition time which will be sent to PWM switches
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
// reshape([0,0,0,0,0,0,0,0,1,2,3,4,5,6,0,2,3,4,5,6,1,0,6,1,2,3,4,5,0,3,4,5,6,1,2,0,5,6,1,2,3,4,0,4,5,6,1,2,3],7,7)

void RotateSpaceVectorRXZ()
{

    switch(SegmentID)
    {

        case _330to030: //case1 - [ 1, 0,  0;       0, 0, 1;      0, 1, 0;]
        {
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis3;



            break;
        }

        case _030to090: //case2 - [ 1, 0,  0;       0, 1, 0;      0, 0, 1;]
        {
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis3;

            break;
        }

        case _090to150: //case3 - [ 0, 1,  0;       1, 0, 0;      0, 0, 1;]
        {
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis3;
            break;
        }

        case _150to210: //case4 - [ 0, 0,  1;       1, 0, 0;      0, 1, 0;]
        {
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis3;



            break;
        }

        case _210to270: //case5 - [ 0, 0,  1;       0, 1, 0;      1, 0, 0;]
        {
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis3;
            break;
        }

        case _270to330: //case6 - [ 0, 1,  0;       0, 0, 1;      1, 0, 0;]
        {
            SpaceVectorTransitionTime.Axis3 = TransitionTimes.Axis1;
            SpaceVectorTransitionTime.Axis1 = TransitionTimes.Axis2;
            SpaceVectorTransitionTime.Axis2 = TransitionTimes.Axis3;
            break;
        }

        default:
        {
            SpaceVectorTransitionTime.Axis1 = 0;
            SpaceVectorTransitionTime.Axis2 = 0;
            SpaceVectorTransitionTime.Axis3 = 0;

            break;
        }
   }
}

//
/*******************************************************************************
*//**
* @brief Generate Space Vector RXZ from angle and Space Vector DQZ
* @param WrDQZ                      - space vector angle (Omega)
* @param SpaceVectorDQZ             - space vector in DQZ
* @retun SpaceVectorRXZ             - Space Vector in RXZ
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//

void GenerateSpaceVectorRXZ()
{

    AngleValue = (WrDQZ * TBASE) + AngleValue;

    if (AngleValue >= 1.0)
    {
        AngleValue = AngleValue - 1.0;
    }
    else if (AngleValue < 0)
    {

        AngleValue = AngleValue + 1;
    }

    if (AngleValue >= 0.5)
    {
        //GPIO_writePin(37,1);
    }
    else
    {
        //GPIO_writePin(37,0);
    }

    //%RotationMatrixDQtoRXZ;
    SpaceVectorRXZ = VectDQZtoRXZ(SpaceVectorDQZ, AngleValue);


    TempSpaceVectorRXZ.Axis1 =  ( (  0 * SpaceVectorRXZ.Axis1) + (     1 * SpaceVectorRXZ.Axis2) + ( 0 * SpaceVectorRXZ.Axis3));
    TempSpaceVectorRXZ.Axis2 =  ( ( -1 * SpaceVectorRXZ.Axis1) + (     0 * SpaceVectorRXZ.Axis2) + ( 0 * SpaceVectorRXZ.Axis3));
    TempSpaceVectorRXZ.Axis3 =  ( (  0 * SpaceVectorRXZ.Axis1) + (     0 * SpaceVectorRXZ.Axis2) + ( 1 * SpaceVectorRXZ.Axis3));

    SpaceVectorRXZ.Axis1 =  TempSpaceVectorRXZ.Axis1;
    SpaceVectorRXZ.Axis2 =  TempSpaceVectorRXZ.Axis2;
    SpaceVectorRXZ.Axis3 =  TempSpaceVectorRXZ.Axis3;


}



//
/*******************************************************************************
*//**
* @brief Utility function to convert RXZ vector in to three phase vector
* @param SpaceVectorRXZ             - space vector in RXZ to be converted in to three phases
* @retun SpaceVector3Phase          - Space Vector in 3 phases
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//

void ConvertRXZtoThreePhase()
{

    //Cosine
    SpaceVector3Phase = VectRXZtoABC(SpaceVectorRXZ);

    //Sine
    SpaceVector3FSin = VectRXZtoABCSin(SpaceVectorRXZ);

    TempDebugVector.Axis1  = SpaceVector3FSin.Axis1;

}

//
/*******************************************************************************
*//**
* @brief Utility function to convert RXZ vector in to three phase vector
* @param SpaceVectorRXZ             - space vector in RXZ to be converted in to three phases
* @retun SpaceVector3Phase          - Space Vector in 3 phases
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//
void ConvertRotatedRXZtoThreePhase(VECTOR RotatedRXZ)
{
    RotatedSpaceVector3Phase = VectRXZtoABC(RotatedRXZ);

}


//
/*******************************************************************************
*//**
* @brief Detecs the space vector segment
* @param SpaceVector3Phase  - Space Vector in 3 phases
* @retun Segment            - Space Vector segment
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//

short DetectSegment(VECTOR vect3Phase)
{
    short Segment_t = 0;

    if ( vect3Phase.Axis1 >=  vect3Phase.Axis2)
    {
        Segment_t = Segment_t + (1 << 2);
    }
    if ( vect3Phase.Axis2 >=  vect3Phase.Axis3)
    {
        Segment_t = Segment_t + (1 << 1);
    }
    if ( vect3Phase.Axis3 >=  vect3Phase.Axis1)
    {
        Segment_t = Segment_t + (1 << 0);
    }


    switch (Segment_t)
        {
            case 1:
            {
                Segment_t = 4;
                break;
            }
            case 2:
            {
                Segment_t = 2;
                 break;
            }
            case 3:
            {
                Segment_t = 3;
                 break;
            }

            case 4:
             {
                 Segment_t = 6;
                  break;
             }

            case 5:
             {
                 Segment_t = 5;
                  break;
             }

            case 6:
             {
                 Segment_t = 1;
                  break;
             }
          }

    return Segment_t;
}

//
/*******************************************************************************
*//**
* @brief Detecs the space vector segment and segment ID
* @param SpaceVectorRXZ     - Space Vector in RXZ
* @param SegmentID          - Space Vector segment ID
* @retun void
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
void DetectSegmentnSegmentID(VECTOR vect3Phase)
{
    switch(StateNum)
    {
        case  STATE_OFF: //0
        {
            if((AngleValue > 0) && (AngleValue < 0.05) & (InverterState == ON))
            {
                StateNum = STATE_0TO30;
                Segment_s = 1;
                SegmentID_s = 1;
                /*PWMEnable.Upper_Phase_A_PWM_Enable = 1;
                PWMEnable.Lower_Phase_A_PWM_Enable = 0;
                PWMEnable.Upper_Phase_B_PWM_Enable = 0;
                PWMEnable.Lower_Phase_B_PWM_Enable = 1;
                PWMEnable.Upper_Phase_C_PWM_Enable = 0;
                PWMEnable.Lower_Phase_C_PWM_Enable = 1;*/

                EALLOW;

                EPwm12Regs.AQCSFRC.bit.CSFA = 0; // Upper Phase A Channel A Enable  - TP28
                EPwm11Regs.AQCSFRC.bit.CSFA = 0; // Lower Phase A Channel A Enable  - TP34
                EPwm10Regs.AQCSFRC.bit.CSFA = 0; // Upper Phase B Channel A Enable  - TP29
                EPwm9Regs.AQCSFRC.bit.CSFA = 0;  // Lower Phase B Channel A Enable  - TP35
                EPwm8Regs.AQCSFRC.bit.CSFA = 0;  // Upper Phase C Channel A Enable  - TP30
                EPwm7Regs.AQCSFRC.bit.CSFA = 0;  // Lower Phase C Channel A Enable  - TP36


                EPwm12Regs.AQCSFRC.bit.CSFB = 0;  // Upper Phase A Channel B Enable  - TP25
                EPwm11Regs.AQCSFRC.bit.CSFB = 1;  // Lower Phase A Channel B Disable - TP31
                EPwm10Regs.AQCSFRC.bit.CSFB = 1;  // Upper Phase B Channel B Disable - TP26
                EPwm9Regs.AQCSFRC.bit.CSFB = 0;   // Lower Phase B Channel B Enable  - TP32
                EPwm8Regs.AQCSFRC.bit.CSFB = 1;   // Upper Phase C Channel B Disable - TP27
                EPwm7Regs.AQCSFRC.bit.CSFB = 0;   // Lower Phase C Channel B Enable  - TP33

                EDIS;
            }

            //pdpu_Disable_All_Phases();

            break;
        }
        case STATE_0TO30: //1
        {

            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis1 > SpaceVector3FSin.Axis3)
                {
                    StateNum = STATE_30TO60;
                    SegmentID_s = 2;

                    pdpu_Enable_Phase_B_UpperChannel_B();
                }

                else if(SpaceVector3FSin.Axis1 < 0)
                {
                    StateNum = STATE_330TO360;
                    Segment_s = 6;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_30TO60: //2
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis3 < 0)
                {
                    StateNum = STATE_60TO90;
                    Segment_s = 2;
                }
                else if(SpaceVector3FSin.Axis1 < SpaceVector3FSin.Axis3)
                {
                    StateNum = STATE_0TO30;
                    SegmentID_s = 1;

                    pdpu_Enable_Phase_B_LowerChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_60TO90: //3
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis3 < SpaceVector3FSin.Axis2)
                {
                    StateNum = STATE_90TO120;
                    SegmentID_s = 3;

                    pdpu_Enable_Phase_A_LowerChannel_B();

                }
                else if(SpaceVector3FSin.Axis3 > 0)
                {
                    StateNum = STATE_30TO60;
                    Segment_s = 1;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }
            break;
        }
        case STATE_90TO120: //4
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis2 > 0)
                {
                    StateNum = STATE_120TO150;
                    Segment_s = 3;

                }
                else if(SpaceVector3FSin.Axis2 < SpaceVector3FSin.Axis3)
                {
                    StateNum = STATE_60TO90;
                    SegmentID_s = 2;

                    pdpu_Enable_Phase_A_UpperChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_120TO150: //5
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis2 > SpaceVector3FSin.Axis1)
                {
                    StateNum = STATE_150TO180;
                    SegmentID_s = 4;

                    pdpu_Enable_Phase_C_UpperChannel_B();

                }
                else if(SpaceVector3FSin.Axis2 < 0)
                {
                    StateNum = STATE_90TO120;
                    Segment_s = 2;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_150TO180: //6
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis1 < 0)
                {
                    StateNum = STATE_180TO210;
                    Segment_s = 4;

                }
                else if(SpaceVector3FSin.Axis2 < SpaceVector3FSin.Axis1)
                {
                    StateNum = STATE_120TO150;
                    SegmentID_s = 3;

                    pdpu_Enable_Phase_C_LowerChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_180TO210: //7
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis1 < SpaceVector3FSin.Axis3)
                {
                    StateNum = STATE_210TO240;
                    SegmentID_s = 5;

                    pdpu_Enable_Phase_B_LowerChannel_B();

                }
                else if(SpaceVector3FSin.Axis1 > 0)
                {
                    StateNum = STATE_150TO180;
                    Segment_s = 3;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_210TO240: //8
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis3 > 0)
                {
                    StateNum = STATE_240TO270;
                    Segment_s = 5;
                }
                else if(SpaceVector3FSin.Axis1 > SpaceVector3FSin.Axis3)
                {
                    StateNum = STATE_180TO210;
                    SegmentID_s = 4;

                    pdpu_Enable_Phase_B_UpperChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_240TO270: //9
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis3 > SpaceVector3FSin.Axis2)
                {
                    StateNum = STATE_270TO300;
                    SegmentID_s = 6;

                    pdpu_Enable_Phase_A_UpperChannel_B();

                }
                else if(SpaceVector3FSin.Axis3 < 0)
                {
                    StateNum = STATE_210TO240;
                    Segment_s = 4;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_270TO300: //10
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis2 < 0)
                {
                    StateNum = STATE_300TO330;
                    Segment_s = 6;
                }

                else if(SpaceVector3FSin.Axis3 < SpaceVector3FSin.Axis2)
                {
                    StateNum = STATE_240TO270;
                    SegmentID_s = 5;

                    pdpu_Enable_Phase_A_LowerChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_300TO330: //11
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis2 < SpaceVector3FSin.Axis1)
                {
                    StateNum = STATE_330TO360;
                    SegmentID_s = 1;

                    pdpu_Enable_Phase_C_LowerChannel_B();

                }
                else if(SpaceVector3FSin.Axis2 > 0)
                {
                    StateNum = STATE_270TO300;
                    Segment_s = 5;
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        case STATE_330TO360: //12
        {
            if(InverterState == ON)
            {
                if(SpaceVector3FSin.Axis1 > 0)
                {
                    StateNum = STATE_0TO30;
                    Segment_s = 1;

                }
                else if(SpaceVector3FSin.Axis2 > SpaceVector3FSin.Axis1)
                {
                    StateNum = STATE_300TO330;
                    SegmentID_s = 6;

                    pdpu_Enable_Phase_C_UpperChannel_B();
                }
            }
            else if(InverterState == OFF)
            {
                StateNum = STATE_OFF;
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

//
/*******************************************************************************
*//**
* @brief Detecs the space vector region
* @param SpaceVectorRXZ     - Space Vector in RXZ
* @param SegmentID          - Space Vector segment ID
* @retun Region             - Space Vector region
*
* Source is mapped to Matlab Model
* To be docuemnted
*
*
*******************************************************************************/
//

void DetectRegion()
{
    short Region_t = 0;;
    VECTOR tempVectRXZ, tempVect3Ph, New_CounterRotatedRXZ1;


    New_CounterRotatedRXZ1.Axis1 = ((         0 * CounterRotatedRXZ.Axis1) + (         -1 * CounterRotatedRXZ.Axis2)  + (          1 * CounterRotatedRXZ.Axis3)) * 0.5;
    New_CounterRotatedRXZ1.Axis2 = ((TWOBYSQRT3 * CounterRotatedRXZ.Axis1) + (-ONEBYSQRT3 * CounterRotatedRXZ.Axis2)  + (-ONEBYSQRT3 * CounterRotatedRXZ.Axis3)) * 0.5;
    New_CounterRotatedRXZ1.Axis3 = ((         0 * CounterRotatedRXZ.Axis1) + (          0 * CounterRotatedRXZ.Axis2)  + (          0 * CounterRotatedRXZ.Axis3)) * 0.5;


    // subtract [1/2;0;0]
    tempVectRXZ.Axis1 = New_CounterRotatedRXZ1.Axis1 - 0.5;
    tempVectRXZ.Axis2 = New_CounterRotatedRXZ1.Axis2;
    tempVectRXZ.Axis3 = New_CounterRotatedRXZ1.Axis3;


    //Convert RXZ to 3 Phase vector
    tempVect3Ph = VectRXZtoABC(tempVectRXZ);


    if ( tempVect3Ph.Axis1 >=  tempVect3Ph.Axis2)
        {
            Region_t = Region_t + (1 << 2);
        }
    if ( tempVect3Ph.Axis2 >=  tempVect3Ph.Axis3)
        {
            Region_t = Region_t + (1 << 1);
        }
    if ( tempVect3Ph.Axis3 >=  tempVect3Ph.Axis1)
        {
            Region_t = Region_t + (1 << 0);
        }

    switch (Region_t)
    {
        case 1:
            {
                Region = 2;
                break;
            }
        case 2:
            {
                Region = 3;
                break;
            }
        case 3:
            {
                Region = 1;
                break;
            }
        case 4:
            {
                Region = 6;
                break;
            }

        case 5:
            {
                Region = 4;
                break;
            }

        case 6:
            {
                Region = 5;
                break;
            }

        default:
            {
                Region = 0;
                break;
            }
    }

}

//
// End of file
//
