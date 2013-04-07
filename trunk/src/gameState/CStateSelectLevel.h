#ifndef _CSTATE_SELECTLEVEL_
#define _CSTATE_SELECTLEVEL_

#include "CGameState.h"

class CStateSelectLevel: public CGameState
{
protected:
	CUIButton	*m_btnMainMenu;
	CUIButton	*m_btnCreateGame;
	CUIListview	*m_lstSelectLevel;

public:
	CStateSelectLevel();
	virtual ~CStateSelectLevel();
	
	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onRender();
	virtual void onUpdate();

	virtual void onEvent(const SEvent& event);
};

#endif