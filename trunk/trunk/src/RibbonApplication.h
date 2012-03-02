#pragma once

#include <uiribbon.h>

class CRibbonApplication: public IUIApplication // Applications must implement IUIApplication.
{		
public:
	~CRibbonApplication(void);

    STDMETHOD(OnViewChanged)(UINT32 nViewID, UI_VIEWTYPE typeID,
    IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode);

    STDMETHOD(OnCreateUICommand)(UINT32 nCmdID, UI_COMMANDTYPE typeID,
    IUICommandHandler** ppCommandHandler);

    STDMETHOD(OnDestroyUICommand)(UINT32 commandId, UI_COMMANDTYPE typeID,
    IUICommandHandler* pCommandHandler);

    static HRESULT CreateInstance(CRibbonApplication **ppHandler, HWND hwnd);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

private:
    CRibbonApplication()
        : m_cRef(1), m_hwnd(NULL)
    {
    }

    LONG m_cRef;
    HWND m_hwnd;
};
