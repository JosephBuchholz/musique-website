/**
 * This file contains things for debugging (such as logging)
 */

#pragma once

#define DEBUG 1

#define WEB

#include "Logging.h"

#if DEBUG

#define SHOW_BOUNDING_BOXES 0

#if SHOW_BOUNDING_BOXES
    #define DEBUG_BOUNDING_BOXES 1
#endif

#endif