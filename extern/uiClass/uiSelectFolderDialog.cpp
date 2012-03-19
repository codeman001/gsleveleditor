#include "uiSelectFolderDialog.h"
#include <shlobj.h>
#include <Shlwapi.h>

using namespace uiClass;

uiSelectFolderDialog::uiSelectFolderDialog()
{
}

uiSelectFolderDialog::~uiSelectFolderDialog()
{
}

int CALLBACK BrowseCallbackProc( HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	if (uMsg==BFFM_INITIALIZED)
		SendMessage(hWnd,BFFM_SETSELECTION,TRUE,lpData);
	return 0;
}

bool uiSelectFolderDialog::doModal( uiWindow *pParent, WCHAR *lpPath, WCHAR* lpTitle )
{
	BROWSEINFO info;
	memset(&info, 0, sizeof(BROWSEINFO));

	WCHAR name[MAX_PATH];

	info.hwndOwner = pParent->getHandle();
    info.pidlRoot = NULL;
	info.pszDisplayName = name;
	info.lpszTitle = lpTitle;
	info.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	info.lpfn = NULL;    
	info.ulFlags = BIF_VALIDATE | 64;

	LPITEMIDLIST items = NULL;	
	items = SHBrowseForFolder(&info);
	if (!items) 
		return false;

	SHGetPathFromIDList(items, m_lpFolderPath);

	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);
	pMalloc->Free(items);
	pMalloc->Release();
	
	return true;
}

void uiSelectFolderDialog::getFolderPath( WCHAR* lpFolderPath )
{
	uiString::copy<WCHAR, WCHAR>( lpFolderPath, m_lpFolderPath );
}