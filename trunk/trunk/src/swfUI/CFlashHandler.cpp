#include "stdafx.h"
#include "CFlashHandler.h"

CFlashHander::CFlashHander()
{
	m_player = new gameswf::player();
	m_root = NULL;
	m_movie = NULL;
	
	m_visible = true;
	m_firstInit = true;

	m_viewportx = 0;
	m_viewporty = 0;
	m_viewportw = 0;
	m_viewporth = 0;
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

		// play movie
		m_root->set_userdata ( this );
	
		m_viewportx = 0;
		m_viewporty = 0;
		m_viewportw = m_root->get_movie_width();
		m_viewporth = m_root->get_movie_height();
		
		return true;
	}

	return false;
}

// start
// start play
void CFlashHander::start(int frame)
{
	m_movie->goto_frame(frame);
	m_movie->set_play_state ( gameswf::character::PLAY );
}

void CFlashHander::update(float timestep)
{
	if ( m_visible && m_root )
	{
		m_player->set_force_realtime_framerate ( true );
		m_root->advance( timestep );

		// goto frame 0 if first time
		if ( m_firstInit == true )
		{
			start(1);
			m_firstInit = false;			
		}		
	}
}

void CFlashHander::render(int x, int y, int w, int h, bool hasBackground)
{
	if ( m_visible && m_root )
	{
		m_viewportx = x;
		m_viewporty = y;
		m_viewportw = w;
		m_viewporth = h;

		m_root->set_display_viewport ( x, y, w, h );
		m_root->set_background_alpha ( hasBackground ? 1.0f : 0.0f );
		m_root->display();
	}
}

void CFlashHander::updateMouseState( int x, int y, bool pressed )
{
	if ( m_visible && m_root && m_viewportw != 0 && m_viewporth != 0 )
	{
		float fx = m_root->get_movie_width()/(float)m_viewportw;
		float fy = m_root->get_movie_height()/(float)m_viewporth;

		float mouseX = m_viewportx + x*fx;
		float mouseY = m_viewporty + y*fy;

		m_root->notify_mouse_state ( (int)mouseX, (int)mouseY, pressed ? 1 : 0 );
	}
}