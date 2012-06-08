#ifndef _GAMEUI_
#define _GAMEUI_

#include "stdafx.h"

#include "base/tu_file.h"
#include "base/utility.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_filters.h"

class CFlashHander;

class CGameUI: public uiSingleton<CGameUI>
{
protected:
	gameswf::render_handler		*m_render;

	map<std::string, CFlashHander*>	m_flash;
public:
	CGameUI();
	virtual ~CGameUI();	

	// openFlash
	// load swf file
	CFlashHander* openFlash( const char *name, const char *url );

	// releaseAllFlash
	// unload all swf
	void releaseAllFlash();
	
	// update
	// update game flash ui
	void update(float timestep);

	// render
	// render game flash ui
	void render(int x, int y, int w, int h, bool hasBackground);

public:
	static tu_file* _fileOpen( const char *lpPath );
	static void _fsCallback( gameswf::character *movie, const char *lpCommand, const char *lpParams );
};

#endif