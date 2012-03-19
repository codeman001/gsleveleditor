// uiListPropertyGroup.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 12/10/2011
// Thong tin:
//

#ifndef _UILISTPROPERTYITEzm_
#define _UILISTPROPERTYITEzm_

#include "uiInclude.h"
#include "uiWindow.h"


// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{	
	class uiListProperty;

	class DLLEXPORT uiListPropertyItem: public uiObject
	{
	public:
		static const int	sm_itemHeight = 18;
	protected:
		uiListProperty		*zm_control;
		uiListPropertyItem	*zm_parent;
		int					zm_itemHeight;
		uiStringW			zm_label;

		int					zm_nColumn;
		vector<int>			zm_nColumSize;

		RECT				zm_rect;

		bool				zm_visible;
		bool				zm_focus;

		int					zm_objectType;
	public:
		uiListPropertyItem(uiListProperty* control, uiListPropertyItem *pParent, int nCol );

		virtual ~uiListPropertyItem();

		// setLabel
		// Dat label cho group item
		void setLabel( LPWSTR lpLabel );

		// getLabel
		// Lay chuoi ky tu label
		LPWSTR getLabel();

		// setHeight
		// thiet lap chieu cao cho height
		void setHeight( int height );

		// getHeight
		// lay chieu cao
		int getHeight();

		// drawGroup
		// ve group
		virtual void drawItem( uiGraphics* pG, int y ) = 0;

		// setParent
		// set parent
		void setParent( uiListPropertyItem* parent );

		// setRect
		// thiet lap rect
		void setRect( int x, int y, int w, int h );

		// setVisible
		// hien thi item hay ko
		void setVisible(bool b);

		// isVisible
		// kiem tra item
		bool isVisible();
		
		// setFocus
		// thiet lap focus cho item
		virtual void setFocus(bool b);

		// isFocus
		// kiem tra item co focus hay ko
		bool isFocus();

		// checkHit
		// kiem tra doi tuong co bi click hay ko
		virtual bool checkHit( int x, int y );

		// getObjectType
		// tra ve kieu obj
		int getObjectType()
		{
			return zm_objectType;
		}

		// setColSize
		// thiet lap chieu cao cho item
		void setColSize(int col, int size);
	};
}

#endif