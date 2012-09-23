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
}

CGameControl::~CGameControl()
{

}

// setNameFx
// set name of flash obj
void CGameControl::setNameFx( const std::string& nameDpadTouch, const std::string& nameDPad,const std::string& nameDpadMove )
{
    m_fxnameDpadTouch = nameDpadTouch;
    m_fxnameDpad = nameDPad;
    m_fxnameDpadMove = nameDpadMove;
    
    // flash gamehud
    CMenuFx *menuFx = CGameUI::getInstance()->getFlash("uiGameHud");
    
    // get touch touch dpad
    CMenuFxObj *dpadTouch = menuFx->findObj((char*) nameDpadTouch.c_str());
    if ( dpadTouch )
    {
        int x,y,w,h;
        float fw, fh;
        
        dpadTouch->getBound(&x, &y, &w, &h);        
        menuFx->getFxScaleRatio(&fw, &fh);
        
        float realX, realY, realW, realH;
        realX = (float)x / fw;
        realY = (float)y / fh;
        realW = (float)w / fw;
        realH = (float)h / fh;
        
		char string[512] = {0};
		sprintf(string,"dpadRect:{%f, %f, %f, %f}: %f, %f", realX, realY, realW, realH, fw, fh);
		os::Printer::log(string);

        m_touchDpad.UpperLeftCorner = core::vector2di( (int)realX, (int)realY);
        m_touchDpad.LowerRightCorner = core::vector2di( (int)(realX+realW), (int)(realY+realH) );
        
        dpadTouch->drop();
    }
    
    // set fx for dpad
    m_moveDpad.setDpadFxName(nameDPad, nameDpadMove);
}

// update
// main loop update
void CGameControl::update()
{
    if ( m_isEnable == false )
        return;

    m_moveDpad.update();
}


// OnEvent
// IEventReceiver implement
bool CGameControl::OnEvent(const SEvent& event)
{
    if ( m_isEnable == false )
        return true;
            
    if ( event.EventType == EET_MOUSE_INPUT_EVENT )
    {
		handleTouchEvent( event );
    }	
	else if ( event.EventType == EET_KEY_INPUT_EVENT )
	{		
		handleKeyEvent( event );
	}	

    return true;
}



// handleTouchEvent
// when player touch on screen
bool CGameControl::handleTouchEvent(const SEvent& event)
{
#if defined (IOS) || defined (ANDROID)
	IrrlichtDevice *device = getIView()->getDevice();
#endif

    s32 controlID = event.EventControlID;
    core::vector2di mousePos(event.MouseInput.X, event.MouseInput.Y);
    
    if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN )
    {
        // touch on dpad
        if ( m_touchDpad.isPointInside( mousePos ) == true )
        {
            if ( m_moveDpad.getControlID() == -1 )
            {
                m_moveDpad.setControlID( controlID );
                m_moveDpad.setDpadPos( event.MouseInput.X, event.MouseInput.Y );                    
            }
        }  
        else if ( m_screenTouchID == -1 )
        {
#if defined (IOS) || defined (ANDROID)
            device->getCursorControl()->setPosition( event.MouseInput.X, event.MouseInput.Y);
#endif
			m_screenTouchID = controlID;

			ISceneManager *smgr = getIView()->getSceneMgr();
            if ( smgr->getActiveCamera() )
                smgr->getActiveCamera()->OnEvent( event );                
        }
    }
    else if ( event.MouseInput.Event == EMIE_MOUSE_MOVED )
    {            
        // move dpad
        if ( m_moveDpad.getControlID() == controlID )
            m_moveDpad.setTouchPos( event.MouseInput.X, event.MouseInput.Y );
        else if ( m_screenTouchID == controlID )
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
        if ( m_moveDpad.getControlID() == controlID )
        {
            m_moveDpad.setControlID( -1 );
        }
        else if ( m_screenTouchID == controlID )
        {

#if defined (IOS) || defined (ANDROID)
            device->getCursorControl()->setPosition( event.MouseInput.X, event.MouseInput.Y);
#endif                
            ISceneManager *smgr = getIView()->getSceneMgr();
            if ( smgr->getActiveCamera() )
                smgr->getActiveCamera()->OnEvent( event );

            m_screenTouchID = -1;			
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

	const int KeyNone	= 0;
	const int KeyUp		= SHIFTBIT(1);
	const int KeyLeft	= SHIFTBIT(2);
	const int KeyRight	= SHIFTBIT(3);
	const int KeyBack	= SHIFTBIT(4);
	const int KeyFire	= SHIFTBIT(5);

	// run flag
	bool updateRunState = false;

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

	// calc player rotation
	float rot = 0.0f;
	if ( runLeft )
	{
		if ( runUp )
			rot = 43.0f;
		else if ( runBack )
			rot = 133.0f;
		else
			rot = 88.0f;
	}
	else if ( runRight )
	{
		if ( runUp )
			rot = -43.0f;
		else if ( runBack )
			rot = -133.0f;
		else
			rot = -88.0f;
	}
	else if ( runBack )
	{
		rot = -178.0f;
	}

	bool runCommand = runUp || runBack || runLeft || runRight;
	
	// send event to player component
	if ( runCommand )		
		sendPlayerRunEvent( 1.0f, rot );		
	else		
		sendPlayerStopEvent();

	return true;
}


// isTouchOnScreen
// check a null touch on screen
bool CGameControl::isTouchOnScreen( int touchID )
{
	if ( touchID == m_screenTouchID )
        return true;
    
	return false;
}

// isTouchOnDPad
// check touch on dpad
bool CGameControl::isTouchOnDPad( int x, int y )
{
    core::vector2di mousePos(x, y);
    return m_touchDpad.isPointInside( mousePos );
}

void CGameControl::sendPlayerStopEvent()
{
    if ( m_isEnable == false )
        return;

	SEvent	playerStop;
	SEventPlayerMove stopEvent;
	stopEvent.rotate = 0.0f;
	stopEvent.strength = 0.0f;
	stopEvent.run = false;

	playerStop.EventType = EET_USER_EVENT;
	playerStop.UserEvent.UserData1 = (s32)EvtPlayerMove;
	playerStop.UserEvent.UserData2 = (s32)((unsigned long)&stopEvent);
	getIView()->getDevice()->postEventFromUser( playerStop );
}

void CGameControl::sendPlayerRunEvent(float f, float rotate)
{
    if ( m_isEnable == false )
        return;

	SEvent	playerMove;
	SEventPlayerMove moveEvent;
	moveEvent.rotate = rotate;
	moveEvent.strength = f;
	moveEvent.run = true;

	playerMove.EventType = EET_USER_EVENT;
	playerMove.UserEvent.UserData1 = (s32)EvtPlayerMove;
	playerMove.UserEvent.UserData2 = (s32)((unsigned long)&moveEvent);
	getIView()->getDevice()->postEventFromUser( playerMove );
}