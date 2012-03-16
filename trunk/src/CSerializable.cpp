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

// getType
// get type from string
NSSerializable::dataType CSerializable::getType( char *lpType )
{
	for ( int i = NSSerializable::unknownType; i <  NSSerializable::numType; i++ )
	{
		if ( strcmp( lpType, s_stringType[i] ) == 0 )
			return (NSSerializable::dataType) i;
	}

	return NSSerializable::unknownType;
}

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

// getProperty
// get property
SSerializableRec* CSerializable::getProperty( char *name )
{
	int i = 0, n = m_data.size();
	for ( i = 0; i < n; i++ )
	{
		SSerializableRec *pRec = &m_data[i];
		if ( strcmp( pRec->name, name ) == 0 )
			return pRec;
	}
	return NULL;
}

void CSerializable::saveItem( std::ofstream& f,  ArraySerializableRecIter& it,  int nTab )
{
	uiStringA tab = "";
	for ( int i = 0; i < nTab; i++ )
		tab += "\t";

	ArraySerializableRecIter end = m_data.end();
	while ( it != end )
	{
		SSerializableRec *p = &(*it);
		
		if ( p->type == NSSerializable::groupInfo )
		{
			it--;
			return;
		}
		else if ( p->type != NSSerializable::arrayByte )
		{
			// save data
			f << tab.c() << p->name  << ": " << p->data << " | " << s_stringType[p->type] << ";\n";
			it++;
		}
		else
		{
			// array Byte
			it++;
		}
	}
}

void CSerializable::saveGroup( std::ofstream& f, ArraySerializableRecIter& it, int nTab )
{
	uiStringA tab = "";
	for ( int i = 0; i < nTab; i++ )
		tab += "\t";

	ArraySerializableRecIter end = m_data.end();
		
	while ( it != end )
	{
		SSerializableRec *p = &(*it);

		if ( p->type == NSSerializable::groupInfo )
		{
			f << tab.c() << p->name << "\n";
			f << tab.c() << "{\n";
			
			it++;
			saveItem( f, it, nTab + 1 );
			
			f << tab.c() << "}\n";
		}

	}

}

void CSerializable::saveData( std::ofstream& f, int nTab )
{
	ArraySerializableRecIter it = m_data.begin();
	
	// save group
	saveGroup( f, it, nTab );	
}

void getBufferString( char *lpBuffer, char *from, char *to )
{
	int len = to - from;
	
	uiString::mid(lpBuffer, from, 0, len);
	uiString::trim(lpBuffer);
}

bool CSerializable::readData( char* &pData )
{
	int i = 0;
	
	char *p		= pData;
	char *from	= p;
	char *split = NULL;
	char *dot	= NULL;

	char lpBuffer[1024];

	SSerializableRec	rec;

	while ( *p != 0 )
	{
		// begin group
		if ( *p == '{' )
		{			
			memset( &rec, 0, sizeof(SSerializableRec) );

			// get group name
			getBufferString( rec.name, from, p );
			rec.type = NSSerializable::groupInfo;

			// add group
			addRow( &rec );
			
			from	= p + 1;
			split	= NULL;
			dot		= NULL;
		}
		// end group
		else if ( *p == '}' )
		{
			pData = p + 1;
			return true;
		}
		else if ( *p == ';' )
		{
			// error data
			if ( dot == NULL || split == NULL )
			{
				pData = p + 1;
				return false;
			}

			memset( &rec, 0, sizeof(SSerializableRec) );

			// get name property
			getBufferString( rec.name, from,		dot );

			// get data property
			getBufferString( rec.data, dot + 1,		split );

			// get data type 
			getBufferString( lpBuffer, split + 1,	p );
			int type = 0;
			for ( type = 0; type < NSSerializable::numType; type++ )
			{
				if ( strcmp( lpBuffer, s_stringType[type] ) == 0 )
					break;
			}
			rec.type = (NSSerializable::dataType)type;

			// add record
			addRow( &rec );

			from	= p + 1;
			split	= NULL;
			dot		= NULL;			
		}
		else if ( *p == '|' )
		{
			split = p;
		}
		else if ( *p == ':' )
		{
			dot = p;
		}

		p++;
	}

	pData = p;
	return false;
}