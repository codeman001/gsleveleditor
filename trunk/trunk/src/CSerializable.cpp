#include "stdafx.h"
#include "CSerializable.h"

const char* CSerializable::s_stringType[] = 
{
	"unknownType",
	"intType",
	"longType",
	"floatType",
	"doubleType",
	"boolType",
	"stringType",
	"filePathType",
	"arrayByte",
	"groupInfo"	
};

CSerializable::CSerializable()
{
	m_cursor = 0;
}

CSerializable::~CSerializable()
{

}

// addRow
// add a property
void CSerializable::addRow(char *name, void* value, dataType type, bool readOnly)
{
	SSerializableRec rec(name, value, type, readOnly);
	m_data.push_back( rec );
}

// addArrayByte
// add array byte to record
void CSerializable::addArrayByte(char *name, char *value, int size, bool readOnly )
{	 
	m_data.push_back( SSerializableRec() );

	SSerializableRec& rec = m_data.back();

	strcpy( rec.name, name );

	rec.readOnly = readOnly;
	rec.type = NSSerializable::arrayByte;

	rec.exData = new char[size];
	memcpy( rec.exData, value, size );

	rec.exDataSize = size;
}


int	CSerializable::readInt()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::intType )
		return 0;

	int value;
	sscanf( rec.data, "%d", &value );
	return value;
}

long CSerializable::readLong()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::longType )
		return 0;

	long value;
	sscanf( rec.data, "%ld", &value );
	return value;
}

float CSerializable::readFloat()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::floatType )
		return 0;

	double value = 0;
	sscanf( rec.data, "%lf", &value );
	return (float)value;
}

double CSerializable::readDouble()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::doubleType )
		return 0;

	double value;
	sscanf( rec.data, "%lf", &value );
	return value;
}

bool CSerializable::readBool()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::boolType )
		return 0;

	if ( strcmp( rec.data, "true" ) == 0 )
		return true;
	
	return false;
}

char* CSerializable::readString()
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::stringType || rec.type != NSSerializable::filePathType )
		return NULL;

	return rec.data;
}

void CSerializable::readArrayByte( char* data, int *size )
{
	SSerializableRec& rec = m_data[ m_cursor++ ];
	if ( rec.type != NSSerializable::arrayByte )
	{
		*size = 0;
		return;
	}

	memcpy( data, rec.exData, rec.exDataSize );
	*size = rec.exDataSize;
}