#pragma once
#include "stdafx.h"
#include <uiribbon.h>
#include "CRibbonItem.h"
#include "RibbonFontUtil.h"
#include <UIRibbonPropertyHelpers.h>

class CRibbonCommandHandler : public IUICommandHandler
{
public:
    STDMETHOD(Execute)(UINT nCmdID,
                       UI_EXECUTIONVERB verb, 
                       const PROPERTYKEY* key,
                       const PROPVARIANT* ppropvarValue,
                       IUISimplePropertySet* pCommandExecutionProperties);

    STDMETHOD(UpdateProperty)(UINT nCmdID,
                              REFPROPERTYKEY key,
                              const PROPVARIANT* ppropvarCurrentValue,
                              PROPVARIANT* ppropvarNewValue);

    static HRESULT CreateInstance(CRibbonCommandHandler **ppHandler);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
protected:	
	CRibbonItem* createRibbonItem( IUICollection* p, wchar_t *lpString, UINT imageID );

public:


protected:
	

public:
	

public:
	void setSelectItem( UINT idCmd, int select );
	void getSelectItem( UINT idCmd, int *select );
	void setColorCommand( UINT idCmd, UINT rgbColor );
	bool getColorCommand( UINT idCmd, UINT *rgbColor );
	void setEnableCommand( UINT idCmd, bool enable );
	void setSelectCommand( UINT	idCmd, bool select );
	void setVisibleCommand( UINT idCmd, bool visible );
	
	void resetDefaultCommand();
	void selectOnlyCommand( UINT idCmd );

	void showContextMenu(int menuID, POINT& ptLocation, HWND hWnd);
	void getDisplayLocation(POINT &pt, HWND hWnd);

public:
	void onUndo();
	void onRedo();

	void onEditorCameraCommand();
	void onSelectedCameraCommand();

	void onAddObjectCommand();
	void onAddCameraCommand();
	void onAddWaypointCommand();

	void onSelectObjectCommand();
	void onRotateObjectCommand();
	void onScaleObjectCommand();
	void onDeleteSelectObjectCommand();

	bool onNewDocumentCommand();
	bool onSaveDocumentCommand();
	bool onOpenDocumentCommand();

	void onShowGridCommand();
	void onShowObjectCommand();

	void onCreateZoneCommand();
	void onDeleteObjectComand();
private:

    CRibbonCommandHandler()
        : m_cRef(1), m_pifbFactory(NULL)
    {
    }

    ~CRibbonCommandHandler()
    {
        if (m_pifbFactory != NULL)
        {
            m_pifbFactory->Release();
        }
    }

    HRESULT CreateUIImageFromBitmapResource(LPCTSTR pszResource, IUIImage **ppimg);
    IUIImageFromBitmap* m_pifbFactory;

    LONG m_cRef;
};