#pragma once
// membuf.h	-- Thatcher Ulrich 2005

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// A simple memory buffer.  Similar to a string, but can hold null
// characters.


#ifndef MEMBUF_H
#define MEMBUF_H


#include "base/tu_config.h"
#include "base/utility.h"

namespace gameswf
{

class tu_string;


struct membuf
{
	membuf();
	membuf(const void* data, int size);
	membuf(const membuf& buf);
	membuf(const tu_string& str);
	~membuf();

	// Construct a read-only membuf that points at the given data,
	// instead of copying it.
	enum read_only_enum { READ_ONLY };
	membuf(read_only_enum e, const void* data, int size);

	int size() const { return m_size; }
	const void* data() const { return m_data; }
	void* data() { assert(!m_read_only); return m_data; }

	// Don't call these mutators on read-only membufs.
	
	void reserve(int new_capacity);
	void resize(int new_size);
	void append(const void* data, int size);
	void append(const membuf& buf);
	void append(Uint8 byte);

	// We do not append the terminating '\0'.
	void append(const tu_string& str);
	void shrink();

	Uint8&	operator[](int index);
	const Uint8&	operator[](int index) const;
	void	operator=(const membuf& buf);
	bool	operator==(const membuf& buf) const;
	bool	operator!=(const membuf& buf) const;

	static int capacity(int size);

private:

	int m_size;
	int m_capacity;
	void* m_data;
	bool m_read_only;
};

inline void membuf::resize(int new_size)
{
	assert(!m_read_only);

	if (new_size == m_size)
	{
		return;
	}

	if(new_size > m_capacity)
	{
		int new_capacity = capacity(new_size);
		reserve(new_capacity);
	}

	assert(m_capacity >= new_size);
	m_size = new_size;
}

inline void membuf::append(Uint8 byte)
{
	assert(m_read_only == false);

	int old_size = size();
	int new_size = old_size + 1;
	
	if(new_size < m_capacity)
		m_size = new_size;
	else
		resize(new_size);

	((Uint8*) m_data)[old_size] = byte;
}

inline Uint8&	membuf::operator[](int index)
{
	assert(index >= 0 && index < m_size); 
	return ((Uint8*) m_data)[index]; 
}

inline const Uint8&	membuf::operator[](int index) const
{
	assert(index >= 0 && index < m_size); 
	return ((Uint8*) m_data)[index]; 
}

}; // namespace gameswf

#endif // MEMBUF_H


// Local Variables:
// mode: C++
// c-basic-offset: 8 
// tab-width: 8
// indent-tabs-mode: t
// End:
