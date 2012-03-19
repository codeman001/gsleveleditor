#ifndef _RIBBONITEM_
#define _RIBBONITEM_

#include <uiribbon.h>
#include <uiribbonpropertyhelpers.h>
#pragma comment(lib,"propsys.lib")
#pragma comment(lib,"shlwapi.lib")

class CRibbonItem
    : public IUISimplePropertySet
{
public:
	uiStringW		m_text;
    int				m_categoryId;
    IUIImage*		m_image;
    int				m_commandId;
    UI_COMMANDTYPE	m_commandType;
protected:
    LONG m_cRef;
public:
	STDMETHOD(GetValue)(REFPROPERTYKEY key, PROPVARIANT *ppropvar);

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID iid, void** ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();
public:
	CRibbonItem();
	virtual ~CRibbonItem();
};

#endif