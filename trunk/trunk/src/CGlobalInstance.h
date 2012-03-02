#ifndef _GLOBAL_INSTANCE_H
#define _GLOBAL_INSTANCE_H

#include "RibbonCommandHandler.h"

class CGlobalInstance
{
protected:
	static CGlobalInstance *s_instance;
protected:
	CGlobalInstance();
	virtual ~CGlobalInstance();

public:
	CRibbonCommandHandler	*m_ribbonCommand;
	IUIFramework			*m_ribbonFramework;
	uiWindow				*m_mainFrame;

public:
	
public:
	static CGlobalInstance *getInstance();
	static void destroyInstance();
};

#endif