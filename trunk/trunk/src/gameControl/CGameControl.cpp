#include "stdafx.h"
#include "CGameControl.h"
#include "IView.h"
#include "gameEvent.h"

#define SHIFTBIT(x)	(1<<x)

CGameControl::CGameControl()
{
    m_isEnable = false;
    
    m_screenTouchID = -1;
	m_keyActionBit = 0;	
    
    
    m_runStrength = 0.0f;
    m_runRot = 0.0f;
    
    m_runState = false;    
    m_shootState = false;
    m_reloadState = false;
}

CGameControl::~CGameControl()
{

}

// update
// main loop update
void CGameControl::update()
{
    if ( m_isEnable == false )
        return;
    
#ifdef IOS
    if ( m_runState )
        sendPlayerRunEvent(m_runStrength, m_runRot);
    else 
        sendPlayerStopEvent();
    
    if ( m_reloadState )
        sendPlayerCommand(false, true);
    else 
    {        
        if ( m_shootState )
            sendPlayerCommand(true, false);
        else
            sendPlayerCommand(false, false);
    }
#endif
}

void CGameControl::setRunParameter(float f, float runRot)
{
    m_runStrength = f;
    m_runRot = runRot;
}

void CGameControl::setRunMode(bool run)
{
    m_runState = run;
}

void CGameControl::setShootMode(bool shoot, bool reload)
{
    m_shootState = shoot;
    m_reloadState = reload;
}


// OnEvent
// IEventReceiver implement
bool CGameControl::OnEvent(const SEvent& event)
{
    if ( m_isEnable == false )
        return true;
            
    if ( event.EventType == EET_MOUSE_INPUT_EVENT )
    {
		handleETouchEvent( event );
    }	
	else if ( event.EventType == EET_KEY_INPUT_EVENT )
	{		
		handleKeyEvent( event );
	}	

    return true;
}



// handleETouchEvent
// when player touch on screen
bool CGameControl::handleETouchEvent(const SEvent& event)
{
#if defined (IOS) || defined (ANDROID)
	IrrlichtDevice *device = getIView()->getDevice();
#endif

    s32 controlID = event.EventControlID;
    core::vector2di mousePos(event.MouseInput.X, event.MouseInput.Y);
    
    if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
    {
		if ( m_screenTouchID == -1 && m_touchIDStatus[controlID] == 0 )
        {
#if defined (IOS) || defined (ANDROID)
            device->getCursorControl()->setPosition( event.MouseInput.X, event.MouseInput.Y);
#endif
			m_screenTouchID = controlID;

			// set flag
			m_touchIDStatus[controlID] = 1;

			ISceneManager *smgr = getIView()->getSceneMgr();
            if ( smgr->getActiveCamera() )
                smgr->getActiveCamera()->OnEvent( event );
        }
    }
    else if ( event.MouseInput.Event == EMIE_MOUSE_MOVED )
    {            
        if ( m_screenTouchID == controlID )
        {
#if defined (IOS) || defined (ANDROID)
            device->getCursorControl()->setPosition( event.MouseInput.X, event.MouseInput.Y);
#endif
			ISceneManager *smgr = getIView()->getSceneMgr();
            if ( smgr->getActiveCamera() )
                smgr->getActiveCamera()->OnEvent( event );
        }
    }
    else if ( event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
    {
        // release dpad move
		if ( m_screenTouchID == controlID )
        {
#if defined (IOS) || defined (ANDROID)
            device->getCursorControl()->setPosition( event.MouseInput.X, event.MouseInput.Y);
#endif                
            ISceneManager *smgr = getIView()->getSceneMgr();
            if ( smgr->getActiveCamera() )
                smgr->getActiveCamera()->OnEvent( event );

            m_screenTouchID = -1;

			// set flag
			m_touchIDStatus[controlID] = 0;
        }
    }

	return true;
}


// handleKeyEvent
// when player press a key
bool CGameControl::handleKeyEvent(const SEvent& event)
{
	// key input
	EKEY_CODE key = event.KeyInput.Key;		

	const int KeyUp		= SHIFTBIT(1);
	const int KeyLeft	= SHIFTBIT(2);
	const int KeyRight	= SHIFTBIT(3);
	const int KeyBack	= SHIFTBIT(4);
	const int KeyFire	= SHIFTBIT(5);    
    
	if ( event.KeyInput.PressedDown )
	{
		if (  key == irr::KEY_UP || key == irr::KEY_KEY_W )
			m_keyActionBit |= KeyUp;
		else if (  key == irr::KEY_DOWN || key == irr::KEY_KEY_S )
			m_keyActionBit |= KeyBack;
		else if (  key == irr::KEY_LEFT || key == irr::KEY_KEY_A )
			m_keyActionBit |= KeyLeft;
		else if (  key == irr::KEY_RIGHT || key == irr::KEY_KEY_D )
			m_keyActionBit |= KeyRight;
		else if (  key == irr::KEY_SPACE )
			m_keyActionBit |= KeyFire;        
	}		
	else
	{     
		if (  key == irr::KEY_UP || key == irr::KEY_KEY_W )
			m_keyActionBit &= ~KeyUp;
		else if (  key == irr::KEY_DOWN || key == irr::KEY_KEY_S )
			m_keyActionBit &= ~KeyBack;
		else if (  key == irr::KEY_LEFT || key == irr::KEY_KEY_A )
			m_keyActionBit &= ~KeyLeft;
		else if (  key == irr::KEY_RIGHT || key == irr::KEY_KEY_D )
			m_keyActionBit &= ~KeyRight;
		else if (  key == irr::KEY_SPACE )
			m_keyActionBit &= ~KeyFire;        
	}

	// update move
	bool runUp		= false;
	bool runBack	= false;
	bool runLeft	= false;
	bool runRight	= false;    
    bool shoot		= false;
    
	// calc rotation
	if ( (m_keyActionBit & KeyLeft) != 0 )
	{
		if ( (m_keyActionBit & KeyBack) != 0 )
			runBack = true;
		else if ( (m_keyActionBit & KeyUp) != 0 )
			runUp = true;
		
		runLeft = true;
	}
	else if ( (m_keyActionBit & KeyRight) != 0 )
	{
		if ( (m_keyActionBit & KeyBack) != 0 )
			runBack = true;
		else if ( (m_keyActionBit & KeyUp) != 0 )
			runUp = true;
		runRight = true;
	}
	else if ( (m_keyActionBit & KeyBack) != 0 )
	{
		runBack = true;
	}
	else if ( (m_keyActionBit & KeyUp) != 0 )
		runUp = true;    
    
    if ( (m_keyActionBit & KeyFire) != 0 )
        shoot = true;
    
	// calc player rotation
	float rot = 0.0f;
	if ( runLeft )
	{
		if ( runUp )
			rot = 45.0f;
		else if ( runBack )
			rot = 135.0f;
		else
			rot = 90.0f;
	}
	else if ( runRight )
	{
		if ( runUp )
			rot = -45.0f;
		else if ( runBack )
			rot = -135.0f;
		else
			rot = -90.0f;
	}
	else if ( runBack )
	{
		rot = -180.0f;
	}

	bool runCommand = runUp || runBack || runLeft || runRight;	
    
	// send event to player component
	if ( runCommand )		
		sendPlayerRunEvent( 1.0f, rot );		
	else
		sendPlayerStopEvent();

    // send event to player component
    if ( shoot )
        sendPlayerCommand(true, false);
    else
        sendPlayerCommand(false, false);
    
	return true;
}

void CGameControl::resetTouchIDStatus()
{
	for ( int i = 0; i < MAX_MULTITOUCH; i++ )
		m_touchIDStatus[i] = 0;
}

// isTouchOnScreen
// check a null touch on screen
bool CGameControl::isTouchOnScreen( int touchID )
{
	if ( touchID == m_screenTouchID )
        return true;
    
	return false;
}

void CGameControl::sendPlayerStopEvent()
{
    if ( m_isEnable == false )
        return;

	static SEvent playerStop;
	static SEventPlayerMove stopEvent;
	stopEvent.rotate = 0.0f;
	stopEvent.strength = 0.0f;
	stopEvent.run = false;    
    
	playerStop.EventType = EET_GAME_EVENT;
	playerStop.GameEvent.EventID = (s32)EvtPlayerMove;
	playerStop.GameEvent.EventData = &stopEvent;
	getIView()->getDevice()->postEventFromUser( playerStop );
}

void CGameControl::sendPlayerRunEvent(float f, float rotate)
{
    if ( m_isEnable == false )
        return;

	static SEvent playerMove;
	static SEventPlayerMove moveEvent;
	moveEvent.rotate = rotate;
	moveEvent.strength = f;
	moveEvent.run = true;    
    
	playerMove.EventType = EET_GAME_EVENT;
	playerMove.GameEvent.EventID = (s32)EvtPlayerMove;
	playerMove.GameEvent.EventData = &moveEvent;
	getIView()->getDevice()->postEventFromUser( playerMove );
}

void CGameControl::sendPlayerCommand(bool shoot, bool reload)
{
    if ( m_isEnable == false )
        return;
    
	static SEvent playerCommand;
	static SEventPlayerCommand command;
	command.shoot = shoot;
    command.reload = reload;
    
	playerCommand.EventType = EET_GAME_EVENT;
	playerCommand.GameEvent.EventID = (s32)EvtPlayerCommand;
	playerCommand.GameEvent.EventData = &command;
	getIView()->getDevice()->postEventFromUser( playerCommand );
}