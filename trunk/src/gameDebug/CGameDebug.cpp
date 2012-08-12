#include "stdafx.h"
#include "IView.h"
#include "CGameDebug.h"
#include "SceneNode/CGameDebugSceneNode.h"

CGameDebug::CGameDebug()
{
	m_nLine = 0;
	m_nBox = 0;

	ISceneManager* smgr = getIView()->getSceneMgr();
	CGameDebugSceneNode* debugNode = new CGameDebugSceneNode( smgr->getRootSceneNode(), smgr, -1 );
	debugNode->drop();
}

CGameDebug::~CGameDebug()
{
}

void CGameDebug::addDrawLine( core::line3df line, SColor color )
{
	if ( m_nLine >= MAX_DEBUGGEOMETRY )
		return;

	m_lines[m_nLine].line	= line;
	m_lines[m_nLine].color	= color;
	m_nLine++;
}

void CGameDebug::addBoudingBox( core::aabbox3df box, SColor color )
{
	if ( m_nBox >= MAX_DEBUGGEOMETRY )
		return;

	m_boxs[m_nBox].box		= box;
	m_boxs[m_nBox].color	= color;
	m_nBox++;
}