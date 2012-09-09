#include "stdafx.h"
#include "CGameControl.h"

CGameControl::CGameControl()
{
    m_isEnable = false;
}

CGameControl::~CGameControl()
{

}

// update
// main loop update
void CGameControl::update()
{
    
}

// OnEvent
// IEventReceiver implement
bool CGameControl::OnEvent(const SEvent& event)
{
    return true;
}