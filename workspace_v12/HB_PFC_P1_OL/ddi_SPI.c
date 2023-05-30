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

#include "ddi_SPI.h"


void ddi_InitializeSpi()
{

    //Initialize the SPI B & C as masters (for DAC & EEPROM)

    ddi_InitializeSpiMaster(SPIB_BASE);
    ddi_InitializeSpiMaster(SPIC_BASE);

    //ddi_InitializeSpiMaster(SPIC_BASE);

}


//
// Configure GPIOs for external loopback.
//
void ddi_ConfigSpiGPIOs(void)
{

    // External Connections for DAC SPI-C Bus:
    // -GPIO50 - SPIC_SIMO
    // -GPIO51 - SPIC_SOMI
    // -GPIO52 - SPIC_CLK
    // -GPIO53 - SPIC_STE
    //
    //
    // GPIO53 is the SPISTEC.
    //
    GPIO_setMasterCore(53, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_53_SPISTEC);
    GPIO_setPadConfig(53, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(53, GPIO_QUAL_ASYNC);


    //
    // GPIO52 is the SPICLKC.
    //
    GPIO_setMasterCore(52, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_52_SPICLKC);
    GPIO_setPadConfig(52, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(52, GPIO_QUAL_ASYNC);

    //
    // GPIO51 is the SPISOMIC.
    //
    GPIO_setMasterCore(51, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_51_SPISOMIC);
    GPIO_setPadConfig(51, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(51, GPIO_QUAL_ASYNC);

    //
    // GPIO50 is the SPISIMOC clock pin.
    //
    GPIO_setMasterCore(50, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_50_SPISIMOC);
    GPIO_setPadConfig(50, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(50, GPIO_QUAL_ASYNC);


    // External Connections for EEPROM SPI-B Bus:
    // -GPIO24 - SPIB_SIMO
    // -GPIO25 - SPIB_SOMI
    // -GPIO26 - SPIB_CLK
    // -GPIO27 - SPIB_STE
    //

    //
    // GPIO27 is the SPISTEB.
    //
    GPIO_setMasterCore(27, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_27_SPISTEB);
    GPIO_setPadConfig(27, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(27, GPIO_QUAL_ASYNC);


    //
    // GPIO26 is the SPICLKB.
    //
    GPIO_setMasterCore(26, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_26_SPICLKB);
    GPIO_setPadConfig(26, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(26, GPIO_QUAL_ASYNC);

    //
    // GPIO25 is the SPISOMIB.
    //
    GPIO_setMasterCore(25, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_25_SPISOMIB);
    GPIO_setPadConfig(25, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(25, GPIO_QUAL_ASYNC);

    //
    // GPIO24 is the SPISIMOB clock pin.
    //
    GPIO_setMasterCore(24, GPIO_CORE_CPU1);
    GPIO_setPinConfig(GPIO_24_SPISIMOB);
    GPIO_setPadConfig(24, GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(24, GPIO_QUAL_ASYNC);

}

//
//
// Function to configure SPI B as master with FIFO enabled.
//
//
void ddi_InitializeSpiMaster(uint32_t spi_base)
{
    //
    // Must put SPI into reset before configuring it
    //
    SPI_disableModule(spi_base);

    //
    // SPI configuration. Use a 500kHz SPICLK and 16-bit word size.
    //
    SPI_setConfig(spi_base, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_MASTER, 500000, 16);
    SPI_disableLoopback(spi_base);
    SPI_setEmulationMode(spi_base, SPI_EMULATION_FREE_RUN);

    //
    // FIFO and interrupt configuration
    //

    //SPI_enableFIFO(spi_base);
    //SPI_clearInterruptStatus(spi_base, SPI_INT_TXFF);
    //SPI_setFIFOInterruptLevel(spi_base, SPI_FIFO_TX2, SPI_FIFO_RX2);
    //SPI_enableInterrupt(spi_base, SPI_INT_TXFF);


    //
    // Non-FIFO and Non-interrupt configuration
    //

    SPI_disableFIFO(spi_base);

    //
    // Configuration complete. Enable the module.
    //
    SPI_enableModule(spi_base);
}


//
//
// Function to configure SPI B as slave with FIFO enabled.
//
//
void ddi_InitializeSpiSlave(uint32_t spi_base)
{
    //
    // Must put SPI into reset before configuring it
    //
    SPI_disableModule(spi_base);

    //
    // SPI configuration. Use a 500kHz SPICLK and 16-bit word size.
    //
    SPI_setConfig(spi_base, DEVICE_LSPCLK_FREQ, SPI_PROT_POL0PHA0,
                  SPI_MODE_SLAVE, 500000, 16);
    SPI_disableLoopback(spi_base);
    SPI_setEmulationMode(spi_base, SPI_EMULATION_FREE_RUN);

    //
    // FIFO and interrupt configuration
    //
    SPI_enableFIFO(spi_base);
    SPI_clearInterruptStatus(spi_base, SPI_INT_RXFF);
    SPI_setFIFOInterruptLevel(spi_base, SPI_FIFO_TX2, SPI_FIFO_RX2);
    SPI_enableInterrupt(spi_base, SPI_INT_RXFF);

    //
    // Configuration complete. Enable the module.
    //
    SPI_enableModule(spi_base);
}



// End of file





