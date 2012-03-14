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
		stringType,
		filePathType,
		arrayByte,
		groupInfo,
		numType
	};	
}

using NSSerializable::dataType;

typedef struct tagSerializableRec
{
	char		name[64];
	char		data[256];
	dataType	type;
	bool		readOnly;
	char		*exData;
	char		exDataSize;
	
	// tag object
	uiObject	*tagObject;

	tagSerializableRec()
	{
		memset( name, 0, 64 );
		memset( data, 0, 256 );

		readOnly = false;
		type = NSSerializable::unknownType;

		exData = NULL;
		exDataSize = 0;

		tagObject = NULL;
	}

	~tagSerializableRec()
	{
		if ( exData )
			delete exData;
	}

	tagSerializableRec( char* name, void *value, dataType type, bool readOnly )
	{
		strcpy( this->name, name );
		
		this->readOnly =  readOnly;
		this->type = type;
		this->exData = NULL;
		this->exDataSize = 0;
		this->tagObject = NULL;

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
		else if ( type == NSSerializable::stringType || type == NSSerializable::filePathType )
		{
			strcpy( this->data, (char*)value );
		}
		else
		{
			this->data[0] = NULL;
		}
	}

	tagSerializableRec& operator= (const tagSerializableRec& s)
	{
		memcpy( name, s.name, 64 );
		memcpy( data, s.data, 256 );
		type = s.type;
		readOnly = s.readOnly;

		if ( s.type == NSSerializable::arrayByte )
		{
			memcpy( exData, s.exData, exDataSize);
			exDataSize = s.exDataSize;
		}
		return *this;
	}
}SSerializableRec;


class CSerializable
{
public:
	static const char *s_stringType[NSSerializable::numType];

protected:	
	vector<SSerializableRec>	m_data;

	int m_cursor;
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

	// addInt
	// add int value to record
	inline void addInt(char *name, int value, bool readOnly = false )
	{
		addRow(name, &value, NSSerializable::intType, readOnly);
	}

	// addLong
	// add long value to record
	inline void addLong(char *name, long value, bool readOnly = false )
	{
		addRow(name, &value, NSSerializable::longType, readOnly);
	}

	// addFloat
	// add float value to record
	inline void addFloat(char *name, float value, bool readOnly = false )
	{
		addRow(name, &value, NSSerializable::floatType, readOnly);
	}

	// addDouble
	// add double value to record
	inline void addDouble(char *name, double value, bool readOnly = false )
	{
		addRow(name, &value, NSSerializable::doubleType, readOnly);
	}

	// addBool
	// add bool value to record
	inline void addBool(char *name, bool value, bool readOnly = false )
	{
		addRow(name, &value, NSSerializable::boolType, readOnly);
	}

	// addString
	// add bool value to record
	inline void addString(char *name, const char *value, bool readOnly = false )
	{
		addRow(name, (void*)value, NSSerializable::stringType, readOnly);
	}

	// add file path
	// add path to record
	inline void addPath(char *name, const char *value, bool readOnly = false )
	{
		addRow(name, (void*)value, NSSerializable::filePathType, readOnly);
	}

	// addGroup
	// add group label for object
	inline void addGroup(const char *name)
	{
		addRow( (char*)name, NULL, NSSerializable::groupInfo, true);
	}

	// addArrayByte
	// add array byte to record
	void addArrayByte(char *name, char *value, int size, bool readOnly = false );

	// setCursorRecord
	// set position of cursor to read record
	inline void setCursorRecord(int cursor)
	{
		m_cursor = cursor;
	}
	
	// getCursorRecord
	// get position of cursor
	inline int getCursorRecord()
	{
		return m_cursor;
	}

	// nextRecord
	// peek the next record
	inline void nextRecord()
	{
		m_cursor++;
	}

	// readRawRecord
	// read all info
	inline SSerializableRec* readRawRecord()
	{
		if ( m_cursor >= (int)m_data.size() )
			return NULL;

		return &m_data[ m_cursor++ ];
	}

	// getAllRecord
	// get all record
	inline vector<SSerializableRec>* getAllRecord()
	{
		return &m_data;
	}

	int		readInt();
	long	readLong();
	float	readFloat();
	double	readDouble();
	bool	readBool();
	char*	readString();
	void	readArrayByte( char* data, int *size );
};

typedef vector<SSerializableRec>			ArraySerializableRec;
typedef vector<SSerializableRec>::iterator	ArraySerializableRecIter;

#endif