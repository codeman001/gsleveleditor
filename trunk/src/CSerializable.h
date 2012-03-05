#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

#include "stdafx.h"

namespace NSSerializable
{
	enum dataType
	{
		unknownType = 0,
		intType,
		longType,
		floatType,
		doubleType,
		boolType,
		stringType
	};
}

using NSSerializable::dataType;

typedef struct tagSerializableRec
{
	char		name[64];
	char		data[256];
	dataType	type;
	bool		readOnly;

	tagSerializableRec()
	{
		memset( name, 0, 64 );
		memset( data, 0, 256 );

		readOnly = false;
		type = NSSerializable::unknownType;
	}

	tagSerializableRec( char* name, void *value, dataType type, bool readOnly )
	{
		strcpy( this->name, name );
		
		this->readOnly =  readOnly;
		this->type = type;

		if ( type == NSSerializable::intType )
		{
			int n = *((int*)value);
			sprintf( this->data, "%d", n );
		}
		if ( type == NSSerializable::longType )
		{
			long n = *((long*)value);
			sprintf( this->data, "%d", n );
		}
		else if ( type == NSSerializable::floatType )
		{
			float f = *((float*)value);
			sprintf( this->data, "%.7f", f );
		}
		else if ( type == NSSerializable::doubleType )
		{
			double f = *((double*)value);
			sprintf( this->data, "%.7f", f );
		}
		else if ( type == NSSerializable::boolType )
		{
			bool b = *((bool*)value);
			if ( b )
				strcpy( this->data, "true" );
			else
				strcpy( this->data, "false" );

		}
		else if ( type == NSSerializable::stringType )
		{
			strcpy( this->data, (char*)value );
		}
	}

	const tagSerializableRec& operator= (const tagSerializableRec& s)
	{
		memcpy( name, s.name, 64 );
		memcpy( data, s.data, 256 );
		type = s.type;
		readOnly = s.readOnly;

		return *this;
	}
}SSerializableRec;

class CSerializable
{
protected:
	vector<SSerializableRec>	m_data;
public:
	CSerializable();

	virtual ~CSerializable();

	// clear
	// clear all property
	void clear()
	{
		m_data.clear();
	}

	// addRow
	// add a property
	void addRow(char *name, void* value, dataType type, bool readOnly = false);
};

#endif