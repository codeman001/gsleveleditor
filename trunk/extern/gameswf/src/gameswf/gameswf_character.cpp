#include "stdafx.h"
// gameswf_character.cpp	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// Some implementation code for the gameswf SWF player library.


#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_render.h"

#ifdef _GLITCH
#include "gameswf/gameswf_scene_node.h"
#include <glitch/scene/ISceneNode.h>
#endif

namespace gameswf
{

	character::character(player* player, character* parent, int id, int type)	:
		as_object(player),
		m_id(id),
		m_parent(parent),
		m_depth(0),
		m_cxform(&cxform::identity),
		m_matrix(&matrix::identity),
		m_effect(&effect::identity),
		m_custom(NULL),
		m_type(type),
		m_cxform_changed(true),
		m_matrix_changed(true),
		//m_effect_changed(true),
		m_ratio(0.0f),
		m_clip_depth(0),
		m_visible(true),
		m_can_handle_mouse_event(false),
		m_need_advance(true)
	{
		// loadMovieClip() requires that the following will be commented out
		// assert((parent == NULL && m_id == -1)	|| (parent != NULL && m_id >= 0));
	}

	void	character::recycle(character* parent, int id)
	{
		m_id = id;
		set_depth(0);
		set_ratio(0);
		set_clip_depth(0);
		set_parent(parent);
		set_const_matrix(&matrix::identity);
		set_const_cxform(&cxform::identity);
		set_const_effect(&effect::identity);
		set_visible(true);
		m_can_handle_mouse_event = false;
		m_need_advance = true;
		m_cxform_changed = true;
		m_matrix_changed = true;
	}

	bool	character::get_member(const tu_stringi& name, as_value* val)
	// Set *val to the value of the named member and
	// return true, if we have the named member.
	// Otherwise leave *val alone and return false.
	{
		if(as_object::get_member(name, val))
			return true;

		// first try character members
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_GLOBAL:
				val->set_as_object(get_global());
				return true;

			case M_STAGE:
				val->set_as_object(m_player->get_stage());
				return true;

			case M_ROOT:
			case M_LEVEL0:
				val->set_as_object(get_root_movie());
				return true;

			case M_THIS:
			case MDOT:
				val->set_as_object(this);
				return true;

			case M_X:
			{
				const matrix&	m = get_matrix();
				val->set_double(TWIPS_TO_PIXELS(m.m_[0][2]));
				return true;
			}
			case M_Y:
			{
				const matrix&	m = get_matrix();
				val->set_double(TWIPS_TO_PIXELS(m.m_[1][2]));
				return true;
			}
			case M_XSCALE:
			{
				const matrix& m = get_matrix();	// @@ or get_world_matrix()?  Test this.
				float xscale = m.get_x_scale();
				val->set_double(xscale * 100);		// result in percent
				return true;
			}
			case M_YSCALE:
			{
				const matrix& m = get_matrix();	// @@ or get_world_matrix()?  Test this.
				float yscale = m.get_y_scale();
				val->set_double(yscale * 100);		// result in percent
				return true;
			}
			case M_ALPHA:
			{
				// Alpha units are in percent.
				val->set_double(get_cxform().m_[3][0] * 100.f);
				return true;
			}
			case M_VISIBLE:
			{
				val->set_bool(get_visible());
				return true;
			}
			case M_WIDTH:
			{
				val->set_double((int) TWIPS_TO_PIXELS(get_width()));
				return true;
			}
			case M_HEIGHT:
			{
				val->set_double((int) TWIPS_TO_PIXELS(get_height()));
				return true;
			}
			case M_ROTATION:
			{
				// Verified against Macromedia player using samples/test_rotation.swf
				float	angle = get_matrix().get_rotation();

				// Result is CLOCKWISE DEGREES, [-180,180]
				angle *= 180.0f / float(M_PI);

				val->set_double(angle);
				return true;
			}
			case M_TARGET:
			{
				// Full path to this object; e.g. "/_level0/sprite1/sprite2/ourSprite"
				character* parent = get_parent();
				if (parent == NULL)	// root
				{
					val->set_tu_string("/");
					return true;
				}

				as_value target;
				parent->get_member("_target", &target);

					// if s != "/"(root) add "/"
				tu_string s = target.to_tu_string();
				s += s == "/" ? "" : "/";

				// add own name
				if (get_name().length() == 0)
				{
					s += "noname";
				}
				else
				{
					s += get_name();
				}

				val->set_tu_string(s);
				return true;
			}
			case M_NAME:
			{
				val->set_tu_string(get_name());
				return true;
			}
			case M_DROPTARGET:
			{
				// Absolute path in slash syntax where we were last dropped (?)
				// @@ TODO
				val->set_string("/_root");
				return true;
			}
			case M_URL:
			{
				// our URL.
				val->set_string("gameswf");
				return true;
			}
			case M_HIGHQUALITY:
			{
				// Whether we're in high quality mode or not.
				val->set_bool(true);
				return true;
			}
			case M_FOCUSRECT:
			{
				// Is a yellow rectangle visible around a focused movie clip (?)
				val->set_bool(false);
				return true;
			}
			case M_SOUNDBUFTIME:
			{
				// Number of seconds before sound starts to stream.
				val->set_double(0.0);
				return true;
			}
			case M_XMOUSE:
			{
				// Local coord of mouse IN PIXELS.
				int	x, y, buttons;
				get_mouse_state(&x, &y, &buttons);

#ifdef _GLITCH
				character* ch = this;
				while(ch != NULL)
				{
					if(ch->m_custom != NULL && ch->m_custom->m_scene_node != NULL)
					{
						ch->m_custom->m_scene_node->get_world_mouse(x, y);
						break;
					}
					ch = ch->get_parent();
				}
#endif
				matrix	m = get_world_matrix();

				point	a(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));
				point	b;

				m.transform_by_inverse(&b, a);

				val->set_double(TWIPS_TO_PIXELS(b.m_x));
				return true;
			}
			case M_YMOUSE:
			{
				// Local coord of mouse IN PIXELS.
				int	x, y, buttons;
				get_mouse_state(&x, &y, &buttons);

#ifdef _GLITCH
				character* ch = this;
				while(ch != NULL)
				{
					if(ch->m_custom != NULL && ch->m_custom->m_scene_node != NULL)
					{
						ch->m_custom->m_scene_node->get_world_mouse(x, y);
						break;
					}
					ch = ch->get_parent();
				}
#endif
				matrix	m = get_world_matrix();

				point	a(PIXELS_TO_TWIPS(x), PIXELS_TO_TWIPS(y));
				point	b;

				m.transform_by_inverse(&b, a);

				val->set_double(TWIPS_TO_PIXELS(b.m_y));
				return true;
			}
			case M_PARENT:
			case MDOT2:
			{
				val->set_as_object(cast_to<as_object>(m_parent.get_ptr()));
				return true;
			}
		}	// end switch

		return false;
	}

	bool	character::set_member(const tu_stringi& name, const as_value& val)
	{
		// first try character members
		as_standard_member	std_member = get_standard_member(name);
		switch (std_member)
		{
			default:
				break;

			case M_X:
			{
				matrix	m = get_matrix();
				m.m_[0][2] = (float) PIXELS_TO_TWIPS(val.to_number());
				set_matrix(m);
				return true;
			}
			case M_Y:
			{
				matrix	m = get_matrix();
				m.m_[1][2] = (float) PIXELS_TO_TWIPS(val.to_number());
				set_matrix(m);
				return true;
			}
			case M_XSCALE:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = val.to_float() / 100.f;	// input is in percent
				float	y_scale = m.get_y_scale();
				float	rotation = m.get_rotation();
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_YSCALE:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = m.get_x_scale();
				float	y_scale = val.to_float() / 100.f;	// input is in percent
				float	rotation = m.get_rotation();
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_ALPHA:
			{
				// Set alpha modulate, in percent.
				cxform	cx = get_cxform();
				cx.m_[3][0] = val.to_float() / 100.f;
				set_cxform(cx);
				return true;
			}
			case M_VISIBLE:
			{
				set_visible(val.to_bool());
				return true;
			}
			case M_WIDTH:
			{
				if (val.to_float() > 0)
				{
					matrix	m = get_matrix();

					// Decompose matrix and insert the desired value.
					float	x_scale = m.get_x_scale();
					float	y_scale = m.get_y_scale();
					float	rotation = m.get_rotation();

					// get current width
					float current_width = TWIPS_TO_PIXELS(get_width());

					// set swfnew scale
					x_scale /= current_width / val.to_float();

					m.set_scale_rotation(x_scale, y_scale, rotation);
					set_matrix(m);
				}
				return true;
			}
			case M_HEIGHT:
			{
				if (val.to_float() > 0)
				{
					matrix	m = get_matrix();

					// Decompose matrix and insert the desired value.
					float	x_scale = m.get_x_scale();
					float	y_scale = m.get_y_scale();
					float	rotation = m.get_rotation();

					// get current height
					float current_height = TWIPS_TO_PIXELS(get_height());

					// set swfnew scale
					y_scale /= current_height / val.to_float();

					m.set_scale_rotation(x_scale, y_scale, rotation);
					set_matrix(m);
				}
				return true;
			}
			case M_ROTATION:
			{
				matrix	m = get_matrix();

				// Decompose matrix and insert the desired value.
				float	x_scale = m.get_x_scale();
				float	y_scale = m.get_y_scale();
				float	rotation = val.to_float() * float(M_PI) / 180.f;	// input is in degrees
				m.set_scale_rotation(x_scale, y_scale, rotation);

				set_matrix(m);
				return true;
			}
			case M_NAME:
			{
				set_name(val.to_tu_string());
				return true;
			}
			case M_HIGHQUALITY:
			{
				// @@ global { 0, 1, 2 }
				//				// Whether we're in high quality mode or not.
				//				val->set(true);
				return true;
			}
			case M_FOCUSRECT:
			{
				//				// Is a yellow rectangle visible around a focused movie clip (?)
				//				val->set(false);
				return true;
			}
			case M_SOUNDBUFTIME:
			{
				// @@ global
				//				// Number of seconds before sound starts to stream.
				//				val->set(0.0);
				return true;
			}
		}	// end switch

		return as_object::set_member(name, val);
	}

	float	character::get_width()
	{
		rect bound;
		get_bound(&bound);
		float w = bound.m_x_max - bound.m_x_min;
		return w >= FLT_MIN ? w : 0;
	}

	float	character::get_height()
	{
		rect bound;
		get_bound(&bound);
		float h = bound.m_y_max - bound.m_y_min;
		return h >= FLT_MIN ? h : 0;
	}

	void character::get_bound(rect* bound)
	{
		character_def* def = get_character_def();
		assert(def);
		def->get_bound(bound);
		get_matrix().transform(bound);
	}

	void character::do_display_callback()
	{
		assert(m_custom && m_custom->m_display_callback);
		
		gameswf::get_render_handler()->begin_display_callback();

		render_state state;
		state.ch = this;
		state.world_matrix = get_world_matrix();
		state.world_cxform = get_world_cxform();

		(*(m_custom->m_display_callback))(state, m_custom->m_display_callback_user_ptr);

		gameswf::get_render_handler()->end_display_callback();
	}

	//bool	character::is_visible()
	//{
	//	// The increase of performance that gives the skipping of invisible characters
	//	// less than expenses for performance of test of visibility
	//	// get_bound() & get_world_matrix() are heavy recursive operations
	//	return true;

	//	rect bound;
	//	get_bound(&bound);
	//	
	//	matrix m;
	//	m.set_inverse(get_matrix());
	//	m.transform(&bound);

	//	m = get_world_matrix();
	//	m.transform(&bound);

	//	return render::is_visible(bound);
	//}

	void character::enumerate(as_environment* env)
	{
		as_object::enumerate(env);
	}

	const matrix&	character::get_world_matrix()
	{
#ifdef ENABLE_OPTIMIZED_WORLD_MATRIX_CXFORM
		character* ch = get_world_matrix_root_change();
		if(ch)
			ch->update_world_matrix();
#else
		if (m_parent != NULL)
		{
			m_world_matrix = m_parent->get_world_matrix();
		}
		else
		{
			m_world_matrix = matrix();
		}
		m_world_matrix.concatenate(get_matrix());
#endif

		return m_world_matrix;
	}
	
	const cxform&	character::get_world_cxform()
	{
#ifdef ENABLE_OPTIMIZED_WORLD_MATRIX_CXFORM
		character* ch = get_world_cxform_root_change();
		if(ch)
			ch->update_world_cxform();
#else
		if (m_parent != NULL)
		{
			m_world_cxform = m_parent->get_world_cxform();
		}
		else
		{
			m_world_cxform = cxform();
		}
		m_world_cxform.concatenate(get_cxform());
#endif

		return m_world_cxform;
	}

	character* character::get_world_matrix_root_change()
	{
		character* root = NULL;
		character* ch = this;
		while(ch != NULL)
		{
			if(ch->m_matrix_changed)
				root = ch;
			ch = ch->m_parent.get_ptr();
		}
		return root;
	}
	
	character* character::get_world_cxform_root_change()
	{
		character* root = NULL;
		character* ch = this;
		while(ch != NULL)
		{
			if(ch->m_cxform_changed)
				root = ch;
			ch = ch->m_parent.get_ptr();
		}
		return root;
	}
	
	void character::update_world_matrix()
	{
		if (m_parent != NULL)
		{
			m_world_matrix = m_parent->get_world_matrix();
		}
		else
		{
			m_world_matrix = matrix();
		}
		m_world_matrix.concatenate(get_matrix());
		m_matrix_changed = false;
	}

	void character::update_world_cxform()
	{
		if (m_parent != NULL)
		{
			m_world_cxform = m_parent->get_world_cxform();
		}
		else
		{
			m_world_cxform = cxform();
		}
		if(m_cxform != &cxform::identity)
			m_world_cxform.concatenate(get_cxform());
		m_cxform_changed = false;
	}

#ifdef _GLITCH
	void character::attach_scene_node(glitch::scene::ISceneNode* node, glitch::core::dimension2d<glitch::s32> texture_size, bool replace)
	{
		if(m_custom == NULL)
			m_custom = swfnew custom();

		assert(m_custom->m_scene_node == NULL);

		glitch::scene::ISceneNode* target = node->getSceneNodeFromName(scene_node::NODE_NAME);

		if(target != NULL && replace)
			static_cast<scene_node*>(target)->clear_characters();

		if(target == NULL)
		{
			target = irrnew scene_node(get_player(), node, texture_size);
			node->addChild(target);
		}

		m_custom->m_scene_node = static_cast<scene_node*>(target);
		static_cast<scene_node*>(target)->attach_character(this);
	}
#endif
}
