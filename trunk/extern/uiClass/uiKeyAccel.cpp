// uiKeyAccel.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 4/3/2009 
// Thong tin:
//

#include "uiKeyAccel.h"
#include "uiInclude.h"
#include "uiApplication.h"

using namespace uiClass;

// getObject
// Tra ve doi tuong UIOBJECT
DWORD uiKeyAccel::getObject()
{
	return UIOBJECT_KEYACCEL;
}

uiKeyAccel::uiKeyAccel(uiKeyAccelEntry* pEntry)
{
	WORD entry = pEntry->getCount();
	ACCEL *p = new ACCEL[ entry ];
	
	for (WORD i = 0; i < entry ; i++ )
		pEntry->getIndex( i, &p[i] );

	zm_hKeyAcell = CreateAcceleratorTableW( p, entry );

	delete p;
}

uiKeyAccel::~uiKeyAccel()
{
	DestroyAcceleratorTable( zm_hKeyAcell );
}

// getHandle
// Tra ve handle ACCEL
HACCEL uiKeyAccel::getHandle()
{
	return zm_hKeyAcell;
}

////////////////////////////////////
// keyAccelEntry
////////////////////////////////////

DWORD uiKeyAccelEntry::getObject()
{
	return UIOBJECT_KEYACCELENTRY;
}

// pushKey
// Them phim tat
void uiKeyAccelEntry::pushKey( BYTE vKey, WORD key, WORD cmd )
{
	ACCEL c;
	c.cmd   = cmd;
	c.fVirt	= vKey;
	c.key	= key;
	
	zm_listAccel.push_back( c );
}

// getCount
// Dem so key
WORD uiKeyAccelEntry::getCount()
{
	return (WORD) zm_listAccel.size();
}

// getIndex
// Lay ACCEL thu index
bool uiKeyAccelEntry::getIndex( WORD nIndex, ACCEL* pAccel )
{
	if ( nIndex  < (WORD)zm_listAccel.size() )
	{
		*pAccel = zm_listAccel[ nIndex ];
		return true;
	}	

	return false;
}