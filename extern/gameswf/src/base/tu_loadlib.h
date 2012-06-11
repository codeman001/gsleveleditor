#pragma once
// tu_loadlib.h	-- Vitaly Alexeev <tishka92@yahoo.com>	2007

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// plugin loader

#ifndef TU_LOADLIB_H
#define TU_LOADLIB_H

#define lib_t void*

namespace gameswf
{

struct tu_loadlib
{
	exported_module tu_loadlib(const char* library_name);
	exported_module ~tu_loadlib();

	exported_module void* get_function(const char* function_name);
	
	private:

	lib_t m_hlib;

};

}; // namespace gameswf


#endif // TU_LOADLIB_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
