// uiButton.h

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/12/2008
// Thong tin:
//		Ngay cuoi nam ko duoc ben em!!!

#ifndef _UIRADIOBUTTON_
#define _UIRADIOBUTTON_

#include "uiInclude.h"
#include "uiEvent.h"
#include "uiWindow.h"

#include <vector>
using std::vector;

namespace uiClass
{

	// Prototype ////////////////////
	class uiRadioGroup;
	class uiRadioButton;
	/////////////////////////////////

	// uiRadioGroup
	// Doi tuong quan ly group button
	class DLLEXPORT uiRadioGroup: public uiObject
	{
	protected:
		vector<uiRadioButton*> zm_aRButton;
	public:
		// addRadioButton
		// Them 1 radio vao group
		void addRadioButton(uiRadioButton* p);

		// removeRadioButton
		// Loai bo 1 radio ra khoi group
		void removeRadioButton(uiRadioButton* p);

		// unCheck
		// Uncheck het tat ca cac radio trong group
		void unCheck();

		// getObject
		// Tra ve doi tuong radioGroup
		virtual DWORD getObject();
	};

	// uiRadioButton
	// Doi tuong control RadioButton
	class DLLEXPORT uiRadioButton: public uiControl
	{
	protected:
		uiEventCallback	eventOnClicked;
		uiEventCallback	eventOnDbClicked;

		uiRadioGroup *zm_pGroup;

		uiRadioButton();
	public:
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnClicked(T* pObject)	{	eventOnClicked.setEvent<T,pTMethod>( pObject ); }
		
		template<class T,void (T::*pTMethod)(uiObject*)>
		void setEventOnDbClicked(T* pObject){	eventOnDbClicked.setEvent<T,pTMethod>( pObject ); }

	public:
		uiRadioButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent);
		uiRadioButton(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, uiRadioGroup *pGroup);
		virtual ~uiRadioButton(void);

		// create
		// Khoi tao uiCheckbox
		virtual bool create(LPCWSTR lpTitle, int x, int y, int w, int h, uiWindow* pParent, uiRadioGroup *pGroup);

		// messageControlMap
		// Phan loai MSGs
		virtual void messageControlMap(UINT uMsg);
		
		// getObject
		// Tra ve UIOBJECT
		virtual DWORD getObject();
		
		// setGroup
		// Thiet lap nhom cho radio button
		void setGroup(uiRadioGroup *pGroup);

		// removeGroup
		// Loai bo nhom cho radio button
		void removeGroup();

		// setCheck
		// Thiet lap check cho radio
		void setCheck(bool b);

		// getCheck
		// Lay trang thai cua radio
		bool getCheck();

	};
}

#endif
