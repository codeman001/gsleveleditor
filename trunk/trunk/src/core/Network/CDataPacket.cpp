#include "stdafx.h"
#include "CDataPacket.h"

#ifdef HAS_MULTIPLAYER

CDataPacket::CDataPacket( int size )
{
	m_messageBody = new unsigned char[size];
	memset( m_messageBody, 0, size );

	m_pos = 0;
	m_size = size;
}

CDataPacket::CDataPacket( unsigned char *data, int size )
{
	m_messageBody = new unsigned char[size];
	memcpy( m_messageBody, data, size );

	m_pos = 0;
	m_size = size;
}

CDataPacket::~CDataPacket()
{
	delete m_messageBody;
}

void CDataPacket::addByte(unsigned char c)
{
	m_messageBody[m_pos++] = c;
}

unsigned char CDataPacket::getByte()
{
	return m_messageBody[m_pos++];
}


void CDataPacket::addShort(unsigned short data)
{
	m_messageBody[m_pos++] = (unsigned char)((data & 0x0000FF00)>>8);
    m_messageBody[m_pos++] = (unsigned char)((data & 0x000000FF));
}

unsigned short CDataPacket::getShort()
{
	unsigned short data = 0;
    data |= (m_messageBody[m_pos++] & 0xFF) << 8;
    data |= (m_messageBody[m_pos++] & 0xFF);

    return data;
}

void CDataPacket::addInt(int data)
{
	m_messageBody[m_pos++] = (unsigned char)((data & 0xFF000000)>>24);
    m_messageBody[m_pos++] = (unsigned char)((data & 0x00FF0000)>>16);
    m_messageBody[m_pos++] = (unsigned char)((data & 0x0000FF00)>>8);
    m_messageBody[m_pos++] = (unsigned char)((data & 0x000000FF));
}

int CDataPacket::getInt()
{
	int data = 0;
	data |= (m_messageBody[m_pos++] & 0xFF) << 24;
    data |= (m_messageBody[m_pos++] & 0xFF) << 16;
    data |= (m_messageBody[m_pos++] & 0xFF) << 8;
    data |= (m_messageBody[m_pos++] & 0xFF);

    return data;
}

void CDataPacket::addFloat(float data)
{
	unsigned char *p = (unsigned char*)&data;

	m_messageBody[m_pos++] = *(p);
	m_messageBody[m_pos++] = *(p+1);
	m_messageBody[m_pos++] = *(p+2);
	m_messageBody[m_pos++] = *(p+3);
}

float CDataPacket::getFloat()
{
	float data = 0.0f;
	unsigned char *p = (unsigned char *)&data;

	*(p)	= m_messageBody[m_pos++];
	*(p+1)	= m_messageBody[m_pos++];
	*(p+2)	= m_messageBody[m_pos++];
	*(p+3)	= m_messageBody[m_pos++];

	return data;
}

#endif