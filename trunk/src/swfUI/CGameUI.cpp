#include "stdafx.h"
#include "CGameUI.h"
#include "CMenuFx.h"
#include "IView.h"

////////////////////////////////////////////
// CGameUI static func implement
////////////////////////////////////////////

tu_file* CGameUI::_fileOpen( const char *lpPath )
{
	return new tu_file(lpPath, "rb");
}

void CGameUI::_fsCallback( gameswf::character *movie, const char *lpCommand, const char *lpParams )
{	
	SEvent	flashEvent;
	flashEvent.EventType = EET_FSCOMMAND_EVENT;
	strcpy(flashEvent.FSEvent.Command, lpCommand);
	strcpy(flashEvent.FSEvent.Param, lpParams);

	// post event to device
	irr::IrrlichtDevice *device = getIView()->getDevice();
	device->postEventFromUser( flashEvent );
}

bool CGameUI::_getFont( const char *font_name, tu_string &file_name, bool is_bold, bool is_italic )
{	
	if ( strcmp("Arial Black",font_name) == 0 )
		file_name = getIView()->getPath("data/font/killerants.ttf");
	else if ( strcmp("Verdana",font_name) == 0 )
		file_name = getIView()->getPath("data/font/zrnic.ttf");
	else
		file_name = getIView()->getPath("data/font/arial.ttf");		
	return true;
}

////////////////////////////////////////////
// CGameUI implement
////////////////////////////////////////////

CGameUI::CGameUI()
{
	m_render = gameswf::create_render_handler_irrlicht( getIView()->getDevice() );
	
	gameswf::register_file_opener_callback	( CGameUI::_fileOpen );
	gameswf::register_fscommand_callback	( CGameUI::_fsCallback );
	gameswf::register_getfont_callback		( CGameUI::_getFont );

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
CMenuFx* CGameUI::openFlash( const char *name, const char *url )
{
	CMenuFx* flash = new CMenuFx();
	m_flash[ name ] = flash;
	flash->loadFlash( url );
	return flash;
}

// releaseAllFlash
// unload all swf
void CGameUI::releaseAllFlash()
{
	std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		if ( (*i).second )
			delete (*i).second;
		i++;
	}
	m_flash.clear();
}

// update
// update game flash ui
void CGameUI::update()
{
	std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		if ( (*i).second )
			(*i).second->update();
		i++;
	}
}

// render
// render game flash ui
void CGameUI::render(int x, int y, int w, int h, bool hasBackground)
{
	std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
	while ( i != end )
	{
		if ( (*i).second )
			(*i).second->render(x, y, w, h);
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
				std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					if ( (*i).second )
						(*i).second->updateMouseState( x,y, irrEvent.MouseInput.isLeftPressed() );
					i++;
				}			
			}
			break;
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			{
				// touch press
				std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					if ( (*i).second )
						(*i).second->updateMouseState( x,y, true );
					i++;
				}
			}
			break;
		case irr::EMIE_LMOUSE_LEFT_UP:
			{
				// touch down
				std::map<std::string, CMenuFx*>::iterator i = m_flash.begin(), end = m_flash.end();
				while ( i != end )
				{
					if ( (*i).second )
						(*i).second->updateMouseState( x,y, false );
					i++;
				}
			}
			break;
		}

	}
	return false;
}