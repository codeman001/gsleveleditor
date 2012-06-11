#include "stdafx.h"

#include "base/container.h"
#include "base/unit_heap.h"
#include "gameswf/gameswf_log.h"

#define mymin(a,b) ((a) < (b) ? (a) : (b))

namespace gameswf
{
	// TODO: add and use mutex when 2+ player contexts (ie loading screen)
	// static mutex s_allocator_mutex;

#if GAMESWF_ENABLE_ALLOCATOR

	const static int HEAP_COUNT = 8;
	static bool s_initialized = false;
	static GrowableUnitHeap s_heaps[HEAP_COUNT];

	inline GrowableUnitHeap* get_heap(size_t size)
	{
		if(size <= 16)
			return &s_heaps[0];
		else if(size <= 32)
			return &s_heaps[1];
		else if(size <= 64)
			return &s_heaps[2];
		else if(size <= 128)
			return &s_heaps[3];
		else if(size <= 192)
			return &s_heaps[4];
		else if(size <= 256)
			return &s_heaps[5];
		else if(size <= 320)
			return &s_heaps[6];
		else if(size <= 512)
			return &s_heaps[7];

		return NULL;
	}

	void init_heap(size_t block_size, int initial_count)
	{
		GrowableUnitHeap* heap = get_heap(block_size);
		assert(heap);
		if(heap)
			heap->Init(block_size, initial_count);
	}

	void init_memory_allocator()
	{
		if(!s_initialized)
		{
			init_heap(16, 2048);
			init_heap(32, 1024);
			init_heap(64, 1024);
			init_heap(128, 1024);
			init_heap(192, 512);
			init_heap(256, 512);
			init_heap(320, 256);
			init_heap(512, 256);
			s_initialized = true;
		}
	}

	void close_memory_allocator()
	{
		if(s_initialized)
		{
			for(int i = 0; i < HEAP_COUNT; i++)
			{
				if(s_heaps[i].GetUsedHeap() > 0)
					log_msg("warning: heap size=%d still has %d bytes used\n", s_heaps[i].GetBlockSize(), s_heaps[i].GetUsedHeap());
				s_heaps[i].Destroy();
			}
			s_initialized = false;
		}
	}

	void dump_memory_allocator()
	{
		if(s_initialized)
		{
			log_msg("gameswf memory allocator:\n");
			int total = 0;
			int peak = 0;
			int used = 0;
			for(int i = 0; i < HEAP_COUNT; i++)
			{
				log_msg("  size: %d heaps: %d used:%.1f peak:%.1f tot:%.1f MB \n", s_heaps[i].GetBlockSize(), s_heaps[i].GetHeapCount(), s_heaps[i].GetUsedHeap()/(1024.0f * 1024.0f), s_heaps[i].GetPeak()/(1024.0f * 1024.0f), s_heaps[i].GetTotalHeap()/(1024.0f * 1024.0f));
				total += s_heaps[i].GetTotalHeap();
				peak += s_heaps[i].GetPeak();
				used += s_heaps[i].GetUsedHeap();
			}
			log_msg("total: used:%.1f peak:%.1f tot:%.1f MB\n", used/(1024.0f * 1024.0f), peak/(1024.0f * 1024.0f), total/(1024.0f * 1024.0f));
		}
	}
#else
	void init_memory_allocator() {}
	void close_memory_allocator() {}
	void dump_memory_allocator() {}
#endif

	void* malloc_internal(size_t size, swf::MemHint hint)
	{
#if GAMESWF_ENABLE_ALLOCATOR
		if(s_initialized)
		{
			GrowableUnitHeap* heap = get_heap(size);
			void* ptr = heap ? heap->AcquireBlock(size) : NULL;

			if(ptr)
				return ptr;
		}
#endif

		return SwfAlloc(size, swf::MEMHINT_NONE);
	}
	
	void* malloc_internal(size_t size, swf::MemHint hint, const char* filename, int line)
	{
#if GAMESWF_ENABLE_ALLOCATOR
		if(s_initialized)
		{
			GrowableUnitHeap* heap = get_heap(size);
			void* ptr = heap ? heap->AcquireBlock(size) : NULL;

			if(ptr)
				return ptr;
		}
#endif
		
		return SwfAlloc(size, swf::MEMHINT_NONE, filename, line);
	}
	
	void  free_internal(void* p, size_t old_size)
	{
#if GAMESWF_ENABLE_ALLOCATOR
		if(s_initialized)
		{
			if(old_size == 0)
			{
				 for(int i = 0; i < HEAP_COUNT; i++)
				 {
					 if(s_heaps[i].ReleaseBlock(p))
						 return;
				 }
			}
			else
			{
				GrowableUnitHeap* heap = get_heap(old_size);
				if(heap && heap->ReleaseBlock(p))
					return;
			}
		}
#endif
		
		SwfFree(p);
	}

	void* realloc_internal(void* optr, size_t nsize, size_t osize, swf::MemHint hint)
	{
#if GAMESWF_ENABLE_ALLOCATOR
		if(s_initialized)
		{
			GrowableUnitHeap* old_heap = get_heap(osize);
			GrowableUnitHeap* new_heap = get_heap(nsize);
			if(old_heap != NULL && new_heap != NULL && old_heap == new_heap && old_heap->IsMemoryInHeap(optr))
			{
				// nothing to do since block large enough
				return optr;
			}
		}
#endif

		void* nptr = gameswf::malloc_internal(nsize, hint);
		memcpy(nptr, optr, mymin(nsize, osize));
		gameswf::free_internal(optr, osize);
		return nptr;
	}

	void* realloc_internal(void* optr, size_t nsize, size_t osize, swf::MemHint hint, const char* filename, int line)
	{
#if GAMESWF_ENABLE_ALLOCATOR
		if(s_initialized)
		{
			GrowableUnitHeap* old_heap = get_heap(osize);
			GrowableUnitHeap* new_heap = get_heap(nsize);
			if(old_heap != NULL && new_heap != NULL && old_heap == new_heap && old_heap->IsMemoryInHeap(optr))
			{
				// nothing to do since block large enough
				return optr;
			}
		}
#endif

		void* nptr = gameswf::malloc_internal(nsize, hint, filename, line);
		memcpy(nptr, optr, mymin(nsize, osize));
		gameswf::free_internal(optr, osize);
		return nptr;
	}

};	// end namespace gameswf

//these three functions need to be implemented on the game side
void* SwfAlloc(size_t size, swf::MemHint hint);
void* SwfAlloc(size_t size, swf::MemHint hint, const char* filename, int line);
void SwfFree(void* p);

void* operator new(size_t size, swf::MemHint hint) 
{
	return gameswf::malloc_internal(size, hint);
}

void* operator new[](size_t size, swf::MemHint hint)
{
	return gameswf::malloc_internal(size, hint);
}

void* operator new(size_t size, swf::MemHint hint, const char* filename, int line) 
{
	return gameswf::malloc_internal(size, hint, filename, line);
}

void* operator new[](size_t size, swf::MemHint hint, const char* filename, int line) 
{
	return gameswf::malloc_internal(size, hint, filename, line);
}

