// uiButton.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/12/2008
// Thong tin:

#include "uiRadioButton.h"
#include "uiInclude.h"
#include "uiApplication.h"

#include <algorithm>
#include <windowsx.h>

using namespace uiClass;

/////////////////////////////////////////////////////////////////
/// RADIO BUTTON
/////////////////////////////////////////////////////////////////

uiRadioButton::uiRadioButton(void)
{
	zm_pGroup = NULL;
}

uiRadioButton::~uiRadioButton(void)
{

}

uiRadioButton::uiRadioButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent)
{
	this->create( lpTitle, x,y,w,h, pParent,NULL );
}

uiRadioButton::uiRadioButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, uiRadioGroup *pGroup)
{
	this->create( lpTitle, x,y,w,h, pParent,pGroup );
}

// create
// Khoi tao uiCheckbox
bool uiRadioButton::create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, uiRadioGroup *pGroup)
{
	assert( pParent );
		
	HINSTANCE hInst = pParent->getInstance();	
	HWND hParent;
	if (pParent == NULL)
		hParent = NULL;
	else
		hParent = pParent->getHandle();		
	
	zm_pParent = pParent;
	 
	this->zm_hInstance = hInst;	

	#pragma warning(disable:4312)

	//Khoi tao cua so
	zm_hWnd = CreateWindowExW(0,
		L"Button",
		lpTitle,
		WS_CHILD | WS_TABSTOP | BS_RADIOBUTTON | WS_VISIBLE,
		x,y,w,h,
		hParent,
		(HMENU) uiApplication::controlRegister(this),
		hInst, NULL);
	#pragma warning(default:4312)

	// Thay doi WND
	this->subClass();

	// Thiet lap quan he
	pParent->addChild(this);
	this->setParent(pParent);
	
	// Thay doi font chu
	SendMessage(zm_hWnd, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT),
               MAKELPARAM(false, 0));

	zm_pGroup = pGroup;

	if ( pGroup )
		pGroup->addRadioButton( this );

	return true;
}

// messageControlMap
// Phan loai MSGs
void uiRadioButton::messageControlMap(UINT uMsg)
{
	bool b = true;
	switch (uMsg)
	{
	case BN_CLICKED:
		
		// Tat toan bo cac button group
		if (zm_pGroup)
			zm_pGroup->unCheck();		
		
		// Set check
		b = this->getCheck();
		if (b == false)
			b = !b;
		this->setCheck( b );

		eventOnClicked(this);
		break;
	case BN_DBLCLK:
		eventOnDbClicked(this);
		break;
	case BN_SETFOCUS:
		eventOnFocus(this);		
		break;
	case BN_KILLFOCUS:
		eventOnLostFocus(this);
		break;
	}
}
		
// getObject
// Tra ve UIOBJECT
DWORD uiRadioButton::getObject()
{
	return UIOBJECT_RADIOBUTTON;
}
		
// rdSetGroup
// Thiet lap nhom cho radio button
void uiRadioButton::setGroup(uiRadioGroup *pGroup)
{
	zm_pGroup = pGroup;
}

// rdRemoveGroup
// Loai bo nhom cho radio button
void uiRadioButton::removeGroup()
{
	zm_pGroup->removeRadioButton( this );
	zm_pGroup = NULL;
}

// rdSetCheck
// Thiet lap check cho radio
void uiRadioButton::setCheck(bool b)
{
	if ( b == true && zm_pGroup )	
		zm_pGroup->unCheck();		
	Button_SetCheck( this->getHandle(), b );
}

// rdGetCheck
// Lay trang thai cua radio
bool uiRadioButton::getCheck()
{
	#pragma warning(disable:4800)
	return (bool) Button_GetCheck( this->getHandle() );
}

/////////////////////////////////////////////////////////////////
/// RADIO GROUP
/////////////////////////////////////////////////////////////////


// addRadioButton
// Them 1 radio vao group
void uiRadioGroup::addRadioButton(uiRadioButton* p)
{
	zm_aRButton.push_back( p );
	p->setGroup( this );

}

// removeRadioButton
// Loai bo 1 radio ra khoi group
void uiRadioGroup::removeRadioButton(uiRadioButton* p)
{
	vector<uiRadioButton*>::iterator i = std::find( zm_aRButton.begin(), zm_aRButton.end(), p );
	
	// Xoa radio
	if ( i != zm_aRButton.end() )
	{
		zm_aRButton.erase( i );
		(*i)->removeGroup();
	}

}

// unCheck
// Uncheck het tat ca cac radio trong group
void uiRadioGroup::unCheck()
{
	vector<uiRadioButton*>::iterator i;
	for (i = zm_aRButton.begin(); i != zm_aRButton.end(); i++)
		(*i)->setCheck(false);
}

// getObject
// Tra ve doi tuong radioGroup
DWORD uiRadioGroup::getObject()
{
	return UIOBJECT_RADIOGROUP;
}