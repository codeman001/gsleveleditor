#pragma once
#ifndef GAMESWF_DEBUGGER_H
#define GAMESWF_DEBUGGER_H

#include "base/tu_config.h"

#if GAMESWF_ENABLE_DEBUGGER

#include "net/net_interface_tcp.h"

namespace gameswf
{
	struct player;
	struct as_object;
	struct character;
	struct membuf;
	struct bitmap_info;
	struct as_value;
	struct action_buffer;
	struct as_environment;
	struct fn_call;

	struct swd
	{
	public:
		struct mapping
		{
			mapping() :
				m_fileid(0),
				m_line(0),
				m_offset(0)
			{
			}

			Uint32 m_fileid;
			Uint32 m_line;
			Uint32 m_offset;
		};

		void load(const membuf& buf);

	//protected:
		hash<Uint32, mapping> m_mappings;
		hash<int, int> m_breakpoints;
	};

	struct debugger
	{
		struct player2debugger
		{
			enum type
			{
				MENU_STATE,
				PROP,
				EXIT,
				CREATE_ANONYMOUS_OBJECT,
				REMOVE_OBJECT,
				TRACE,
				TARGET_ERROR,
				RECURSION_DEPTH_ERROR,
				WITH_ERROR,
				PROTO_LIMIT_ERROR,
				SET_FIELD,
				DELETE_FIELD,
				MOVIE_ATTRIBUTE,
				PLACE_OBJECT,
				SWD_FILE_ENTRY,
				ASK_BREAKPOINTS,
				BREAKPOINT_HIT,
				BREAK,
				SET_LOCAL_VARS,
				BREAKPOINTS,
				NUM_SWD_FILE_ENTRIES,
				REMOVE_SWD_FILE_ENTRY,
				REMOVE_BREAKPOINT,
				NOT_SYNCED,
				URL_OPEN_ERROR,
				PROCESS_TAG,
				VERSION,
				BREAKPOINT_HIT_EX,
				SET_FIELD_2,
				SQUELCH,
				GET_FIELD,
				FUNCTION_FRAME,
				DEBUGGER_OPTION,
				WATCH,
				SWF_IMAGE,
				SWD_IMAGE,
				EXCEPTION,
				STACK_UNDERFLOW,
				DIVIDE_BY_0,
				SCRIPT_STUCK,
				SUSPEND_REASON,
				ACTIONS,
				SWF_INFO,
				CONSTANT_POOL,
				CONSOLE_ERROR,
				FUNCTION_INFO,

				// gameswf specific
				PLAYER_INFOS = 129,
				TEXTURE_CACHES,
				WIREFRAME,
				CHARACTER_SELECTION,
				TEXTURE_SELECTION,
				TEXTURES,
				SET_PROFILER,
				DUMP_PROFILER,
				FONTS,
				VERBOSE_ACTION
			};
		};

		struct debugger2player
		{
			enum type
			{
				ZOOM_IN,
				ZOOM_OUT,
				ZOOM_100,
				HOME,
				SET_QUALITY,
				PLAY,
				LOOP,
				REWIND,
				FORWARD,
				BACK,
				PRINT,
				SET_FIELD,
				SET_PROPERTY,
				END_SESSION,
				REQUEST_PROPERTIES,
				CONTINUE,
				SUSPEND,
				SET_BREAKPOINT,
				CLEAR_BREAKPOINT,
				CLEAR_ALL_BREAKPOINTS,
				STEP_OVER,
				STEP_INTO,
				STEP_OUT,
				PROCESSED_TAG,
				SET_SQUELCH,
				GET_FIELD,
				GET_FUNCTION_FRAME,
				GET_DEBUGGER_OPTION,
				SET_DEBUGGER_OPTION,
				ADD_WTACH,
				REMOVE_WTACH,
				STEP_CONTINUE,
				GET_SWF_CONTENT,
				GET_SWD_CONTENT,

				// gameswf specific
				PLAYER_INFOS = 129,
				TEXTURE_CACHES,
				WIREFRAME,
				CHARACTER_SELECTION,
				TEXTURE_SELECTION,
				TEXTURES,
				SET_PROFILER,
				DUMP_PROFILER,
				FONTS,
				VERBOSE_ACTION
			};
		};

		enum property_id
		{
			_x,
			_y,
			_xscale,
			_yscale,
			_currentframe,
			_totalframes,
			_alpha,
			_visible,
			_width,
			_height,
			_rotation,
			_target,
			_framesloaded,
			_name,
			_droptarget,
			_url,
			_highquality,
			_focusrect,
			_soundbuftime,
			_quality,
			_xmouse,
			_ymouse
		};

		enum amf_types
		{
			AMF_NUMBER,
			AMF_BOOLEAN,
			AMF_STRING,
			AMF_OBJECT,
			AMF_MOVIE_CLIP, // reserved
			AMF_NULL,
			AMF_UNDEFINED,
			AMF_REFERENCE,
			AMF_ECMA_ARRAY,
			AMF_OBJECT_END,
			AMF_STRICT_ARRAY,
			AMF_DATE,
			AMF_LONG_STRING, // unused
			AMF_UNSUPPORTED,
			AMF_RECORD_SET

		};

		enum state
		{
			STATE_CONTINUE,
			STATE_STEP_OVER,
			STATE_STEP_INTO,
			STATE_STEP_OUT,
			STATE_SUSPENDED
		};

		struct call_entry
		{
			const char* name;
			const as_environment* env;
			int local_stack_top;
		};

		struct packet_writer
		{
			packet_writer& init(int type)
			{
				m_type = type;
				m_data.clear();
				return *this;
			}

			packet_writer& write(const void* d, int len)
			{
				const char* ptr = (const char*)d;
				for(int i = 0; i < len; i++)
					m_data.push_back(ptr[i]);
				return *this;
			}

			packet_writer& write(const char* s)
			{
				write(s, strlen(s));
				write("\0", 1);
				return *this;
			}

			packet_writer& write(int i)
			{
				return write(&i, sizeof(int));
			}

			packet_writer& write(short i)
			{
				return write(&i, sizeof(short));
			}

			packet_writer& write(char i)
			{
				return write(&i, sizeof(char));
			}

			packet_writer& write(const as_value& value);

			void flush(net_socket* s)
			{
				int size = m_data.size();
				s->write(&size, sizeof(size), 1);
				s->write(&m_type, sizeof(m_type), 1);
				if(size > 0)
					s->write(&m_data[0], m_data.size(), 1);
			}

			array<char> m_data;
			int m_type;
		};

		debugger();
		~debugger();

		void connect(player* p);
		void close();

		void trace(const char* str);
		void profile_trace(const char* str);
		void update();
		void update_script(const action_buffer* ab, as_environment* env, int& pc);
		void notify_enter_scope(const char* name, as_environment* env, int local_stack_top);
		void notify_exit_scope();
		character* get_character_selection();
		bitmap_info* get_texture_selection();

	protected:
		as_object* address_to_object(int address);
		void send_properties(as_object* obj);
		void send_property(int addr, property_id id, const char* value);
		void send_property(int addr, property_id id, float value);
		void send_property(int addr, property_id id, int value);
		void send_field(int addr, const char* key, const as_value& value);

		void send_hierarchy(character* ch, tu_string& target, int depth);
		void load_file(const char* filename, membuf& buf);
		void send_file(player2debugger::type id, const char* filename);
		void send_swd(player* p);
		void send_breakpoint_hitex(int swfindex, int address);
		void send_function_frame(int depth);

		net_interface_tcp* m_tcp;
		net_socket* m_ns;
		packet_writer m_packet_writer;
		player* m_player;
		
		character* m_character_selection;
		bitmap_info* m_texture_selection;
		
		state m_state;
		int m_step_depth;
		array<call_entry> m_callstack;
		swd m_swd;
	};
};
#endif // GAMESWF_ENABLE_DEBUGGER

#endif // GAMESWF_DEBUGGER_H
