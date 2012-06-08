#include "stdafx.h"
#include "CFlashHandler.h"

CFlashHander::CFlashHander()
{
	m_player = new gameswf::player();
	m_root = NULL;
	m_movie = NULL;
	m_visible = true;
}

CFlashHander::~CFlashHander()
{	
	m_root = NULL;
	m_movie = NULL;
	delete m_player;
}


bool CFlashHander::loadFlash( const char *url )
{	
	m_root = m_player->load_file ( url );

	if ( m_root != NULL )
	{
		m_movie = m_root->get_root_movie();
		m_root->set_userdata ( this );
		return true;
	}

	return false;
}

void CFlashHander::update(float timestep)
{
	if ( m_visible && m_root )
	{
		m_player->set_force_realtime_framerate ( true );
		m_root->advance( timestep );
	}
}

void CFlashHander::render(int x, int y, int w, int h, bool hasBackground)
{
	if ( m_visible && m_root )
	{
		m_root->set_display_viewport ( x, y, w, h );
		m_root->set_background_alpha ( hasBackground ? 1.0f : 0.0f );
		m_root->display();
	}
}