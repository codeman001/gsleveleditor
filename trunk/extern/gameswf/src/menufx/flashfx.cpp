#include "stdafx.h"
#include "menufx/flashfx.h"

namespace gameswf
{
	//--------------------------------------------------------------------------
	// Helper functions
	//--------------------------------------------------------------------------

	inline FlashFX* GetFlashFX(const fn_call& fn)
	{
		return static_cast<FlashFX*>(fn.get_player()->m_userdata);
	}

	//--------------------------------------------------------------------------
	// Native action script sound functions
	//--------------------------------------------------------------------------

	void NativePlaySound(const fn_call& fn)
	{
		FlashFX::SoundEvent event;
		fn_call_iterator it(fn);
		if(it.has_next()) event.name = it.next().to_string();
		if(it.has_next()) event.volume = (float)it.next().to_number();

		fn.result->set_bool(GetFlashFX(fn)->StartSound(event));
	}

	void NativeStopSound(const fn_call& fn)
	{
		FlashFX::SoundEvent event;
		fn_call_iterator it(fn);
		if(it.has_next()) event.name = it.next().to_string();

		fn.result->set_bool(GetFlashFX(fn)->StopSound(event));
	}

	//---------------------------------------------------------------------------
	// Native action script string functions
	//---------------------------------------------------------------------------

	void NativeSetLanguage(const fn_call& fn)
	{
		fn.result->set_bool(false);

		if(fn.nargs == 1)
		{
			const char* lang = fn.arg(0).to_string();
			FlashFX* fx = GetFlashFX(fn);
			if(fx->SetLanguage(lang))
			{
				as_value args[] = {as_value(swfnew as_string(lang))};
				RenderFX::CharacterIterator& it = fx->FindCharacters(fx->GetFlashRoot());
				for(int i = 0; i < it.size(); i++)
				{
					if(it.get(i)->is(gameswf::AS_SPRITE))
						fx->InvokeASCallback(it.get(i), "onChangeLanguage", args, 1);
				}
				fn.result->set_bool(true);
			}
		}
	}

	void NativeGetString(const fn_call& fn)
	{
		const char* table = NULL;
		const char* id = NULL;
		fn_call_iterator it(fn);
		if(fn.nargs == 1)
		{
			id = it.next().to_string();
		}
		else if(fn.nargs == 2)
		{
			table = it.next().to_string();
			id = it.next().to_string();
		}

		const char* str = GetFlashFX(fn)->GetString(table, id);
		fn.result->set_string(swfnew as_string(str));
	}

	void NativePreloadGlyphs(const fn_call& fn)
	{
		character* context = NULL;
		if(fn.nargs == 1)
		{
			context = cast_to<character>(fn.arg(0).to_object());
		}

		GetFlashFX(fn)->PreloadGlyphs(context);
		fn.result->set_bool(true);
	}

	//---------------------------------------------------------------------------
	// Native action script controller functions
	//---------------------------------------------------------------------------

	void NativeGetKeyState(const fn_call& fn)
	{
		FlashFX::KeyState state;
		fn_call_iterator it(fn);
		if(it.has_next()) state.controller = it.next().to_int();
		if(it.has_next()) state.key = it.next().to_int();

		fn.result->set_int(GetFlashFX(fn)->GetKeyState(state));
	}

	void NativeGetCursorState(const fn_call& fn)
	{
		int controller = 0;

		if(fn.nargs == 1)
		{
			controller = fn.arg(0).to_int();
		}

		FlashFX* fx = GetFlashFX(fn);
		fn.result->set_as_object(GetFlashFX(fn)->GetCursorState(controller));
	}

	//---------------------------------------------------------------------------
	// Native action script data functions
	//---------------------------------------------------------------------------

	void NativeGetGameData(const fn_call& fn)
	{
		const char* desc = NULL;
		FlashFX::Data data;
		fn_call_iterator it(fn);
		if(it.has_next()) desc = it.next().to_string();
		if(it.has_next()) data.obj = it.next();

		fn.result->set_bool(GetFlashFX(fn)->GetGameData(desc, data));
	}

	void NativeSetGameData(const fn_call& fn)
	{
		const char* desc = NULL;
		FlashFX::Data data;
		fn_call_iterator it(fn);
		if(it.has_next()) desc = it.next().to_string();
		if(it.has_next()) data.obj = it.next();

		fn.result->set_bool(GetFlashFX(fn)->SetGameData(desc, data));
	}

	//---------------------------------------------------------------------------
	// Init
	//---------------------------------------------------------------------------

	FlashFX::FlashFX() :
		m_stringX(NULL),
		m_stringY(NULL),
		m_stringState(NULL)
	{
		SetEventListener(this);
		SetInputBehavior(INPUT_BEHAVIOR_STANDARD_FLASH);
		// disable inputs from other cursors by default
		SetControllerEnabled(1, false);
		SetControllerEnabled(2, false);
		SetControllerEnabled(3, false);

		register_as_native_function("playSound", NativePlaySound);
		register_as_native_function("stopSound", NativeStopSound);
		register_as_native_function("getString", NativeGetString);
		register_as_native_function("setLanguage", NativeSetLanguage);
		register_as_native_function("preloadGlyphs", NativePreloadGlyphs);
		register_as_native_function("getKeyState", NativeGetKeyState);
		register_as_native_function("getCursorState", NativeGetCursorState);
		register_as_native_function("getGameData", NativeGetGameData);
		register_as_native_function("setGameData", NativeSetGameData);
	}

	//---------------------------------------------------------------------------
	// C++ -> Flash functions
	//---------------------------------------------------------------------------

	void FlashFX::SendKeyState(const KeyState& state)
	{
		as_value args[] = {as_value(state.controller), as_value(state.key), as_value(state.state)};
		InvokeASCallback(GetFlashRoot(), "onReceiveKeyState", args, 3);
	}
	
	void FlashFX::SendControllerState(const ControllerState& state)
	{
		as_value args[] = {as_value(state.controller), as_value(state.enabled)};
		InvokeASCallback(GetFlashRoot(), "onReceiveControllerState", args, 2);
	}

	//-----------------------------------
	// RenderFX override
	//-----------------------------------

	void FlashFX::Unload()
	{
		m_stringX = m_stringY = m_stringState = NULL;
		m_cursors.clear();
		m_cursors.release_buffer();

		RenderFX::Unload();
	}

	void FlashFX::UpdateCursor(Cursor& cursor, int controllerID)
	{
		const Cursor& internalCursor = m_controllers[controllerID].cursor;
		int oldState = internalCursor.buttonState;

		RenderFX::UpdateCursor(cursor, controllerID);

		// notify cursor state changed
		if(internalCursor.buttonState != oldState)
		{
			as_value args[] = {as_value(controllerID), as_value(internalCursor.buttonState)};
			InvokeASCallback(GetFlashRoot(), "onReceiveCursorState", args, 2);
		}
	}

	as_object* FlashFX::GetCursorState(int controller)
	{
		if(m_cursors.size() == 0)
		{
			for(int i = 0; i < CONTROLLER_COUNT; i++)
				m_cursors.push_back(swfnew as_object(m_player.get_ptr()));

			m_stringX = &m_player->get_const_string("x").to_tu_stringi();
			m_stringY = &m_player->get_const_string("y").to_tu_stringi();
			m_stringState = &m_player->get_const_string("state").to_tu_stringi();
		}

		gameswf::point p(m_controllers[controller].cursor.x, m_controllers[controller].cursor.y);
		m_root->screen_to_logical(p);

		as_object* cursor = m_cursors[controller].get_ptr();
		cursor->set_member(*m_stringX, p.m_x);
		cursor->set_member(*m_stringY, p.m_y);
		cursor->set_member(*m_stringState, m_controllers[controller].cursor.buttonState);

		return cursor;
	}

}; // namespace gameswf