// uiSelectFolderDialog.h

// Lap trinh vien: Pham Hong Duc
// Email: duc.phamhong@gameloft.com
// Ngay viet: 25/2/2005
// Thong tin:
//

#ifndef _UISELECTOPENDIALOG_
#define _UISELECTOPENDIALOG_

#include "uiInclude.h"
#include "uiWindow.h"

using std::vector;

// Ko can bao warning vector<dll class>
#pragma warning(disable: 4251)

namespace uiClass
{
	class DLLEXPORT uiSelectFolderDialog: public uiObject
	{
	protected:
		WCHAR m_lpFolderPath[MAX_PATH];

	public:
		uiSelectFolderDialog();

		virtual ~uiSelectFolderDialog();
	
		bool doModal( uiWindow *pParent, WCHAR *lpPath, WCHAR* lpTitle);

		void getFolderPath( WCHAR* lpFolderPath );
	};
}

#endif