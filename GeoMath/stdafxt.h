// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>


// TODO: reference additional headers your program requires here

#include "GM_Prefix.h"
#include "GM_3dPoint.h"
#include "GM_3dVector.h"
#include "GM_3dBasis.h"
#include "GM_3dLine.h"
#include "GM_3dPlane.h"
#include "GM_3dTriangle.h"
#include "GM_Matrix.h"
#include "GM_3dTrasf.h"
#include "GM_Rototraslation.h"



extern FILE *testLogFile;
extern bool consoleTrace;