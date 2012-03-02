#include "StdAfx.h"
#include "RibbonApplication.h"
#include "RibbonCommandHandler.h"
#include "ids.h"
#include "CGlobalInstance.h"

CRibbonApplication::~CRibbonApplication(void)
{
}

//
//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
//
//  PURPOSE: Called when the state of a View (Ribbon is a view) changes, for example, created, destroyed, or resized.
//
//
STDMETHODIMP CRibbonApplication::OnViewChanged(UINT32 nViewID, UI_VIEWTYPE typeID,
IUnknown* pView, UI_VIEWVERB verb, INT32 uReasonCode)
{
    UNREFERENCED_PARAMETER(nViewID);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(pView);
    UNREFERENCED_PARAMETER(uReasonCode);

	return S_OK;
}

//
//  FUNCTION: OnCreateUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler)
//
//  PURPOSE: Called by the Ribbon framework for each command specified in markup, to allow
//           the host application to bind a command handler to that command.
//
//  COMMENTS:
//
//    In this Gallery sample, there is one handler for each gallery, and one for all of
//    the buttons in the Size and Color gallery.
//
//
STDMETHODIMP CRibbonApplication::OnCreateUICommand(UINT32 nCmdID, UI_COMMANDTYPE typeID,
IUICommandHandler** ppCommandHandler)
{
    UNREFERENCED_PARAMETER(typeID);
   
	CRibbonCommandHandler *pCommandHandler = NULL;
	HRESULT	hr = CRibbonCommandHandler::CreateInstance(&pCommandHandler);

	CGlobalInstance::getInstance()->m_ribbonCommand = pCommandHandler;

    if (SUCCEEDED(hr))
    {
		hr = pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
        pCommandHandler->Release();
	}        
	
	return S_OK;
}

//
//  FUNCTION: OnDestroyUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler*)
//
//  PURPOSE: Called by the Ribbon framework for each command at the time of ribbon destruction.
//
//
STDMETHODIMP CRibbonApplication::OnDestroyUICommand(UINT32 commandId, UI_COMMANDTYPE typeID,
IUICommandHandler* pCommandHandler)
{
    UNREFERENCED_PARAMETER(commandId);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(pCommandHandler);
    return E_NOTIMPL;
}

HRESULT CRibbonApplication::CreateInstance(CRibbonApplication **ppApplication, HWND hwnd)
{
    if (!ppApplication)
    {
        return E_POINTER;
    }
    if (!hwnd)
    {
        return E_INVALIDARG;
    }

    *ppApplication = NULL;

    HRESULT hr = S_OK;

    CRibbonApplication* pApplication = new CRibbonApplication();

    if (pApplication != NULL)
    {
        pApplication->m_hwnd = hwnd;
        *ppApplication = pApplication;
        
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown methods.
STDMETHODIMP_(ULONG) CRibbonApplication::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CRibbonApplication::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CRibbonApplication::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUIApplication))
    {
        *ppv = static_cast<IUIApplication*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}
