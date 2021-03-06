#ifndef _STATE_MAINMENU_
#define _STATE_MAINMENU_

#include "CGameState.h"

class CStateMainMenu: public CGameState
{
protected:
	int				m_menuChoice;
	CGameLevel		*m_level;

    CUIButton       *m_btnCreateGame;
    CUIButton       *m_btnJoinGame;
	CUIButton       *m_btnOption;
	CUIButton       *m_btnAbout;

public:
	CStateMainMenu();
	virtual ~CStateMainMenu();

	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );	
	
	virtual void onRender();
	virtual void onUpdate();

    virtual void onEvent(const SEvent& event);
    
	// getLevel
	// get 3d level
	CGameLevel* getLevel()
	{
		return m_level;
	}
};

#endif