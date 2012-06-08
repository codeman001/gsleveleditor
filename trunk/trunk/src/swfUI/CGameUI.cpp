#include "stdafx.h"
#include "CGameUI.h"
#include "CFlashHandler.h"

////////////////////////////////////////////
// CGameUI static func implement
////////////////////////////////////////////

tu_file* CGameUI::_fileOpen( const char *lpPath )
{
	return new tu_file(lpPath, "rb");
}

void CGameUI::_fsCallback( gameswf::character *movie, const char *lpCommand, const char *lpParams )
{
}

////////////////////////////////////////////
// CGameUI implement
////////////////////////////////////////////

CGameUI::CGameUI()
{
	m_render = gameswf::create_render_handler_ogl();	

	gameswf::register_file_opener_callback ( CGameUI::_fileOpen );
	gameswf::register_fscommand_callback ( CGameUI::_fsCallback );

	gameswf::set_render_handler ( m_render );
	gameswf::set_glyph_provider ( gameswf::create_glyph_provider_freetype() );

	m_render->open();
}

CGameUI::~CGameUI()
{
	releaseAllFlash();		
	delete m_render;
}

// openFlash
// load swf file
CFlashHander* CGameUI::openFlash( const char *name, const char *url )
{
	CFlashHander* flash = new CFlashHander();
	m_flash[ name ] = flash;
	flash->loadFlash( url );
	return flash;
}

// releaseAllFlash
// unload all swf
void CGameUI::releaseAllFlash()
{
	map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		delete (*i).second;
		i++;
	}
	m_flash.clear();
}

// update
// update game flash ui
void CGameUI::update(float timestep)
{
	map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		(*i).second->update(timestep);
		i++;
	}
}

// render
// render game flash ui
void CGameUI::render(int x, int y, int w, int h, bool hasBackground)
{
	map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		(*i).second->render(x, y, w, h, hasBackground);
		i++;
	}
}