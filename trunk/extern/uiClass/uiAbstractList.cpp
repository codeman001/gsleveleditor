// uiComboBox.cpp

// Lap trinh vien: Pham Hong Duc
// Email: hong_duc_it87@yahoo.com
// Ngay viet: 31/12/2008
// Thong tin:

#include "uiAbstractList.h"
#include "uiInclude.h"
#include "uiApplication.h"

using namespace uiClass;

uiAbstractListItem::uiAbstractListItem( uiAbstractListControl* p )
{
	zm_pListControl = p;
	zm_pData = NULL;
}

uiAbstractListItem::~uiAbstractListItem()
{

}

// setData
// Thiet lap data user
void uiAbstractListItem::setData(uiObject *p)
{
	zm_pData = p;
}

// getData
// Thiet lap data user
uiObject* uiAbstractListItem::getData()
{
	return zm_pData;	
}

// updateItem
// Cap nhat item
void uiAbstractListItem::update()
{
	zm_pListControl->updateItem( this );
}

uiAbstractListControl::~uiAbstractListControl()
{
	uiAbstractArrayItem::iterator i = zm_listItem.begin();
	while ( i!= zm_listItem.end() )
	{
		delete (*i);
		i++;
	}
}

// insertAbstractItem
// Them 1 phan tu vao mang		
void uiAbstractListControl::insertAbstractItem(int nIndex, uiAbstractListItem* pItem )
{
	if (nIndex < 0 )
		return;

	if (nIndex == (int) zm_listItem.size())
		zm_listItem.push_back( pItem );
	else if ( nIndex > (int) zm_listItem.size() )
		zm_listItem.push_back( pItem );
	else	
		zm_listItem.insert(zm_listItem.begin() + nIndex, pItem );	
}

// removeAbstractItem
// Xoa phan tu thu nIndex
void uiAbstractListControl::removeAbstractItem(int nIndex)
{
	if (nIndex > (int) zm_listItem.size() || nIndex < 0)
		return;
	else
	{
		delete zm_listItem[nIndex];
		zm_listItem.erase( zm_listItem.begin() + nIndex );		
	}
}


//////////////////////////////////////////////////////
///		uiIconListControl
//////////////////////////////////////////////////////


uiIconListControl::uiIconListControl()
{
	zm_hLargeIcon = ImageList_Create( 32,32, ILC_COLOR32|ILC_MASK, 5, 10 );
	zm_hSmallIcon = ImageList_Create( 16,16, ILC_COLOR32|ILC_MASK, 5, 10 );
}

uiIconListControl::~uiIconListControl()
{
	ImageList_Destroy( zm_hLargeIcon );
	ImageList_Destroy( zm_hSmallIcon );
}
		
// pushImage
// Them 1 hinh anh vao list
DWORD uiIconListControl::pushLargeImage( uiBitmap* pImage )
{
	if ( pImage == NULL )	
		return 0;	
	else	
		ImageList_Add( zm_hLargeIcon, (HBITMAP) pImage->getSafeObject(), NULL );

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;
}

DWORD uiIconListControl::pushLargeImage( uiIcon*	pImage )
{		
	if ( pImage == NULL )	
		return 0;	
	else	
		ImageList_AddIcon ( zm_hLargeIcon,(HICON) pImage->getHandle() );

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;
}

DWORD uiIconListControl::pushLargeImage( uiBitmap* pImage, uiColor mask )
{
	if ( pImage == NULL )	
		return 0;	
	else	
		ImageList_AddMasked( zm_hLargeIcon, (HBITMAP) pImage->getSafeObject(), (COLORREF) mask.getColor() );

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;
}

// removeImage
// Xoa 1 image da push vao list
void uiIconListControl::removeLargeImage( DWORD nIndex )
{
	ImageList_Remove( zm_hLargeIcon, nIndex );
}

// pushImage
// Them 1 hinh anh vao list
DWORD uiIconListControl::pushSmallImage( uiBitmap* pImage )
{
	if ( pImage == NULL )	
		ImageList_Add( zm_hLargeIcon, (HBITMAP) 0, NULL );	
	else
		ImageList_Add( zm_hSmallIcon, (HBITMAP) pImage->getSafeObject(), NULL );	

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;
}		

DWORD uiIconListControl::pushSmallImage( uiIcon*	pImage )
{
	if ( pImage == NULL )	
		ImageList_Add( zm_hLargeIcon, (HBITMAP) 0, NULL );	
	else
		ImageList_AddIcon ( zm_hSmallIcon,(HICON) pImage->getHandle() );

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;	
}

DWORD uiIconListControl::pushSmallImage( uiBitmap* pImage, uiColor mask  )
{
	if ( pImage == NULL )	
		return 0;	
	else	
		ImageList_AddMasked( zm_hSmallIcon, (HBITMAP) pImage->getSafeObject(), (COLORREF) mask.getColor() );

	return ImageList_GetImageCount( zm_hSmallIcon ) - 1;
}

// removeImage
// Xoa 1 image da push vao list
void uiIconListControl::removeSmallImage( DWORD nIndex )
{
	ImageList_Remove( zm_hSmallIcon, nIndex );
}

// removeAllSmall
// Xoa toan bo small icon
void uiIconListControl::removeAllSmall()
{
	ImageList_RemoveAll( zm_hSmallIcon );
}

// removeAllLarge
// Xoa toan bo large icon
void uiIconListControl::removeAllLarge()
{
	ImageList_RemoveAll( zm_hSmallIcon );
}

// getHandle
// Lay handle ICON lon
HIMAGELIST uiIconListControl::getHandleLargeIcon()
{
	return zm_hLargeIcon;
}

// getHandleSmall
// Lay handle ICON nho
HIMAGELIST uiIconListControl::getHandleSmallIcon()
{
	return zm_hSmallIcon;
}