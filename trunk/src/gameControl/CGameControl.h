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
    
    CDpad       m_moveDpad;
    
    core::recti m_touchDpad;
    
    std::string m_fxnameDpadTouch;
    
    std::string m_fxnameDpad;
    std::string m_fxnameDpadMove;
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
    
};

#endif