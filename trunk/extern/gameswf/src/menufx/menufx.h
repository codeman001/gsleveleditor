#pragma once
#ifndef MENUFX_H
#define MENUFX_H

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_player.h"
#include "gameswf/gameswf_root.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_dlist.h"
#include "gameswf/gameswf_sprite.h"
#include "base/tu_timer.h"
#include "base/container.h"

// RenderFX is the interface between application and gameswf (flash).
// Accessing gameswf structures directly is permitted but code can change anytime, breaking your application compilation.
// It's strongly recommended to only use RenderFX or MenuFX.

namespace glf
{
	// forward decl for glf's IOStream
	class IOStream;
};

class RenderFX
{
public:
	RenderFX();
	virtual ~RenderFX();

	enum InputBehavior
	{
		INPUT_BEHAVIOR_DEFAULT = 0,
		
		// Always keep an entity in focus, until another one receives focus.
		INPUT_BEHAVIOR_KEEP_FOCUS = 1 << 0,

		// By default, search top entity from context only. With INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT,
		// the search will be from the root.
		INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT = 1 << 2,

		// Enable current menu and disable others in the stack.
		// In combinaision with INPUT_BEHAVIOR_TOPMOST_SEARCH_ROOT, this will help usage of clickable widgets on top of all menus.
		INPUT_BEHAVIOR_ENABLE_CURRENT_MENU_ONLY = 1 << 3,

		// Set focus only if cursor is pressed (not only when cursor is over).
		INPUT_BEHAVIOR_FOCUS_ON_PRESS = 1 << 4,

		// Lock input while button is released and waiting for animation to end to trigger EVENT_CLICKED.
		INPUT_BEHAVIOR_LOCK_INPUT_UNTIL_EVENT_CLICKED = 1 << 5,

		// Keep only events supported by flash
		INPUT_BEHAVIOR_STANDARD_FLASH = 1 << 6,

		// Give focus to control when dragging over it, even if another control already has the focus
		INPUT_BEHAVIOR_DRAG_OVER_CONTROL_GIVES_FOCUS = 1 << 7,
	};

	// Events forwarded by RenderFX to application.
	enum EventType
	{
		// Character received focus.
		EVENT_GOTFOCUS,
		// Character lost focus.
		EVENT_LOSTFOCUS,
		// Invoked when 'released' or 'clicked' animation finished. This is not standard to flash but very handy to trigger action after animation ends.
		EVENT_CLICKED,
		// Cursor released on a character.
		EVENT_KEY_RELEASED,
		// Cursor pressed on a character.
		EVENT_CURSOR_PRESSED,
		// Cursor dragged on a character.
		EVENT_CURSOR_DRAGGED,
		// Cursor released on a character.
		EVENT_CURSOR_RELEASED,
		// Cursor released outside of focused character.
		EVENT_CURSOR_RELEASED_OUTSIDE,
		// Cursor entered on a character.
		EVENT_CURSOR_ROLLOVER,
		// Cursor exited from a character.
		EVENT_CURSOR_ROLLOUT,
		// Cursor draggged back on a character.
		EVENT_CURSOR_DRAGOVER,
		// Cursor draged out from a character.
		EVENT_CURSOR_DRAGOUT
	};

	static const int KEY_UP		= 1 << 0;
	static const int KEY_DOWN	= 1 << 1;
	static const int KEY_LEFT	= 1 << 2;
	static const int KEY_RIGHT	= 1 << 3;
	static const int KEY_SELECT	= 1 << 4;
	static const int KEY_BACK	= 1 << 5;

	static const int CONTROLLER_COUNT	= 4;

	struct Cursor
	{
		Cursor()
		{
			angle = x = y = 0.0f;
			buttonState = 0;
		}

		float x;
		float y;
		float angle;
		int buttonState;
	};

	struct Controller
	{
		Controller()
		{
			enabled = true;
			Reset();
		}

		void Reset()
		{
			currentActiveEntity = NULL;
			topMostEntity = NULL;
			cursorCharacter = NULL;
			clickedCharacter = NULL;
			draggingCharacter = NULL;
		}

		Cursor cursor;
		gameswf::smart_ptr<gameswf::character> currentActiveEntity;
		gameswf::smart_ptr<gameswf::character> topMostEntity;
		gameswf::smart_ptr<gameswf::character> cursorCharacter;
		gameswf::smart_ptr<gameswf::character> clickedCharacter;
		gameswf::smart_ptr<gameswf::character> draggingCharacter;
		bool enabled;
	};

	struct Event
	{
		Event(EventType type, gameswf::character* ch, int controllerID)
		{
			this->type = type;
			this->ch = ch;
			this->characterName = ch->m_name.c_str();
			this->characterPath[0] = '\0';
			this->key = 0;
			this->consumed = false;
			this->controllerID = controllerID;
		}

		void Consume()
		{
			consumed = true;
		}

		bool IsConsumed() const
		{
			return consumed;
		}

		const char* GetCharacterPath()
		{
			if(characterPath[0] == '\0')
			{
				// build character path
				gameswf::character* ch = this->ch;
				const char* stack[32];
				int depth = 0;
				while(ch)
				{
					assert(depth < 32);
					if(strlen(ch->m_name.c_str()) > 0)
						stack[depth++] = ch->m_name.c_str();
					ch = ch->get_parent();
				}
				strcpy(characterPath, "");
				while(depth > 0)
				{
					--depth;
					strcat(characterPath, stack[depth]);
					if(depth > 0)
						strcat(characterPath, ".");
				}
			}
			
			return characterPath;
		}

		gameswf::character* ch;
		const char* characterName;
		EventType type;
		Cursor cursor;
		int key;
		int controllerID;

	private:
		bool consumed;
		char characterPath[256];
	};

	class EventListener
	{
	public:
		virtual void OnEvent(Event& event) = 0;
		virtual void OnFSCommand(const char* command, const char* arg) = 0;
		virtual bool CanHandleEvent(Event& event) { return true; }
	};

	static const int COLLECT_VISIBLE	= 1<<0;
	static const int COLLECT_ENABLED	= 1<<1;
	static const int COLLECT_NAMED		= 1<<2;
	static const int COLLECT_NEED_ADVANCE= 1<<3;

	// Iterate through a set of characters from a search query
	class CharacterIterator
	{
	public:
		CharacterIterator()
		{
		}

		gameswf::character* get(int i) const
		{
			return m_collectList[i];
		}

		int size() const
		{
			return m_collectList.size();
		}

	private:
		friend class RenderFX;

		gameswf::array<gameswf::character*> m_collectList;
	};

	class SearchIndex
	{
	public:
		struct Entry
		{
			gameswf::character* ch;
			char characterPath[256];
		};
		typedef gameswf::array<Entry> EntryList;

		~SearchIndex()
		{
			Clear();
		}

		void Clear()
		{
			for(gameswf::string_hash<EntryList*>::iterator it = m_index.begin(); it != m_index.end(); ++it)
			{
				swfdelete (it.get_value());
			}
			m_index.clear();
		}
		void Init(RenderFX& renderFX);
		gameswf::character* Find(const char* characterName);

	private:
		gameswf::string_hash<EntryList*> m_index;
	};

	struct Size
	{
		Size(int width, int height) :
			width(width),
			height(height)
		{
		}

		int width;
		int height;
	};

	struct InitializationParameters
	{
		InitializationParameters() :
			videoDriver(NULL),
			textureLoaderCallback(NULL),
			fileOpenerCallback(NULL),
			glyphTextureCacheSize(0, 0),
			bitmapGlyphTextureCacheSize(0, 0),
			loadFontsInMemory(true),
			fontScale(1.0f)
		{
		}

		gameswf::video_driver* videoDriver;
		gameswf::video_texture* (*textureLoaderCallback)(const char* export_name, int width, int height);
		glf::IOStream* (*fileOpenerCallback)(const char* filename, const char* mode);
		Size glyphTextureCacheSize;
		Size bitmapGlyphTextureCacheSize;
		bool loadFontsInMemory; // true: load fonts in memory, false: always read from file
		float fontScale; // internal true-type font size mutiplicator to provide sharper text
	};

	// Initialize RenderFX and gameswf default context. Should be called only once at the beginning of application.
	static void Initialize(InitializationParameters& parameters);
	// Initialize RenderFX and gameswf default context. Should be called only once at the beginning of application.
	// Deprecated. Use Initialize(InitializationParameters& parameters) instead.
	static void Initialize(gameswf::video_driver* videoDriver);
	// Initialize the glyph texture cache to speedup rendering of glyph (text).
	// Deprecated. Use Initialize(InitializationParameters& parameters) instead.
	static void InitializeGlyphTextureCache(int width, int height);
	// Finalize RenderFX and gameswf default context. Should be called only once when gameswf is no longer necessary, ie: when application exits.
	static void Finalize();

	// Create a gameswf context.
	static gameswf::player_context* CreateContext(InitializationParameters& parameters);
	// Destroy a gameswf context. The context pointer is deleted.
	static void DestroyContext(gameswf::player_context* context);
	
	// Set rendering mode to wire frame.
	static void SetWireFrame(bool enabled);

	// Force textures to be transferred to VRAM
	// drawTextures: if true, also draw shapes to force driver to use the textures
	// context: if NULL, using default context
	static void ForceTexturesToVRAM(bool drawTextures = false, gameswf::player_context* context = NULL);
	
	// Unload all textures from VRAM. After this call, textures can't be rendered or transfered to VRAM again.
	// Useful for multiple threads: call from rendering/main thread, before calling Unload() from worker thread
	// context: if NULL, using default context
	static void UnloadTextures(gameswf::player_context* context = NULL);

	// Clear all fonts and text fields (ie: to allow swaping of fonts). Should be called for each instance of RenderFX.
	// context: if NULL, using default context
	static void ClearFonts(gameswf::player_context* context = NULL);

	// Clear glyph and bitmap texture caches.
	// context: if NULL, using default context
	static void ClearGlyphTextureCaches(gameswf::player_context* context = NULL);

	// Load a .swf flash file specified by a path. Loading can be called from a separated thread,
	// gameswf is not thread safe, so a different context should be used to render a flash while loading another one.
	// 
	virtual void Load(const char* infile, gameswf::player_context* context = NULL);
	// Unloads the flash.
	virtual void Unload();
	// Register the event lister to receive events.
	void SetEventListener(EventListener* listener);
	// Get player context
	gameswf::player_context* GetPlayerContext() const
	{
		return m_player != NULL ? m_player->m_context : NULL;
	}

	// Update timelines in milli seconds, execute action script, etc.
	virtual void Update(int dt, bool multipleUpdates = false);
	// Update a keypad input.
	void UpdateInput(int keyState, int controllerID = 0);
	// Update a cursor input.
	virtual void UpdateCursor(Cursor& cursor, int controllerID = 0);
	// Set character to be used as cursor. The position of character will be updated automatically.
	void SetCursor(const char* characterName, int controllerID = 0);
	// Manually set focus to a character.
	void SetFocus(const char* characterName, int controllerID = 0);
	// Manually clear focus.
	void ResetFocus(int controllerID = 0);
	// Get controller infos.
	const Controller& GetController(int controllerID = 0);
	// Enable/disable a controller.
	void SetControllerEnabled(int controllerID, bool enabled);
	// Modify the input behavior.
	void SetInputBehavior(int inputBehavior);
	int GetInputBehavior() const { return m_inputBehavior; }

	//character* version, make sure ptr stay valid, these method are much more faster (do not do a "Find(characterName")
	const char* GetText(gameswf::character* character);
	void SetText(gameswf::character* character, const char* text, bool html = false);
	void FormatText(gameswf::character* character, const char* format, ...);
	void FormatHTML(gameswf::character* character, const char* format, ...);
	void SetEnabled(gameswf::character* character, bool enabled);
	void SetColorTransform(gameswf::character* character, unsigned int multARGB, unsigned int addARGB);
	void SetColorFilter(gameswf::character* character, int filterIndex, unsigned int newARGB);
	bool InvokeASCallback(gameswf::character* node, const char* funcName, const gameswf::as_value* arguments = NULL, int argumentCount = 0);
	void SetFocus(gameswf::character* next, int controllerID);

	// Set character's visibility. Hidden character will not be rendered, timeline not updated and action script not executed.
	void SetVisible(const char* characterName, bool visible);
	// Set character's position, in pixels.
	void SetPosition(const char* characterName, int x, int y);	
	// Set character's enable flag, and call 'disabled' or 'activated/focus_out' animation.
	// Disabled character will not receive any event.
	void SetEnabled(const char* characterName, bool enabled);
	// Set character's alpha between 0.0f (invisible) and 1.0f (fully visible).
	void SetAlpha(const char* characterName, float alpha);
	// Set character's color transform, with 'multiplied' part and 'add' part. This is recommended to modify a text field color.
	void SetColorTransform(const char* characterName, unsigned int multARGB, unsigned int addARGB);
	// Set character's filter color. such as glow or drop shadow color.
	void SetColorFilter(const char* characterName, int filterIndex, unsigned int newARGB);
	// Set text field's text or html.
	void SetText(const char* characterName, const char* text, bool html = false);
	// Get text field's text.
	const char* GetText(const char* characterName);
	// Format text field's text using sprintf.
	void FormatText(const char* characterName, const char* format, ...);
	// Format text field's html using sprintf.
	void FormatHTML(const char* characterName, const char* format, ...);
	
	// Set member of character.
	void SetMember(const char* characterName, const char* variable, gameswf::as_value& value);
	void SetMember(const char* characterName, const char* member, const char* value);
	void SetMember(const char* characterName, const char* member, int value);
	
	// Set local variable of sprite.
	void SetLocalVariable(const char* spriteName, const char* variableName, gameswf::as_value& value);

	// Replace a character by a texture.
	void SetTexture(const char* characterName, gameswf::video_texture* texture, bool keepBounds = true);
	// Play an animation (label), equivalent to gotoAndPlay.
	void PlayAnim(const char* characterName, const char* animName, int flags = 0);
	// Play an animation (label), equivalent to gotoAndPlay/gotoAndStop.
	void GotoFrame(const char* characterName, const char* label, bool play);
	// Play an animation (label), equivalent to gotoAndPlay/gotoAndStop.
	void GotoFrame(const char* characterName, int frame, bool play);
	// Get character's timeline frame count.
	int GetFrameCount(const char* characterName);
	// Check if character's timeline reached last frame.
	bool IsAnimOver(const char* characterName);
	// Invoked when a fscommand happens.
	void FSCommand(const char* command, const char* arg);
	// Invoke an action script function call back.
	bool InvokeASCallback(const char* characterName, const char* funcName, const gameswf::as_value* arguments = NULL, int argumentCount = 0);
	// Register a display function call back what will be invoked when the character is rendered.
	// This is very handy to render customized stuff such as 3D assets or platform specific api calls.
	bool RegisterDisplayCallback(const char* characterName, void (*callback)(gameswf::render_state&, void*), void* user_ptr);
	bool RegisterDisplayCallback(gameswf::character* ch, void (*callback)(gameswf::render_state&, void*), void* user_ptr);

	// Set the viewport to render the flash. This will reset the bounds to the entire viewport.
	void SetViewport(int x, int y, int w, int h);
	// Set the bounds of the flash inside the viewport.
	void SetBounds(int x, int y, int w, int h, gameswf::scale_mode mode = gameswf::SCALE_EXACTFIT);
	// Set the orientation (rotation).
	void SetOrientation(gameswf::orientation_mode mode);

	// PreRender should be called before any drawing as it may modify frame buffer.
	void PreRender();

	// Perform actual rendering.
	void Render();

	// Call once before calling character's display().
	void BeginDisplay();

	// Call once after calling character's display().
	void EndDisplay();

	// Debug some information.
	void DBG_TraceContextInformation( gameswf::character* context );
	void TraceHierarchy( gameswf::character* node = NULL, int flags = 0, int depth = 0 );
	
	bool PlayAnim(gameswf::character* ch, const char* animName, int flags);
	void SetPosition(gameswf::character* ch, int x, int y);
	bool GotoFrame(gameswf::character* ch, const char* label, bool play);
	void GotoFrame(gameswf::character* ch, int frame, bool play);
	int GetFrameCount(gameswf::character* ch);
	bool IsAnimOver(gameswf::character* ch);

	float GetAlpha( gameswf::character* ch) const;

	// Find a character by name and return the pointer. Carefull because this pointer can become invalid.
	gameswf::character* Find(const char* characterName);
	gameswf::character* Find(const char* characterName, gameswf::character* context);
	
	// Get search index to accelerate many successive finds
	SearchIndex& GetSearchIndex();

	// Character iterator.
	CharacterIterator& FindCharacters(gameswf::character* context, const char* prefix = NULL, int flags = 0);


	gameswf::character* GetFlashRoot() const;
	gameswf::root* GetRoot() const;
	gameswf::tu_string GetFileName() const { return m_filename; }
	
	// Enable/disable text buffering to display all text at the end of all characters/movie clips, for performance. Disabled by default.
	void SetTextBufferingEnabled(bool enabled);

	// Enable/disable render caching to optimize performance. Disabled by default.
	// Note: currently, only text rendering is improved
	void SetRenderCachingEnabled(bool enabled);

	// Preload an array of glyphs to texture cache.
	// Returns number of glyph actually written.
	int PreloadGlyphs(unsigned short* codes, int count, const char* fontName, int fontSize, bool bold = false, bool italic = false, const gameswf::filter* filter = NULL);
	
	// Preload a string of glyphs to texture cache.
	// Returns number of glyph actually written.
	int PreloadGlyphs(const char* text, const char* fontName, int fontSize, bool bold = false, bool italic = false, const gameswf::filter* filter = NULL);

	// Preload glyphs based on content of all text fields to texture cache.
	// Param ch is the root context to start iteration.
	bool PreloadGlyphs(gameswf::character* context = NULL);

	void SetContext(gameswf::character* context);
	gameswf::character* GetContext();

protected:
	void SendEvent(Event& event);
	void CollectCharacters(gameswf::character* node, const char* prefix, int flags);
	gameswf::character* DepthSearch(int depth);
	
	struct DepthSearchEntry
	{
		gameswf::character* ch;
		const char* path;
	};

	struct DepthSearchData
	{
		gameswf::array<int> m_startIndexes;
		gameswf::array<DepthSearchEntry> m_container;

		void Init(gameswf::character* ch, const char* path)
		{
			m_container.clear();
			m_startIndexes.clear();
			
			AddDepth();
			AddToCurrentDepth(ch, path);
		}

		void AddDepth()
		{
			m_startIndexes.push_back(m_container.size());
		}

		void AddToCurrentDepth(gameswf::character* ch, const char* path)
		{
			DepthSearchEntry e;
			e.ch = ch;
			e.path = path;
			m_container.push_back(e);
		}
	};
	
	// collect container for find
	CharacterIterator m_characterIterator;
	SearchIndex m_searchIndex;
	DepthSearchData m_depthSearchData;

	// swf player
	gameswf::smart_ptr<gameswf::player> m_player;
	gameswf::smart_ptr<gameswf::root> m_root;
	gameswf::character* m_context;
	gameswf::tu_string m_filename;

	Controller m_controllers[CONTROLLER_COUNT];
	int m_inputBehavior;
	EventListener* m_listener;

	static gameswf::player_context* s_default_context;

public:
	inline bool IsInputBehavior(InputBehavior behavior) const
	{
		return (m_inputBehavior & behavior) != 0;
	}
};

class LoadingFX : public RenderFX, public RenderFX::EventListener
{
public:
	LoadingFX() :
		m_loadingContext(NULL)
	{
		m_loadingFXParameters.glyphTextureCacheSize = RenderFX::Size(256, 256);
		m_loadingFXParameters.bitmapGlyphTextureCacheSize = RenderFX::Size(256, 256);
	}

	~LoadingFX()
	{
		Unload();
	}

	void SetInitializationParameters(const RenderFX::InitializationParameters& params)
	{
		m_loadingFXParameters = params;
	}

	// RenderFX override
	virtual void Load(const char* infile, gameswf::player_context* context = NULL)
	{
		assert(m_loadingContext == NULL);
		m_loadingContext = RenderFX::CreateContext(m_loadingFXParameters);
		RenderFX::Load(infile, m_loadingContext);
	}
	virtual void Unload()
	{
		RenderFX::Unload();

		if(m_loadingContext != NULL)
		{
			RenderFX::DestroyContext(m_loadingContext);
			m_loadingContext = NULL;
		}
	}

	// EventListener override
	virtual void OnEvent(Event& event) {}
	virtual void OnFSCommand(const char* command, const char* arg) {}
	virtual bool CanHandleEvent(Event& event) { return true; }

protected:
	gameswf::player_context* m_loadingContext;
	RenderFX::InitializationParameters m_loadingFXParameters;
};

class MenuFX : public RenderFX, public RenderFX::EventListener
{
public:
	class State
	{
	public:
		friend class MenuFX;

		enum TimelineAnim
		{
			TIMELINE_UNKNOWN,
			TIMELINE_SHOW,
			TIMELINE_HIDE,
			TIMELINE_FOCUS_IN,
			TIMELINE_FOCUS_OUT,
		};

		State(const char* name)
		{
			strcpy(m_name, name);
			m_character = NULL;
			m_focus = NULL;
			m_timelineAnim = TIMELINE_UNKNOWN;
		}
		virtual ~State() 
		{

		}

		virtual void Create() {}
		virtual void Show() {}
		virtual void Hide() {}
		virtual void GotFocus() {}
		virtual void LostFocus() {}
		virtual void Update() {}
		virtual void Update(int dt) { Update(); }
		virtual void UpdateBackground() {}
		virtual void UpdateBackground(int dt) { UpdateBackground(); }
		virtual void OnEvent(Event& event) {};
		virtual void OnFSCommand(const char* command, const char* arg) {};
		virtual bool CanHandleEvent(Event& event) { return true; }

		const char* GetName() const
		{
			return m_name;
		}

		gameswf::character* GetCharacter() const
		{
			return m_character.get_ptr();
		}

		MenuFX* m_menuFX;
		char m_name[64];
		gameswf::weak_ptr<gameswf::character> m_character;
		gameswf::weak_ptr<gameswf::character> m_focus;
		TimelineAnim m_timelineAnim;
	};

	MenuFX();
	~MenuFX();

	// EventListener override
	virtual void OnEvent(Event& event);
	virtual void OnFSCommand(const char* command, const char* arg);
	virtual bool CanHandleEvent(Event& event);

	// RenderFX override
	virtual void Unload();
	virtual void SetFocusDefault();
	virtual void Update(int dt, bool multipleUpdates = false);

	// Menu Stack
	void RegisterStates(State** states, int count);
	void RegisterState(State* state, const char* characterName = NULL);
	State* GetState(const char* name);
	State* GetStateHandler(gameswf::character* ch);
	State* GetCurrentState();
	int GetCurrentDepth() const { return m_stack.size(); }

	virtual void PushMenu(const char* name);
	virtual void SwitchMenu(const char* name);
	virtual void PopMenu();
	
	void PopAll();

	bool IsStateInStack( State* ptr );
protected:
	void SetMenu(const char* name, bool push);
public:
	// menu stack
	gameswf::array<State*> m_states;
	gameswf::array<State*> m_stack;
};

#endif
