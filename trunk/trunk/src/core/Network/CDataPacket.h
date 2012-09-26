#ifndef _DATAPACKET_H_
#define _DATAPACKET_H_

#ifdef HAS_MULTIPLAYER

#include "stdafx.h"

class CDataPacket
{
protected:
	unsigned char	*m_messageBody;
	int				m_pos;
	int				m_size;
public:
	CDataPacket( int size );
	CDataPacket( unsigned char *data, int size );

	virtual ~CDataPacket();

	// addData
	// push data to packet
	inline void addData( const void* data, int size )
	{
		memcpy( (void*)(m_messageBody + m_pos), data, size );
		m_pos += size;
	}

	// getData
	// read data
	inline void getData( void *data, int size )
	{
		memcpy( data, (void*)(m_messageBody + m_pos), size );
		m_pos += size;
	}

	// add data
	// ----------------------
	void	addByte(unsigned char c);
	unsigned char getByte();

	void	addShort(unsigned short data);
	unsigned short getShort();

	void	addInt(int data);
	int		getInt();

	void	addFloat(float data);
	float	getFloat();

	
	// packData
	// calc checksum before send data
	void	packData();

	// checkData
	// check data revc
	bool	checkData();

	// calcDataChecksum
	unsigned char* calcDataChecksum();

	// getMessageData
	// get data as bytes
	inline void* getMessageData()
	{
		return m_messageBody;
	}

	// getMessageSize
	// get data size
	inline int getMessageSize()
	{
		return m_pos;
	}
};

#endif

#endif