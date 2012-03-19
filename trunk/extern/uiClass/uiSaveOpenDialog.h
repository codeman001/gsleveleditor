// uiSaveOpenDialog.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 31/11/2011
// Thong tin:
//

#ifndef _UISAVEOPENDIALOG_
#define _UISAVEOPENDIALOG_

#include "uiInclude.h"
#include "uiWindow.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{
	class DLLEXPORT uiSaveOpenDialog: public uiObject
	{
	protected:
		WCHAR m_lpFileName[MAX_PATH];

		list<uiStringW> m_stringTitle;
		list<uiStringW> m_stringExt;
	public:
		uiSaveOpenDialog();

		virtual ~uiSaveOpenDialog();
		
		void clearAllFileExt();

		void addFileExt( WCHAR* title, WCHAR *ext );

		bool doModal( uiWindow *pParent,bool saveDialog);

		void getFileName( WCHAR* lpFileName );
	};
}

#endif