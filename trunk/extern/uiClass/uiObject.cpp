// uiObject.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 18/12/2008
// Thong tin:

#include "uiObject.h"
#include "uiInclude.h"
#include "uiApplication.h"

using namespace uiClass;

// getObject
// Tra ve ID de xac dinh doi tuong
DWORD uiObject::getObject()
{
	return 0;
}

uiObject::uiObject()
{
	zm_refOject = NULL;
	zm_bNew		= false;
}

uiObject::~uiObject()
{
	// Xoa chinh ban than no
	if ( zm_bNew )
	{
		*zm_thisObject = NULL;
	}
	

	if ( zm_refOject )
	{
		list<uiObject*>::iterator i = zm_refOject->begin();
		while ( i != zm_refOject->end() )
		{
			// Neu chua xoa
			if ( *i )
				// Xoa doi tuong
				delete (*i);
			
			i++;
		}
		zm_refOject->clear();
		delete zm_refOject;
	}	
	
}

void uiObject::refClear()
{
	if ( zm_refOject )
	{
		list<uiObject*>::iterator i = zm_refOject->begin();
		while ( i != zm_refOject->end() )
		{
			// Neu chua xoa
			if ( *i )
				// Xoa doi tuong
				delete (*i);
			
			i++;
		}
		zm_refOject->clear();
		delete zm_refOject;
		zm_refOject = NULL;
	}
}