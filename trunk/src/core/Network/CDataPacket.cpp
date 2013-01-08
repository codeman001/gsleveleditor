#include "stdafx.h"
#include "CDataPacket.h"

#include "md5.h"

#define	CHECKSUMBYTE	2

CDataPacket::CDataPacket( int size )
{
	m_messageBody = new unsigned char[size];
	memset( m_messageBody, 0, size );
	
	m_pos = CHECKSUMBYTE;	
	m_size = size;
    m_cacheSize = size;
}

CDataPacket::CDataPacket( unsigned char *data, int size )
{
	m_messageBody = new unsigned char[size];
	memcpy( m_messageBody, data, size );
		
	m_pos = CHECKSUMBYTE;
	m_size = size;
    m_cacheSize = size;
}

void CDataPacket::resetPacket()
{
	memset( m_messageBody, 0, m_cacheSize );	
	m_pos = CHECKSUMBYTE;    
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


// packData
// calc checksum before send data
void CDataPacket::packData()
{
	m_size = m_pos;

	unsigned char *sum = calcDataChecksum();
	memcpy(m_messageBody, sum, CHECKSUMBYTE);
}


// checkData
// check data revc
bool CDataPacket::checkData()
{
	short ret = 0;

	// get checksum from data
	memcpy(&ret, m_messageBody, CHECKSUMBYTE);

	// calc data checksum
	short sum = *((short*)(calcDataChecksum()));

	// check checksum
	if ( ret == sum )
		return true;

	return false;
}

// calcDataChecksum
unsigned char* CDataPacket::calcDataChecksum()
{
	unsigned char ret[16];
	memset(ret, 0, 16);

	// clear checksum data
	memset(m_messageBody, 0, CHECKSUMBYTE);

	// calc md5 checksum
	MD5_CTX	md5Contex;
	MD5_Init( &md5Contex );
	MD5_Update( &md5Contex, m_messageBody, (unsigned long)m_size );
	MD5_Final( ret, &md5Contex );

	// calc sign number
	static short value = 0;
	value = 0;
	for (int i = 0; i < 16; i++)
		value += ret[i];

	return (unsigned char*)&value;
}
