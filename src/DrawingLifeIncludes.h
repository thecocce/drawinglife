/*=======================================================
 Copyright (c) avp::ptr, 2010
=======================================================*/

#ifndef _DRAWINGLIFEINCLUDES_H_
#define _DRAWINGLIFEINCLUDES_H_

// -----------------------------------------------------------------------------

#define APP_NAME_STR "DrawingLife"
#define APP_AUTHOR_STR "avp::ptr"
#define APP_VERSION_STR "4.1.1"

// -----------------------------------------------------------------------------

#define EXTRA_DEBUG_MODE false

// -----------------------------------------------------------------------------

#define SAFE_DELETE(p) {if(p) {delete (p); (p)=NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p) {delete[] (p); (p)=NULL;}}

// -----------------------------------------------------------------------------
// include openFrameworks source tree.
// -----------------------------------------------------------------------------

#include "ofMain.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#include "Logger.h"

// -----------------------------------------------------------------------------

#include "DrawingLifeTypes.h"

// -----------------------------------------------------------------------------

#include "AppSettings.h"

// -----------------------------------------------------------------------------

#endif // _DRAWINGLIFEINCLUDES_H_
