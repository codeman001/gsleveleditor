//
//  CStateIGM.h
//  gameplay
//
//  Created by Pham Hong Duc on 2/23/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef _STATE_IGM_
#define _STATE_IGM_

#include "CGameState.h"

class CStateIGM: public CGameState
{
protected:
    CUIButton *m_resumeButton;
    CUIButton *m_mainMenuButton;
    
    int m_menuChoice;
    
public:
	CStateIGM();
	virtual ~CStateIGM();
	
	virtual void onCreate();
	virtual void onDestroy();
	virtual void onFsCommand( const char *command, const char *param );
	virtual void onRender();
	virtual void onUpdate();
    
    virtual void onEvent(const SEvent& event);
};

#endif
