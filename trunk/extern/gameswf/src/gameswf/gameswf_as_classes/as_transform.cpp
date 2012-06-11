#include "stdafx.h"
// as_transform.cpp	-- Julien Hamaide <julien.hamaide@gmail.com>	2008

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.


#include "gameswf/gameswf_as_classes/as_transform.h"

namespace gameswf
{

	// Tranform( mc:MovieClip )
	void	as_global_transform_ctor(const fn_call& fn)
	{
		if (fn.nargs == 1)
		{
			smart_ptr<as_transform>	obj;
			if (character* movie = cast_to<character>(fn.arg(0).to_object()))
			{
				obj = swfnew as_transform(fn.get_player(), movie);
			}
			fn.result->set_as_object(obj.get_ptr());
		}
	}

	as_transform::as_transform(player* player, character* movie_clip) :
		as_object(player),
		m_movie(movie_clip)
	{
		init();
	}

	void	as_transform::init()
	{
		builtin_member("colorTransform", as_value(swfnew as_color_transform(get_player(), &(m_movie->get_cxform()))));
		builtin_member("concatenatedColorTransform", as_value(swfnew as_color_transform(get_player(), &(m_movie->get_world_cxform()))));
		builtin_member("matrix", as_value(swfnew as_matrix(get_player(), &(m_movie->get_matrix()))));
		builtin_member("concatenatedMatrix", as_value(swfnew as_matrix(get_player(), &(m_movie->get_world_matrix()))));
	}


	bool	as_transform::set_member(const tu_stringi& name, const as_value& val)
	{
		as_standard_member member = get_standard_member(name);

		switch(member)
		{
			case M_COLORTRANSFORM:
			{
				as_color_transform* trans = cast_to<as_color_transform>( val.to_object() );
				if(trans != NULL)
				{
					as_object::set_member(name, val);
					m_movie->set_cxform( trans->m_color_transform );
				}
				return true;
			}

			case M_MATRIX:
			{
				as_matrix* mat = cast_to<as_matrix>( val.to_object() );
				if(mat != NULL)
				{
					as_object::set_member(name, val);
					m_movie->set_matrix( mat->m_matrix );
				}
				return true;
			}
		
			case M_WORLDCOLORTRANSFORM:
			case M_WORLDMATRIX:
			{
				//read-only
				return true;
			}
		}
		
		//if(name == "pixelBounds")
		//{
		//	assert( false && "todo" );
		//	return true;
		//}

		return as_object::set_member( name, val );
	}

	bool	as_transform::get_member(const tu_stringi& name, as_value* val)
	{
		as_standard_member member = get_standard_member(name);

		switch(member)
		{
		case M_WORLDCOLORTRANSFORM:
			//m_world_cxform->m_color_transform = m_movie->get_world_cxform();
			if(as_object::get_member( name, val ))
			{
				as_color_transform* trans = cast_to<as_color_transform>(val->to_object());
				if(trans)
					trans->m_color_transform = m_movie->get_world_cxform();
			}
			return true;

		case M_WORLDMATRIX:
			if(as_object::get_member( name, val ))
			{
			}
			//m_world_matrix->m_matrix = m_movie->get_world_matrix();
			return true;

		}
		
		//if(name == "pixelBounds")
		//{
		//	assert( false && "todo" );
		//	return true;
		//}

		return as_object::get_member( name, val );
	}
}
