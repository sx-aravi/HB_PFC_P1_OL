/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file pfc_board.c
* @brief PFC control board configuration & initialization
*
* This module provides the functions requried for configuring & initializing
* the PFC control board I/O
*
* @author Sarinx LLC
*******************************************************************************/

#include <siu_PfcBoard.h>

//*****************************************************************************
//
// Board Configurations
// Initializes the rest of the modules. 
// Call this function in your application if you wish to do all module 
// initialization.
// If you wish to not use some of the initializations, instead of the 
// Board_init use the individual Module_inits
//
//*****************************************************************************
void siu_ConfigurePfcBoard()
{
    EALLOW;
	ddi_ConfigureGPIOs();
	ddi_ConfigureCAN();
	ddi_ConfigSpiGPIOs();
	ddi_ConfigureADCs();
	SYNC_init();
	EDIS;
}


//*****************************************************************************
//
// SYNC Scheme Configurations
//
//*****************************************************************************
void SYNC_init()
{
	SysCtl_setSyncOutputConfig(SYSCTL_SYNC_OUT_SRC_EPWM1SYNCOUT);
	//
	// For EPWM1, the sync input is: SYSCTL_SYNC_IN_SRC_EXTSYNCIN1
	//
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM7, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_EPWM10, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP1, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	SysCtl_setSyncInputConfig(SYSCTL_SYNC_IN_ECAP4, SYSCTL_SYNC_IN_SRC_EPWM1SYNCOUT);
	//
	// SOCA
	//
	//SysCtl_enableExtADCSOCSource(0);
	//
	// SOCB
	//
	//SysCtl_enableExtADCSOCSource(0);
}


//*****************************************************************************
//
// GPIO PinMux Configurations
//
//*****************************************************************************




