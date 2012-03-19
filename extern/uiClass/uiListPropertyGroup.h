// uiListPropertyGroup.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 12/10/2011
// Thong tin:
//

#ifndef _UILISTPROPERTYGROUP_
#define _UILISTPROPERTYGROUP_

#include "uiInclude.h"
#include "uiWindow.h"
#include "uiListPropertyItem.h"

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{	
	class uiListProperty;


	class DLLEXPORT uiListPropertyGroup: public uiListPropertyItem
	{
	protected:
		uiColor zm_bgColor;
		uiColor zm_textColor;
		uiFont	zm_font;
		
		int		zm_textPadding;

		bool	zm_enableColText;
		vector<uiStringW>	zm_colText;
	public:	

		// mapPropertyGroupItem
		// Constructor
		uiListPropertyGroup( uiListProperty *pControl, uiListPropertyItem* pParent, LPWSTR lpLabel, int nColumn );

		virtual ~uiListPropertyGroup();

		// drawGroup
		// ve group
		virtual void drawItem( uiGraphics* pG, int y );

		// enableColText
		// thiet lap group hien thi text theo column
		virtual void enableColText( bool b );

		// setColText
		// thiet lap text cho column
		virtual void setColText( WCHAR *lpString, int col );
	};

}
#endif