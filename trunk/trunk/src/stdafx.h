// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef GSEDITOR
#include "targetver.h"
#endif

#define HAS_MULTIPLAYER

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifdef HAS_MULTIPLAYER

	#if defined(WIN32)
		#include <memory.h>
		#include <Ws2tcpip.h>
		#include <Winsock2.h>
		#pragma comment (lib, "Ws2_32.lib")
		#include <stdlib.h>
	#endif

	#if defined(LINUX) || defined(IOS) || defined(ANDROID)
		#include <unistd.h>
		#include <netinet/in.h>
		#include <net/if.h>
		#include <arpa/inet.h>
		#include <memory.h>
		#include <fcntl.h>
		#include <errno.h>
		#include <sys/ioctl.h>
		#include <sys/types.h>
		#include <sys/select.h>
		#include <sys/socket.h>
		#include <netdb.h>

        #define SOCKET  int
        #define INVALID_SOCKET  -1
        #define SOCKET_ERROR -1
	#endif

	#if defined(LINUX)
		#include <syslog.h>
		#include <stdarg.h>
	#endif

#endif

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
