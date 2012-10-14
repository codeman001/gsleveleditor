#include "stdafx.h"
#include "IView.h"
#include "CGameDebug.h"
#include "CGameDebugSceneNode.h"

CGameDebug::CGameDebug()
{
	m_nLine = 0;
	m_nBox = 0;
    m_n2DRect = 0;

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

void CGameDebug::add2DRect( core::recti rect, SColor color )
{
    if ( m_n2DRect >= MAX_DEBUGGEOMETRY )
		return;
    
    m_2dRect[m_n2DRect].rect	= rect;
	m_2dRect[m_n2DRect].color	= color;
	m_n2DRect++;
}

void CGameDebug::draw2D()
{
    IVideoDriver *driver = getIView()->getDriver();
    
    for ( int i = 0; i < m_n2DRect; i++ )
    {
        driver->draw2DRectangleOutline( m_2dRect[i].rect, m_2dRect[i].color );
    }
}

