#include "stdafx.h"
#include "base/tu_file.h"
#include "base/utility.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_types.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_freetype.h"
#include "gameswf/gameswf_bitmapfont.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_text.h"
#include "gameswf/gameswf_filters.h"
#include "gameswf/gameswf_allocator.h"
#include "gameswf/gameswf_debugger.h"

#include "menufx.h"

#ifdef _GLITCH
#include <glitch/glitch.h>
#include <glitch/debugger/CDebugger.h>
#endif

namespace gameswf
{
	void standard_method_map_init();
	void clear_standard_fnames();
	void clear_standard_method_map();
	void clear_standard_property_map();
	void clears_tag_loaders();
	void clear_disasm();
	void removeHTML(tu_string& str);
	void set_file_opener(glf::IOStream* (*callback)(const char* filename, const char* mode));
}

static void	message_log(const char* message)
// Process a log message.
{
	if (gameswf::get_verbose_parse())
	{
		fputs(message, stdout);
		fflush(stdout);
	}
}


static void	log_callback(bool error, const char* message)
// Error callback for handling gameswf messages.
{
	if (error)
	{
		// Log, and also print to stderr.
		message_log(message);
		fputs(message, stderr);
		fflush(stderr);
	}
	else
	{
		message_log(message);
	}
}


static gameswf::tu_file*	file_opener(const char* url)
// Callback function.  This opens files for the gameswf library.
{
	return swfnew gameswf::tu_file(url, "rb");
}


static void	fs_callback(gameswf::character* movie, const char* command, const char* args)
// For handling notification callbacks from ActionScript.
{
//	message_log("fs_callback: '");
//	message_log(command);
//	message_log("' '");
//	message_log(args);
//	message_log("'\n");
}

static void fscommand_callback_handler(gameswf::character* movie, const char* command, const char* arg)
{
	gameswf::player* player = movie->get_player();
	//printf("fscommand %s %s", command, arg);

	RenderFX* renderFX = (RenderFX*)player->m_userdata;
	renderFX->FSCommand(command, arg);
}

bool CanHandleMouseEvent(gameswf::character* ch);

bool sprite_can_handle_mouse_event_callback(gameswf::character* ch)
{
	return CanHandleMouseEvent(ch);
}

//-------------------------
// RenderFX
//-------------------------

gameswf::player_context* RenderFX::s_default_context = NULL;

RenderFX::RenderFX() :
	m_context(NULL),
	m_inputBehavior(INPUT_BEHAVIOR_DEFAULT),
	m_listener(NULL)
{
}

RenderFX::~RenderFX()
{
	Unload();
}

gameswf::render_handler* gameswf::create_render_handler_irrlicht(void* videoDriver);

void RenderFX::Initialize(gameswf::video_driver* videoDriver)
{
	gameswf::log_msg("RenderFX::Initialize(gameswf::video_driver* videoDriver) deprecated. Use RenderFX::Initialize(InitializationParameters& parameters) instead.\n");

	InitializationParameters parameters;
	parameters.videoDriver = videoDriver;
	Initialize(parameters);
}

void RenderFX::Initialize(InitializationParameters& parameters)
{
	if(s_default_context != NULL)
		return;

#if GAMESWF_ENABLE_ALLOCATOR
	gameswf::init_memory_allocator();
#endif

#if GAMESWF_LINK_GLF
	assert(parameters.fileOpenerCallback != NULL);
	gameswf::set_file_opener(parameters.fileOpenerCallback);
#endif

	assert(gameswf::tu_types_validate());

	gameswf::register_file_opener_callback(file_opener);
	gameswf::register_fscommand_callback(fs_callback);
	if (gameswf::get_verbose_parse())
	{
		gameswf::register_log_callback(log_callback);
	}
	gameswf::register_fscommand_callback(fscommand_callback_handler);

	gameswf::sound_handler*	sound = NULL;
	//if (do_sound) {
	//	sound = gameswf::create_sound_handler_sdl();
	//	gameswf::set_sound_handler(sound);
	//}
	gameswf::render_handler* render = gameswf::create_render_handler_irrlicht(parameters.videoDriver);
	gameswf::set_render_handler(render);

	render->open();
	render->set_antialiased(true);

	// Texture loader callback
	if(parameters.textureLoaderCallback != NULL)
	{
		gameswf::register_texture_loader_callback(parameters.textureLoaderCallback);
	}

	gameswf::standard_method_map_init();

	// Create default context
	s_default_context = CreateContext(parameters);
}

void RenderFX::InitializeGlyphTextureCache(int width, int height)
{
	gameswf::log_msg("RenderFX::InitializeGlyphTextureCache(int width, int height) deprecated. Use RenderFX::Initialize(InitializationParameters& parameters) instead.\n");
}

void RenderFX::Finalize()
{
	// Free default context
	DestroyContext(s_default_context);
	s_default_context = NULL;

	// Free render handler
	gameswf::render_handler* render = gameswf::get_render_handler();
	if(render)
	{
		swfdelete (render);
		gameswf::set_render_handler(NULL);
	}

	// Clear shared stuff only when all players are deleted
	gameswf::clears_tag_loaders();
	gameswf::clear_shared_libs();
	gameswf::clear_standard_fnames();
	gameswf::clear_standard_method_map();
	gameswf::clear_standard_property_map();
	gameswf::clear_disasm();

#if GAMESWF_ENABLE_ALLOCATOR
	gameswf::close_memory_allocator();
#endif
}

// Create a gameswf context.
gameswf::player_context* RenderFX::CreateContext(InitializationParameters& parameters)
{
	gameswf::player_context* context = swfnew gameswf::player_context();

	// Initialize glyph provider and its texture cache
	context->m_glyph_provider = swfnew gameswf::glyph_provider(parameters.glyphTextureCacheSize.width, parameters.glyphTextureCacheSize.height, parameters.loadFontsInMemory, parameters.fontScale);

	// Initialize bitmap font glyph provider and its texture cache
	context->m_bitmap_glyph_provider = swfnew gameswf::default_bitmap_glyph_provider(parameters.bitmapGlyphTextureCacheSize.width, parameters.bitmapGlyphTextureCacheSize.height, parameters.loadFontsInMemory);

	context->m_video_driver = parameters.videoDriver;

	return context;
}

// Destroy a gameswf context.
void RenderFX::DestroyContext(gameswf::player_context* context)
{
	if(context != NULL)
		swfdelete (context);
}

void RenderFX::SetWireFrame(bool enabled)
{
	gameswf::get_render_handler()->set_wire_frame(enabled);
}

void RenderFX::ForceTexturesToVRAM(bool drawTextures, gameswf::player_context* context)
{
	gameswf::rect bounds;
	bounds.m_x_min = 0;
	bounds.m_y_min = 0;
	bounds.m_x_max = 0;
	bounds.m_y_max = 0;

	gameswf::rect uv_bounds;
	uv_bounds.m_x_min = 0;
	uv_bounds.m_y_min = 0;
	uv_bounds.m_x_max = 1;
	uv_bounds.m_y_max = 1;

	gameswf::rgba color(255, 255, 255, 255);
	gameswf::matrix m;

	if(context == NULL)
		context = s_default_context;

	assert(context != NULL);

	// layout texture caches
	if(context->m_glyph_provider->get_texture_cache() != NULL)
		context->m_glyph_provider->get_texture_cache()->get_bitmap_info()->layout();

	if(context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
		context->m_bitmap_glyph_provider->get_texture_cache()->get_bitmap_info()->layout();

	for(int it = 0; it < context->m_instances.size(); it++)
	{
		gameswf::player* player = context->m_instances[it];
		assert(player);
		
		if(drawTextures)
			player->get_root()->begin_display();

		gameswf::array<gameswf::smart_ptr<gameswf::bitmap_info> >& list = player->get_root()->m_def->m_bitmap_list;
		for(int i = 0; i< list.size(); i++)
		{
			list[i]->layout();
			if(drawTextures)
				gameswf::get_render_handler()->draw_bitmap(m, list[i].get_ptr(), bounds, uv_bounds, color);
		}

		if(drawTextures)
			player->get_root()->end_display();
	}
}

void RenderFX::UnloadTextures(gameswf::player_context* context)
{
	if(context == NULL)
		context = s_default_context;

	for(int it = 0; it < context->m_instances.size(); it++)
	{
		gameswf::player* player = context->m_instances[it];
		assert(player);

		gameswf::array<gameswf::smart_ptr<gameswf::bitmap_info> >& list = player->get_root()->m_def->m_bitmap_list;
		for(int i = 0; i < list.size(); i++)
		{
			list[i]->unlayout();
		}
	}
}

void RenderFX::Load(const char* infile, gameswf::player_context* context)
{
	if(context == NULL)
		context = s_default_context;
	assert(context != NULL);

	m_filename = infile;
	m_player = swfnew gameswf::player(context);
	m_player->m_userdata = this;

	// use this for multifile games
	// workdir is used when LoadMovie("myfile.swf", _root) is called
	{
		gameswf::tu_string workdir;
		// Find last slash or backslash.
		const char* ptr = infile + strlen(infile);
		for (; ptr >= infile && *ptr != '/' && *ptr != '\\'; ptr--) {}
		// Use everything up to last slash as the "workdir".
		int len = ptr - infile + 1;
		if (len > 0)
		{
			gameswf::tu_string workdir(infile, len);
			m_player->set_workdir(workdir.c_str());
		}
	}

	m_root = m_player->load_file(infile);
	assert(m_root != NULL);
	
	SetContext(m_root->m_movie.get_ptr());
}

void RenderFX::Unload()
{
	gameswf::render_handler* handler = gameswf::get_render_handler();
	if(handler)
		handler->reset();

	for(int i = 0; i < CONTROLLER_COUNT; i++)
		m_controllers[i].Reset();

	m_root = NULL;
	m_player = NULL;
	m_filename.clear();
	SetContext(NULL);
}

void RenderFX::SetEventListener(EventListener* listener)
{
	m_listener = listener;
}

void RenderFX::Update(int dt, bool multipleUpdates)
{
#ifdef GLITCH_ENABLE_DEBUGGER
	glitch::debugger::SScopeEvent dbgEvent(m_player->m_context->m_video_driver, "RenderFX::Update");
#endif

	float speed_scale = 1.0f;

	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();

	root->advance(dt * speed_scale / 1000.0f, multipleUpdates);

	// Check to see if clicked item animation stopped
	if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
	{
		for(int id = 0; id < CONTROLLER_COUNT; id++)
		{
			if(m_controllers[id].clickedCharacter != NULL && m_controllers[id].clickedCharacter->get_play_state() == gameswf::character::STOP)
			{
				Event clickEvent(EVENT_CLICKED, m_controllers[id].clickedCharacter.get_ptr(), id);
				SendEvent(clickEvent);
				m_controllers[id].clickedCharacter = NULL;
			}
		}
	}
}

typedef struct CharacterMatch
{
	CharacterMatch()
	{
		dist = (float)0x7fffffff;
		ch = NULL;
	}

	float dist;
	gameswf::character* ch;
} CharacterMatch;

void RenderFX::UpdateInput(int keyState, int controllerID)
{
	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	Controller& controller = m_controllers[controllerID];

	gameswf::smart_ptr<gameswf::character> current = controller.currentActiveEntity;
	if(current == 0)
		return;

	if(keyState == 0)
		return;

	if(controller.clickedCharacter != NULL)
		return;

	// check if the event has been consumed before doing anything...
	Event keyEvent(EVENT_KEY_RELEASED, current.get_ptr(), controllerID);
	keyEvent.key = keyState;
	SendEvent(keyEvent);
	if(keyEvent.IsConsumed())
		return;

	// TODO: apply scale + translation
	gameswf::matrix matrix = current->get_world_matrix();
	float x = matrix.m_[0][2];
	float y = matrix.m_[1][2];
	const float THRESHOLD_DISTANCE = 0;//PIXELS_TO_TWIPS(50);
	const float X_SCALE = (keyState & KEY_LEFT) || (keyState & KEY_RIGHT) ?  1.0f : 10.0f;
	const float Y_SCALE = (keyState & KEY_UP) || (keyState & KEY_DOWN) ? 1.0f : 10.0f;

	CharacterMatch nearestUp;
	CharacterMatch nearestDown;
	CharacterMatch nearestLeft;
	CharacterMatch nearestRight;

	CharacterIterator& characterIterator = FindCharacters(m_context, "btn", COLLECT_VISIBLE | COLLECT_ENABLED);
	for(int i = 0; i < characterIterator.size(); i++)
	{
		CharacterMatch test;
		test.ch = characterIterator.get(i);
		
		gameswf::matrix testMatrix = test.ch->get_world_matrix();
		float testX = testMatrix.m_[0][2];
		float testY = testMatrix.m_[1][2];
		float dx = (testX - x) * X_SCALE;
		float dy = (testY - y) * Y_SCALE;
		test.dist = dx * dx + dy * dy;

		if(dy < 0 && abs(dy) > THRESHOLD_DISTANCE && test.dist < nearestUp.dist)
			nearestUp = test;
		if(dy > 0 && abs(dy) > THRESHOLD_DISTANCE && test.dist < nearestDown.dist)
			nearestDown = test;
		if(dx < 0 && abs(dx) > THRESHOLD_DISTANCE && test.dist < nearestLeft.dist)
			nearestLeft = test;
		if(dx > 0 && abs(dx) > THRESHOLD_DISTANCE && test.dist < nearestRight.dist)
			nearestRight = test;
	}

	// focus on swfnew selection
	if((keyState & KEY_UP) && nearestUp.ch)
		SetFocus(nearestUp.ch, controllerID);
	else if((keyState & KEY_DOWN) && nearestDown.ch)
		SetFocus(nearestDown.ch, controllerID);
	else if((keyState & KEY_LEFT) && nearestLeft.ch)
		SetFocus(nearestLeft.ch, controllerID);
	else if((keyState & KEY_RIGHT) && nearestRight.ch)
		SetFocus(nearestRight.ch, controllerID);
	else if((keyState & KEY_SELECT) && m_listener)
	{
		if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
		{
			PlayAnim(current.get_ptr(), "clicked", 0);
			controller.clickedCharacter = current;
		}
	}
}

bool CanHandleMouseEvent(gameswf::character* ch)
{
	if(ch == NULL)
		return false;

	if(strstr(ch->m_name.c_str(), "btn") != 0)
	{
		if(ch->is(gameswf::AS_SPRITE))
			return ((gameswf::sprite_instance*)ch)->m_enabled;
		else
			return true;
	}

	return ch->m_can_handle_mouse_event;
}

gameswf::point GetLocalPosition(gameswf::character* ch, float mouseX, float mouseY)
{
	// transform world position to local position
	gameswf::point worldpos(PIXELS_TO_TWIPS(mouseX), PIXELS_TO_TWIPS(mouseY));
	gameswf::point localpos(0, 0);
	gameswf::matrix worldmatrix = ch->get_world_matrix();
	gameswf::matrix matrix;
	matrix.set_inverse(worldmatrix);
	matrix.transform(&localpos, worldpos);
	return localpos;
}

void RenderFX::UpdateCursor(RenderFX::Cursor& cursor, int controllerID)
{
#ifdef GLITCH_ENABLE_DEBUGGER
	glitch::debugger::SScopeEvent dbgEvent(m_player->m_context->m_video_driver, "RenderFX::UpdateCursor");
#endif

	GAMESWF_PROFILER_SCOPE_EVENT("RenderFX::UpdateCursor");

	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	if(controllerID < 0 || controllerID >= CONTROLLER_COUNT)
		return;

	Controller& controller = m_controllers[controllerID];
	Cursor oldCursor = controller.cursor;
	controller.cursor = cursor;

	gameswf::point p(cursor.x, cursor.y);
	m_root->set_mouse_screen_pos(cursor.x, cursor.y);
	m_root->set_active_controller(controllerID);
	m_root->screen_to_logical(p);
	float mouseX = p.m_x;
	float mouseY = p.m_y;

	// update cursor position, if any
	if(controller.cursorCharacter.get_ptr())
	{
		gameswf::matrix mat;
		mat.concatenate_translation(PIXELS_TO_TWIPS(mouseX), PIXELS_TO_TWIPS(mouseY));
		mat.set_scale_rotation(1.0f, 1.0f, cursor.angle);
		controller.cursorCharacter->set_matrix(mat);
	}

	if(	(controller.enabled == false) ||
		(m_context == NULL) ||
		((m_inputBehavior & INPUT_BEHAVIOR_LOCK_INPUT_UNTIL_EVENT_CLICKED) && controller.clickedCharacter != NULL) )
	{
		return;
	}

	m_root->notify_mouse_state((int)mouseX, (int)mouseY, 0);
	
	bool pressed = (cursor.buttonState != 0) && (oldCursor.buttonState == 0);
	bool released = (cursor.buttonState == 0) && (oldCursor.buttonState != 0);
	bool dragged = (cursor.buttonState != 0) && (cursor.x != oldCursor.x || cursor.y != oldCursor.y);

	gameswf::smart_ptr<gameswf::character> searchContext = (m_inputBehavior & INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT) ? m_root->m_movie : m_context;
	gameswf::smart_ptr<gameswf::character> topmost = searchContext->get_topmost_mouse_entity(PIXELS_TO_TWIPS(mouseX), PIXELS_TO_TWIPS(mouseY));
	gameswf::smart_ptr<gameswf::character> current = controller.currentActiveEntity;
	if(topmost != NULL)
	{
		while(topmost != NULL)
		{
			break;
			topmost = topmost->m_parent;
		}
	}

	// Set focus to top character
	if(controller.draggingCharacter == NULL)
	{
		const bool focusOnPress = (m_inputBehavior & INPUT_BEHAVIOR_FOCUS_ON_PRESS) != 0;
		const bool focusOnDrag = (m_inputBehavior & INPUT_BEHAVIOR_DRAG_OVER_CONTROL_GIVES_FOCUS) != 0;
		if((pressed && focusOnPress) || !focusOnPress || (focusOnDrag && topmost.get_ptr()))
		{
			SetFocus(topmost.get_ptr(), controllerID);
		}
	}
	else if ((m_inputBehavior & INPUT_BEHAVIOR_DRAG_OVER_CONTROL_GIVES_FOCUS) != 0)
	{
		if ((topmost.get_ptr() != 0) /*&&
			(controller.draggingCharacter != topmost.get_ptr())*/)
		{
			SetFocus(topmost.get_ptr(), controllerID);
		}		
	}

	// Cancel any previously clicked character to trigger EVENT_CLICKED, instead of locking all input
	if((pressed || released) && (controller.currentActiveEntity != controller.clickedCharacter))
	{
		controller.clickedCharacter = NULL;
	}

	// Cancel dragging on disabled button
	if(controller.draggingCharacter != NULL && !CanHandleMouseEvent(controller.draggingCharacter.get_ptr()))
	{
		controller.draggingCharacter = NULL;
	}

	// Send rollover/out events to current activity
	if(current != controller.currentActiveEntity)
	{
		if(controller.topMostEntity != NULL && CanHandleMouseEvent(controller.topMostEntity.get_ptr()))
		{
			gameswf::point localpos = GetLocalPosition(controller.topMostEntity.get_ptr(), mouseX, mouseY);
			Event rollOverEvent(EVENT_CURSOR_ROLLOUT, controller.topMostEntity.get_ptr(), controllerID);
			rollOverEvent.cursor.buttonState = cursor.buttonState;
			rollOverEvent.cursor.x = localpos.m_x;
			rollOverEvent.cursor.y = localpos.m_y;

			if(m_listener->CanHandleEvent(rollOverEvent)) 
				SendEvent(rollOverEvent);
		}

		if(topmost != NULL && CanHandleMouseEvent(topmost.get_ptr()))
		{
			gameswf::point localpos = GetLocalPosition(topmost.get_ptr(), mouseX, mouseY);
			Event rollOverEvent(EVENT_CURSOR_ROLLOVER, topmost.get_ptr(), controllerID);
			rollOverEvent.cursor.buttonState = cursor.buttonState;
			rollOverEvent.cursor.x = localpos.m_x;
			rollOverEvent.cursor.y = localpos.m_y;

			if(m_listener->CanHandleEvent(rollOverEvent)) 
				SendEvent(rollOverEvent);
		}
	}
	// Send dragOver/dragOut events to current activity
	if(dragged && controller.currentActiveEntity != NULL)
	{
		if(controller.topMostEntity == controller.currentActiveEntity && topmost != controller.currentActiveEntity && CanHandleMouseEvent(controller.currentActiveEntity.get_ptr()))
		{
			Event dragOutEvent(EVENT_CURSOR_DRAGOUT, controller.currentActiveEntity.get_ptr(), controllerID);
			gameswf::point localpos = GetLocalPosition(controller.currentActiveEntity.get_ptr(), mouseX, mouseY);
			dragOutEvent.cursor.buttonState = cursor.buttonState;
			dragOutEvent.cursor.x = localpos.m_x;
			dragOutEvent.cursor.y = localpos.m_y;

			if(m_listener->CanHandleEvent(dragOutEvent)) 
				SendEvent(dragOutEvent);
		}

		if(controller.topMostEntity != controller.currentActiveEntity && topmost == controller.currentActiveEntity && CanHandleMouseEvent(topmost.get_ptr()))
		{
			Event dragOverEvent(EVENT_CURSOR_DRAGOVER, controller.currentActiveEntity.get_ptr(), controllerID);
			gameswf::point localpos = GetLocalPosition(controller.currentActiveEntity.get_ptr(), mouseX, mouseY);
			dragOverEvent.cursor.buttonState = cursor.buttonState;
			dragOverEvent.cursor.x = localpos.m_x;
			dragOverEvent.cursor.y = localpos.m_y;

			if(m_listener->CanHandleEvent(dragOverEvent)) 
				SendEvent(dragOverEvent);
		}
	}
	controller.topMostEntity = topmost;

	gameswf::smart_ptr<gameswf::character> focus = controller.currentActiveEntity;
	if(focus.get_ptr() && CanHandleMouseEvent(focus.get_ptr()))
	{
		// transform world position to local position
		gameswf::point worldpos(PIXELS_TO_TWIPS(mouseX), PIXELS_TO_TWIPS(mouseY));
		gameswf::point localpos(0, 0);
		gameswf::matrix worldmatrix = focus->get_world_matrix();
		gameswf::matrix matrix;
		matrix.set_inverse(worldmatrix);
		matrix.transform(&localpos, worldpos);

		if(pressed)
		{
			// If pressed outside a button, clear focus
			if(!(m_inputBehavior & INPUT_BEHAVIOR_KEEP_FOCUS) && (topmost == NULL))
			{
				ResetFocus(controllerID);
			}
			else
			{
				if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
				{
					PlayAnim(focus.get_ptr(), "pressed", 0);
				}

				// cursor pressed event
				Event pressedEvent(EVENT_CURSOR_PRESSED, focus.get_ptr(), controllerID);
				pressedEvent.cursor.buttonState = cursor.buttonState;
				pressedEvent.cursor.x = localpos.m_x;
				pressedEvent.cursor.y = localpos.m_y;
				SendEvent(pressedEvent);

				controller.draggingCharacter = focus;
			}
		}
		else if(released)
		{
			// check if released while over focus
			if(!(m_inputBehavior & INPUT_BEHAVIOR_KEEP_FOCUS) && (focus != topmost))
			{
				// released outside, remove the focus
				Event releasedOutsideEvent(EVENT_CURSOR_RELEASED_OUTSIDE, focus.get_ptr(), controllerID);
				releasedOutsideEvent.cursor.buttonState = cursor.buttonState;
				releasedOutsideEvent.cursor.x = localpos.m_x;
				releasedOutsideEvent.cursor.y = localpos.m_y;
				SendEvent(releasedOutsideEvent);

				ResetFocus(controllerID);
			}
			else
			{
				Event releasedEvent(EVENT_CURSOR_RELEASED, focus.get_ptr(), controllerID);
				releasedEvent.cursor.buttonState = cursor.buttonState;
				releasedEvent.cursor.x = localpos.m_x;
				releasedEvent.cursor.y = localpos.m_y;

				if(m_listener->CanHandleEvent(releasedEvent))
				{
					bool releasedCalled = false;

					if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
					{
						releasedCalled = PlayAnim(focus.get_ptr(), "released", 0);
						if(!releasedCalled)
							releasedCalled = PlayAnim(focus.get_ptr(), "clicked", 0);
					}

					// cursor released event
					SendEvent(releasedEvent);
					
					if(releasedCalled)
					{
						// keep instance and wait for anim to end
						controller.clickedCharacter = focus;
					}
					else
					{
						// since animation has not been called, invoke "clicked" now
						Event clickEvent(EVENT_CLICKED, focus.get_ptr(), controllerID);
						clickEvent.cursor.x = localpos.m_x;
						clickEvent.cursor.y = localpos.m_y;
						SendEvent(clickEvent);
					}
				}
			}

			controller.draggingCharacter = NULL;
		}
		else if(dragged)
		{
			// cursor dragged event
			if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
			{
				Event draggedEvent(EVENT_CURSOR_DRAGGED, focus.get_ptr(), controllerID);
				draggedEvent.cursor.buttonState = cursor.buttonState;
				draggedEvent.cursor.x = localpos.m_x;
				draggedEvent.cursor.y = localpos.m_y;
				SendEvent(draggedEvent);
			}
			controller.draggingCharacter = focus;
		}
		else if(!(m_inputBehavior & INPUT_BEHAVIOR_KEEP_FOCUS) && (topmost == NULL) && (controller.draggingCharacter == NULL))
		{
			// If moved outside a button, clear focus
			ResetFocus(controllerID);
		}
	}
}

const RenderFX::Controller& RenderFX::GetController(int controllerID)
{
	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	return m_controllers[controllerID];
}

void RenderFX::SetControllerEnabled(int controllerID, bool enabled)
{
	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	m_controllers[controllerID].enabled = enabled;
}

void RenderFX::SetInputBehavior(int inputBehavior)
{
	m_inputBehavior = inputBehavior;
}

void RenderFX::SetCursor(const char* characterName, int controllerID)
{
	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	m_controllers[controllerID].cursorCharacter = Find(characterName);
}

gameswf::character* RenderFX::GetFlashRoot() const
{
	return m_root->m_movie.get_ptr();
}

gameswf::root* RenderFX::GetRoot() const
{
	return m_root.get_ptr();
}

void RenderFX::SetTextBufferingEnabled(bool enabled)
{
	m_root->set_text_buffering_enabled(enabled);
}

void RenderFX::SetRenderCachingEnabled(bool enabled)
{
	m_player->m_render_caching = enabled;
}

int RenderFX::PreloadGlyphs(unsigned short* codes, int count, const char* fontName, int fontSize, bool bold, bool italic, const gameswf::filter* filter)
{
	gameswf::smart_ptr<gameswf::font> font = swfnew gameswf::font(m_player.get_ptr());
	font->set_bold(bold);
	font->set_italic(italic);
	font->set_name(fontName);
	gameswf::rect bounds;

	gameswf::player_context* context = m_player->m_context;

	int i = 0;
	for(; i < count; i++)
	{
		gameswf::glyph g;
		g.m_fontsize = fontSize;
		g.m_code = codes[i];
		if (font->get_glyph(&g, codes[i], fontSize))
		{
			if (context->m_glyph_provider->get_texture_cache() == NULL && context->m_bitmap_glyph_provider->get_texture_cache() == NULL)
				continue;

			gameswf::bitmap_info* bi = g.m_fontlib_glyph.get_ptr();
			
			if(bi == context->m_glyph_provider->get_texture_cache()->get_bitmap_info())
			{
				// TrueType Font
				gameswf::glyph_texture_cache::filter_info info;
				info.stroke = 0;
				info.blurx = 0;
				info.blury = 0;

				if(filter != NULL)
				{
					if(filter->m_id == gameswf::filter::GLOW)
					{
						info.stroke = (unsigned char)(filter->blur.blur_x > filter->blur.blur_y ? filter->blur.blur_x : filter->blur.blur_y);
					}
					else if(filter->m_id == gameswf::filter::BLUR || filter->m_id == gameswf::filter::DROP_SHADOW)
					{
						info.blurx = (unsigned char)filter->blur.blur_x;
						info.blury = (unsigned char)filter->blur.blur_y;
					}
				}

				context->m_glyph_provider->get_texture_cache()->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, info, bounds);
			}
			else
			{
				// Bitmap Font
				context->m_bitmap_glyph_provider->get_texture_cache()->get_glyph_region(g.m_code, g.m_fe, g.m_fontsize, bounds);
			}
		}
	}
	return i;
}

int RenderFX::PreloadGlyphs(const char* text, const char* fontName, int fontSize, bool bold, bool italic, const gameswf::filter* filter)
{
	gameswf::array<unsigned short> codes;
	while (gameswf::Uint32 code = gameswf::decode_next_unicode_character(&text))
		codes.push_back(code);

	if(codes.size() > 0)
		return PreloadGlyphs(&codes[0], codes.size(), fontName, fontSize, bold, italic, filter);
	else
		return 0;
}

bool RenderFX::PreloadGlyphs(gameswf::character* context)
{
	if(context == NULL)
		context = m_root->m_movie.get_ptr();

	CharacterIterator& characterIterator = FindCharacters(context);
	for(int i = 0; i < characterIterator.size(); i++)
	{
		if(characterIterator.get(i)->is(gameswf::AS_EDIT_TEXT))
		{
			gameswf::edit_text_character* edit_text = (gameswf::edit_text_character*)characterIterator.get(i);
			for(int j = 0; j < edit_text->get_effect().m_filters.size(); j++)
			{
				const gameswf::filter* filter = &edit_text->get_effect().m_filters[j];
				gameswf::tu_string textNoHTML = edit_text->m_text;
				gameswf::removeHTML(textNoHTML);
				PreloadGlyphs(textNoHTML.c_str(), edit_text->m_font->get_name().c_str(), (int)TWIPS_TO_PIXELS(edit_text->m_text_height), edit_text->m_font->is_bold(), edit_text->m_font->is_italic(), filter);
			}
			gameswf::tu_string textNoHTML = edit_text->m_text;
			gameswf::removeHTML(textNoHTML);
			PreloadGlyphs(textNoHTML.c_str(), edit_text->m_font->get_name().c_str(), (int)TWIPS_TO_PIXELS(edit_text->m_text_height), edit_text->m_font->is_bold(), edit_text->m_font->is_italic());
		}
	}

	return true;
}

void RenderFX::ClearFonts(gameswf::player_context* context)
{
	if(context == NULL)
		context = s_default_context;
	assert(context != NULL);

	// clear providers and caches
	if(context->m_glyph_provider != NULL)
		context->m_glyph_provider->clear();

	if(context->m_bitmap_glyph_provider != NULL)
		context->m_bitmap_glyph_provider->clear();

	// loop for each RenderFX instance
	for(int it = 0; it < context->m_instances.size(); it++)
	{
		gameswf::player* player = context->m_instances[it];
		assert(player);

		RenderFX* renderFX = (RenderFX*)player->m_userdata;
		assert(renderFX);

		// clear text fields
		CharacterIterator& characterIterator = renderFX->FindCharacters(renderFX->m_root->m_movie.get_ptr());
		for(int i = 0; i < characterIterator.size(); i++)
		{
			if(characterIterator.get(i)->is(gameswf::AS_EDIT_TEXT))
			{
				gameswf::edit_text_character* edit_text = (gameswf::edit_text_character*)characterIterator.get(i);
				edit_text->set_text_value("");
			}
		}
	}
}

void RenderFX::ClearGlyphTextureCaches(gameswf::player_context* context)
{
	if(context == NULL)
		context = s_default_context;
	assert(context != NULL);

	// clear caches (not providers)
	if(context->m_glyph_provider != NULL && context->m_glyph_provider->get_texture_cache() != NULL)
		context->m_glyph_provider->get_texture_cache()->reset();

	if(context->m_bitmap_glyph_provider != NULL && context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
		context->m_bitmap_glyph_provider->get_texture_cache()->reset();
}

gameswf::character* RenderFX::Find(const char* characterName)
{
	// try to resolve from current context, if fails try from root
	gameswf::character* ch = Find(characterName, m_context);
	if(ch == NULL)
		ch = Find(characterName, m_root->m_movie.get_ptr());
	if(ch == NULL)
	{
		gameswf::as_object* obj = m_root->get_root_movie()->find_target(characterName);
		if(obj != NULL && obj->is(gameswf::AS_CHARACTER))
			return (gameswf::character*)obj;
	}

	return ch;
}

gameswf::character* RenderFX::Find(const char* characterName, gameswf::character* context)
{
	if(characterName == NULL || context == NULL)
		return NULL;

	m_depthSearchData.Init(context, characterName);
	gameswf::character* ds = DepthSearch(0);
	return ds;
}

void RenderFX::SearchIndex::Init(RenderFX& renderFX)
{
	Clear();

	// collect named characters
	RenderFX::CharacterIterator& it = renderFX.FindCharacters(renderFX.m_context, NULL, COLLECT_NAMED);
	for(int i = 0; i < it.size(); i++)
	{
		gameswf::character* ch = it.get(i);
		if(ch->get_name().size() > 0)
		{
			EntryList* list = NULL;
			if(!m_index.get(ch->get_name(), &list))
			{
				list = swfnew EntryList();
				m_index.add(ch->get_name(), list);
			}

			Entry e = Entry();
			e.ch = ch;
			
			// build character path
			const char* stack[32];
			int depth = 0;
			while(ch)
			{
				assert(depth < 32);
				if(strlen(ch->m_name.c_str()) > 0)
					stack[depth++] = ch->m_name.c_str();
				ch = ch->get_parent();
			}
			strcpy(e.characterPath, "");
			while(depth > 0)
			{
				--depth;
				strcat(e.characterPath, stack[depth]);
				if(depth > 0)
					strcat(e.characterPath, ".");
			}

			list->push_back(e);
		}
	}
}



gameswf::character* RenderFX::SearchIndex::Find(const char* characterName)
{
	int len = strlen(characterName);
	const char* stop = characterName + len;
	const char* start = strrchr(characterName, '.');
	if(!start)
		start = characterName;
	else
		start++;
	
	EntryList* list = NULL;
	gameswf::tu_string name(start, stop - start);
	if(!m_index.get(name, &list))
		return NULL;

	// check integrality
	//for(int i = 0; i < list->size(); i++)
	//{
	//	Entry& e = (*list)[i];
	//	if(strcmp(e.characterPath, characterName) == 0)
	//		return e.ch;
	//}

	// check keywords
	for(int i = 0; i < list->size(); i++)
	{
		Entry& e = (*list)[i];
		
		const char* path = e.characterPath;
		const char* keyword = characterName;

		while(true)
		{
			const char* keywordEnd = strchr(keyword, '.');
			if(!keywordEnd)
				keywordEnd = stop;
			
			int keywordLength = keywordEnd - keyword;
			char buffer[128];
			memcpy(buffer, keyword, keywordLength);
			buffer[keywordLength] = '\0';

			const char* found = strstr(path, buffer);
			if(!found)
				break;
			
			path = found + keywordLength;
			keyword += keywordLength + 1;

			if(*path == '\0')
				return e.ch;
		}
	}
	
	return NULL;
}

RenderFX::SearchIndex& RenderFX::GetSearchIndex()
{
	m_searchIndex.Init(*this);
	return m_searchIndex;
}

void RenderFX::SetFocus(const char* characterName, int controllerID)
{
	gameswf::character* next = Find(characterName);
	SetFocus(next, controllerID);
}

void RenderFX::ResetFocus(int controllerID)
{
	SetFocus((gameswf::character*)NULL, controllerID);
	m_controllers[controllerID].draggingCharacter = NULL;
}

void RenderFX::SetFocus(gameswf::character* next, int controllerID)
{
	assert(controllerID >= 0 && controllerID < CONTROLLER_COUNT);
	gameswf::character* current = m_controllers[controllerID].currentActiveEntity.get_ptr();

	if(current == next)
		return;

	if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
	{
		// Play focus_out on previous if enabled
		if(current && current->is(gameswf::AS_SPRITE) && ((gameswf::sprite_instance*)current)->m_enabled)
		{
			PlayAnim(current, "focus_out", 0);
			Event focusEvent(EVENT_LOSTFOCUS, current, controllerID);
			SendEvent(focusEvent);
		}
	}

	m_controllers[controllerID].currentActiveEntity = next;

	if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
	{
		// Play focus_in on next
		if(next)
		{
			Event focusEvent(EVENT_GOTFOCUS, next, controllerID);
			if(m_listener->CanHandleEvent(focusEvent))
			{
				PlayAnim(next, "focus_in", 0);
				SendEvent(focusEvent);
			}
			else
			{
				m_controllers[controllerID].currentActiveEntity = NULL;
			}
		}
	}
}

void RenderFX::SetAlpha(const char* characterName, float alpha)
{
	gameswf::character* ch = Find(characterName);
	if(ch)
	{
		gameswf::cxform cxform = gameswf::cxform::identity;
		cxform.m_[3][0] = alpha;
		ch->set_cxform(cxform);
	}
}

float RenderFX::GetAlpha( gameswf::character* ch) const 
{
	if(ch)
	{
		const gameswf::cxform& cxform = ch->get_cxform();
		return cxform.m_[3][0];
	}

	return 0.0f;
}

void RenderFX::SetColorTransform(const char* characterName, unsigned int multARGB, unsigned int addARGB)
{
	SetColorTransform(Find(characterName), multARGB, addARGB);
}

void RenderFX::SetColorTransform(gameswf::character* character, unsigned int multARGB, unsigned int addARGB)
{
	if(character)
	{
		gameswf::cxform cxform;
		// mult part
		cxform.m_[0][0] = ((multARGB >> 16) & 0xff) / 255.0f; // R
		cxform.m_[1][0] = ((multARGB >> 8) & 0xff) / 255.0f; // G
		cxform.m_[2][0] = ((multARGB >> 0) & 0xff) / 255.0f; // B
		cxform.m_[3][0] = ((multARGB >> 24) & 0xff) / 255.0f; // A
		// add part
		cxform.m_[0][1] = (float)((addARGB >> 16) & 0xff); // R
		cxform.m_[1][1] = (float)((addARGB >> 8) & 0xff); // G
		cxform.m_[2][1] = (float)((addARGB >> 0) & 0xff); // B
		cxform.m_[3][1] = (float)((addARGB >> 24) & 0xff); // A
		character->set_cxform(cxform);
	}
}

void RenderFX::SetColorFilter(const char* characterName, int filterIndex, unsigned int newARGB)
{
	SetColorFilter(Find(characterName), filterIndex, newARGB);
}

void RenderFX::SetColorFilter(gameswf::character* character, int filterIndex, unsigned int newARGB)
{
	if(character && filterIndex >= 0 && filterIndex < character->get_effect().m_filters.size())
	{
		gameswf::effect eff = character->get_effect();
		gameswf::filter& f = eff.m_filters[filterIndex];
		switch(f.m_id)
		{
		case gameswf::filter::DROP_SHADOW:
			f.drop_shadow.drop_shadow_color[0] = (newARGB >> 0) & 0xff; // B
			f.drop_shadow.drop_shadow_color[1] = (newARGB >> 8) & 0xff; // G
			f.drop_shadow.drop_shadow_color[2] = (newARGB >> 16) & 0xff; // R
			f.drop_shadow.drop_shadow_color[3] = (newARGB >> 24) & 0xff; // A
			break;

		case gameswf::filter::GLOW:
			f.glow.glow_color[0] = (newARGB >> 0) & 0xff; // B
			f.glow.glow_color[1] = (newARGB >> 8) & 0xff; // G
			f.glow.glow_color[2] = (newARGB >> 16) & 0xff; // R
			f.glow.glow_color[3] = (newARGB >> 24) & 0xff; // A
			break;
		}
		character->set_effect(eff);
	}
}

void RenderFX::SetVisible(const char* characterName, bool visible)
{
	gameswf::character* ch = Find(characterName);
	if(ch)
	{
		ch->set_visible(visible);
	}
}

void RenderFX::SetPosition(const char* characterName, int x, int y)
{
	gameswf::character* ch = Find(characterName);
	SetPosition(ch,x,y);
}

void RenderFX::SetPosition(gameswf::character* ch, int x, int y)
{
	if(ch)
	{
		const gameswf::matrix& curMat = ch->get_matrix();
		gameswf::matrix mat;
		mat.concatenate_translation(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));
		mat.set_scale_rotation( curMat.get_x_scale(), curMat.get_y_scale(), curMat.get_rotation() );
		ch->set_matrix(mat);
	}
}

void RenderFX::SetEnabled(const char* characterName, bool enabled)
{
		gameswf::character* ch = Find(characterName);
		SetEnabled(ch, enabled);
}
void RenderFX::SetEnabled(gameswf::character* character, bool enabled)
{
	if(character && character->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)character;
		if(si->m_enabled != enabled)
		{
			if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
			{
				if(enabled)
				{
					if(PlayAnim(character, "activated", 0) == false)
						PlayAnim(character, "focus_out", 0);
				}
				else
				{
					PlayAnim(character, "disabled", 0);
				}
			}
		}
		si->m_enabled = enabled;
	}
}

void RenderFX::SetText(gameswf::character* character, const char* text, bool html)
{
	if(character && character->is(gameswf::AS_EDIT_TEXT))
	{
		gameswf::edit_text_character* edit_text = (gameswf::edit_text_character*)character;
		edit_text->set_text_value(gameswf::tu_string(text), html);
	}
}

const char* RenderFX::GetText(gameswf::character* character)
{
	if(character && character->is(gameswf::AS_EDIT_TEXT))
	{
		gameswf::edit_text_character* edit_text = (gameswf::edit_text_character*)character;
		return edit_text->m_text.c_str();
	}

	return NULL;
}


static const int s_stringBufferSize = 8192;	//this shit can explose ! (credits string for example)
static char s_stringBuffer[s_stringBufferSize];

void RenderFX::FormatText(gameswf::character* character, const char* format, ...)
{	
	assert( strlen( format ) < s_stringBufferSize );

	va_list vlist;
	va_start(vlist, format);
	vsprintf(s_stringBuffer, format, vlist);
	va_end(vlist);

	SetText(character, s_stringBuffer);
}

void RenderFX::FormatHTML(gameswf::character* character, const char* format, ...)
{	
	assert( strlen( format ) < s_stringBufferSize );

	va_list vlist;
	va_start(vlist, format);
	vsprintf(s_stringBuffer, format, vlist);
	va_end(vlist);

	SetText(character, s_stringBuffer, true);
}

void RenderFX::SetText(const char* characterName, const char* text, bool html)
{
	gameswf::character* ch = Find(characterName);
	SetText( ch, text, html );
}

const char* RenderFX::GetText(const char* characterName)
{
	return GetText(Find(characterName));
}

void RenderFX::FormatText(const char* characterName, const char* format, ...)
{
	assert( strlen( format ) < s_stringBufferSize );

	va_list vlist;
	va_start(vlist, format);
	vsprintf(s_stringBuffer, format, vlist);
	va_end(vlist);

	SetText( characterName, s_stringBuffer );
}

void RenderFX::FormatHTML(const char* characterName, const char* format, ...)
{
	assert( strlen( format ) < s_stringBufferSize );

	va_list vlist;
	va_start(vlist, format);
	vsprintf(s_stringBuffer, format, vlist);
	va_end(vlist);

	SetText( characterName, s_stringBuffer, true );
}

void RenderFX::SetMember(const char* characterName, const char* member, gameswf::as_value& value)
{
	gameswf::tu_string name(member);
	
	gameswf::as_object* obj = Find(characterName);
	if(!obj)
		obj = m_root->get_root_movie()->find_target(characterName);
	if(obj)
		obj->set_member(name, value);
}

void RenderFX::SetMember(const char* characterName, const char* member, const char* value)
{
	gameswf::as_value objvalue(value);
	SetMember(characterName, member, objvalue);
}

void RenderFX::SetMember(const char* characterName, const char* member, int value)
{
	gameswf::as_value objvalue(value);
	SetMember(characterName, member, objvalue);
}

void RenderFX::SetLocalVariable(const char* spriteName, const char* variableName, gameswf::as_value& value)
{
	gameswf::tu_string name(variableName);
	
	gameswf::as_object* obj = Find(spriteName);
	if(!obj)
		obj = m_root->get_root_movie()->find_target(spriteName);
	if(obj && obj->is(gameswf::AS_SPRITE))
		((gameswf::sprite_instance*)obj)->get_environment()->set_local(name, value);
}

void RenderFX::SetTexture(const char* characterName, gameswf::video_texture* texture, bool keepBounds)
{
	gameswf::character* target = Find(characterName);
	if(target)
	{
		gameswf::bitmap_info* bi = gameswf::render::create_bitmap_info_texture(texture);
		gameswf::bitmap_character* image = NULL;

		if(keepBounds)
		{
			gameswf::rect bounds;
			target->get_bound(&bounds);

			gameswf::matrix m;
			m.set_inverse(target->get_matrix());
			m.transform(&bounds);

			image = swfnew gameswf::bitmap_character(target->get_player(), bi, &bounds);
		}
		else
		{
			image = swfnew gameswf::bitmap_character(target->get_player(), bi);
		}

		target->replace_me(image);
	}
}

void RenderFX::PlayAnim(const char* characterName, const char* animName, int flags)
{
	 GotoFrame(characterName, animName, true);
}

bool RenderFX::PlayAnim(gameswf::character* ch, const char* animName, int flags)
{
	return GotoFrame(ch, animName, true);
}

void RenderFX::GotoFrame(const char* characterName, const char* label, bool play)
{
	gameswf::character* ch = Find(characterName);
	GotoFrame(ch, label, play);
}

bool RenderFX::GotoFrame(gameswf::character* ch, const char* label, bool play)
{
	if(ch && ch->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)ch;
		if(si->goto_labeled_frame(label))
		{
			si->set_play_state(play ? gameswf::sprite_instance::PLAY : gameswf::sprite_instance::STOP);
			return true;
		}
	}
	return false;
}

void RenderFX::DBG_TraceContextInformation( gameswf::character* context )
{
	CharacterIterator& characterIterator = FindCharacters(context, NULL, 0);

	for(int i = 0; i < characterIterator.size(); i++)
	{
		const char* childName =  characterIterator.get(i)->m_name.c_str();
		printf( "Found child named %s\n", childName );
		if(characterIterator.get(i)->is(gameswf::AS_SPRITE))
		{
			printf( "child is a sprite ...\n" );
			gameswf::sprite_instance* si = (gameswf::sprite_instance*)characterIterator.get(i);
			
		}
		
		
	}
}

void RenderFX::TraceHierarchy( gameswf::character* node, int flags, int depth )
{
	if(node == NULL)
		node = m_root->m_movie.get_ptr();

	if(depth == 0)
		gameswf::log_msg("Hierarchy:\n");

	if((flags & COLLECT_VISIBLE) && (!node->m_visible || node->get_world_cxform().m_[3][0] == 0.0f))
		return;

	if((flags & COLLECT_NEED_ADVANCE) && !node->m_need_advance)
		return;

	char tabs[256] = {0};
	for(int i = 0; i < depth; i++)
		strcat(tabs, "   ");

	if(node->is(gameswf::AS_EDIT_TEXT))
	{
		gameswf::edit_text_character* edit_text = (gameswf::edit_text_character*)node;
		gameswf::log_msg("%s edit: '%s' text='%s' @ 0x%x\n", tabs, node->get_name().c_str(), edit_text->m_text.c_str(), node);
	}
	else if(node->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* sprite = (gameswf::sprite_instance*)node;
		gameswf::display_list& display_list = sprite->m_display_list;
		gameswf::log_msg("%s sprite: '%s' f=%d %s @ 0x%x\n", tabs, node->get_name().c_str(), sprite->get_current_frame(), sprite->get_play_state() == gameswf::character::PLAY ? "PLAY" : "STOP", node);

		for(int i = 0; i < display_list.size(); i++)
			TraceHierarchy(display_list.get_character(i), flags, depth + 1);
	}
	else
	{
		gameswf::log_msg("%s ch: '%s' @ 0x%x\n", tabs, node->get_name().c_str(), node);
	}

	//node->dump(gameswf::tu_string("   "));
}

void RenderFX::GotoFrame(const char* characterName, int frame, bool play)
{
	gameswf::character* ch = Find(characterName);
	if(ch)
	{
		GotoFrame(ch, frame, play);
	}
}

void RenderFX::GotoFrame(gameswf::character* ch, int frame, bool play)
{
	if(ch && ch->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)ch;
		si->goto_frame(frame);
		si->set_play_state(play ? gameswf::sprite_instance::PLAY : gameswf::sprite_instance::STOP);
	}
}

int RenderFX::GetFrameCount(const char* characterName)
{
	gameswf::character* ch = Find(characterName);
	if(ch)
	{
		return GetFrameCount(ch);
	}
	else
		return 0;
}

int RenderFX::GetFrameCount(gameswf::character* ch)
{
	if(ch && ch->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)ch;
		return si->get_frame_count();
	}
	else
		return 0;
}

bool RenderFX::IsAnimOver(const char* characterName)
{
	gameswf::character* ch = Find(characterName);
	if(ch)
	{
		return IsAnimOver(ch);
	}
	else
		return false;
}

bool RenderFX::IsAnimOver(gameswf::character* ch)
{
	if(ch && ch->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)ch;
		int CurrentFrame = si->get_current_frame();
		int FrameCount = si->get_frame_count();
		return (si->get_current_frame() >= si->get_frame_count()-1);
	}
	else
		return 0;
}

void RenderFX::FSCommand(const char* command, const char* arg)
{
	if(m_listener)
		m_listener->OnFSCommand(command, arg);
}

void RenderFX::SendEvent(Event& event)
{
	m_listener->OnEvent(event);

	if(!event.IsConsumed())
	{
		switch(event.type)
		{
		case EVENT_GOTFOCUS:
			InvokeASCallback(event.ch, "on_focus_in");
			break;
		case EVENT_LOSTFOCUS:
			InvokeASCallback(event.ch, "on_focus_out");
			break;
		case EVENT_CLICKED:
			InvokeASCallback(event.ch, "on_clicked");
			break;
		case EVENT_CURSOR_PRESSED:
			InvokeASCallback(event.ch, "onPress");
			break;
		case EVENT_CURSOR_RELEASED:
			InvokeASCallback(event.ch, "onRelease");
			break;
		case EVENT_CURSOR_RELEASED_OUTSIDE:
			InvokeASCallback(event.ch, "onReleaseOutside");
			break;
		case EVENT_CURSOR_ROLLOVER:
			InvokeASCallback(event.ch, "onRollOver");
			break;
		case EVENT_CURSOR_ROLLOUT:
			InvokeASCallback(event.ch, "onRollOut");
			break;
		case EVENT_CURSOR_DRAGOVER:
			InvokeASCallback(event.ch, "onDragOver");
			break;
		case EVENT_CURSOR_DRAGOUT:
			InvokeASCallback(event.ch, "onDragOut");
			break;
		}
	}
}

bool RenderFX::InvokeASCallback(const char* characterName, const char* funcName, const gameswf::as_value* arguments, int argumentCount)
{
	return InvokeASCallback(Find(characterName), funcName, arguments, argumentCount);
}

bool RenderFX::InvokeASCallback(gameswf::character* node, const char* funcName, const gameswf::as_value* arguments, int argumentCount)
{
	if(node)
	{
		gameswf::sprite_instance* sprite = node->is(gameswf::AS_SPRITE) ? (gameswf::sprite_instance*)node : NULL;
		if(sprite == NULL)
			sprite = node->m_parent.get_ptr() && node->m_parent.get_ptr()->is(gameswf::AS_SPRITE) ? (gameswf::sprite_instance*)node->m_parent.get_ptr() : NULL;
		if(sprite == NULL)
			return false;
		
		// Keep m_as_environment alive during any method calls!
		gameswf::smart_ptr<gameswf::as_object>	this_ptr(node);

		// In ActionScript 2.0, event method names are CASE SENSITIVE.
		// In ActionScript 1.0, event method names are CASE INSENSITIVE.
		gameswf::call_method(sprite->get_environment(), node, funcName, arguments, argumentCount);
		return true;
	}
	return false;
}

bool RenderFX::RegisterDisplayCallback(const char* characterName, void (*callback)(gameswf::render_state&, void*), void* user_ptr)
{
	gameswf::character* ch = Find(characterName);
	return RegisterDisplayCallback( ch, callback, user_ptr);
}

bool RenderFX::RegisterDisplayCallback(gameswf::character* ch, void (*callback)(gameswf::render_state&, void*), void* user_ptr)
{	
	if(ch)
	{
		ch->set_display_callback(callback, user_ptr);
		return true;
	}
	return false;
}

void RenderFX::SetViewport(int x, int y, int w, int h)
{
	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	root->set_display_viewport(x, y, w, h);
}

void RenderFX::SetBounds(int x, int y, int w, int h, gameswf::scale_mode mode)
{
	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	root->set_display_bounds(x, y, w, h, mode);
}

void RenderFX::SetOrientation(gameswf::orientation_mode mode)
{
	gameswf::get_render_handler()->set_orientation(mode);
}

void RenderFX::PreRender()
{
	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	gameswf::filter_engine* fe = gameswf::filter_engine::get_instance();
	if(fe)
		return fe->run(root.get_ptr());
}

void RenderFX::BeginDisplay()
{
	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	root->begin_display();
}

void RenderFX::EndDisplay()
{
	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	root->end_display();
}

void RenderFX::Render()
{
#ifdef GLITCH_ENABLE_DEBUGGER
	glitch::debugger::SScopeEvent dbgEvent(m_player->m_context->m_video_driver, "RenderFX::Render");
#endif

	gameswf::smart_ptr<gameswf::root> root = m_player->get_root();
	root->display();
}

void RenderFX::SetContext(gameswf::character* context)
{
	m_context = context;
}

gameswf::character* RenderFX::GetContext()
{
	return m_context;
}

RenderFX::CharacterIterator& RenderFX::FindCharacters(gameswf::character* context, const char* prefix, int flags)
{
	m_characterIterator.m_collectList.clear();
	CollectCharacters(context, prefix, flags);
	return m_characterIterator;
}

void RenderFX::CollectCharacters(gameswf::character* node, const char* prefix, int flags)
{
	bool collect = true;
	
	if((flags & COLLECT_VISIBLE) && !node->m_visible)
		collect = false;

	if(node->is(gameswf::AS_SPRITE))
	{
		gameswf::sprite_instance* si = (gameswf::sprite_instance*)node;
		if((flags & COLLECT_ENABLED) && !si->m_enabled)
			collect = false;
	}

	if(collect)
	{
		// collect the character node
		if(prefix == NULL || strstr(node->m_name.c_str(), prefix) != 0)
		{
			if(!((flags & COLLECT_NAMED) && node->m_name.size() == 0))
			{
				m_characterIterator.m_collectList.push_back(node);
			}
		}

		// collect children
		if(node->is(gameswf::AS_SPRITE))
		{
			gameswf::sprite_instance* sprite = (gameswf::sprite_instance*)node;
			gameswf::display_list& display_list = sprite->m_display_list;

			for(int i = 0; i < display_list.size(); i++)
			{
				CollectCharacters(display_list.get_character(i), prefix, flags);
			}
		}
	}
}

gameswf::character* RenderFX::DepthSearch(int depth)
{
	const int startIndex = m_depthSearchData.m_startIndexes[depth];
	const int stopIndex = depth + 1 < m_depthSearchData.m_startIndexes.size() ? m_depthSearchData.m_startIndexes[depth+1] : m_depthSearchData.m_container.size();

	const char* lastPath = NULL;
	const char* lastEndWord = NULL;
	m_depthSearchData.AddDepth();
	for(int i = startIndex; i < stopIndex; i++)
	{
		const char* path = m_depthSearchData.m_container[i].path;
		const char* endWord;
		if(path == NULL)
		{
			// security check
			return NULL;
		}
		else if(path != lastPath)
		{
			endWord = strchr(path, '.');
			if(endWord == NULL)
				endWord = path + strlen(path);

			lastPath = path;
			lastEndWord = endWord;
		}
		else
		{
			endWord = lastEndWord;
		}

		bool endLoop = false;
		const int wordLength = endWord - path;
		const char* newPath = path;
		gameswf::character* ch = m_depthSearchData.m_container[i].ch;
		if(ch->m_name.size() == wordLength && strncmp(ch->m_name.c_str(), path, wordLength) == 0)
		{
			// end of search string and found character
			if(endWord[0] == '\0')
				return ch;

			newPath = endWord + 1;
			endLoop = true;
		}

		// collect childen
		if(ch->is(gameswf::AS_SPRITE))
		{
			gameswf::sprite_instance* s = (gameswf::sprite_instance*)ch;
			for(int k = 0; k < s->m_display_list.size(); k++)
				m_depthSearchData.AddToCurrentDepth(s->m_display_list.get_character(k), newPath);
		}

		//if(endLoop)
		//	break;
	}

	if(m_depthSearchData.m_startIndexes.back() == m_depthSearchData.m_container.size())
		return NULL;
	else
		return DepthSearch(depth + 1);
}

//-------------------------
// MenuFX
//-------------------------

MenuFX::MenuFX()
{
	SetEventListener(this);
}

MenuFX::~MenuFX()
{
}

void MenuFX::Unload()
{
	RenderFX::Unload();

	// reset controllers
	for(int i = 0; i < CONTROLLER_COUNT; i++)
		m_controllers[i].Reset();

	m_states.clear();
	m_stack.clear();
}

void MenuFX::SetFocusDefault()
{
	// Focus on 1st button found
	CharacterIterator& characterIterator = FindCharacters(m_context, "btn", COLLECT_VISIBLE | COLLECT_ENABLED);

	if(characterIterator.size() > 0)
		SetFocus(characterIterator.get(0), 0);
	else
		ResetFocus(0);
}

void MenuFX::OnEvent(Event& event)
{
	// default implementation of character call back simply delegates to owning state
	if(m_stack.size() > 0)
		return GetStateHandler(event.ch)->OnEvent(event);
}

void MenuFX::OnFSCommand(const char* command, const char* arg)
{
	// default implementation of call back simply delegates to current state
	if(m_stack.size() > 0)
		m_stack.back()->OnFSCommand(command, arg);
}

bool MenuFX::CanHandleEvent(Event& event)
{
	// default implementation of call back simply delegates to owning state
	if(m_stack.size() > 0)
		return GetStateHandler(event.ch)->CanHandleEvent(event);

	return true;
}

void MenuFX::RegisterStates(MenuFX::State** states, int count)
{
	for(int i = 0; i < count; i++)
	{
		states[i]->m_menuFX = this;
		m_states.append(&states[i], 1);
	}

	for(int i = 0; i < count; i++)
	{
		// Hide menu by default
		const char* name = states[i]->GetName();
		gameswf::character* chMenu = Find(name);

		if(chMenu) 
		{
			states[i]->m_character = chMenu;
			chMenu->set_visible(false);
			states[i]->Create();
		}
		else 
		{
			char buffer[128];
			sprintf(buffer, "menu %s not found", name);
			message_log(buffer);
		}
	}

	// Enable all "btn"
	CharacterIterator& characterIterator = FindCharacters(m_context, "btn", 0);
	for(int i = 0; i < characterIterator.size(); i++)
	{
		if(characterIterator.get(i)->is(gameswf::AS_SPRITE))
		{
			gameswf::sprite_instance* si = (gameswf::sprite_instance*)characterIterator.get(i);
			si->m_enabled = true;
		}
	}
}

void MenuFX::RegisterState(MenuFX::State* state, const char* characterName)
{
	state->m_menuFX = this;
	m_states.append(&state, 1);

	gameswf::character* chMenu = Find(characterName ? characterName : state->GetName());
	assert(chMenu);
	state->m_character = chMenu;
	chMenu->set_visible(false);

	state->Create();
}

void MenuFX::Update(int dt, bool multipleUpdates)
{
	RenderFX::Update(dt, multipleUpdates);

	if(m_stack.size() > 0)
	{
		m_stack.back()->Update(dt);
	}

	for(int i = m_stack.size() - 2; i >= 0; i--)
	{
		if(m_stack[i]->m_character->get_visible())
			m_stack[i]->UpdateBackground(dt);
	}

	// Update states
	for(int i = 0; i < m_states.size(); i++)
	{
		// set _visible flag to false when "hide" anim ends
		if((m_states[i]->m_timelineAnim == MenuFX::State::TIMELINE_HIDE) && (m_states[i]->m_character->get_play_state() == gameswf::sprite_instance::STOP) && m_states[i]->m_character->get_visible())
		{
			// need to wait for clickedCharacter to prevent freezing
			bool pass = true;
			for(int c = 0; c < 4; c++)
			{
				if(m_controllers[c].clickedCharacter != NULL)
				{
					pass = false;
					break;
				}
			}

			if(pass)
				m_states[i]->m_character->set_visible(false);
		}
	}
}

MenuFX::State* MenuFX::GetState(const char* name)
{
	for(int i = 0; i < m_states.size(); i++)
	{
		if(strcmp(m_states[i]->GetName(), name) == 0)
			return m_states[i];
	}

	return NULL;
}

MenuFX::State* MenuFX::GetStateHandler(gameswf::character* ch)
{
	for(int i = m_stack.size() - 1; i >= 0; i--)
	{
		gameswf::character* parent = ch;
		while(parent != NULL)
		{
			if(m_stack[i]->m_character == parent)
				return m_stack[i];

			parent = parent->get_parent();
		}
	}

	return GetCurrentState();
}

MenuFX::State* MenuFX::GetCurrentState()
{
	return m_stack.size() > 0 ? m_stack.back() : NULL;
}

void MenuFX::SetMenu(const char* name, bool push)
{
	State* state = GetState(name);
	assert(state);
	if(state == NULL)
		return;

	if(m_stack.size() > 0)
	{
		State* current = m_stack.back();
		assert(current);
		// Tell state it lost focus
		current->LostFocus();

		if(!push)
			current->Hide();

		// Hide current menu
		if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
		{
			if(push && PlayAnim(current->m_character.get_ptr(), "focus_out", 0))
			{
				current->m_timelineAnim = State::TIMELINE_FOCUS_OUT;
			}
			else if(PlayAnim(current->m_character.get_ptr(), "hide", 0))
			{
				current->m_timelineAnim = State::TIMELINE_HIDE;
			}
		}
		
		// Remember focus in that state
		current->m_focus = m_controllers[0].currentActiveEntity;

		if((m_inputBehavior & INPUT_BEHAVIOR_ENABLE_CURRENT_MENU_ONLY) && state->m_character->is(gameswf::AS_SPRITE))
		{
			((gameswf::sprite_instance*)current->m_character.get_ptr())->m_enabled = false;
		}
	}
	
	if(!push)
		m_stack.pop_back();

	m_stack.append(&state, 1);

	assert(state->m_character.get_ptr());
	state->m_character->set_visible(true);

	if((m_inputBehavior & INPUT_BEHAVIOR_ENABLE_CURRENT_MENU_ONLY) && state->m_character->is(gameswf::AS_SPRITE))
	{
		((gameswf::sprite_instance*)state->m_character.get_ptr())->m_enabled = true;
	}

	SetContext(state->m_character.get_ptr());
	
	// Tell state it is shown and got focus
	if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
	{
		PlayAnim(state->m_character.get_ptr(), "show", 0);
	}

	if(m_inputBehavior & INPUT_BEHAVIOR_KEEP_FOCUS)
	{
		SetFocusDefault();
	}
	else if((m_inputBehavior & INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT) == 0)
	{
		// Kill focus
		//ResetFocus(0);

	}
	state->Show();
	state->GotFocus();
	state->m_timelineAnim = State::TIMELINE_SHOW;
}

void MenuFX::PushMenu(const char* name)
{
	SetMenu(name, true);
}

void MenuFX::SwitchMenu(const char* name)
{
	SetMenu(name, false);
}

void MenuFX::PopMenu()
{
	assert(m_stack.size() > 0);

	

	m_stack.back()->LostFocus();
	m_stack.back()->Hide();
	if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
	{
		PlayAnim(m_stack.back()->m_character.get_ptr(), "hide", 0);
	}
	m_stack.back()->m_timelineAnim = State::TIMELINE_HIDE;

	if((m_inputBehavior & INPUT_BEHAVIOR_ENABLE_CURRENT_MENU_ONLY) && m_stack.back()->m_character->is(gameswf::AS_SPRITE))
	{
		((gameswf::sprite_instance*)m_stack.back()->m_character.get_ptr())->m_enabled = false;
	}

	SetContext(m_root->m_movie.get_ptr());

	m_stack.pop_back();

	if(m_stack.size() > 0)
	{
		m_stack.back()->m_character->set_visible(true);

		if((m_inputBehavior & INPUT_BEHAVIOR_ENABLE_CURRENT_MENU_ONLY) && m_stack.back()->m_character->is(gameswf::AS_SPRITE))
		{
			((gameswf::sprite_instance*)m_stack.back()->m_character.get_ptr())->m_enabled = true;
		}

		SetContext(m_stack.back()->m_character.get_ptr());

		if((m_inputBehavior & INPUT_BEHAVIOR_STANDARD_FLASH) == 0)
		{
			// Tell state it got focus back again
			if(PlayAnim(m_stack.back()->m_character.get_ptr(), "focus_in", 0) == false)
				PlayAnim(m_stack.back()->m_character.get_ptr(), "show", 0);
		}
		// Give back focus to character
		if((m_inputBehavior & INPUT_BEHAVIOR_KEEP_FOCUS) && m_stack.back()->m_focus.get_ptr())
		{
			// force even if it was already in focus
			ResetFocus(0);
			SetFocus(m_stack.back()->m_focus.get_ptr(), 0);
		}
		else if((m_inputBehavior & INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT) == 0)
		{
			// Kill focus
			//ResetFocus(0);
		}
		m_stack.back()->GotFocus();
		m_stack.back()->m_timelineAnim = State::TIMELINE_FOCUS_IN;
	}
}

bool MenuFX::IsStateInStack( State* ptr )
{
	int size = m_stack.size();
	for(int i = 0; i < size; i++)
	{
		if( m_stack[i] == ptr)
		{
			return true;
		}
	}

	return false;
}

void MenuFX::PopAll()
{
	//just call the hide callback and remove from stack.
	while(GetCurrentDepth() > 0 ) 
	{		
		m_stack.back()->Hide();
		m_stack.back()->m_timelineAnim = State::TIMELINE_HIDE;
		m_stack.pop_back();
	}


	SetContext(m_root->m_movie.get_ptr());
}
