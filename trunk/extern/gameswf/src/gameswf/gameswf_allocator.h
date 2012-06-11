#pragma once
// gameswf_memory.h

#ifndef GAMESWF_ALLOCATOR_H
#define GAMESWF_ALLOCATOR_H

namespace gameswf
{
	void init_memory_allocator();
	void close_memory_allocator();
	void dump_memory_allocator();
};	// end namespace gameswf


#endif // GAMESWF_ALLOCATOR_H

