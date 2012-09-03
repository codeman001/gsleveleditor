// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef GSEDITOR
#include "targetver.h"
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifdef WIN32

// Windows Header Files:
#include <windows.h>

#if defined(GSEDITOR) || defined(PARTICLE_EDITOR) || defined(GSANIMATION)

// Windows Header Files:
#include <windows.h>
#include <uxtheme.h>
#include <Vsstyle.h>
#pragma comment(lib,"uxtheme.lib")

#endif

#endif

// C RunTime Header Files
#include <stdlib.h>
#ifndef IOS
#include <malloc.h>
#endif
#include <memory.h>
#include <stdio.h>
#include <math.h>

#ifdef WIN32
#include <tchar.h>
#endif

#include "AppInclude.h"

// TODO: reference additional headers your program requires here
