#include "stdafx.h"
#include "CSerializable.h"

CSerializable::CSerializable()
{

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