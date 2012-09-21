#ifndef _GAMECONTROL_H_
#define _GAMECONTROL_H_

#include "stdafx.h"
#include "CDpad.h"
#include "swfUI/CMenuFxObj.h"

class CGameControl: 
    public uiSingleton<CGameControl>,
    public IEventReceiver
{
protected:
    bool        m_isEnable;
                    
    std::string m_fxnameDpadTouch;
    
    std::string m_fxnameDpad;
    std::string m_fxnameDpadMove;
    
    int         m_screenTouchID;
	
	CDpad       m_moveDpad;
	core::recti m_touchDpad;

	int			m_keyActionBit;
public:
	CGameControl();
	virtual ~CGameControl();

    // setNameFx
    // set name of flash obj
    void setNameFx( const std::string& nameDpadTouch, const std::string& nameDPad,const std::string& nameDpadMove );
    
    // update
    // main loop update
    virtual void update();    
    
    // OnEvent
    // IEventReceiver implement
    virtual bool OnEvent(const SEvent& event);
    
    // setEnable
    // enable control
    inline void setEnable(bool b)
    {
        m_isEnable = b;
        if ( b == false )
            m_screenTouchID = -1;
    }
    
    // isEnable
    // return enable control
    inline bool isEnable()
    {
        return m_isEnable;
    }

	// isTouchOnScreen
	// check a null touch on screen
	bool isTouchOnScreen( int touchID );
    
    // isTouchOnDPad
    // check touch on dpad
    bool isTouchOnDPad( int x, int y );

protected:

	// handleTouchEvent
	// when player touch on screen
	bool handleTouchEvent(const SEvent& event);

	// handleKeyEvent
	// when player press a key
	bool handleKeyEvent(const SEvent& event);
};

#endif