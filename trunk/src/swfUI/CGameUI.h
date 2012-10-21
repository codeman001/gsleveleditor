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

class CMenuFx;

class CGameUI: 
	public uiSingleton<CGameUI>,
	public IEventReceiver
{
protected:
	gameswf::render_handler				*m_render;
	std::map<std::string, CMenuFx*>		m_flash;
    gameswf::glyph_provider             *m_glyphProvider;
public:
	CGameUI();
	virtual ~CGameUI();	

	// openFlash
	// load swf file
	CMenuFx* openFlash( const char *name, const char *url );

	// releaseAllFlash
	// unload all swf
	void releaseAllFlash();
	
	// update
	// update game flash ui
	void update();

	// render
	// render game flash ui
	void render(int x, int y, int w, int h, bool hasBackground);

	// getFlash
	// get flash player
	CMenuFx* getFlash( const char *lpName )
	{
		return m_flash[ lpName ];
	}

	// releaseFlash
	// free flash
	void releaseFlash( const char *lpName );

	// handle event
	virtual bool OnEvent(const SEvent& irrEvent);
    
    // resetGlyphProvider
    // clean font cache
    inline void resetGlyphProvider()
    {
        if ( m_glyphProvider )
            m_glyphProvider->reset_provider();
    }
public:
	static tu_file* _fileOpen( const char *lpPath );
	static void _fsCallback( gameswf::character *movie, const char *lpCommand, const char *lpParams );
	static bool _getFont( const char *font_name, tu_string &file_name, bool is_bold, bool is_italic );
	static bool _getFontSize( const char *font_name, int &fontSize );
};

#endif