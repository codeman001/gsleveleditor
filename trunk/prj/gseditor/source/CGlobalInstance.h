#ifndef _GLOBAL_INSTANCE_H
#define _GLOBAL_INSTANCE_H

#include "RibbonCommandHandler.h"

class CGlobalInstance: public uiSingleton<CGlobalInstance>
{
public:
	CGlobalInstance();
	virtual ~CGlobalInstance();


	CRibbonCommandHandler	*m_ribbonCommand;
	IUIFramework			*m_ribbonFramework;
	uiWindow				*m_mainFrame;

public:
	
public:
	
};

#endif