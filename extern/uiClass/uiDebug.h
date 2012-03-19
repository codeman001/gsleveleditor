#ifndef _UIDEBUG_
#define _UIDEBUG_

#include "uiInclude.h"

#ifdef _DEBUG	

    // Ho tro Leak
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>

	// UIDEBUG_DUMPLEAK
    // In danh sach Memory bi LEAK
    #define UIDEBUG_DUMPLEAK() _CrtDumpMemoryLeaks()    

	// UIDEBUG_TRACE
	// Trace log ra cua so Output cua
	#define UIDEBUG_TRACE(FORMAT,...) \
    { \
        char debugString[512]; \
		uiClass::uiString::format<char>(debugString,FORMAT, __VA_ARGS__); \
        OutputDebugStringA(debugString); \
    }


#else
	#define UIDEBUG_TRACE(FORMAT,...)
	#define UIDEBUG_DUMPLEAK()
#endif

#endif
