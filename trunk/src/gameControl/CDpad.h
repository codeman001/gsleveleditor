#ifndef _CDpad_H_
#define _CDpad_H_

#include "stdafx.h"

#include "swfUI/CMenuFxObj.h"
#include "swfUI/CMenuFx.h"
#include "swfUI/CGameUI.h"

class CDpad
{
protected:
    bool    m_visible;
    
    core::position2di   m_dpadPos;
    core::position2di   m_touchPos;
    core::position2di   m_renderTouchPos;
    
    CMenuFxObj*         m_fxDpad;
    CMenuFxObj*         m_fxDpadMove;
    
    int                 m_controlID;
public:
    CDpad();
    virtual ~CDpad();
    
    // update
    // main loop update
    virtual void update();
    
    // setVisible
    // show joystick on screen
    void setVisible( bool b );
    
    // setDpadPos
    // set position of joystick
    void setDpadPos( int x, int y );
    
    // setTouchPos
    // set position of touch
    void setTouchPos( int x, int y );
    
    // setDpadFxName
    void setDpadFxName( const std::string& name, const std::string& move );
    
    // setControlID
    // set touch control id
    inline void setControlID(int id)
    {
        m_controlID = id;
    }
    
    // getControlID
    // get touch id
    inline int getControlID()
    {
        return m_controlID;
    }
    
    
};

#endif
