// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef GSEDITOR
#include "targetver.h"
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#ifdef GSEDITOR

// Windows Header Files:
#include <windows.h>
#include <uxtheme.h>
#include <Vsstyle.h>
#pragma comment(lib,"uxtheme.lib")

#endif

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <math.h>

#include "AppInclude.h"

// TODO: reference additional headers your program requires here
