#include "stdafx.h"

#include "unit_heap.h"

namespace gameswf
{
	///
	///  Unit heap
	///

	void UnitHeap::Init(int blockSize, int blockCount)
	{
		///printf("UNIT HEAP %i uses %i KB", blockSize, (blockCount * blockSize + 2 * blockCount * sizeof(UNIT_HEAP_TABLE_TYPE))/1024);
		m_blockSize = blockSize;
		m_max_blocks_count = blockCount;

		m_num_blocks_assigned_high_watermark = 0;
		m_total_allocs = 0;

		m_mem_buffer = (char*)SwfAlloc( blockCount * blockSize, swf::MEMHINT_NONE, __FILE__, __LINE__ );
		assert(m_mem_buffer);

		m_block_offset_table = (UNIT_HEAP_TABLE_TYPE*)SwfAlloc( blockCount * sizeof(UNIT_HEAP_TABLE_TYPE), swf::MEMHINT_NONE, __FILE__, __LINE__ );
		assert(m_block_offset_table);

		m_block_index_table = (UNIT_HEAP_TABLE_TYPE*)SwfAlloc( blockCount * sizeof(UNIT_HEAP_TABLE_TYPE), swf::MEMHINT_NONE, __FILE__, __LINE__ );
		assert(m_block_index_table);

		m_bufferSize = blockCount * blockSize;
		m_num_blocks_assigned = 0;

		// initialize tables
		char* ptr = m_mem_buffer;
		for ( int i = 0 ; i < blockCount ; i++ )
		{
			m_block_index_table[i] = i;
			m_block_offset_table[i] = i;
		}
	}

	void UnitHeap::Destroy()
	{
	#ifdef _DEBUG
		unsigned int totalSize = m_max_blocks_count * m_blockSize + 2 * m_max_blocks_count * sizeof(UNIT_HEAP_TABLE_TYPE);
		unsigned int usedSize = m_num_blocks_assigned_high_watermark * m_blockSize + 2 * m_num_blocks_assigned_high_watermark * sizeof(UNIT_HEAP_TABLE_TYPE);
		int spent = totalSize - usedSize;
		//printf("UNIT HEAP %i used %i/%i (%i KB spent)", m_blockSize, m_num_blocks_assigned_high_watermark, m_max_blocks_count, spent/1024);
	#endif

		SwfFree( m_mem_buffer );
		SwfFree( m_block_offset_table );
		SwfFree( m_block_index_table );
	}

	void* UnitHeap::AcquireBlock(int size)
	{
		if ((size <= m_blockSize ) && ( m_num_blocks_assigned < m_max_blocks_count ))
		{
			void* ptr = GetPtr( m_num_blocks_assigned++ );

			if ( m_num_blocks_assigned > m_num_blocks_assigned_high_watermark )
			{
				m_num_blocks_assigned_high_watermark = m_num_blocks_assigned;
			}
			
			m_total_allocs++;

			return ptr;
		}
		return 0;
	}

	bool UnitHeap::IsMemoryInHeap(void* ptr) const
	{
		if ( m_mem_buffer && ptr )
		{
			int ptrOffset = (char*)ptr - m_mem_buffer ;
			return ( ptrOffset >= 0 ) && ( ptrOffset < m_bufferSize );
		}
		return false;
	}

	bool UnitHeap::ReleaseBlock(void* ptr)
	{
		if ( ptr && m_blockSize && IsMemoryInHeap(ptr) )
		{
			// find index in block table
			int ptrOffset = (char*)ptr - m_mem_buffer ;
			assert( ( ptrOffset % m_blockSize ) == 0 );
			int currIndx = ptrOffset / m_blockSize ;
			if ( ( currIndx >= 0 ) && ( currIndx < m_max_blocks_count ) )
			{
				// move the last used block pointer into the hole

				int currBlockId = m_block_index_table[ currIndx ] ;
				char* currBlockPtr = (char*)ptr ;
				assert( currBlockPtr == GetPtr( currBlockId ) );

				m_num_blocks_assigned--;

				int lastBlockId = m_num_blocks_assigned;
				char* lastBlockPtr = (char*)GetPtr( lastBlockId );
				int lastptrOffset = (char*)lastBlockPtr - m_mem_buffer ;
				int lastIndex = lastptrOffset / m_blockSize ;
			
				// Swap with last entry
				if ( currIndx != lastIndex )
				{
					m_block_index_table[ currIndx ] = lastBlockId;
					m_block_index_table[ lastIndex ] = currBlockId;
					m_block_offset_table[ currBlockId ] = lastIndex;
					m_block_offset_table[ lastBlockId ] = currIndx;
				}

				return true;
			}
			else
			{
				assert(0); // memory does not belong to this heap
			}
		}

		return false;
	}

	void* UnitHeap::GetPtr(int blockId)
	{
		return m_mem_buffer + m_block_offset_table[ blockId ] * m_blockSize;
	}

	int UnitHeap::GetUsedHeap() const
	{
		return m_num_blocks_assigned * m_blockSize;
	}
	
	int UnitHeap::GetTotalHeap() const
	{
		return m_bufferSize;
	}

	int UnitHeap::GetPeak() const
	{
		return m_num_blocks_assigned_high_watermark * m_blockSize;
	}

	///
	///  Growable Unit heap
	///

	GrowableUnitHeap::GrowableUnitHeap() :
		m_blockSize(0),
		m_initialBlockCount(0),
		m_heapCount(0)
	{
	}

	void GrowableUnitHeap::Init(int blockSize, int initialBlockCount)
	{
		//[WARNING!]	ducph rem this assert (crash when load game)
		//ensure not already initialized
		//assert(m_blockSize == 0 && m_initialBlockCount == 0);

		m_blockSize = blockSize;
		m_initialBlockCount = initialBlockCount;
		m_heapCount = 0;
		memset(m_heaps, 0, sizeof(m_heaps));
	}

	void GrowableUnitHeap::Destroy()
	{
		for(int i = 0; i < m_heapCount; i++)
		{
			m_heaps[i]->Destroy();
			SwfFree(m_heaps[i]);
			m_heaps[i] = NULL;
		}
		m_heapCount = 0;
	}

	bool GrowableUnitHeap::CanAllocate(int size) const
	{
		for(int i = 0; i < m_heapCount; i++)
		{
			if(m_heaps[i]->CanAllocate(size))
				return true;
		}
		return false;
	}
	
	void* GrowableUnitHeap::AcquireBlock(int size)
	{
		for(int i = 0; i < m_heapCount; i++)
		{
			void* ptr = m_heaps[i]->AcquireBlock(size);
			if(ptr)
				return ptr;
		}

		// Need to expand heap
		if(m_heapCount + 1 >= MAX_GROW_COUNT)
		{
			//printf("Not enough room to allocate memory block %d\n", size);
			return NULL;
		}
		
		int blockCount = m_initialBlockCount; //(m_heapCount == 0 ? 1 : (int)pow(1.0f, (m_heapCount - 1)/2)) * m_initialBlockCount;
		UnitHeap* heap = (UnitHeap*)SwfAlloc(sizeof(UnitHeap), swf::MEMHINT_NONE, __FILE__, __LINE__);
		heap->Init(m_blockSize, blockCount);
		memmove(&m_heaps[1], &m_heaps[0], m_heapCount * sizeof(UnitHeap*));
		m_heaps[0] = heap;
		m_heapCount++;
		
		void* ptr = heap->AcquireBlock(size);
		assert(ptr != NULL);
		return ptr;
	}
	
	bool GrowableUnitHeap::IsMemoryInHeap(void* ptr) const
	{
		for(int i = 0; i < m_heapCount; i++)
		{
			if(m_heaps[i]->IsMemoryInHeap(ptr))
				return true;
		}
		return false;
	}
	
	bool GrowableUnitHeap::ReleaseBlock(void* ptr)
	{
		for(int i = 0; i < m_heapCount; i++)
		{
			if(m_heaps[i]->ReleaseBlock(ptr))
				return true;
		}
		return false;
	}

	int GrowableUnitHeap::GetUsedHeap() const
	{
		int total = 0;
		
		for(int i = 0; i < m_heapCount; i++)
			total += m_heaps[i]->GetUsedHeap();

		return total;
	}

	int GrowableUnitHeap::GetTotalHeap() const
	{
		int total = 0;
		
		for(int i = 0; i < m_heapCount; i++)
			total += m_heaps[i]->GetTotalHeap();

		return total;
	}

	int GrowableUnitHeap::GetPeak() const
	{
		int total = 0;
		
		for(int i = 0; i < m_heapCount; i++)
			total += m_heaps[i]->GetPeak();

		return total;
	}

	//void InitUnitMem()
	//{
	//	if (!s_heapsInitialized)
	//	{
	//		Init( &s_unitHeaps[0], 4, 32*1024 );
	//		Init( &s_unitHeaps[1], 16, 29*1024 );
	//		Init( &s_unitHeaps[2], 32, 5*1024 );
	//		Init( &s_unitHeaps[3], 64, 3*1024 );
	//		Init( &s_unitHeaps[4], 128, 12*1024 );
	//		Init( &s_unitHeaps[5], 256, 3*1024 );
	//		s_heapsInitialized = true;
	//	}
	//}

}; // namespace gameswf
