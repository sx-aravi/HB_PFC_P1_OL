/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file dmu_logData.h
* @brief Sensor data logging header
*
* This module collects the system behavior data by adding key variables to profile data
* store for both real-time (t-x Seconds) and off-line analysis
*
* @author Sarinx LLC
*******************************************************************************/

#ifndef DMU_LOGDATA_H_
#define DMU_LOGDATA_H_

#include "sx_types.h"

/** system performance log levels  */

typedef enum {
  SXLOG_TRACE_LEVEL=100,
  SXLOG_DEBUG_LEVEL,
  SXLOG_INFO_LEVEL,
  SXLOG_WARNING_LEVEL,
  SXLOG_ERROR_LEVEL,
  SXLOG_CRITICAL_LEVEL,
  SXLOG_ALWAYS_LEVEL
} sxlog_level_t;


#define SXLOG_ENABLED


#ifdef SXLOG_ENABLED
  #define SXLOG_INIT() SXLOG_init()
  #define SXLOG_SUBSCRIBE(a, b) SXLOG_subscribe(a, b)
  #define SXLOG_UNSUBSCRIBE(a) SXLOG_unsubscribe(a)
  #define SXLOG_LEVEL_NAME(a) SXLOG_level_name(a)
  #define SXLOG(...) SXLOG_message(__VA_ARGS__)
  #define SXLOG_TRACE(...) SXLOG_message(SXLOG_TRACE_LEVEL, __VA_ARGS__)
  #define SXLOG_DEBUG(...) SXLOG_message(SXLOG_DEBUG_LEVEL, __VA_ARGS__)
  #define SXLOG_INFO(...) SXLOG_message(SXLOG_INFO_LEVEL, __VA_ARGS__)
  #define SXLOG_WARNING(...) SXLOG_message(SXLOG_WARNING_LEVEL, __VA_ARGS__)
  #define SXLOG_ERROR(...) SXLOG_message(SXLOG_ERROR_LEVEL, __VA_ARGS__)
  #define SXLOG_CRITICAL(...) SXLOG_message(SXLOG_CRITICAL_LEVEL, __VA_ARGS__)
  #define SXLOG_ALWAYS(...) SXLOG_message(SXLOG_ALWAYS_LEVEL, __VA_ARGS__)
#else
  // SXLOG vanishes when disabled at compile time...
  #define SXLOG_INIT() do {} while(0)
  #define SXLOG_SUBSCRIBE(a, b) do {} while(0)
  #define SXLOG_UNSUBSCRIBE(a) do {} while(0)
  #define SXLOG_LEVEL_NAME(a) do {} while(0)
  #define SXLOG(s, f, ...) do {} while(0)
  #define SXLOG_TRACE(f, ...) do {} while(0)
  #define SXLOG_DEBUG(f, ...) do {} while(0)
  #define SXLOG_INFO(f, ...) do {} while(0)
  #define SXLOG_WARNING(f, ...) do {} while(0)
  #define SXLOG_ERROR(f, ...) do {} while(0)
  #define SXLOG_CRITICAL(f, ...) do {} while(0)
  #define SXLOG_ALWAYS(f, ...) do {} while(0)
#endif

typedef enum {
  SXLOG_ERR_NONE = 0,
  SXLOG_ERR_SUBSCRIBERS_EXCEEDED,
  SXLOG_ERR_NOT_SUBSCRIBED,
} sxlog_err_t;

// define the maximum number of concurrent subscribers
#ifndef SXLOG_MAX_SUBSCRIBERS
#define SXLOG_MAX_SUBSCRIBERS 6
#endif
// maximum length of formatted log message
#ifndef SXLOG_MAX_MESSAGE_LENGTH
#define SXLOG_MAX_MESSAGE_LENGTH 120
#endif
/**
 * @brief: prototype for SXLOG subscribers.
 */
typedef void (*sxlog_function_t)(sxlog_level_t severity, char *msg);

void sxlog_init(void);
sxlog_err_t sxlog_subscribe(sxlog_function_t fn, sxlog_level_t threshold);
sxlog_err_t sxlog_unsubscribe(sxlog_function_t fn);
const char *sxlog_level_name(sxlog_level_t level);
void sxlog_message(sxlog_level_t severity, const char *fmt, ...);


#endif /* DMU_LOGDATA_H_ */
