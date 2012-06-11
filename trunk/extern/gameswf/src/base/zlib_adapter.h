#pragma once
// zlib_adapter.h	-- Thatcher Ulrich 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Code to wrap zlib compression/decompression around a tu_file
// stream.


#ifndef ZLIB_ADAPTER_H
#define ZLIB_ADAPTER_H


#include "base/tu_config.h"

namespace gameswf
{
	class tu_file;

namespace zlib_adapter
{
	// NOTE: these functions will return NULL if
	// TU_CONFIG_LINK_TO_ZLIB is set to 0.

	// Returns a read-only tu_file stream that inflates the remaining
	// content of the given input stream, as you read data from the
	// swfnew stream.
	//
	// The caller owns the returned tu_file*.  The caller also owns
	// the input tu_file*; don't delete it until you've deleted the
	// returned tu_file.
	// @@ Here's a good case for ref-counting...
	gameswf::tu_file*	make_inflater(gameswf::tu_file* in);

	// Returns a write-only tu_file stream that deflates the remaining
	// content of the given input stream.
	gameswf::tu_file*	make_deflater(gameswf::tu_file* out);
}

}; // gameswf

#endif // ZLIB_ADAPTER_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
