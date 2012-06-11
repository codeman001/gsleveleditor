#ifndef _CFLASHHANDER_
#define _CFLASHHANDER_

#include "CGameUI.h"

class CFlashHander
{
protected:
	gameswf::player						*m_player;	
	gameswf::gc_ptr<gameswf::root>		m_root;
	gameswf::gc_ptr<gameswf::character>	m_movie;

	bool								m_visible;
	bool								m_firstInit;

	int		m_viewportx;
	int		m_viewporty;
	int		m_viewportw;
	int		m_viewporth;
public:
	CFlashHander();
	virtual ~CFlashHander();

	// loadFlash
	// load swf file
	bool loadFlash( const char *url );

	// update
	// update timestep for flash
	void update(float timestep);

	// render
	// render flash at viewport x,y,w,h
	void render(int x, int y, int w, int h, bool hasBackground);
	
	// updateMouseState
	// update mouse event to flash
	void updateMouseState( int x, int y, bool pressed );

	// setVisible
	// show/hide flash player
	void setVisible( bool b )
	{
		m_visible = b;
	}	

	// start
	// start play
	void start(int frame = 0);
	
};

#endif