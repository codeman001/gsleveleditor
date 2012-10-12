#include "stdafx.h"
#include "CMenuFx.h"
#include "CMenuFxObj.h"

#include "IView.h"

//////////////////////////////////////////////////
// CMenuFx Implement
//////////////////////////////////////////////////

CMenuFx::CMenuFx()
{
	m_player = new gameswf::player();
	m_root = NULL;
	m_movie = NULL;
	
	m_visible = true;
	m_firstInit = true;
	m_hasAlpha = false;

	m_viewportx = 0;
	m_viewporty = 0;
	m_viewportw = 0;
	m_viewporth = 0;
}

CMenuFx::~CMenuFx()
{	
	m_root = NULL;
	m_movie = NULL;
	delete m_player;
}

bool CMenuFx::loadFlash( const char *url )
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
void CMenuFx::start(int frame)
{
	m_movie->goto_frame(frame);
	m_movie->set_play_state ( gameswf::character::PLAY );
}

void CMenuFx::update()
{
	if ( m_visible && m_root )
	{		
		float timestep = getIView()->getTimeStep()/1000.0f;
		const float maxTimeStep = 1.0f/20.0f;
		if ( timestep > maxTimeStep )
			timestep = maxTimeStep;

		//m_player->set_force_realtime_framerate ( true );
		m_root->advance( timestep );

		// goto frame 0 if first time
		if ( m_firstInit == true )
		{
			start(1);
			m_firstInit = false;			
		}		
	}
}

void CMenuFx::render(int x, int y, int w, int h)
{
	if ( m_visible && m_root )
	{
		m_viewportx = x;
		m_viewporty = y;
		m_viewportw = w;
		m_viewporth = h;

		m_root->set_display_viewport ( x, y, w, h );
		m_root->set_background_alpha ( !m_hasAlpha ? 1.0f : 0.0f );
		m_root->display();
	}
}

// getFxSize
// get size of flash
void CMenuFx::getFxSize(int *w, int *h)
{
    *w = m_root->get_movie_width();
    *h = m_root->get_movie_height();
}

void CMenuFx::getFxScaleRatio(float *fw, float *fh)
{
    if ( getIView()->getDriver()->getOrientation() == video::EOO_0 )
    {
        *fw = (float)m_root->get_movie_width()/(float)m_viewportw;
        *fh = (float)m_root->get_movie_height()/(float)m_viewporth;
    }
    else 
    {
        *fw = (float)m_root->get_movie_width()/(float)m_viewporth;
        *fh = (float)m_root->get_movie_height()/(float)m_viewportw;
    }
}

void CMenuFx::updateMouseState( int x, int y, bool pressed )
{
	if ( m_visible && m_root && m_viewportw != 0 && m_viewporth != 0 )
	{
        float fx, fy;
        getFxScaleRatio(&fx, &fy);

		float mouseX = m_viewportx + x*fx;
		float mouseY = m_viewporty + y*fy;
        
		m_root->notify_mouse_state ( (int)mouseX, (int)mouseY, pressed ? 1 : 0 );
	}
}

// getMenuObj
// get obj on flash menu
CMenuFxObj CMenuFx::getObj( const char *path )
{
	if ( m_movie == NULL )
		return CMenuFxObj();
	
	std::vector<std::string>	folder;

	char stringc[1024] = {0};
	int len = uiString::length<const char>(path);	
	int begin = 0;
	int i = 0;
	
	for ( i = 0; i < len; i++ )
	{
		if ( path[i] == '.' || path[i] == '/' || path[i] == '\\' )
		{
			if ( i > begin  )
			{
				uiString::mid<char, const char>(stringc, path, begin, i);
				folder.push_back( stringc );
			}
			begin = i+1;
		}
	}	
	if ( i > begin )
	{
		uiString::mid<char, const char>(stringc, path, begin, i);
		folder.push_back( stringc );
	}
		
	std::queue<gameswf::character*>	queueObj;
	
	gameswf::display_list* displayList = m_movie.get_ptr()->get_display_list();
	if ( displayList )
	{
		int childCount = displayList->size();
		for ( int j = 0; j < childCount; j++ )
		{
			queueObj.push( displayList->get_character(j) );
		}
	}

	int folderDepth = 0;
	
	while ( queueObj.size() )
	{
		gameswf::character* ch = queueObj.front();
		queueObj.pop();

		const char *name = ch->get_name().c_str();
		
		bool continueSearch = true;
		
		if ( folder[folderDepth] == name )
		{
			folderDepth++;
			continueSearch = true;

			// found the obj
			if ( folderDepth == folder.size() )
			{				
				return CMenuFxObj(this, ch);
			}
		}
		
		if ( continueSearch )
		{
			gameswf::display_list* displayList = ch->get_display_list();
			if ( displayList )
			{
				int childCount = displayList->size();
				for ( int j = 0; j < childCount; j++ )
				{
					queueObj.push( displayList->get_character(j) );
				}
			}
		}
	}


	return CMenuFxObj();
}
	
// findObj
// find obj on flash menu
CMenuFxObj CMenuFx::findObj( char *name )
{
	std::queue<gameswf::character*>	queueObj;

	gameswf::display_list* displayList = m_movie.get_ptr()->get_display_list();
	if ( displayList )
	{
		int childCount = displayList->size();
		for ( int j = 0; j < childCount; j++ )
		{
			queueObj.push( displayList->get_character(j) );
		}
	}
	
	while ( queueObj.size() )
	{
		gameswf::character* ch = queueObj.front();
		queueObj.pop();

		const char *chName = ch->get_name().c_str();
		
		if ( strcmp(name, chName) == 0  )
		{
			return CMenuFxObj(this, ch);
		}
		
		
		gameswf::display_list* displayList = ch->get_display_list();
		if ( displayList )
		{
			int childCount = displayList->size();
			for ( int j = 0; j < childCount; j++ )
			{
				queueObj.push( displayList->get_character(j) );
			}
		}

	}
	return CMenuFxObj();
}