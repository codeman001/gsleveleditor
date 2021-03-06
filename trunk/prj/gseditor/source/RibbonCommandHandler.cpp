#include "StdAfx.h"
#include "RibbonCommandHandler.h"
#include "ids.h"
#include "Resource.h"
#include "CGlobalInstance.h"

#include "IView.h"
#include "IDoc.h"
#include "IController.h"

#include "CZone.h"
#include "CDocument.h"

STDMETHODIMP CRibbonCommandHandler::Execute(UINT nCmdID,
                   UI_EXECUTIONVERB verb, 
                   const PROPERTYKEY* key,
                   const PROPVARIANT* ppropvarValue,
                   IUISimplePropertySet* pCommandExecutionProperties)
{
    
	switch (verb)
    {
    case UI_EXECUTIONVERB_PREVIEW:				
        break;
    case UI_EXECUTIONVERB_CANCELPREVIEW:		
        break;
    case UI_EXECUTIONVERB_EXECUTE:
		if ( nCmdID == ID_SELECT )
			onSelectObjectCommand();
		else if ( nCmdID == ID_ROTATE )
			onRotateObjectCommand();
		else if ( nCmdID == ID_SCALE )
			onScaleObjectCommand();
		else if ( nCmdID == ID_ONTERRAIN )
			onSnapObjOnTerrainCommand();
		else if ( nCmdID == ID_CAMERAVIEW )
		{
			if ( key )
			{
				UINT selected;
				UIPropertyToUInt32(*key, *ppropvarValue, &selected);
				
				if ( selected == 0 )
					onEditorCameraCommand();
				else
					onSelectedCameraCommand();
			}
		}

		else if ( nCmdID == ID_ADDOBJECT )
			onAddObjectCommand();		
		else if ( nCmdID == ID_ADDWAYPOINT )
			onAddWaypointCommand();
		else if ( nCmdID == ID_ADDCAMERA )
			onAddCameraCommand();
		else if ( nCmdID == ID_ADDTRIGGER )
			onAddTriggerCommand();
		else if ( nCmdID == ID_ADDLIGHT )
			onAddLightCommand();

		else if ( nCmdID == ID_NEW )
			onNewDocumentCommand();
		else if ( nCmdID == ID_SAVE )
			onSaveDocumentCommand();
		else if ( nCmdID == ID_OPEN )
			onOpenDocumentCommand();
		else if ( nCmdID == ID_DRAWGRID )
			onShowGridCommand();
		else if ( nCmdID == ID_DRAWOBJECT )
			onShowObjectCommand();
		else if ( nCmdID == ID_CREATEZONE )
			onCreateZoneCommand();
		else if ( nCmdID == ID_DELETEZONE )
			onDeleteObjectComand();
		else if ( nCmdID == ID_DELETEOBJECT )
			onDeleteObjectComand();

		break;
    }
	
	// active window
	uiApplication::getRoot()->setFocus();

	return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    Depending on the value of key, this will populate the gallery, update the selected item or 
//    text, or enable/disable the gallery.
//
//
STDMETHODIMP CRibbonCommandHandler::UpdateProperty(UINT nCmdID,
                              REFPROPERTYKEY key,
                              const PROPVARIANT* ppropvarCurrentValue,
                              PROPVARIANT* ppropvarNewValue)
{
	IView	*pView = getIView();
	IDoc	*pDoc = NULL;
	if ( pView )
		pDoc = pView->getDocument();

	if ( nCmdID == ID_DRAWOBJECT )
	{		
		CGameObject *pObj = pView->getObjectForContext();
		if ( pObj )
		{
			bool visible = pObj->isUIVisible();
			setSelectCommand( nCmdID, visible );
		}
		else
		{
			setSelectCommand( nCmdID, true );
		}		
	}
	else if ( nCmdID == ID_DRAWGRID )
	{
		CDocument *p = (CDocument*) pDoc;
		setSelectCommand( nCmdID, p->isShowGrid() );		
	}
	else if ( nCmdID == ID_CAMERAVIEW )
	{
		if (key == UI_PKEY_ItemsSource)
		{				
			IUICollection* pCollection;
			ppropvarCurrentValue->punkVal->QueryInterface(IID_PPV_ARGS(&pCollection));

			createRibbonItem( pCollection, L"Editor camera",		NULL );
			createRibbonItem( pCollection, L"Selected camera",		NULL );			

			pCollection->Release();							
		}
		else if ( key == UI_PKEY_SelectedItem )
		{
			UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, 0, ppropvarNewValue);
		}
	}

	return S_OK;
}

HRESULT CRibbonCommandHandler::CreateInstance(CRibbonCommandHandler **ppHandler)
{
    if (!ppHandler)
    {
        return E_POINTER;
    }

    *ppHandler = NULL;

    HRESULT hr = S_OK;

    CRibbonCommandHandler* pHandler = new CRibbonCommandHandler();

    if (pHandler != NULL)
    {
        *ppHandler = pHandler;        
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown methods.
STDMETHODIMP_(ULONG) CRibbonCommandHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CRibbonCommandHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CRibbonCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler))
    {
        *ppv = static_cast<IUICommandHandler*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

HRESULT CRibbonCommandHandler::CreateUIImageFromBitmapResource(LPCTSTR pszResource, IUIImage **ppimg)
{
    HRESULT hr = E_FAIL;

    *ppimg = NULL;

    if (NULL == m_pifbFactory)
    {
        hr = CoCreateInstance(CLSID_UIRibbonImageFromBitmapFactory, NULL, CLSCTX_ALL, IID_PPV_ARGS(&m_pifbFactory));
        if (FAILED(hr))
        {
            return hr;
        }
    }

    // Load the bitmap from the resource file.
    HBITMAP hbm = (HBITMAP) LoadImage(GetModuleHandle(NULL), pszResource, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    if (hbm)
    {
        // Use the factory implemented by the framework to produce an IUIImage.
        hr = m_pifbFactory->CreateImage(hbm, UI_OWNERSHIP_TRANSFER, ppimg);
        if (FAILED(hr))
        {
            DeleteObject(hbm);
        }
    }
    return hr;
}

CRibbonItem* CRibbonCommandHandler::createRibbonItem( IUICollection *pCollection, wchar_t *lpString, UINT imageID )
{
	CRibbonItem *pItem = new CRibbonItem();	
	pItem->m_text = lpString;

	if ( imageID > 0 )
	{
		IUIImage *pImg;
		if ( CreateUIImageFromBitmapResource( MAKEINTRESOURCE( imageID ), &pImg) == S_OK )
			pItem->m_image = pImg;
	}	
	
	pCollection->Add( pItem );
	pItem->Release();

	return pItem;
}

void CRibbonCommandHandler::setSelectItem( UINT idCmd, int select )
{	
	PROPVARIANT ret;	
	UIInitPropertyFromUInt32(UI_PKEY_SelectedItem, select, &ret);
	CGlobalInstance::getInstance()->m_ribbonFramework->SetUICommandProperty( idCmd, UI_PKEY_SelectedItem, ret );
}

void CRibbonCommandHandler::getSelectItem( UINT idCmd, int *select )
{
	PROPVARIANT ret;
	CGlobalInstance::getInstance()->m_ribbonFramework->GetUICommandProperty( idCmd, UI_PKEY_SelectedItem, &ret );

	UINT uSelect;
	UIPropertyToUInt32(UI_PKEY_SelectedItem, ret, &uSelect);
	*select = (int)uSelect;
}

void CRibbonCommandHandler::setColorCommand( UINT idCmd, UINT rgbColor )
{
	
}

bool CRibbonCommandHandler::getColorCommand( UINT idCmd, UINT *rgbColor )
{		
	PROPVARIANT ret;
	CGlobalInstance::getInstance()->m_ribbonFramework->GetUICommandProperty( idCmd, UI_PKEY_ColorType, &ret );

	// Retrieve color type.
	UINT type = UI_SWATCHCOLORTYPE_NOCOLOR;
    UIPropertyToUInt32(UI_PKEY_ColorType, ret, &type);

	*rgbColor = 0;

	if ( type == UI_SWATCHCOLORTYPE_RGB )
	{
		PROPVARIANT ret;
		
		CGlobalInstance::getInstance()->m_ribbonFramework->GetUICommandProperty( idCmd, UI_PKEY_Color, &ret );
		UIPropertyToUInt32(UI_PKEY_Color, ret, rgbColor);

		// select color
		return true;
	}
	
	// no color
	return false;
}

void CRibbonCommandHandler::setEnableCommand( UINT idCmd, bool enable )
{
	// enable, disable font control
	PROPVARIANT ret;
	UIInitPropertyFromBoolean(UI_PKEY_Enabled, enable, &ret);

	CGlobalInstance::getInstance()->m_ribbonFramework->SetUICommandProperty( idCmd, UI_PKEY_Enabled, ret );
}

void CRibbonCommandHandler::setSelectCommand( UINT	idCmd, bool select )
{
	PROPVARIANT ret;
	UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, select, &ret);
	CGlobalInstance::getInstance()->m_ribbonFramework->SetUICommandProperty( idCmd, UI_PKEY_BooleanValue, ret );
}

void CRibbonCommandHandler::setVisibleCommand( UINT idCmd, bool visible )
{
	PROPVARIANT ret;
	UIInitPropertyFromBoolean(UI_PKEY_BooleanValue, visible, &ret);
	CGlobalInstance::getInstance()->m_ribbonFramework->SetUICommandProperty( idCmd, UI_PKEY_Viewable, ret );
}


void CRibbonCommandHandler::resetDefaultCommand()
{	
	setSelectCommand( ID_SELECT,	false );
	setSelectCommand( ID_ROTATE,	false );
	setSelectCommand( ID_SCALE,		false );

	setSelectCommand( ID_CAMERAVIEW,	false );
	setSelectCommand( ID_ADDOBJECT,		false );
	setSelectCommand( ID_ADDWAYPOINT,	false );
	setSelectCommand( ID_ADDCAMERA,		false );
	setSelectCommand( ID_ADDTRIGGER,	false );
	setSelectCommand( ID_ADDLIGHT,		false );


	IUIFramework *ribbonFramework = CGlobalInstance::getInstance()->m_ribbonFramework;

	// refresh control
	ribbonFramework->InvalidateUICommand( ID_SELECT,	UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_ROTATE,	UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_SCALE,		UI_INVALIDATIONS_STATE, NULL );
	
	ribbonFramework->InvalidateUICommand( ID_CAMERAVIEW,	UI_INVALIDATIONS_STATE, NULL );

	ribbonFramework->InvalidateUICommand( ID_ADDOBJECT,		UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_ADDWAYPOINT,	UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_ADDCAMERA,		UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_ADDTRIGGER,	UI_INVALIDATIONS_STATE, NULL );
	ribbonFramework->InvalidateUICommand( ID_ADDLIGHT,		UI_INVALIDATIONS_STATE, NULL );
	
}

void CRibbonCommandHandler::selectOnlyCommand( UINT idCmd )
{
	resetDefaultCommand();
	setSelectCommand( idCmd, true );
}

void CRibbonCommandHandler::showContextMenu(int menuID, POINT& ptLocation, HWND hWnd)
{
	getDisplayLocation(ptLocation, hWnd);

    HRESULT hr = E_FAIL;
	
	IUIFramework*	pFramework = CGlobalInstance::getInstance()->m_ribbonFramework;

    // The basic pattern of how to show Contextual UI in a specified location.
    IUIContextualUI* pContextualUI = NULL;
    if (SUCCEEDED(pFramework->GetView( menuID, IID_PPV_ARGS(&pContextualUI))))
    {
        hr = pContextualUI->ShowAtLocation(ptLocation.x, ptLocation.y);
        pContextualUI->Release();
    }
}

void CRibbonCommandHandler::getDisplayLocation(POINT &pt, HWND hWnd)
{
	if (pt.x == -1 && pt.y == -1)
    {
        HRESULT hr = E_FAIL;

        // Display the menu in the upper-left corner of the client area, below the ribbon.
		IUIFramework*	pFramework = CGlobalInstance::getInstance()->m_ribbonFramework;

        IUIRibbon* pRibbon;
        hr = pFramework->GetView(0, IID_PPV_ARGS(&pRibbon));
        if (SUCCEEDED(hr))
        {
            UINT32 uRibbonHeight = 0;
            hr = pRibbon->GetHeight(&uRibbonHeight);
            if (SUCCEEDED(hr))
            {
                pt.x = 0;
                pt.y = uRibbonHeight;
                ClientToScreen(hWnd, &pt);
            }
            pRibbon->Release();
        }
        if (FAILED(hr))
        {
            // Default to just the upper-right corner of the entire screen.
            pt.x = 0;
            pt.y = 0;
        }
    }
}


/////////////////////////////////////////////////////////
// COMMAND 
/////////////////////////////////////////////////////////

void CRibbonCommandHandler::onUndo()
{
	if ( CHistoryManager::getInstance()->undo() == false )
		getIView()->alertError( L"Can not undo action" );
}

void CRibbonCommandHandler::onRedo()
{
	if ( CHistoryManager::getInstance()->redo() == false )
		getIView()->alertError( L"Can not redo action" );
}

void CRibbonCommandHandler::onEditorCameraCommand()
{
	selectOnlyCommand( ID_CAMERAVIEW );
	CControllerManager::getInstance()->setController( IController::NullController );
	getIView()->enableFreeCamera( true );
	
	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();	

	// set editor design camera
	CGameCamera *pDesignCam = pDoc->getDesignCamera();
	getIView()->setActiveCamera( pDesignCam );
}

void CRibbonCommandHandler::onSelectedCameraCommand()
{
	selectOnlyCommand( ID_CAMERAVIEW );
	CControllerManager::getInstance()->setController( IController::NullController );
	
	IView *pView = getIView();

	pView->enableFreeCamera( true );

	// clear select
	IDoc *pDoc = pView->getDocument();	

	// set camera
	CGameObject *pSelected = pDoc->getFirstObjSelect();
	if ( pSelected && pSelected->getObjectType() == CGameObject::CameraObject )
	{
		pView->setActiveCamera( (CGameCamera*) pSelected );
		pView->setObjectProperty( NULL );
		pDoc->clearSelect();
	}
	else
	{
		getIView()->alertError( L"Please select a camera object\n" );
	}
}

void CRibbonCommandHandler::onAddObjectCommand()
{
	selectOnlyCommand( ID_ADDOBJECT );
	CControllerManager::getInstance()->setController( IController::AddObject );
	getIView()->enableFreeCamera( false );

	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();	
}

void CRibbonCommandHandler::onAddCameraCommand()
{
	selectOnlyCommand( ID_ADDCAMERA );
	CControllerManager::getInstance()->setController( IController::AddCamera );
	getIView()->enableFreeCamera( false );

	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();	
}

void CRibbonCommandHandler::onAddWaypointCommand()
{
	selectOnlyCommand( ID_ADDWAYPOINT );
	CControllerManager::getInstance()->setController( IController::AddWaypoint );
	getIView()->enableFreeCamera( false );

	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();	
}

void CRibbonCommandHandler::onAddTriggerCommand()
{
	selectOnlyCommand( ID_ADDTRIGGER );
	CControllerManager::getInstance()->setController( IController::AddTrigger );
	getIView()->enableFreeCamera( false );

	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();
}

void CRibbonCommandHandler::onAddLightCommand()
{
	selectOnlyCommand( ID_ADDLIGHT );
	CControllerManager::getInstance()->setController( IController::AddLight );
	getIView()->enableFreeCamera( false );

	// clear select
	IDoc *pDoc = getIView()->getDocument();
	pDoc->clearSelect();
}

void CRibbonCommandHandler::onSelectObjectCommand()
{
	selectOnlyCommand( ID_SELECT );
	CControllerManager::getInstance()->setController( IController::SelectObject );
	getIView()->getDocument()->setStateForSelectObject( CGameObject::Move );
	getIView()->enableFreeCamera( false );
}

void CRibbonCommandHandler::onRotateObjectCommand()
{
	selectOnlyCommand( ID_ROTATE );
	CControllerManager::getInstance()->setController( IController::RotateObject );
	getIView()->getDocument()->setStateForSelectObject( CGameObject::Rotation );
	getIView()->enableFreeCamera( false );
}

void CRibbonCommandHandler::onScaleObjectCommand()
{
	selectOnlyCommand( ID_SCALE );
	CControllerManager::getInstance()->setController( IController::ScaleObject );
	getIView()->getDocument()->setStateForSelectObject( CGameObject::Scale );
	getIView()->enableFreeCamera( false );
}

void CRibbonCommandHandler::onSnapObjOnTerrainCommand()
{
	IDoc *pDoc = getIView()->getDocument();

	ArrayGameObject* listSelect = pDoc->getSelectObject();
	ArrayGameObjectIter i = listSelect->begin(), iEnd = listSelect->end();

	CHistoryManager *pHistory = CHistoryManager::getInstance();
	pHistory->beginHistory();

	while ( i != iEnd )
	{
		CGameObject *pObj = (*i);
		CZone *pZone = (CZone*)pObj->getParent();
		
		if ( pZone != NULL && pObj->getComponent( IObjectComponent::Terrain ) == NULL )
		{
			core::vector3df pos = pObj->getPosition();
			
			float h;
			if ( pZone->getHeigthFromTerrain( pos, &h) == true )
			{
				pos.Y = h;
				pHistory->addHistoryBeginModifyObj( pObj );
				pObj->setPosition( pos );
				pHistory->addHistoryEndModifyObj( pObj );
			}
		}

		i++;
	}
	pHistory->endHistory();

	if ( listSelect->size() > 0 )
		getIView()->setObjectProperty( listSelect->front() );

}

void CRibbonCommandHandler::onDeleteSelectObjectCommand()
{
	IDoc *pDoc = getIView()->getDocument();
	
	getIView()->setObjectProperty( NULL );

	// get current select
	ArrayGameObject listSelect = *pDoc->getSelectObject();
	pDoc->clearSelect();

	CHistoryManager *pHistory = CHistoryManager::getInstance();
	pHistory->beginHistory();

	// delete object
	ArrayGameObjectIter i = listSelect.begin(), iEnd = listSelect.end();
	while ( i != iEnd )
	{
		CGameObject* pObj = (*i);
		
		CZone *pZone = (CZone*)pObj->getParent();
		if ( pZone )
		{
			pHistory->addHistoryDeleteObj( pObj );
			pZone->removeObject( pObj );
		}
		i++;
	}

	pHistory->endHistory();
}

bool CRibbonCommandHandler::onNewDocumentCommand()
{
	IView	*pView = getIView();
	IDoc	*pDoc = pView->getDocument();

	if ( pDoc->isEmptyDocument() == false )
	{
		if ( pView->yesNoQuestion(L"Do you want to save current level?") == true )
		{
			// request save document
			if ( onSaveDocumentCommand() == false )
			{
				return false;
			}
		}				
	}
	
	// renew document
	pView->newDocument();
	
	// set title for app
	uiStringW caption = TEXT(STR_APP_TITLE);
	uiApplication::getRoot()->setCaption( caption.c() );

	// change to camera command
	onEditorCameraCommand();
	return true;
}

bool CRibbonCommandHandler::onSaveDocumentCommand()
{
	IView	*pView = getIView();
	IDoc	*pDoc = pView->getDocument();

	WCHAR lpPath[ MAX_PATH ] = {0};	
	WCHAR lpFileName[ MAX_PATH ] = {0};

	// show dialog save if the doc is not save
	if ( pDoc->getCurrentFile() == NULL )
	{
		uiSaveOpenDialog dialog;	
		dialog.clearAllFileExt();
		dialog.addFileExt( L"Level info (.lv)", L"*.lv" );
		dialog.addFileExt( L"All files (.*)", L"*.*" );
		if ( dialog.doModal( uiApplication::getRoot(), true ) == false )
			return false;

		dialog.getFileName( lpPath );
	}
	else
	{
		uiString::copy<WCHAR,wchar_t>( lpPath, pDoc->getCurrentFile() );
	}

	pView->setStatusText(0, L"Write file...");
	if ( pDoc->saveDocument( lpPath ) == true )
	{
		pView->setStatusText(0, L"Write file sucess!");
		
		// set title for app
		uiStringW caption = TEXT(STR_APP_TITLE);
		caption += L" - ";
		uiString::getFileName<WCHAR, WCHAR>(lpPath, lpFileName);
		caption += lpFileName;

		uiApplication::getRoot()->setCaption( caption.c() );
		return true;
	}
	else
	{
		uiStringW error = L"Write file error";
		error += lpPath;
		getIView()->alertError( error.c() );
	}

	pView->setStatusText(0, L"Write file is not sucess!");	
	return false;
}
	
bool CRibbonCommandHandler::onOpenDocumentCommand()
{
	IView *pView = getIView();
	WCHAR lpPath[ MAX_PATH ] = {0};	
	WCHAR lpFileName[ MAX_PATH ] = {0};
	
	uiSaveOpenDialog dialog;	
	dialog.clearAllFileExt();
	dialog.addFileExt( L"Level info (.lv)", L"*.lv" );
	dialog.addFileExt( L"All files (.*)", L"*.*" );
	if ( dialog.doModal( uiApplication::getRoot(), false ) == false )
		return false;
	
	// new renew document
	if ( onNewDocumentCommand() == false )
		return false;

	dialog.getFileName( lpPath );

	pView->setStatusText(0, L"Reading file...");
	if ( getIView()->getDocument()->openDocument( lpPath ) == true )
	{		
		pView->setStatusText(0, L"Read file sucess!");

		// set title for app
		uiStringW caption = TEXT(STR_APP_TITLE);
		caption += L" - ";
		uiString::getFileName<WCHAR, WCHAR>(lpPath, lpFileName);
		caption += lpFileName;

		uiApplication::getRoot()->setCaption( caption.c() );
		return true;
	}

	pView->alertError(L"File is error!");
	pView->setStatusText(0, L"File is error!");
	return false;
}

void CRibbonCommandHandler::onShowGridCommand()
{
	CDocument *p = (CDocument*) getIView()->getDocument();
	p->showGrid( !p->isShowGrid() );	
}

void CRibbonCommandHandler::onShowObjectCommand()
{
	CGameObject *pObj = getIView()->getObjectForContext();
	if ( pObj )
	{
		bool isShow = pObj->isUIVisible();
		pObj->setUIVisible( !isShow );
		isShow = pObj->isUIVisible();

		int icon = 0;
		int iconActive = 0;

		if ( isShow == true )
		{
			if ( pObj->getObjectType() == CGameObject::ZoneObject )
			{
				icon = 1;
				iconActive = 2;
			}
			else
			{
				icon = 4;
				iconActive = 4;
			}
		}
		else
		{
			icon = 3;
			iconActive = 3;
		}

		uiTreeViewItem *pItem = pObj->getTreeItem();
		pItem->setIconIndex( icon );
		pItem->setIconStateIndex( iconActive );
		pItem->update();
	}
}

void CRibbonCommandHandler::onCreateZoneCommand()
{
	CDocument *p = (CDocument*) getIView()->getDocument();
	p->createZone();
}

void CRibbonCommandHandler::onDeleteObjectComand()
{	
	CGameObject *pObj = getIView()->getObjectForContext();

	if ( pObj )
	{
		getIView()->setObjectProperty( NULL );

		wchar_t lpString[1024];
		uiString::format<wchar_t>(lpString, L"Are you sure delete object name: %s", (wchar_t*)pObj->getName() );		

		if ( getIView()->yesNoQuestion( lpString ) == true )
		{
			if ( pObj->getObjectType() == CGameObject::ZoneObject )		
			{
				CDocument *p = (CDocument*) getIView()->getDocument();
				p->removeZone( pObj );		
			}
			else
			{
				getIView()->getDocument()->clearSelectObject( pObj );
				CZone *p = (CZone*)pObj->getParent();
				p->removeObject( pObj );
			}
		}
	}
}

/////////////////////////////////////////////////////////
// END COMMAND 
/////////////////////////////////////////////////////////