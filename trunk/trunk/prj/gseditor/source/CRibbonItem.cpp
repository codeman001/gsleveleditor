#include "stdafx.h"
#include "CRibbonItem.h"

CRibbonItem::CRibbonItem()
{
	m_image = NULL;
	m_categoryId = UI_COLLECTION_INVALIDINDEX;
	m_commandId = -1;
	m_commandType = UI_COMMANDTYPE_UNKNOWN;
	m_cRef = 1;
}

CRibbonItem::~CRibbonItem()
{
}


// IUnknown methods.
STDMETHODIMP_(ULONG) CRibbonItem::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CRibbonItem::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CRibbonItem::QueryInterface(REFIID iid, void** ppv)
{
    if (!ppv)
    {
        return E_POINTER;
    }

    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUISimplePropertySet))
    {
        *ppv = static_cast<IUISimplePropertySet*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}


//
//  FUNCTION: GetValue(REFPROPERTYKEY, PROPVARIANT*)
//
//  PURPOSE: Retrieves the value of one of the properties used when adding something to a gallery.
//
//  COMMENTS:
//    This will be called by the framework on the property sets returned for the ItemsSource and 
//    Categories properties in order to create and display the contents of the gallery.
//
//
STDMETHODIMP CRibbonItem::GetValue(REFPROPERTYKEY key, PROPVARIANT *ppropvar)
{
    if (key == UI_PKEY_ItemImage)
    {
		if (m_image)
        {
            return UIInitPropertyFromImage(UI_PKEY_ItemImage, m_image, ppropvar);
        }
        return S_FALSE;
    }
    else if (key == UI_PKEY_Label)
    {
        return UIInitPropertyFromString(UI_PKEY_Label, m_text.c(), ppropvar);
    }
    else if (key == UI_PKEY_CategoryId)
    {
        return UIInitPropertyFromUInt32(UI_PKEY_CategoryId, m_categoryId, ppropvar);
    }
    else if (key == UI_PKEY_CommandId)
    {
        if(m_commandId != -1)
        {
            return UIInitPropertyFromUInt32(UI_PKEY_CommandId, m_commandId, ppropvar);
        }
        return S_FALSE;
    }
    else if (key == UI_PKEY_CommandType)
    {
        return UIInitPropertyFromUInt32(UI_PKEY_CommandType, m_commandType, ppropvar);
    }
    return E_FAIL;
}