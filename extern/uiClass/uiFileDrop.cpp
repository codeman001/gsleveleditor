// uiSizeEvent.cpp
// 
// Lap trinh vien: Pham Hong Duc
// Email: hongduc.pr@gmail.com
// Ngay viet: 8/8/2010
// Thong tin:
//		- Khai bao cac function cho uiSizeEvent

#include "uiEvent.h"
#include "uiInclude.h"

using namespace uiClass;

// add file to list
void uiFileDrop::addFile( WCHAR* lpFile )
{
	m_listFile.push_back( uiStringW(lpFile) );
}
		
// getObject
// Tra ve UIOBJECT_FILEDROP
DWORD uiFileDrop::getObject()
{
	return UIOBJECT_FILEDROP;
}

// getFileCount
// Lay so luong file
int uiFileDrop::getFileCount()
{
	return (int)m_listFile.size();
}

// getFilePath
// Lay duong dan file
WCHAR* uiFileDrop::getFilePath( int index )
{
	if ( index >= (int)m_listFile.size() )
		return NULL;

	return m_listFile[index].c();
}
