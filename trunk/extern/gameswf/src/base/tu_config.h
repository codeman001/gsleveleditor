#pragma once
// tu_config.h	-- by Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Generic include file for configuring tu-testbed.


#ifndef TU_CONFIG_H
#define TU_CONFIG_H


// In case you need some unusual stuff to be predefined.  For example,
// maybe you #define swfnew/delete to be something special.
#include "compatibility_include.h"
#include "gameswf_config.h"

#include "base/dlmalloc.h"

namespace swf {

	enum MemHint {
		MEMHINT_NONE,
	}; //enum MemHint {

} //namespace swf {

namespace gameswf
{
	void* malloc_internal(size_t size, swf::MemHint hint);
	void* malloc_internal(size_t size, swf::MemHint hint, const char* filename, int line);
	void  free_internal(void* p, size_t old_size);
	void* realloc_internal(void* p, size_t nsize, size_t osize, swf::MemHint hint);
	void* realloc_internal(void* p, size_t nsize, size_t osize, swf::MemHint hint, const char* filename, int line);

	struct array_allocator
	{
		static void operator delete[](void* p) { return gameswf::free_internal(p, 0); }
	};
	
	template<typename T>
	inline void destruct(const T* p)
	{
		if(p)
		{
			p->~T();
			free_internal((T*)p, 0);
		}
	}

	template<typename T>
	inline void destructa(const T* p, size_t size)
	{
		if(p)
		{
			for(int i = size - 1; i >= 0; i--)
				p[i].~T();
			delete[] (array_allocator*)p;
		}
	}

	template<typename T>
	inline void destructp(const T* p, size_t size)
	{
		if(p)
		{
			delete[] (array_allocator*)p;
		}
	}
};

void* operator new(size_t size, swf::MemHint hint);
void* operator new[](size_t size, swf::MemHint hint);

void* operator new(size_t size, swf::MemHint hint, const char* filename, int line);
void* operator new[](size_t size, swf::MemHint hint, const char* filename, int line);

// to turn off warnings
inline void operator delete(void* p, swf::MemHint hint) {}
inline void operator delete[](void* p, swf::MemHint hint) {}
inline void operator delete(void* p, swf::MemHint hint, const char* filename, int line) {}
inline void operator delete[](void* p, swf::MemHint hint, const char* filename, int line) {}

void* SwfAlloc(size_t size, swf::MemHint hint);
void* SwfAlloc(size_t size, swf::MemHint hint, const char* filename, int line);
void  SwfFree(void* p);

#if defined(_DEBUG) && WIN32

	#define swfnew								new(swf::MEMHINT_NONE, __FILE__, __LINE__)
	#define swfnewh(hint)						new(hint)

	#define tu_malloc(size)						gameswf::malloc_internal(size, swf::MEMHINT_NONE, __FILE__, __LINE__)
	#define tu_free(optr, old_size)				gameswf::free_internal(optr, old_size)
	#define tu_realloc(optr, nsize, osize)		gameswf::realloc_internal(optr, nsize, osize, swf::MEMHINT_NONE, __FILE__, __LINE__)

#else

	#define swfnew								new(swf::MEMHINT_NONE)
	#define swfnewh(hint)						new(hint)

	#define tu_malloc(size)						gameswf::malloc_internal(size, swf::MEMHINT_NONE)
	#define tu_free(optr, old_size)				gameswf::free_internal(optr, old_size)
	#define tu_realloc(optr, nsize, osize)		gameswf::realloc_internal(optr, nsize, osize, swf::MEMHINT_NONE)

#endif

#define swfdelete(p)							gameswf::destruct(p)
#define swfdelete_array(p, size)				gameswf::destructa(p, size)
#define swfdelete_primitives(p, size)			gameswf::destructp(p, size)

// tu_error_exit() is for fatal errors; it should not return!
// You can #define it to something else in compatibility_include.h; e.g. you could
// throw an exception, halt, whatever.
#ifndef tu_error_exit
#include <stdlib.h>	// for exit()
#define tu_error_exit(error_code, error_message) { fprintf(stderr, error_message); exit(error_code); }
#endif


// define TU_CONFIG_LINK_TO_JPEGLIB to 0 to exclude jpeg code from
// your build.  Be aware of what you're doing -- it may break
// features!
#ifndef TU_CONFIG_LINK_TO_JPEGLIB
#define TU_CONFIG_LINK_TO_JPEGLIB 0
#endif

// define TU_CONFIG_LINK_TO_ZLIB to 0 to exclude zlib code from your
// build.  Be aware of what you're doing -- it may break features that
// you need!
#ifndef TU_CONFIG_LINK_TO_ZLIB
#define TU_CONFIG_LINK_TO_ZLIB 1
#endif

// define TU_CONFIG_LINK_TO_LIBPNG to 0 to exclude libpng code from
// your build.  Be aware of what you're doing -- it may break
// features!
#ifndef TU_CONFIG_LINK_TO_LIBPNG
#define TU_CONFIG_LINK_TO_LIBPNG 1
#endif

// define TU_CONFIG_LINK_TO_LIB3DS to 1 to include 3DS file support in
// gameswf, depending on the lib3ds library
#ifndef TU_CONFIG_LINK_TO_LIB3DS
#define TU_CONFIG_LINK_TO_LIB3DS 0
#endif

// define TU_CONFIG_LINK_TO_FFMPEG to 1 to include MP3 & video support in
// gameswf, depending on the libavcode, libavutil & libavformat libraries
#ifndef TU_CONFIG_LINK_TO_FFMPEG
#define TU_CONFIG_LINK_TO_FFMPEG 0
#endif

// define TU_CONFIG_LINK_TO_FREETYPE to 1 to include dynamic font support in
// gameswf, depending on the freetype library
#ifndef TU_CONFIG_LINK_TO_FREETYPE
#define TU_CONFIG_LINK_TO_FREETYPE 1
#endif

// define TU_CONFIG_LINK_TO_THREAD to 0 to switch in gameswf to single thread mode
// define TU_CONFIG_LINK_TO_THREAD to 1 to include SDL thread & mutex support in gameswf
// define TU_CONFIG_LINK_TO_THREAD to 2 to include ... (TODO: pthread support)
#ifndef TU_CONFIG_LINK_TO_THREAD
#define TU_CONFIG_LINK_TO_THREAD 0
#endif
#if TU_CONFIG_LINK_TO_THREAD == 0 && TU_CONFIG_LINK_TO_FFMPEG == 1
#error video & MP3 requires multi thread support
#endif

#ifndef TU_USES_FLOAT_AS_COORDINATE_COMPONENT
#define TU_USES_FLOAT_AS_COORDINATE_COMPONENT 1
#endif

// Internal storage capacity for gameswf strings (default: 15) to be used with short strings.
// For longer strings, heap buffers will be used.
#ifndef GAMESWF_STRING_INTERNAL_CAPACITY
#define GAMESWF_STRING_INTERNAL_CAPACITY	15
#endif

// Specify if standard tesselation library (libtess) from GNU is used.
#ifndef GAMESWF_ENABLE_LIBTESS
#define GAMESWF_ENABLE_LIBTESS 0
#endif

// enable gameswf memory allocator to improve efficiency
#ifndef GAMESWF_ENABLE_ALLOCATOR
#define GAMESWF_ENABLE_ALLOCATOR 0
#endif

// enable gameswf profiler
#ifndef GAMESWF_ENABLE_PROFILER
#define GAMESWF_ENABLE_PROFILER 0
#endif

// enable gameswf remote debugger
#ifndef GAMESWF_ENABLE_DEBUGGER
#define GAMESWF_ENABLE_DEBUGGER 0
#endif

// GLF support for files/threads/sockets
#ifndef GAMESWF_LINK_GLF
#define GAMESWF_LINK_GLF 0
#endif

// define TU_CONFIG_LINK_STATIC to 1 to link gameswf statically 
#ifndef TU_CONFIG_LINK_STATIC
#	define TU_CONFIG_LINK_STATIC 1
#endif // TU_CONFIG_LINK_STATIC

#ifndef exported_module
#	ifdef WIN32
#		if TU_CONFIG_LINK_STATIC == 0
#			define exported_module __declspec(dllexport)
#		else
#			define exported_module
#		endif // TU_CONFIG_LINK_STATIC == 0
#	else
#		define exported_module
#	endif // WIN32
#endif // exported_module

// define TU_CONFIG_VERBOSE to 1 to allow verbose debugging
#ifndef TU_CONFIG_VERBOSE
#	ifdef _DEBUG
#		define TU_CONFIG_VERBOSE 1
#	else
#		define TU_CONFIG_VERBOSE 0
#	endif
#endif // TU_CONFIG_VERBOSE

#endif // TU_CONFIG_H
