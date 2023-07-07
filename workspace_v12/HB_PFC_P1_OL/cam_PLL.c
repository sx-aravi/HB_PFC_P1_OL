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

#include <cam_PLL.h>


float magABC = 0.5;
float freqABC = 0.4;
float theta_pll;

VECTOR RefVoltageVector = {1, 0, 0};
VECTOR GridVoltageDQZ   = {1, 0, 0};
VECTOR GridVelocityVectDQZ = {0, 0, 1};
VECTOR VoltageCap3Phase;
VECTOR VoltageCapRXZ;
VECTOR VoltageCapDQZ;
VECTOR NormalizedVoltageCapDQZ;
VECTOR PLLCrossVect;
float VelocityDQZ;

//----------------------------------------------------------------------
//  phaselock
//----------------------------------------------------------------------
void phaselock()
{
    float VoltageCapDQZ_Magnitude, VelocityOutput;

    VoltageCapRXZ = VectABCtoRXZ(VoltageCap3Phase);

    VoltageCapDQZ = VectRXZtoDQZ(VoltageCapRXZ, AngleValue);

    VoltageCapDQZ_Magnitude = ComputeVectorMagnitude(VoltageCapDQZ);

    NormalizedVoltageCapDQZ = NormalizeVoltageCapDQZ(VoltageCapDQZ, VoltageCapDQZ_Magnitude);

    PLLCrossVect = VectorCrossProduct(RefVoltageVector, NormalizedVoltageCapDQZ);

    VelocityOutput = UpdatePLLRegulator(PLLCrossVect.Axis3);

    GridVelocityVectDQZ.Axis3 = GenerateGridVelocityDQZ(VelocityOutput);

    VelocityDQZ = (VelocityOutput * WR);

    GridVoltageDQZ.Axis1 = NormalizedVoltageCapDQZ.Axis1;

//DlogCh1_SVM = VoltageCap3Phase.Axis1;
//DlogCh2_SVM = AngleValue;

}

//----------------------------------------------------------------------
//  GenerateGridVelocityDQZ
//----------------------------------------------------------------------
float GenerateGridVelocityDQZ(float inputVelocity)
{
    static float PrevGridVelocity = 0;
    static float GridVelocity;
    float VelocityError, scaledVelocityError, OutGridVelocity;

    VelocityError = inputVelocity - PrevGridVelocity;

    //W_pll/(2*Z_pll) * Ts
    scaledVelocityError = VelocityError * W_PLL_BY_2Z_PLL * PMW_PERIOD;

    GridVelocity = scaledVelocityError + PrevGridVelocity;

    if(GridVelocity > 1.5)
    {
        GridVelocity = 1.5;
    }
    else if(GridVelocity < -1.5)
    {
        GridVelocity = -1.5;
    }

    // Send out the previous value
    OutGridVelocity = PrevGridVelocity;

    PrevGridVelocity = GridVelocity;

    return OutGridVelocity;
}

//----------------------------------------------------------------------
//  UpdatePLLRegulator
//----------------------------------------------------------------------
float UpdatePLLRegulator(float inputpll)
{
    static float integPreviousVal, integPresentVal;
    float scaledinput, OutputVal;

    //(W_pll^2/Wr)
    scaledinput = inputpll * W_PLL_TIMES_W_PLL_BY_WR;

    integPresentVal = (scaledinput * PMW_PERIOD) + integPreviousVal;

    if(integPresentVal > 1.5)
    {
        integPresentVal = 1.5;
    }
    else if(integPresentVal < -1.5)
    {
        integPresentVal = -1.5;
    }

    // (2*Z_pll)/W_pll
    OutputVal = integPresentVal + (scaledinput * 2 * Z_PLL_BY_W_PLL);

    if(OutputVal > 1.5)
    {
        OutputVal = 1.5;
    }
    else if(OutputVal < -1.5)
    {
        OutputVal = -1.5;
    }

    return OutputVal;
}

//----------------------------------------------------------------------
//  NormalizeVoltageCapDQZ
//----------------------------------------------------------------------
VECTOR NormalizeVoltageCapDQZ(VECTOR VectDQZ, float VectDQZ_magnitude)
{
    float inverse_magnitude;
    VECTOR normalized_DQZ;

    if(VectDQZ_magnitude < 0.2)
    {
        VectDQZ_magnitude = 0.2;
    }
    else if(VectDQZ_magnitude > 2)
    {
        VectDQZ_magnitude = 2;
    }

    inverse_magnitude = 1/VectDQZ_magnitude;

    normalized_DQZ.Axis1 = ( VoltageCapDQZ.Axis1 * inverse_magnitude );
    normalized_DQZ.Axis2 = ( VoltageCapDQZ.Axis2 * inverse_magnitude );
    normalized_DQZ.Axis3 = ( VoltageCapDQZ.Axis3 * inverse_magnitude );

    return normalized_DQZ;

}



//
//End of file
//




