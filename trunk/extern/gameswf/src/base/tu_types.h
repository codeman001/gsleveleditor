#pragma once
// tu_types.h	-- Ignacio CastaÒo, Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Minimal typedefs.  Follows SDL conventions; falls back on SDL.h if
// platform isn't obvious.


#ifndef TU_TYPES_H
#define TU_TYPES_H


#include "base/tu_config.h"
#include <stdio.h>

#if !defined(_OPENGL) && !defined(_OPENGLES)
	#if defined(_GLITCH)
		namespace glitch{
	#else
		#define _IRRLICHT
		namespace irr{
	#endif
			namespace video{
				class IVideoDriver;
				class ITexture;
			}
		}
#endif

namespace gameswf
{

#if !defined(_OPENGL) && !defined(_OPENGLES)
	#if defined(_GLITCH)
		typedef glitch::video::IVideoDriver video_driver;
		typedef glitch::video::ITexture video_texture;
	#else
		typedef irr::video::IVideoDriver video_driver;
		typedef irr::video::ITexture video_texture;
	#endif
#else
	class video_driver
	{
	};
	class video_texture
	{
	};
#endif

#if defined(TARGET_IPHONE_SIMULATOR) || defined(TARGET_OS_IPHONE)
	#ifndef IPHONEOS
		#define IPHONEOS
	#endif
#endif

#if defined(__i386__) || defined(WIN32) || defined(_PS3) || defined(IPHONEOS) || defined(RVL_OS) ||defined(_XBOX) || defined(DALI)

	// On known little-endian platforms, define this stuff.

#if defined(_PS3) || defined(RVL_OS) || defined(_XBOX)
	#define _TU_LITTLE_ENDIAN_	0
#else
	#define _TU_LITTLE_ENDIAN_	1
#endif
	
	#ifndef _SDL_stdinc_h
		typedef unsigned char	Uint8;
		typedef signed char	Sint8;
		typedef unsigned short	Uint16;
		typedef short	Sint16;
		typedef unsigned int	Uint32;
		typedef int	Sint32;
	#endif

	#ifndef _MSC_VER
		// Probably gcc or something compatible
		typedef unsigned long long	Uint64;
		typedef long long Sint64;
	#else	// _MSC_VER
		//ICI WIN32
		typedef unsigned __int64	Uint64;
		typedef __int64	Sint64;
	#endif	// _MSC_VER

#else	// not __I386__ and not WIN32

	// On unknown platforms, rely on SDL
	#include <SDL.h>
	
	//#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	//	#define _TU_LITTLE_ENDIAN_ 1
	//#else
	//	#undef _TU_LITTLE_ENDIAN_
	//#endif // SDL_BYTEORDER == SDL_LIL_ENDIAN

#endif	// not __I366__ and not WIN32


typedef Uint8 uint8;
typedef Sint8 sint8;
typedef Sint8 int8;
typedef Uint16 uint16;
typedef Sint16 sint16;
typedef Sint16 int16;
typedef Uint32 uint32;
typedef Sint32 sint32;
typedef Sint32 int32;
typedef Uint64 uint64;
typedef Sint64 sint64;
typedef Sint64 int64;

#if TU_USES_FLOAT_AS_COORDINATE_COMPONENT
	typedef float coord_component;
#else
	typedef Sint16 coord_component;
#endif


// A function to run some validation checks.
#ifdef WIN32
__declspec(dllexport) bool	tu_types_validate();
#else // WIN32
bool	tu_types_validate();
#endif

#ifdef FLIP_TCOORD_Y
	#undef FLIP_TCOORD_Y
#endif

#if defined(IPHONEOS) || defined(WIN32) || defined(_PS3)
	#define USE_ALPHA_TEXTURES
#endif

#if defined(_GLITCH) && defined(USE_ALPHA_TEXTURES)
	#include <glitch/Revision.h>
	#if GLITCH_REVISION <= 5561
		// ALPHA textures not working with older revisions of glitch
		#undef USE_ALPHA_TEXTURES
	#endif
#endif // _GLITCH

}; // namespace gameswf

#endif // TU_TYPES_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
