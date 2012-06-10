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
	printf("fsCallback: %s %s\n", lpCommand, lpParams);
}

////////////////////////////////////////////
// CGameUI implement
////////////////////////////////////////////

CGameUI::CGameUI()
{
	m_render = gameswf::create_render_handler_ogl();	

	gameswf::register_file_opener_callback	( CGameUI::_fileOpen );
	gameswf::register_fscommand_callback	( CGameUI::_fsCallback );

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

// handle event
bool CGameUI::OnEvent(const SEvent& irrEvent)
{
	if ( irrEvent.EventType == EET_MOUSE_INPUT_EVENT )
	{
		int state = irrEvent.MouseInput.Event;
		int x = irrEvent.MouseInput.X;
		int y = irrEvent.MouseInput.Y;		

		switch ( state )
		{
		case irr::EMIE_MOUSE_MOVED:
			{				
				// drag touch
				map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					(*i).second->updateMouseState( x,y, irrEvent.MouseInput.isLeftPressed() );
					i++;
				}			
			}
			break;
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			{
				// touch press
				map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					(*i).second->updateMouseState( x,y, true );
					i++;
				}
			}
			break;
		case irr::EMIE_LMOUSE_LEFT_UP:
			{
				// touch down
				map<std::string, CFlashHander*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					(*i).second->updateMouseState( x,y, false );
					i++;
				}
			}
			break;
		}

	}
	return false;
}