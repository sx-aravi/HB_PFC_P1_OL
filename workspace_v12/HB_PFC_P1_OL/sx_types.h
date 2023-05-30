/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
* Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file sx_types.h
* @brief Texas Instruments standard types defination Header
*
* Provide global basic type defintions for F28xx, These types are also defined
* by other TI components. They are bracketed with _TI_STD_TYPES to avoid
* warnings for duplicate definition.
*
* You may get warnings about duplicate type definitions when using this
* header file with earlier versions of DSP/BIOS and CSL
* You can use the '-pds303' compiler option to suppress these warnings.
*
* Uint8, Uint16, Uint32, etc are defined to be "smallest unit of
* available storage that is large enough to hold unsigned or integer
* of specified size
*
* Duplicates are created to allow alternative capitalisation to support
* any legacy code
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef SX_TYPES_H_
#define SX_TYPES_H_

//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<<<< PRE-PROCESSOR DIRECTIVES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|
/** define this to avoid declaration clash with DSP280x_Device.h*/
#define DSP28_DATA_TYPES

//-----------------------------------------------------------------------------|
//<<<<<<<<<<<<<<<<<<<<<< USER DEFINED DATA TYPES >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>|
//-----------------------------------------------------------------------------|
/** type definition of unsigned char to Uint8*/
typedef unsigned char        Uint8;
/** Duplicate for case sensitive */
typedef Uint8                uint8;


/** type definition of char to Int8*/
typedef char                 Int8;
/** Duplicate for case sensitive */
typedef Int8                 int8;


typedef unsigned long        Uint32;

/** type definition of long to Int32*/
typedef long                 Int32;
/** Duplicate for case sensitive */
//typedef Int32                int32;

/** type definition of int to Int16*/
typedef int                  Int16;
/** Duplicate for case sensitive */
//typedef Int16                int16;


/** type defination of unsigned int to Uint16*/
typedef unsigned int         Uint16;
/** Duplicate for case sensitive */
typedef Uint16               uint16;

/** type defination of Uint16 to BITMAP_TYPE*/
typedef Uint16               BITMAP_TYPE;
typedef Uint32               uint32;


/** Vector to store the three axis of the three phase current / voltage
*   for control algorithm */
struct VECTOR
{
    float Axis1;    /**< Phase A or Axis 1 */
    float Axis2;    /**< Phase B or Axis 2 */
    float Axis3;    /**< Phase C or Axis 3 */
};


#endif /* SX_TYPES_H_ */
