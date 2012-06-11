#pragma once
#ifndef _BASE_UNITHEAP_H_
#define _BASE_UNITHEAP_H_

#include "container.h"

namespace gameswf
{
	//
	// Simple unit heap implementation. O(1) allocations and deallocations, fixed block size.
	//

	struct UnitHeap
	{
	public:
		typedef unsigned short UNIT_HEAP_TABLE_TYPE;

		void Init(int blockSize, int blockCount);
		void Destroy();

		bool CanAllocate(int size) const;
		void* AcquireBlock(int size);
		bool IsMemoryInHeap(void* ptr) const;
		bool ReleaseBlock(void* ptr);

		int GetUsedHeap() const;
		int GetTotalHeap() const;
		int GetPeak() const;

	private:
		void* GetPtr(int blockId);

		int m_blockSize;
		int m_bufferSize;
		int m_num_blocks_assigned;
		int m_max_blocks_count;

		int m_num_blocks_assigned_high_watermark;
		int m_total_allocs;

		char* m_mem_buffer;
		UNIT_HEAP_TABLE_TYPE* m_block_offset_table; // uses 4 bytes per block
		UNIT_HEAP_TABLE_TYPE* m_block_index_table;
	};

	inline bool UnitHeap::CanAllocate(int size) const
	{
	#ifdef _DEBUG
		if (size <= m_blockSize)
		{
			assert(m_num_blocks_assigned < m_max_blocks_count); // ==> MUST INCREASE HEAP SIZE!
		}
	#endif
		return ( size <= m_blockSize ) && ( m_num_blocks_assigned < m_max_blocks_count );
	}

	//
	// Growable unit heap implementation.
	//

	struct GrowableUnitHeap
	{
	public:
		GrowableUnitHeap();

		void Init(int blockSize, int initialBlockCount);
		void Destroy();
		bool CanAllocate(int size) const;
		void* AcquireBlock(int size);
		bool IsMemoryInHeap(void* ptr) const;
		bool ReleaseBlock(void* ptr);
		int GetUsedHeap() const;
		int GetTotalHeap() const;
		int GetPeak() const;
		int GetBlockSize() const { return m_blockSize; }
		int GetHeapCount() const { return m_heapCount; }

	private:
		const static int MAX_GROW_COUNT = 32;
		UnitHeap* m_heaps[MAX_GROW_COUNT];
		int m_heapCount;
		int m_blockSize;
		int m_initialBlockCount;
	};

}; // namespace gameswf

#endif // _BASE_UNITHEAP_H_
