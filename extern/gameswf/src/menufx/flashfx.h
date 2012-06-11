#pragma once
#ifndef GAMESWF_NATIVE_LIBRARY_H
#define GAMESWF_NATIVE_LIBRARY_H

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_value.h"
#include "gameswf/gameswf_function.h"
#include "menufx/menufx.h"

namespace gameswf
{
	// FlashFX is intended to be used with flash library (see: /media/flash_library)
	// Usage: extend FlashFX and override virtual functions.

	class FlashFX : public RenderFX, public RenderFX::EventListener
	{
	public:
		//---------------------------------------------------------------------------
		// Helper structs to be used for function arguments that can be extended
		// in the future, without affecting function definition.
		//---------------------------------------------------------------------------

		struct ControllerState
		{
			ControllerState() : controller(0), enabled(true) {}
			int controller;
			bool enabled;
		};

		struct KeyState
		{
			// PS3-Specific Buttons
			static const int PS3_CROSS		= 0x01;
			static const int PS3_CIRCLE		= 0x02;
			static const int PS3_SQUARE		= 0x03;
			static const int PS3_TRIANGLE	= 0x04;

			// XBOX-Specific Buttons
			static const int XBOX_A			= 0x01;
			static const int XBOX_B			= 0x02;
			static const int XBOX_X			= 0x03;
			static const int XBOX_Y			= 0x04;

			// Wii-Specific Buttons
			static const int WII_A			= 0x01;
			static const int WII_B			= 0x02;
			static const int WII_1			= 0x03;
			static const int WII_2			= 0x04;

			// Generic Buttons
			static const int BTN_A			= 0x01;
			static const int BTN_B			= 0x02;
			static const int BTN_C			= 0x03;
			static const int BTN_D			= 0x04;

				// Analog-Left
			static const int LANALOG_CLICK	= 0x10;
			static const int LANALOG_ANGLE	= 0x11;
			static const int LANALOG_AMOUN	= 0x12;

			// Analog-Right
			static const int RANALOG_CLICK	= 0x20;
			static const int RANALOG_ANGLE	= 0x21;
			static const int RANALOG_AMOUN	= 0x22;

			// Buttons
			static const int L1				= 0x30;
			static const int L2				= 0x31;
			static const int R1				= 0x32;
			static const int R2				= 0x33;
			static const int SELECT			= 0x34;
			static const int START			= 0x35;

			// DPad
			static const int DPAD_UP		= 0x40;
			static const int DPAD_DOWN		= 0x41;
			static const int DPAD_LEFT		= 0x42;
			static const int DPAD_RIGHT		= 0x43;

			KeyState() : controller(0), key(0), state(0) {}
			int controller;
			int key;
			int state;
		};

		struct SoundEvent
		{
			SoundEvent() : name(0), volume(1.0f) {}

			const char* name;
			float volume;
		};

		struct Data
		{
			struct Item
			{
				const char* name;
				as_value val;
				Data* data;

				void operator= (const as_value& val)
				{
					if(data->obj.is_object())
						data->obj.to_object()->set_member(name, val);
				}

				as_value* operator->()
				{
					data->obj.get_member(name, &val);
					return &val;
				}
			};

			Item operator[] (const char* name)
			{
				Item item;
				item.name = name;
				item.data = this;
				return item;
			};

			as_value obj;
		};

		FlashFX();

		//-------------------------
		// C++ -> Flash
		//-------------------------

		void SendKeyState(const KeyState& state);
		void SendControllerState(const ControllerState& state);

		//-------------------------
		// Flash -> C++
		//-------------------------

		// playSound(name:String, [volume:Number]):Boolean
		virtual bool StartSound(const SoundEvent& event) { return false; }
		// stopSound([name:String]):Boolean
		virtual bool StopSound(const SoundEvent& event) { return false; }
		
		// getString(table:String, id:String):String
		virtual const char* GetString(const char* table, const char* id) { return NULL; }
		// setLanguage(lang:String):Boolean
		// lang: en, fr, it...
		virtual bool SetLanguage(const char* lang) { return false; }
		
		// getKeyState(controller:Number, key:String):String
		virtual int GetKeyState(const KeyState& state) { return 0; }

		// getCursorState(controller:Number):String
		virtual as_object* GetCursorState(int controller);
		
		// getGameData(desc:String, obj:Object):Boolean
		// Serializes data to action script.
		// Example:
		// var $sound:Object = new Object();
		// getGameData("sound", $sound);
		// trace($sound.musicLevel);
		// trace($sound.fxLevel);
		virtual bool GetGameData(const char* desc, Data& data) { return false; }

		// setGameData(desc:String, obj:Object):Boolean
		// Receive data from action script.
		// Example:
		// var $sound:Object = {musicLevel:75, fxLevel:50};
		// setGameData("sound", $sound);
		virtual bool SetGameData(const char* desc, Data& data) { return false; }

		//-----------------------------------
		// RenderFX::EventListener override
		//-----------------------------------

		virtual void OnEvent(Event& event) {}
		virtual void OnFSCommand(const char* command, const char* arg) {}
		virtual bool CanHandleEvent(Event& event) { return true; }

		//-----------------------------------
		// RenderFX override
		//-----------------------------------

		// Unloads the flash.
		virtual void Unload();

		// Update a cursor input.
		virtual void UpdateCursor(Cursor& cursor, int controllerID = 0);

	private:
		const tu_stringi* m_stringX;
		const tu_stringi* m_stringY;
		const tu_stringi* m_stringState;
		array<smart_ptr<as_object> > m_cursors;
	};

}; // namespace gameswf

#endif // GAMESWF_NATIVE_LIBRARY_H