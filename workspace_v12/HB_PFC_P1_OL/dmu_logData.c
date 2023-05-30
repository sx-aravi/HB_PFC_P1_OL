/******************************************************************************
* Sarinx LLC Proprietary Data.
* This document contains information, which is the property of Sarinx LLC
*  Except for rights expressly granted by contract to the United
* States Government, this document may not, in whole or in part, be duplicated
* or disclosed, or used for design or manufacturing purposes without the prior
* written permission of Sarinx LLC
* -----------------------------------------------------------------------------
*//**
* @file dmu_logData.c
* @brief data logging module
*
* This module collects the system behavior data by adding key variables to profile data
* store for both real-time (t-x Seconds) and off-line analysis
*
* @author Sarinx LLC
*******************************************************************************/

#include "dmu_logData.h"

#ifdef SXLOG_ENABLED  // execute whole file

// =============================================================================
// types and definitions

typedef struct {
  sxlog_function_t fn;
  sxlog_level_t threshold;
} subscriber_t;

// =============================================================================
// local storage

static subscriber_t s_subscribers[SXLOG_MAX_SUBSCRIBERS];
//static char s_message[SXLOG_MAX_MESSAGE_LENGTH];

// =============================================================================
// user-visible code

void sxlog_init() {
  //memset(s_subscribers, 0, sizeof(s_subscribers));
}

// search the s_subscribers table to install or update fn
sxlog_err_t sxlog_subscribe(sxlog_function_t fn, sxlog_level_t threshold) {
  int available_slot = -1;
  int i;
  for (i=0; i<SXLOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn == fn) {
      // already subscribed: update threshold and return immediately.
      s_subscribers[i].threshold = threshold;
      return SXLOG_ERR_NONE;

    } else if (s_subscribers[i].fn == 0) {
      // found a free slot
      available_slot = i;
    }
  }
  // fn is not yet a subscriber.  assign if possible.
  if (available_slot == -1) {
    return SXLOG_ERR_SUBSCRIBERS_EXCEEDED;
  }
  s_subscribers[available_slot].fn = fn;
  s_subscribers[available_slot].threshold = threshold;
  return SXLOG_ERR_NONE;
}

// search the s_subscribers table to remove
sxlog_err_t sxlog_unsubscribe(sxlog_function_t fn) {
  int i;
  for (i=0; i<SXLOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn == fn) {
      s_subscribers[i].fn = 0;    // mark as empty
      return SXLOG_ERR_NONE;
    }
  }
  return SXLOG_ERR_NOT_SUBSCRIBED;
}

const char *sxlog_level_name(sxlog_level_t severity) {
  switch(severity) {
   case SXLOG_TRACE_LEVEL: return "TRACE";
   case SXLOG_DEBUG_LEVEL: return "DEBUG";
   case SXLOG_INFO_LEVEL: return "INFO";
   case SXLOG_WARNING_LEVEL: return "WARNING";
   case SXLOG_ERROR_LEVEL: return "ERROR";
   case SXLOG_CRITICAL_LEVEL: return "CRITICAL";
   case SXLOG_ALWAYS_LEVEL: return "ALWAYS";
   default: return "UNKNOWN";
  }
}

void sxlog_message(sxlog_level_t severity, const char *fmt, ...) {
  /*va_list ap;
  int i;
  va_start(ap, fmt);
  vsnprintf(s_message, SXLOG_MAX_MESSAGE_LENGTH, fmt, ap);
  va_end(ap);

  for (i=0; i<SXLOG_MAX_SUBSCRIBERS; i++) {
    if (s_subscribers[i].fn != NULL) {
      if (severity >= s_subscribers[i].threshold) {
        s_subscribers[i].fn(severity, s_message);
      }
    }
  }*/
}

// =============================================================================
//

#endif  // #ifdef SXLOG_ENABLED


