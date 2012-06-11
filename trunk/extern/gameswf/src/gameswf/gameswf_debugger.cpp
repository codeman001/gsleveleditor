#include "stdafx.h"
#include "gameswf_debugger.h"
#include "gameswf_log.h"
#include "gameswf_player.h"
#include "gameswf_root.h"
#include "gameswf_sprite.h"
#include "gameswf_stream.h"
#include "gameswf_freetype.h"
#include "gameswf_bitmapfont.h"
#include "gameswf_action.h"
#include "gameswf_as_classes/as_array.h"
#include "base/tu_file.h"
#include "base/membuf.h"
#include "menufx/menufx.h"

#if GAMESWF_ENABLE_DEBUGGER

namespace gameswf
{
	//-----------
	// source
	//-----------

	void swd::load(const membuf& buf)
	{
		m_mappings.clear();

		assert(buf.size() > 3);

		// http://osflash.org/ofd
		if(memcmp((const char*)buf.data(), "FWD", 3) != 0)
			return;

		const char* data = ((const char*)buf.data()) + 3;
		tu_file file(tu_file::memory_buffer, buf.size(), (char*)data);
		Uint8 version = file.read_byte();

		// read tags
		while(!file.get_eof())
		{
			stream in(&file, false);
			Uint32 id = file.read_le32();
			switch(id)
			{
			case 0: // source
				{
					file.read_le32();
					file.read_le32();
					tu_string temp;
					in.read_string(&temp);
					in.read_string(&temp);
				}
				break;

			case 1: // mapping
				{
					mapping m;
					m.m_fileid = file.read_le32();
					m.m_line = file.read_le32();
					m.m_offset = file.read_le32();
					m_mappings.set(m.m_offset, m);
					
					//log_msg("fileIndex=%d\n", fileIndex);
					//log_msg("line=%d\n", mapping.m_line);
					//log_msg("offset=%d\n", mapping.m_offset);
					//log_msg("\n");
				}
				break;

			case 3: // timestamp
				{
					Uint8 timestamp[16];
					file.read_bytes(timestamp, 16);
				}
				break;

			default: // invalid
				//assert(false);
				//return;
				break;
			}
		}
	}

	debugger::packet_writer& debugger::packet_writer::write(const as_value& value)
	{
		// http://opensource.adobe.com/wiki/download/attachments/1114283/amf0_spec_121207.pdf
		int flags = 0;

		//if(value.is_undefined())
		//{
		//	write((short)AMF_UNDEFINED);
		//	write(flags);
		//}
		//else if(value.is_null())
		//{
		//	write((short)AMF_NULL);
		//	write(flags);
		//}
		//else if(value.is_bool())
		//{
		//	write((short)AMF_BOOLEAN);
		//	write(flags);
		//	write(value.to_bool() ? (char)1 : 0);
		//}
		//else if(value.is_number())
		//{
		//	write((short)AMF_NUMBER);
		//	write(flags);
		//	write(.to_string(buffer).c_str());
		//}
		if(value.is_object())
		{
			write((short)AMF_OBJECT);
			write(flags);
			write((int)value.to_object());
		}
		else
		{
			tu_string buffer;
			write((short)AMF_STRING);
			write(flags);
			write(value.to_string(buffer).c_str());
		}
		return *this;
	}

	//-----------
	// debugger
	//-----------

	debugger::debugger() :
		m_ns(NULL),
		m_player(NULL),
		m_character_selection(NULL),
		m_texture_selection(NULL),
		m_state(STATE_CONTINUE),
		m_step_depth(0)
	{
	}

	debugger::~debugger()
	{
		close();
	}

	void debugger::connect(player* p)
	{
		net_init();

		assert(m_player == NULL);
		m_player = p;
		m_tcp = swfnew net_interface_tcp();
		m_ns = m_tcp->connect("localhost", 7935);

		if(m_ns != NULL)
		{
			m_packet_writer.init(player2debugger::VERSION).write(8).flush(m_ns);
			m_packet_writer.init(player2debugger::MOVIE_ATTRIBUTE).write("movie").write("actionscript3.swf").flush(m_ns);
			//m_packet_writer.init(player2debugger::MOVIE_ATTRIBUTE).write("password").write("abc").flush(m_ns);
		}
		else
		{
			log_msg("failed to connect to remote debugger\n");
		}
	}
	
	void debugger::close()
	{
		if(m_ns)
			swfdelete (m_ns);
		m_ns = NULL;

		if(m_tcp)
			swfdelete (m_tcp);
		m_tcp = NULL;

		m_player = NULL;
	}

	as_object* debugger::address_to_object(int address)
	{
		if(address != 0 && m_player != NULL)
		{
			for(int i = 0; i < m_player->m_heap.size(); i++)
			{
				if((int)m_player->m_heap[i].get_ptr() == address)
					return (as_object*)address;
			}
		}
		return NULL;
	}

	void debugger::trace(const char* str)
	{
		if(m_ns != NULL)
		{
			m_packet_writer.init(player2debugger::TRACE);
			m_packet_writer.write(str);
			m_packet_writer.flush(m_ns);
		}
	}

	void debugger::profile_trace(const char* str)
	{
		if(m_ns != NULL)
		{
			m_packet_writer.init(player2debugger::DUMP_PROFILER);
			m_packet_writer.write(str);
			m_packet_writer.flush(m_ns);
		}
	}

	const static float DEFAULT_TIMEOUT = 1.0f; // 5 sec

	void debugger::update()
	{
		// http://osflash.org/ofd

		if(m_ns == NULL || m_player == NULL)
			return;

		char stack_buffer[256];

		while(m_ns->is_readable())
		{
			int len = 0;
			int id = 0;
			int count = 0;
			count = m_ns->read(&len, sizeof(len), DEFAULT_TIMEOUT);
			assert(count == sizeof(len));

			count = m_ns->read(&id, sizeof(id), DEFAULT_TIMEOUT);
			assert(count == sizeof(id));

			//printf("receive id=%d len=%d\n", id, len);

			char* data = len > 0 ? stack_buffer : NULL;
			if(len > 0)
			{
				if(len > sizeof(stack_buffer))
					data = swfnew char[len];
				count = m_ns->read(data, len, DEFAULT_TIMEOUT);
				assert(count == len);
			}

			switch(id)
			{
				case debugger2player::SET_PROPERTY: // set property
				{
					int addr = *(int*)data;
					short prop = *(short*)(data + 4);
					const char* type = (data + 6);
					const char* val = type + strlen(type) + 1;
					as_value value(val);
					if(strcmp(type, "number") == 0)
						value.set_double(value.to_number());
					else if(strcmp(type, "boolean") == 0)
						value.set_bool(value.to_bool());
					else if(strcmp(type, "null") == 0)
						value.set_null();

					character* ch = (character*)addr;
					switch(prop)
					{
						case _x: ch->set_member("_x", value); break;
						case _y: ch->set_member("_y", value); break;
						case _xscale: ch->set_member("_xscale", value); break;
						case _yscale: ch->set_member("_yscale", value); break;
						case _alpha: value *= 100.0f; ch->set_member("_alpha", value); break;
						case _visible: ch->set_member("_visible", value); break;
						case _width: ch->set_member("_width", value); break;
						case _height: ch->set_member("_height", value); break;
						case _rotation: ch->set_member("_rotation", value); break;
						case _name: ch->set_member("_name", value); break;
					}

					// send back all properties
					send_properties(ch);
					break;
				}

				case debugger2player::REQUEST_PROPERTIES: // request properties 
				{
					int addr = *(int*)data;
					if(addr == 0)
					{
						// character hierarchy
						tu_string level0("_level0");
						send_hierarchy(m_player->get_root()->get_root_movie(), level0, 0);
						
						// globals
						m_packet_writer.init(player2debugger::PLACE_OBJECT).write((int)m_player->m_global.get_ptr()).write("globals").flush(m_ns);
					}
					else
					{
						as_object* obj = address_to_object(addr);
						if(obj != NULL)
							send_properties(obj);
					}
					
					//send_swd(p);
					//send_file(player2debugger::SWF_IMAGE, "actionscript3.swf");
					//send_file(player2debugger::SWD_IMAGE, "actionscript3.swd");

					//m_packet_writer.init(player2debugger::ASK_BREAKPOINTS).flush(m_ns);
					//m_packet_writer.init(player2debugger::BREAKPOINT_HIT).write((short)1).write((short)4).write(1067).write("myFunc").flush(m_ns);

					//m_packet_writer.init(player2debugger::CREATE_ANONYMOUS_OBJECT).write("movie").write("actionscript3.swf").flush(m_ns);
					
					//sendSWD();

					//{
					//	ActionScriptSource src;
					//	src.Load("./data/actionscript3.swd");
					//	short fileID = 26500;
					//	short line = 20;
					//	int addr = (int)this;
					//	const char* func = "";
					//	m_packet_writer.init(player2debugger::BREAKPOINT_HIT).write(fileID).write(line).write(addr).write(func).flush(m_ns);
					//}
					break;
				}

				case debugger2player::CONTINUE:
				{
					m_state = STATE_CONTINUE;
					break;
				}

				case debugger2player::SUSPEND:
				{
					m_state = STATE_SUSPENDED;
					break;
				}

				case debugger2player::STEP_INTO:
				{
					m_state = STATE_STEP_INTO;
					m_step_depth = m_callstack.size();
					break;
				}

				case debugger2player::STEP_OVER:
				{
					m_state = STATE_STEP_OVER;
					m_step_depth = m_callstack.size();
					break;
				}

				case debugger2player::STEP_OUT:
				{
					m_state = STATE_STEP_OUT;
					m_step_depth = m_callstack.size();
					break;
				}

				case debugger2player::SET_BREAKPOINT:
				case debugger2player::CLEAR_BREAKPOINT:
				{
					int file_id = *(int*)data;
					int offset = *(int*)(data + 4);
					int bp = 1;

					if(id == debugger2player::SET_BREAKPOINT)
						m_swd.m_breakpoints.set(offset, bp);
					else if(id == debugger2player::CLEAR_BREAKPOINT)
						m_swd.m_breakpoints.erase(offset);
					break;
				}

				case debugger2player::CLEAR_ALL_BREAKPOINTS:
				{
					m_swd.m_breakpoints.clear();
					break;
				}

				case debugger2player::GET_SWD_CONTENT:
				{
					int swf_index = *(int*)data;
					if(m_player->m_current_root->m_def->m_file_id == swf_index)
					{
						bool has_debug = false;
						for(int i = 0; i < 16; i++)
						{
							if(m_player->m_current_root->m_def->m_debug_id[i] != 0)
								has_debug = true;
						}

						if(!has_debug)
							break;

						tu_string path =((RenderFX*)m_player->m_userdata)->GetFileName();
						path[path.length() - 1] = 'd';
						membuf buf;
						load_file(path.c_str(), buf);

						if(buf.size() > 0)
						{
							// locally parse swd only for line offsets
							m_swd.load(buf);

							m_packet_writer.init(player2debugger::SWD_IMAGE);
							m_packet_writer.write(buf.data(), buf.size());
							m_packet_writer.flush(m_ns);
						}
						break;
					}
					break;
				}

				case debugger2player::GET_FUNCTION_FRAME:
				{
					int depth = *(int*)data;
					send_function_frame(depth);
					break;
				}

				case debugger2player::PLAYER_INFOS:
				{
					m_packet_writer.init(player2debugger::PLAYER_INFOS);
					m_packet_writer.write((int)m_player); // address
					m_packet_writer.write(((RenderFX*)m_player->m_userdata)->GetFileName().c_str()); // filename
					m_packet_writer.write(m_player->m_current_root->m_def->m_file_id); // file id
					m_packet_writer.write(m_player->m_current_root->m_def->m_debug_id, 16); // debug id
					m_packet_writer.flush(m_ns);
					break;
				}

				case debugger2player::TEXTURE_CACHES:
				{
					m_packet_writer.init(player2debugger::TEXTURE_CACHES);
					player_context* context = m_player->m_context;
					texture_cache* caches[2] = {NULL};
					int count = 0;
					
					if(context->m_glyph_provider != NULL && context->m_glyph_provider->get_texture_cache() != NULL)
						caches[count++] = context->m_glyph_provider->get_texture_cache();

					if(context->m_bitmap_glyph_provider != NULL && context->m_bitmap_glyph_provider->get_texture_cache() != NULL)
						caches[count++] = context->m_bitmap_glyph_provider->get_texture_cache();

					m_packet_writer.write(count);
					for(int i = 0; i < count; i++)
					{
						bitmap_info* bitmap = caches[i]->get_bitmap_info();
						m_packet_writer.write(caches[i]->get_bpp());
						m_packet_writer.write(bitmap->get_width());
						m_packet_writer.write(bitmap->get_height());
						void* pixels = bitmap->lock();
						m_packet_writer.write(pixels, bitmap->get_width() * bitmap->get_height() * caches[i]->get_bpp());
						bitmap->unlock();
					}
					m_packet_writer.flush(m_ns);
					break;
				}

				case debugger2player::WIREFRAME:
				{
					int wireframe = *(int*)data;
					get_render_handler()->set_wire_frame(wireframe != 0);
					break;
				}

				case debugger2player::CHARACTER_SELECTION:
				{
					int address = *(int*)data;
					as_object* obj = address_to_object(address);
					m_character_selection = cast_to<character>(obj);
					break;
				}

				case debugger2player::TEXTURE_SELECTION:
				{
					int address = *(int*)data;
					m_texture_selection = (bitmap_info*)address;
					break;
				}

				case debugger2player::TEXTURES:
				{
					m_packet_writer.init(player2debugger::TEXTURES);
					array<smart_ptr<bitmap_info> >& list = m_player->get_root()->m_def->m_bitmap_list;
					m_packet_writer.write(list.size());
					for(int i = 0; i< list.size(); i++)
					{
						m_packet_writer.write((int)list[i].get_ptr());
						m_packet_writer.write("");
						m_packet_writer.write(4); // bpp
						m_packet_writer.write(list[i]->get_internal_width());
						m_packet_writer.write(list[i]->get_internal_height());
						m_packet_writer.write(list[i]->is_layout());
					}
					m_packet_writer.flush(m_ns);
					break;
				}

				case debugger2player::SET_PROFILER:
				{
					int enabled = *(int*)data;
					profiler::set_enabled(enabled != 0);
					break;
				}

				case debugger2player::FONTS:
				{
					player_context* context = m_player->m_context;
					if(context->m_glyph_provider != NULL)
					{
						m_packet_writer.init(player2debugger::FONTS);
						const string_hash<smart_ptr<face_entity> >& list = context->m_glyph_provider->get_face_entities();
						m_packet_writer.write(list.size());
						for(string_hash<smart_ptr<face_entity> >::const_iterator it = list.begin(); it != list.end(); ++it)
						{
							face_entity* fe = it.get_value().get_ptr();
							m_packet_writer.write(it.get_key().c_str());
							m_packet_writer.write(fe->m_filename.c_str());
							m_packet_writer.write(fe->m_fontdata != NULL ? fe->m_fontdata->size() : 0);
							m_packet_writer.write(fe->m_ge.size());
						}
						m_packet_writer.flush(m_ns);
					}

					break;
				}

				case debugger2player::VERBOSE_ACTION:
				{
					int enabled = *(int*)data;
					set_verbose_action(enabled != 0);
					break;
				}
			}

			if(data != NULL && data != stack_buffer)
				swfdelete_primitives (data, len);
		}
	}

	void debugger::update_script(const action_buffer* ab, as_environment* env, int& pc)
	{
		if(m_ns == NULL || m_player == NULL)
			return;

		const int offset = ab->get_stream_position() + pc;

		if(m_swd.m_breakpoints.size() > 0)
		{
			int bp;
			if(m_swd.m_breakpoints.get(offset, &bp))
				m_state = STATE_SUSPENDED;
		}

		swd::mapping m;
		switch(m_state)
		{
		case STATE_STEP_INTO:
			if(m_swd.m_mappings.get(offset, &m))
				m_state = STATE_SUSPENDED;
			break;

		case STATE_STEP_OVER:
			if(m_callstack.size() == m_step_depth && m_swd.m_mappings.get(offset, &m))
				m_state = STATE_SUSPENDED;
			break;

		case STATE_STEP_OUT:
			if(m_callstack.size() < m_step_depth && m_swd.m_mappings.get(offset, &m))
				m_state = STATE_SUSPENDED;
			break;
		}

		if(m_state == STATE_SUSPENDED)
		{
			send_breakpoint_hitex(m_player->m_current_root->m_def->m_file_id, offset);
			while(m_state == STATE_SUSPENDED)
			{
				update();
				tu_timer::sleep(5);
			}
		}
	}

	void debugger::notify_enter_scope(const char* name, as_environment* env, int local_stack_top)
	{
		call_entry entry;
		entry.name = name;
		entry.env = env;
		entry.local_stack_top = local_stack_top;
		m_callstack.push_back(entry);
	}
	
	void debugger::notify_exit_scope()
	{
		m_callstack.pop_back();
	}

	void debugger::send_properties(as_object* obj)
	{
		int addr = (int)obj;
		character* ch = cast_to<character>(obj);
		if(ch != NULL)
		{
			sprite_instance* sprite = cast_to<sprite_instance>(obj);
			if(sprite != NULL)
			{
				send_property(addr, _currentframe, sprite->get_current_frame());
				send_property(addr, _totalframes, sprite->get_frame_count());
				send_property(addr, _framesloaded, sprite->get_frame_count());
			}

			send_property(addr, _x, TWIPS_TO_PIXELS(ch->get_matrix().get_raw_x()));
			send_property(addr, _y, TWIPS_TO_PIXELS(ch->get_matrix().get_raw_y()));
			send_property(addr, _xscale, ch->get_matrix().m_[0][0]);
			send_property(addr, _yscale, ch->get_matrix().m_[1][1]);
			send_property(addr, _alpha, ch->get_cxform().m_[3][0]);
			send_property(addr, _visible, ch->get_visible() ? "true" : "false");
			send_property(addr, _width, TWIPS_TO_PIXELS(ch->get_width()));
			send_property(addr, _height, TWIPS_TO_PIXELS(ch->get_height()));
			send_property(addr, _rotation, ch->get_matrix().get_rotation());
			as_value target;
			ch->get_member("_target", &target);
			send_property(addr, _target, target.to_string());
			send_property(addr, _name, ch->get_name().c_str());
			//send_property(addr, _droptarget, "");
			//send_property(addr, _url, "actionscript3.swf");
			//send_property(addr, _highquality, "1");
			//send_property(addr, _focusrect, "false");
			//send_property(addr, _soundbuftime, "0");
			//send_property(addr, _quality, "BEST");
			//send_property(addr, _xmouse, "0");
			//send_property(addr, _ymouse, "0");
		}

		//as_environment env(m_player);
		//obj->enumerate(&env);
		//as_value value;
		// for(int i = 0; i < env.m_stack.size(); i++)
		for(as_member_hash::const_iterator it = obj->m_members.begin(); it != obj->m_members.end(); ++it)
		{
			const stringi_pointer& key = it.get_key();
			const as_value& value = it.get_value();

			//tu_string buffer;
			//tu_string key = env.m_stack[i].to_string(buffer);
			//obj->get_member(key.to_tu_stringi(), &value);

			send_field(addr, key.c_str(), value);
		}

		as_array* array = cast_to<as_array>(obj);
		if(array != NULL)
		{
			char index[32];
			for(int i = 0; i < array->m_values.size(); i++)
			{
				sprintf(index, "%d", i);
				send_field(addr, index, array->m_values[i]);
			}
		}

		// properties "object end"
		//int flags = 0;
		//m_packet_writer.init(SET_FIELD);
		//m_packet_writer.write(&addr, sizeof(addr));
		//m_packet_writer.write("");
		//m_packet_writer.write((short)AMF_OBJECT_END);
		//m_packet_writer.flush(m_ns);
	}

	void debugger::send_property(int addr, property_id id, float value)
	{
		char buffer[64];
		sprintf(buffer, "%f", value);

		send_property(addr, id, buffer);
	}

	void debugger::send_property(int addr, property_id id, int value)
	{
		char buffer[64];
		sprintf(buffer, "%d", value);

		send_property(addr, id, buffer);
	}

	void debugger::send_property(int addr, property_id id, const char* value)
	{
		float timeout = 60;

		m_packet_writer.init(player2debugger::PROP);
		m_packet_writer.write(&addr, sizeof(addr));

		short sid = id;
		m_packet_writer.write(&sid, sizeof(sid));

		for(int i = 0; i < strlen(value); i++)
			m_packet_writer.write(&value[i], 1);
		m_packet_writer.write("\0", 1);

		m_packet_writer.flush(m_ns);
	}

	void debugger::send_field(int addr, const char* key, const as_value& value)
	{
		m_packet_writer.init(player2debugger::SET_FIELD);
		m_packet_writer.write(&addr, sizeof(addr));
		m_packet_writer.write(key);
		m_packet_writer.write(value);
		m_packet_writer.flush(m_ns);
	}

	void debugger::send_hierarchy(gameswf::character* ch, tu_string& target, int depth)
	{
		char buffer[256];
		//as_value target;
		//ch->get_member("_target", &target);
		//strcpy(buffer, target.to_string());
		//for(int i = strlen(buffer) - 1; i >= 0; i--)
		//		buffer[i] = buffer[i] == '/' ? '.' : buffer[i];
		sprintf(buffer, "<0x%x>", ch);
		
		tu_string new_target = target;
		if(depth > 0)
		{
			new_target += ".";
			new_target += ch->get_name().length() > 0 ? ch->get_name() : tu_string(buffer);
		}
		
		m_packet_writer.init(player2debugger::PLACE_OBJECT).write((int)ch).write(new_target.c_str()).flush(m_ns);

		sprite_instance* s = cast_to<sprite_instance>(ch);
		if(s != NULL)
		{
			for(int i = s->m_display_list.size() - 1; i >= 0; i--)
			{
				send_hierarchy(s->m_display_list.get_character(i), new_target, depth + 1);
			}
		}
	}

	character* debugger::get_character_selection()
	{
		as_object* obj = address_to_object((int)m_character_selection);
		return cast_to<character>(obj);
	}

	bitmap_info* debugger::get_texture_selection()
	{
		if(m_player != NULL)
		{
			array<smart_ptr<bitmap_info> >& list = m_player->get_root()->m_def->m_bitmap_list;
			for(int i = 0; i < list.size(); i++)
			{
				if(list[i] == m_texture_selection)
					return m_texture_selection;
			}
		}
		return NULL;
	}

	void debugger::load_file(const char* filename, membuf& buf)
	{
		tu_file file(filename, "rb");
		if(file.is_open())
		{
			file.copy_to(&buf);
		}
	}

	void debugger::send_file(player2debugger::type id, const char* filename)
	{
		membuf buf;
		load_file(filename, buf);

		if(buf.size() > 0)
		{
			m_packet_writer.init(id);
			m_packet_writer.write(buf.data(), buf.size());
			m_packet_writer.flush(m_ns);
		}
	}

	void debugger::send_swd(player* p)
	{
		/*membuf swf;
		load_file("actionscript3.swf", swf);

		m_packet_writer.init(player2debugger::SWF_INFO);
		m_packet_writer.write(1); // swfcount
		m_packet_writer.write((short)0); // swfIndex
		m_packet_writer.write((int)p); // addr
		m_packet_writer.write((char)0); // debugComing
		m_packet_writer.write((char)8); // vmVersion
		m_packet_writer.write((short)0); // reserved
		m_packet_writer.write(swf.size()); // swfSize
		m_packet_writer.write(1); // numSWDs
		m_packet_writer.write(20); // numLines
		m_packet_writer.write(0); // numBreakpoints
		m_packet_writer.write(7935); // port
		m_packet_writer.write("actionscript3.swf"); // path
		m_packet_writer.write("actionscript3.swf"); // url
		m_packet_writer.write("localhost"); // host
		m_packet_writer.write(1); // swdfilecount
		m_packet_writer.write(1); // swdLocalIndex
		m_packet_writer.write(20042); // swdFileID
		m_packet_writer.flush(m_ns);

		for(int i = 0; i < m_swd.m_sources.size(); i++)
		{
			const swd::source* src = m_swd.m_sources[i];

			m_packet_writer.init(player2debugger::SWD_FILE_ENTRY);
			m_packet_writer.write(1);//src->m_file_index);
			m_packet_writer.write(src->m_unknown_index);
			m_packet_writer.write(src->m_name);
			m_packet_writer.write(src->m_source);
			m_packet_writer.write(0);
			m_packet_writer.flush(m_ns);
		}*/
	}

	void debugger::send_breakpoint_hitex(int swfindex, int address)
	{
		m_packet_writer.init(player2debugger::BREAKPOINT_HIT_EX);
		m_packet_writer.write(swfindex);
		m_packet_writer.write(address);
		m_packet_writer.write(m_callstack.size());
		for(int i = m_callstack.size() - 1; i >= 0; i--)
			m_packet_writer.write(m_callstack[i].name);
		m_packet_writer.flush(m_ns);
	}

	void debugger::send_function_frame(int depth)
	{
		if(depth >= 0 && depth < m_callstack.size())
		{
			m_packet_writer.init(player2debugger::FUNCTION_FRAME);
			m_packet_writer.write(depth);
			const as_environment* env = m_callstack[depth].env;
			const array<as_environment::frame_slot>& local_frames = env->m_local_frames;
			
			for(int i = m_callstack[depth].local_stack_top + 1; i < local_frames.size(); i++)
			{
				if(local_frames[i].m_name.length() == 0)
					break;

				m_packet_writer.write(local_frames[i].m_name.c_str());
				m_packet_writer.write(local_frames[i].m_value);
			}
			m_packet_writer.flush(m_ns);
		}
	}
};

#endif // GAMESWF_ENABLE_DEBUGGER

