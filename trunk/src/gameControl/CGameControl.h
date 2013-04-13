#ifndef _GAMECONTROL_H_
#define _GAMECONTROL_H_

#include "stdafx.h"
#include "swfUI/CMenuFxObj.h"

class CGameControl: 
    public uiSingleton<CGameControl>,
    public IEventReceiver
{
protected:
    bool        m_isEnable;                           
    int         m_screenTouchID;
		
	int			m_keyActionBit;

	int			m_touchIDStatus[ MAX_MULTITOUCH ];

protected:
    float       m_runStrength;
    float       m_runRot;
    bool        m_runState;
    bool        m_runFastState;
    bool        m_shootState;
    bool        m_reloadState;
public:
	CGameControl();
	virtual ~CGameControl();
    
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

		resetTouchIDStatus();
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
    

	// sendPlayerStopEvent
	// send event to player stop run
	void sendPlayerStopEvent();
	void sendPlayerRunEvent(float f, float rotate);
    void sendPlayerCommand(bool shoot, bool reload);
    
	// setTouchIDStatus
	// set touch
	inline void setTouchIDStatus(int id, int status)
	{
		if ( id >= 0 && id < MAX_MULTITOUCH )
			m_touchIDStatus[id] = status;
	}

	// getTouchIDStatus
	inline int getTouchIDStatus(int id)
	{
		if ( id >= 0 && id < MAX_MULTITOUCH )
			return m_touchIDStatus[id];
		else
			return 1;
	}

	void resetTouchIDStatus();

    // action
    void setRunParameter(float f, float runRot);
    void setRunMode(bool run);
    void setRunFastMode(bool runFast);
    void setShootMode(bool shoot, bool reload);
    
protected:

	// handleETouchEvent
	// when player touch on screen
	bool handleETouchEvent(const SEvent& event);

	// handleKeyEvent
	// when player press a key
	bool handleKeyEvent(const SEvent& event);
};

#endif