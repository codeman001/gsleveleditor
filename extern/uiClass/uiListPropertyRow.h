// uiListPropertyGroup.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 12/10/2011
// Thong tin:
//

#ifndef _UILISTPROPERTYROW_
#define _UILISTPROPERTYROW_

#include "uiInclude.h"
#include "uiWindow.h"
#include "uiListPropertyItem.h"

#include "uiListPropertyComboBox.h"
#include "uiListPropertyEdit.h"

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{	
	class uiListProperty;
	
	class DLLEXPORT uiListPropertyRow: public uiListPropertyItem
	{
	public:
				
		// Constructor
		uiListPropertyRow( uiListProperty *pControl, uiListPropertyItem* pParent, LPWSTR lpLabel, int nColumn );

		virtual ~uiListPropertyRow();

		// drawGroup
		// ve group
		virtual void drawItem( uiGraphics* pG, int y );

		// isHitOnColum
		// kiem tra xem co click tren colum hay ko
		int isHitOnCol(int x, int y);

		int getColClick();
	
		// getColumnPos
		// lay vi tri col hien tai
		int getColumnPos( int col );

		// getColumnWidth
		// lay kich thuoc cot
		int getColumnWidth( int col );

		virtual bool checkHit( int x, int y );

		// setFocus
		// thiet lap focus cho item
		virtual void setFocus(bool b);
	
		// setActiveCol
		// thiet lap col duoc active
		void setActiveCol( int col );

		// getActiveCol
		// lay col duoc active
		int getActiveCol();

		void setText( LPWSTR lpString, int col );
		
		void getText( LPWSTR lpString, int col );

		// setControl
		// thiet lap control tai cot vi tri
		// controlType = 0 (textEdit)
		// controlType = 1 (comboBox)
		// controlType = 3 (custom control)
		uiWindow* setControl( int controlType, int col, uiWindow *pControl );

		uiWindow* getControl( int col );
	protected:
		
		uiColor	zm_paddingColor;
		uiColor zm_paddingColorH;
		uiColor	zm_lineColor;
		uiFont	zm_font;

		int zm_padding;
		int zm_paddingText;

		int zm_colClick;
		int zm_activeCol;	// the col is mouse over
	protected:
		vector<int>			zm_colPos;
		vector<uiWindow*>	zm_colWindow;
		vector<bool>		zm_colUsercontrol;
		vector<uiStringW>	zm_colText;
	};
}

#endif