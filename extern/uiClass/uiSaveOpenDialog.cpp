#include "uiSaveOpenDialog.h"

using namespace uiClass;

uiSaveOpenDialog::uiSaveOpenDialog()
{
	m_lpFileName[0] = 0;
}

uiSaveOpenDialog::~uiSaveOpenDialog()
{
}

void uiSaveOpenDialog::clearAllFileExt()
{
	m_stringTitle.clear();
	m_stringExt.clear();
}

void uiSaveOpenDialog::addFileExt( WCHAR* title, WCHAR *ext )
{
	m_stringTitle.push_back( title );
	m_stringExt.push_back( ext );
}

bool uiSaveOpenDialog::doModal(uiWindow *pParent, bool saveDialog)
{
	WCHAR filter[1024];
	
	list<uiStringW>::iterator iTitle = m_stringTitle.begin(), titleEnd = m_stringTitle.end();
	list<uiStringW>::iterator iExt = m_stringExt.begin();
	
	int pos = 0;

	while ( iTitle != titleEnd )
	{
		memcpy( filter + pos, (*iTitle).c(), (*iTitle).getLength() * sizeof(WCHAR) );
		pos += (*iTitle).getLength();

		filter[ pos++ ] = NULL;

		memcpy( filter + pos, (*iExt).c(), (*iExt).getLength() * sizeof(WCHAR) );
		pos += (*iExt).getLength();

		filter[ pos++ ] = NULL;

		iTitle++;
		iExt++;
	}
	filter[ pos++ ] = NULL;

	OPENFILENAME ofn;
    
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = pParent == NULL ? NULL : pParent->getHandle();
	ofn.lpstrFilter = filter;
    ofn.lpstrFile = m_lpFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"*.*";

	if ( saveDialog )
	{
		if ( GetSaveFileName(&ofn) )
		{
			return true;
		}
		else
		{
			m_lpFileName[0] = 0;
			return false;
		}
	}
	else
	{
		if( GetOpenFileName(&ofn) )
		{
			return true;
		}
		else
		{
			m_lpFileName[0] = 0;
			return false;
		}
	}

	return false;
}

void uiSaveOpenDialog::getFileName( WCHAR* lpFileName )
{
	uiString::copy<WCHAR, WCHAR>( lpFileName, m_lpFileName );
}