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
    
    std::string m_fxnameDpadTouch;
    
    std::string m_fxnameDpad;
    std::string m_fxnameDpadMove;
public:
	CGameControl();
	virtual ~CGameControl();

    // setNameFx
    // set name of flash obj
    inline void setNameFx( const std::string& nameDpadTouch, const std::string& nameDPad,const std::string& nameDpadMove )
    {
        m_fxnameDpadTouch = nameDpadTouch;
        m_fxnameDpad = nameDPad;
        m_fxnameDpadMove = nameDpadMove;
        
        // set fx for dpad
        m_moveDpad.setDpadFxName(nameDPad, nameDpadMove);
    }
    
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
};

#endif