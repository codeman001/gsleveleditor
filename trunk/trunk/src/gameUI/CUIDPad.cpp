#include "stdafx.h"
#include "CUIDPad.h"
#include "gameControl/CGameControl.h"

CUIDPad::CUIDPad( const char *name, CUIWidget* parent, CMenuFxObj flashObj )
	:CUIWidget(name, parent, flashObj)
{
	m_mouseOver = false;
	
	m_dpadBase = flashObj.findObj("mcDPadBase");
	m_dpadMove = flashObj.findObj("mcDPadMove");	

	m_dpadBase.setVisible( false );
	m_dpadMove.setVisible( false );
    
    m_isEnable = false;
}

CUIDPad::~CUIDPad()
{
}

// update
// update per frame
void CUIDPad::update()
{
	CUIWidget::update();
}

// onTouchEvent
// update touch event
bool CUIDPad::onEvent( const SEvent& gameEvent)
{
	if ( (m_controlID != -1 && m_controlID != gameEvent.EventControlID) || m_isEnable == false )
		return false;

	if ( gameEvent.EventType == EET_MOUSE_INPUT_EVENT )
	{
		// check mouse hit this button
		core::position2di mousePos(gameEvent.MouseInput.X,gameEvent.MouseInput.Y);

        // check mouse over on button
		m_mouseOver = m_rect.isPointInside(mousePos);
 
		float fw = 1.0f, fh = 1.0f;
		CMenuFx *menuFx = m_flashObj.getMenu();
		menuFx->getFxScaleRatio( &fw, &fh );
		
		// delta drag
		const float k_maxMove = 100.0f*fw;

		if (gameEvent.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN && m_mouseOver == true )				
		{			
			// press
			m_controlID = gameEvent.EventControlID;

			// set flag
			CGameControl::getInstance()->setTouchIDStatus(m_controlID, 1);

			// set position
			m_dpadBasePos = mousePos;
			m_dpadMovePos = mousePos;

			// show flash obj
			m_dpadBase.setVisible( true );
			m_dpadMove.setVisible( true );

			// position ref to flash
			core::position2df flashMousePos(mousePos.X/fw, mousePos.Y/fh);

			m_dpadBase.setAbsolutePosition( (int)flashMousePos.X, (int)flashMousePos.Y );
			m_dpadMove.setAbsolutePosition( (int)flashMousePos.X, (int)flashMousePos.Y );
		}
		else if ( gameEvent.MouseInput.Event == EMIE_MOUSE_MOVED && getControlID() != -1 )
		{
			// move
			core::position2di delta = mousePos - m_dpadBasePos;
			core::position2df moveVec((float)delta.X, (float)delta.Y);

			float length = moveVec.getLength();
			if ( length > k_maxMove )
				length = k_maxMove;

			// calc run angle
			moveVec.normalize();
			
			// run event
			float f = length/k_maxMove;
			if ( f >= 0.3f )
				CGameControl::getInstance()->sendPlayerRunEvent( f, (float)moveVec.getAngle() - 90.0f, false );
			else
				CGameControl::getInstance()->sendPlayerStopEvent();

			// recalc touch pos to render
			moveVec *= length;
			m_dpadMovePos = m_dpadBasePos + core::position2di((int)moveVec.X, (int)moveVec.Y);

			// set position
			core::position2df flashMousePos(m_dpadMovePos.X/fw, m_dpadMovePos.Y/fh);
			m_dpadMove.setAbsolutePosition( (int)flashMousePos.X, (int)flashMousePos.Y );
		}
		else if ( gameEvent.MouseInput.Event == EMIE_LMOUSE_LEFT_UP )
		{						
			// set flag
			if ( m_controlID != -1 )
				CGameControl::getInstance()->setTouchIDStatus(m_controlID, 0);
			
			// release
			m_controlID = -1;

			// hide flash obj
			m_dpadBase.setVisible( false );
			m_dpadMove.setVisible( false );

			// stop
			CGameControl::getInstance()->sendPlayerStopEvent();
		}

	}
	return false;
}