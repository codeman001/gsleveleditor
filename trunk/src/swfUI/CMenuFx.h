#ifndef _CMENUFX_H_
#define _CMENUFX_H_

#include "CGameUI.h"

class CMenuFxObj;

class CMenuFx
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

	bool	m_hasAlpha;
public:
	CMenuFx();
	virtual ~CMenuFx();

	// loadFlash
	// load swf file
	bool loadFlash( const char *url );

	// update
	// update timestep for flash
	void update();

	// render
	// render flash at viewport x,y,w,h
	void render(int x, int y, int w, int h);
	
    // getFxSize
    // get size of flash
    void getFxSize(int *w, int *h);
    void getFxScaleRatio(float *fw, float *fh);
    
	// updateMouseState
	// update mouse event to flash
	void updateMouseState( int x, int y, bool pressed );

	// setVisible
	// show/hide flash player
	void setVisible( bool b )
	{
		m_visible = b;
	}	

	void setBackgroundTransparent( bool b )
	{
		m_hasAlpha = b;
	}

	// start
	// start play
	void start(int frame = 1);

	// getMenuObj
	// get obj on flash menu
	CMenuFxObj* getObj( const char *path );
	
	// findObj
	// find obj on flash menu
	CMenuFxObj* findObj( char *name );

};

#endif