// uiKeyAccel.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 4/3/2008
// Thong tin:
//

#ifndef _UIKEYACCEL_
#define _UIKEYACCEL_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiObject.h"
#include "uiMenu.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{
	class uiKeyAccel;
	class uiKeyAccelEntry;
		
	// uiKeyAccel
	// Doi tuong keyAccel
	class DLLEXPORT uiKeyAccel: public uiObject
	{
	protected:
		HACCEL zm_hKeyAcell;
	public:
		// getObject
		// Tra ve doi tuong UIOBJECT
		virtual DWORD getObject();

		uiKeyAccel(uiKeyAccelEntry* pEntry);		
		virtual ~uiKeyAccel();

		// getHandle
		// Tra ve handle ACCEL
		HACCEL getHandle();
	};

	// uiKeyAccelEntry
	// Doi tuong keyAccellEntry
	class DLLEXPORT uiKeyAccelEntry: public uiObject
	{
	protected:
		vector<ACCEL> zm_listAccel;
	public:
		// getObject
		// Tra ve doi tuong UIOBJECT
		virtual DWORD getObject();
		
		// pushKey
		// Them phim tat
		void pushKey( BYTE vKey, WORD key, WORD cmd );

		// getCount
		// Dem so key
		WORD getCount();

		// getIndex
		// Lay ACCEL thu index
		bool getIndex( WORD nIndex, ACCEL* pAccel );

	};

}

#endif
