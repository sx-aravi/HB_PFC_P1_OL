/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file ddi_SPI.h
* @brief SPI bus device driver interface header
*
* This module is wrapper for SPI bus TI device driver library for specific functions
*
* Created on: May 21, 2023
* @author Sarinx LLC
*******************************************************************************/

#ifndef DDI_SPI_H_
#define DDI_SPI_H_

#include "device.h"
#include "driverlib.h"


extern uint16_t DacSendDataBuf[2];         // Send data buffer
extern uint16_t DacReceiveDataBuf[2];      // Receive data buffer


void ddi_InitializeSpi(void);
void ddi_ConfigSpiGPIOs(void);
void ddi_InitializeSpiMaster(uint32_t spi_base);
void ddi_InitializeSpiSlave(uint32_t spi_base);


#endif /* DDI_SPI_H_ */
