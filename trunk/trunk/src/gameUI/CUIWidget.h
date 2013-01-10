#ifndef _UIWIDGET_H_
#define _UIWIDGET_H_

#include "stdafx.h"
#include "IEventReceiver.h"

#include "swfUI/CGameUI.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CMenuFxObj.h"

#include "gameControl/CTouchManager.h"
#include "gameDebug/CGameDebug.h"

class CUIWidget
{
public:
    struct SButtonLabel
    {
        std::string     label;
        std::string     text;
        std::wstring    textw;
    };
    
protected:
	CUIWidget				*m_parent;
	std::vector<CUIWidget*>	m_childs;
    std::string             m_widgetName;
    
	core::recti				m_rect;
	int						m_controlID;
	bool					m_visible;
    
    bool                    m_lockActionOnChild;
    
    std::vector<SButtonLabel>   m_labels;    
    CMenuFxObj                  m_flashObj;
public:
	CUIWidget();
	CUIWidget(const char *name, CUIWidget* parent);
	CUIWidget(const char *name, CUIWidget* parent, CMenuFxObj flashObj);

	virtual ~CUIWidget();

	// addChild
	// add child widget
	void addChild( CUIWidget *child );
	
	// removeChild
	// remove a child from childs list
	void removeChild( CUIWidget *child );

	// destroyChild
	// destroy all child
	void destroyChild();

	// remove
	// remove from parent
	void remove();
	
	// getRect
	// get bounding rect of control
	const core::recti& getRect()
	{
		return m_rect;
	}

	// setControlID
	// set touch id
	inline void setControlID( int id )
	{
		m_controlID = id;
	}

	// getControlID
	// get touch id
	inline int getControlID()
	{
		return m_controlID;
	}

	// isVisible
	// check widget is visible
	inline bool isVisible()
	{
		return m_visible;
	}

	// setVisible
	// set visible
	virtual void setVisible( bool b )
	{
		m_visible = b;
	}

    // setName
    // set name for button
    inline void setName( const char *name )
    {
        m_widgetName = name;
    }
    
    // lockActionOnChilld
    inline void lockActionOnChilld( bool b )
    {
        m_lockActionOnChild = b;
    }
    
    inline bool isLockActionOnChild()
    {
        return m_lockActionOnChild;
    }
    
    // getName
    // get name of widget
    const char* getName()
    {
        return m_widgetName.c_str();
    }
    
	// getRectByFxObj
	// get rect from flash obj
	static void getRectByFxObj( CMenuFxObj fxObj, core::recti& rect );

	// update
	// update per frame
	virtual void update();

	// onTouchEvent
	// update touch event
	virtual bool onEvent( const SEvent& gameEvent);
    
	// setText
	// set string
	void setText( const char *fxName, const char *string );
	void setText( const char *fxName, const wchar_t *string );
    
    // getRootWidget
    // get root
    CUIWidget* getRootWidget();
    
	// setPosition
	// set pos
	void setPosition(int x, int y);
	
	inline int getHeight()
	{
		return m_rect.getHeight();
	}

	inline int getWidth()
	{
		return m_rect.getWidth();
	}

protected:
    void registerLabel(const SButtonLabel &label);
    void replaceTextOnLabel();

};

#endif