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
#define ONEBYSQRT3  0.57735
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
float WrDQZ = 60;



short SegmentID;
//short RegionID;

float DlogCh1_SVM = 0;
float DlogCh2_SVM = 0;

VECTOR SpaceVectorDQZ = {0.75, 0, 0};
VECTOR SpaceVectorRXZ;
VECTOR CounterRotatedRXZ;
VECTOR TransitionTimes;
VECTOR SpaceVectorTransitionTime;
VECTOR SpaceVector3Phase;
VECTOR RotatedSpaceVector3Phase;

/** Vector to store the three axis of the three phase current / voltage
*   for control algorithm */

typedef enum
{
    FAULT_S,
    _0TO60,
    _60TO120,
    _120TO180,
    _180TO240,
    _240TO300,
    _300TO360
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
    REGION_2A,
    REGION_1B,
    REGION_2B,
    REGION_3A,
    REGION_4A,
    REGION_3B,
    REGION_4B,
    REGION_5A,
    REGION_6A,
    REGION_5B,
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


//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<< GLOBAL FUNCTION DEFINITIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|

void svm(void)
{
    VECTOR RotatedSpaceVectorRXZ;

    GenerateSpaceVectorRXZ();
    ConvertRXZtoThreePhase();
    Segment = DetectSegment(SpaceVector3Phase);
    RotatedSpaceVectorRXZ = RotateRXZby30Degree(SpaceVectorRXZ);
    ConvertRotatedRXZtoThreePhase(RotatedSpaceVectorRXZ);
    SegmentID = DetectSegment(RotatedSpaceVector3Phase);
    DetectRegion();
    CounterRotateSpaceVectorRXZ();
    CalculateTransitions();
    RotateSpaceVectorRXZ();

    DlogCh1_SVM = SpaceVectorTransitionTime.Axis1;
    DlogCh2_SVM = SpaceVectorTransitionTime.Axis3;

    pdpu_UpdateCompareReg(EPWM12_BASE,SpaceVectorTransitionTime, SpaceVector3Phase);

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

    switch (Segment)
    {

        case _0TO60: // (1/sqrt(3))*[0, 2, sqrt(2);   -sqrt(3), -1, sqrt(2);  sqrt(3), -1, sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = ((     0 * SpaceVectorRXZ.Axis1) + ( 2 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = (( SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

                break;
            }

        case _60TO120: // (1/sqrt(3))*[-sqrt(3), 1, -sqrt(2);  0, -2, -sqrt(2);   sqrt(3), 1, -sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = ((     0 * SpaceVectorRXZ.Axis1) + (-2 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = (( SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

                break;
            }

        case _120TO180: // (1/sqrt(3))*[-sqrt(3), -1, sqrt(2);   sqrt(3), -1, sqrt(2);  0, 2, sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = (( SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = ((     0 * SpaceVectorRXZ.Axis1) + ( 2 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

                break;
            }

        case _180TO240: // (-1/sqrt(3))*[0, 2, sqrt(2);   -sqrt(3), -1, sqrt(2);  sqrt(3), -1, sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = ((     0 * SpaceVectorRXZ.Axis1) + (-2 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = (( SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

                //BiasOut = BiasIn;
                break;
            }

        case _240TO300: // (-1/sqrt(3))*[-sqrt(3), 1, -sqrt(2);  0, -2, -sqrt(2);   sqrt(3), 1, -sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = (( SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = ((     0 * SpaceVectorRXZ.Axis1) + ( 2 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + (-1 * SpaceVectorRXZ.Axis2) + (SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

                break;
            }

        case _300TO360: // (-1/sqrt(3))*[-sqrt(3), -1, sqrt(2);   sqrt(3), -1, sqrt(2);  0, 2, sqrt(2);]
            {
                CounterRotatedRXZ.Axis1 = (( SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis2 = ((-SQRT3 * SpaceVectorRXZ.Axis1) + ( 1 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;
                CounterRotatedRXZ.Axis3 = ((     0 * SpaceVectorRXZ.Axis1) + (-2 * SpaceVectorRXZ.Axis2)  + (-SQRT2 * SpaceVectorRXZ.Axis3)) * ONEBYSQRT3;

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
    VECTOR TransitionVector;
    float TransitionBias;
    short region_index;

    short TransitionLookUpTable [7][7] = {{0,0,0,0,0,0,0},
                                        {0,1,2,5,8,9,12},
                                        {0,3,4,7,6,11,10},
                                        {0,1,2,5,8,9,12},
                                        {0,3,4,7,6,11,10},
                                        {0,1,2,5,8,9,12},
                                        {0,3,4,7,6,11,10}
                                       };

    region_index = TransitionLookUpTable[SegmentID][Region];

    switch (region_index)
    {

        case REGION_1A: //case 1 -  [ 0, 0, -1;   2, 0, 1;   0, 0, 1;], + [1; 0 ; 0;]
            {
                TransitionVector.Axis1 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis2 = ((     2 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) + 0;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) + 0;

                break;
            }
        case REGION_1B: //case 3 - [ 0, 0, -1;   -2, 0, -1;   0, 0, 1;], + [1; 1 ; 0;]
            {
                TransitionVector.Axis1 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis2 = ((    -2 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) + 0;

                break;
            }

        case REGION_2A: //case 2 - [ -1, 0, 0;   1, 0, 2;   1, 0,  0;], + [1; 0 ; 0;]
            {
                TransitionVector.Axis1 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (0 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis2 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (2 * CounterRotatedRXZ.Axis3)) + 0;
                TransitionVector.Axis3 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (0 * CounterRotatedRXZ.Axis3)) + 0;

                break;
            }

        case REGION_2B: //case 4 - [ -1, 0, 0;   -1, 0, -2;   1, 0,  0;], + [1; 1 ; 0;]
            {
                TransitionVector.Axis1 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis2 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-2 * CounterRotatedRXZ.Axis3)) + 1;
                TransitionVector.Axis3 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0;

                break;
            }
        case REGION_3A: //case 5 - [ 1, 0, 0;   -1, 0, -2;   -1, 0, 0;], + [1; 3 ; 1;]/2
            {
                TransitionVector.Axis1 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0.5;
                TransitionVector.Axis2 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-2 * CounterRotatedRXZ.Axis3)) + 1.5;
                TransitionVector.Axis3 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0.5;

                break;
            }

        case REGION_3B: //case 7 - [ 1, 0, 0;   1, 0, 2;   -1, 0, 0;], + [1; -1 ; 1;]/2
            {
                TransitionVector.Axis1 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0.5;
                TransitionVector.Axis2 = ((     1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 2 * CounterRotatedRXZ.Axis3)) - 0.5;
                TransitionVector.Axis3 = ((    -1 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0.5;

                break;
            }

       case REGION_4A: //case 6 - [ 0, 0, 1;   2, 0, 1;   0, 0, -1;], + [1; -1 ; 1;]/2
           {
                TransitionVector.Axis1 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) + 0.5;
                TransitionVector.Axis2 = ((     2 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) - 0.5;
                TransitionVector.Axis3 = ((     0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 0.5;

                break;
           }

       case REGION_4B: //case 8 - [ 0, 0, 1;   -2, 0, -1;   0, 0, -1;], + [1; 3 ; 1;]/2
           {
               TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + ( 1 * CounterRotatedRXZ.Axis3)) + 0.5;
               TransitionVector.Axis2 = ((   -2 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 1.5;
               TransitionVector.Axis3 = ((    0 * CounterRotatedRXZ.Axis1) + (0 * CounterRotatedRXZ.Axis2)  + (-1 * CounterRotatedRXZ.Axis3)) + 0.5;

               break;
           }

       case REGION_5A: //case 9 - [ 0, -1, 0;   0, 1, 2;   0,  1, 0;], + [0; 0 ; 1;]
           {
               TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis2 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 2 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis3 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 1;

               break;
           }

       case REGION_5B: //case 11 - [ 0, -1, 0;   0, -1, -2;   0,  1, 0;], + [0; 1 ; 1;]
           {
               TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis2 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + (-2 * CounterRotatedRXZ.Axis3)) + 1;
               TransitionVector.Axis3 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 1;

               break;
           }

       case REGION_6A: //case 10 - [ 0, -1, 0;   0, 1, 2;   0,  1, 0;], + [0; 1 ; 1;]
           {
               TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis2 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 2 * CounterRotatedRXZ.Axis3)) + 1;
               TransitionVector.Axis3 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 1;

               break;
           }

       case REGION_6B: //case 12 - [ 0, -1, 0;   0, -1, -2;   0,  1, 0;], + [0; 0 ; 1;]
           {
               TransitionVector.Axis1 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis2 = ((    0 * CounterRotatedRXZ.Axis1) + (-1 * CounterRotatedRXZ.Axis2)  + (-2 * CounterRotatedRXZ.Axis3)) + 0;
               TransitionVector.Axis3 = ((    0 * CounterRotatedRXZ.Axis1) + ( 1 * CounterRotatedRXZ.Axis2)  + ( 0 * CounterRotatedRXZ.Axis3)) + 1;

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

    short RotationLookUpTableIndex;

    short RotationLookUpTable [7][7] = {{0,0,0,0,0,0,0},
                                      {0,1,2,6,3,5,4},
                                      {0,2,3,1,4,6,5},
                                      {0,3,4,2,5,1,6},
                                      {0,4,5,3,6,2,1},
                                      {0,5,6,4,1,3,2},
                                      {0,6,1,5,2,4,3}
                                     };

    RotationLookUpTableIndex = RotationLookUpTable [SegmentID][Region];

    switch (RotationLookUpTableIndex)
    {

        case ABCtoACB: //case1 - [ 1, 0,  0;       0, 0, 1;      0, 1, 0;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));

            break;
        }

        case ABCtoABC: //case2 - [ 1, 0,  0;       0, 1, 0;      0, 0, 1;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));

            break;
        }

        case ABCtoBAC: //case3 - [ 0, 1,  0;       1, 0, 0;      0, 0, 1;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));

            break;
        }

        case ABCtoCAB: //case4 - [ 0, 0,  1;       1, 0, 0;      0, 1, 0;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));

            break;
        }

        case ABCtoCBA: //case5 - [ 0, 0,  1;       0, 1, 0;      1, 0, 0;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));

            break;
        }

        case ABCtoBCA: //case6 - [ 0, 1,  0;       0, 0, 1;      1, 0, 0;]
        {
            SpaceVectorTransitionTime.Axis1 = ((  0 * TransitionTimes.Axis1) + ( 1 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis2 = ((  0 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 1 * TransitionTimes.Axis3));
            SpaceVectorTransitionTime.Axis3 = ((  1 * TransitionTimes.Axis1) + ( 0 * TransitionTimes.Axis2) + ( 0 * TransitionTimes.Axis3));

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

        AngleValue = AngleValue + 1.0;
    }

    if (AngleValue >= 0.5)
    {
        //GPIO_writePin(37,1);
    }

    if (AngleValue <= 0.5)
    {
        //GPIO_writePin(37,0);
    }


    //%RotationMatrixDQtoRXZ;
    SpaceVectorRXZ = VectDQZtoRXZ(SpaceVectorDQZ, AngleValue);

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
    SpaceVector3Phase = VectRXZtoABC(SpaceVectorRXZ);

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
    VECTOR tempVectRXZ, tempVect3Ph;

    switch (SegmentID)
    {
        case _0TO60: //0 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (1 * SpaceVectorRXZ.Axis1) + (0     * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( (0 * SpaceVectorRXZ.Axis1) + (1     * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (0 * SpaceVectorRXZ.Axis1) + (0     * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }

        case _60TO120: //0 to -60 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (       0.5 * SpaceVectorRXZ.Axis1) + ( SQRT3BY2  * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( ( -SQRT3BY2 * SpaceVectorRXZ.Axis1) + (      0.5  * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (         0 * SpaceVectorRXZ.Axis1) + (        0  * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }
        case _120TO180: //0 to -120 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (      -0.5 * SpaceVectorRXZ.Axis1) + ( SQRT3BY2  * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( ( -SQRT3BY2 * SpaceVectorRXZ.Axis1) + (     -0.5  * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (         0 * SpaceVectorRXZ.Axis1) + (        0  * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }

        case _180TO240: //0 to -180 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (      -1 * SpaceVectorRXZ.Axis1) + (        0    * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( (       0 * SpaceVectorRXZ.Axis1) + (       -1    * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (       0 * SpaceVectorRXZ.Axis1) + (        0    * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }

        case _240TO300: //0 to -240 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (     -0.5 * SpaceVectorRXZ.Axis1) + (  -SQRT3BY2 * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( ( SQRT3BY2 * SpaceVectorRXZ.Axis1) + (       -0.5 * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (        0 * SpaceVectorRXZ.Axis1) + (          0 * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }

        case _300TO360: //0 to -300 deg rotation
            {
                tempVectRXZ.Axis1 =  ( (      0.5 * SpaceVectorRXZ.Axis1) + (   -SQRT3BY2 * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis2 =  ( ( SQRT3BY2 * SpaceVectorRXZ.Axis1) + (         0.5 * SpaceVectorRXZ.Axis2) + (   0  * SpaceVectorRXZ.Axis3));
                tempVectRXZ.Axis3 =  ( (        0 * SpaceVectorRXZ.Axis1) + (           0 * SpaceVectorRXZ.Axis2) + (   1  * SpaceVectorRXZ.Axis3));

                break;
            }
        default:
            {
                tempVectRXZ.Axis1 = 0;
                tempVectRXZ.Axis2 = 0;
                tempVectRXZ.Axis3 = 0;

                break;
            }
    }

    // subtract [1/2;0;0]
    tempVectRXZ.Axis1 = tempVectRXZ.Axis1 - 0.5;
    tempVectRXZ.Axis2 = tempVectRXZ.Axis2 - 0;
    tempVectRXZ.Axis3 = tempVectRXZ.Axis3 - 0;

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




