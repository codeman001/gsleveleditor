#ifndef _STATE_MAINMENU_
#define _STATE_MAINMENU_

#include "CGameState.h"

class CStateMainMenu: public CGameState
{
protected:
	int				m_menuChoice;

	CGameLevel		*m_level;

	CMenuFxObj		m_txtServerName;
	CMenuFxObj		m_btnJointButton;
public:
	CStateMainMenu();
	virtual ~CStateMainMenu();

	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );	
	
	virtual void onRender();
	virtual void onUpdate();

	// getLevel
	// get 3d level
	CGameLevel* getLevel()
	{
		return m_level;
	}
};

#endif