#include "stdafx.h"
#include "CGlobalInstance.h"

CGlobalInstance *CGlobalInstance::s_instance = NULL;

CGlobalInstance::CGlobalInstance()
{
	m_ribbonCommand		= NULL;
	m_ribbonFramework	= NULL;	
}

CGlobalInstance::~CGlobalInstance()
{
}

CGlobalInstance* CGlobalInstance::getInstance()
{
	if ( s_instance == NULL )
		s_instance = new CGlobalInstance();

	return s_instance;
}

void CGlobalInstance::destroyInstance()
{
	if ( s_instance != NULL )
		delete s_instance;
}