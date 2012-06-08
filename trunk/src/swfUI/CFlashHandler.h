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
public:
	CFlashHander();
	virtual ~CFlashHander();

	bool loadFlash( const char *url );

	void update(float timestep);
	void render(int x, int y, int w, int h, bool hasBackground);

	void setVisible( bool b )
	{
		m_visible = b;
	}
};

#endif