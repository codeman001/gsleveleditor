#pragma once
#ifndef GAMESWF_SCENE_NODE_H
#define GAMESWF_SCENE_NODE_H

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_player.h"

#ifdef _GLITCH
#include <glitch/glitch.h>
#include <glitch/video/CMaterial.h>
#include <glitch/scene/CMeshSceneNode.h>
#include <glitch/scene/ITriangleSelector.h>
#include <glitch/glitch.h>
#include <glitch/Revision.h>

namespace gameswf
{
	class scene_node : public glitch::scene::ISceneNode
	{
	public:
		scene_node(player* p, glitch::scene::ISceneNode* node, glitch::core::dimension2d<glitch::s32> texture_size);

		void clear_characters()
		{
			m_characters.clear();
		}

		void attach_character(character* ch)
		{
			m_characters.push_back(ch);
		}

		void get_world_mouse(int& x, int& y);
		void get_local_mouse(character* ch, float& x, float& y);

		// virtual functions
		virtual bool onRegisterSceneNode();

		//! Renders the node.
		virtual void render(void* renderData = 0)
		{
		}

		virtual const glitch::core::aabbox3d<glitch::f32>& getBoundingBox() const
		{
			return m_bounding_box;
		}

	protected:
		void reset_last_point()
		{
			m_last_point.m_x = -1;
			m_last_point.m_y = -1;
		}
		
		void build_dlist(character* ch);

	public:
		static const char* NODE_NAME;;

	protected:
		void init_corners();
		void collect_uvs(const glitch::scene::IMeshConstPtr& mesh, point* uvs, unsigned int count);
		bool get_collision_uv(glitch::core::line3d<glitch::f32>& ray, point& uv);
		void update_inverse_transform();

		glitch::scene::ISceneNode* m_node;
		glitch::video::ITexturePtr m_texture;
		glitch::video::IRenderTargetPtr m_target;
		glitch::u16 m_id_texture;
		glitch::core::triangle3df m_triangles[2];
		glitch::core::vector3df m_vertices[6];
		glitch::u16 m_top_left_index;
		glitch::u16 m_bottom_left_index;
		glitch::u16 m_top_right_index;
		glitch::u16 m_bottom_right_index;
		glitch::core::matrix4 m_last_node_transform;
		glitch::core::aabbox3d<glitch::f32> m_bounding_box;

		player* m_player;
		array<character*> m_characters;
		array<character*> m_dlist;
		point m_last_point;
		point m_world_point;
	};
}; // namespace gameswf

#endif // _GLITCH

#endif // GAMESWF_SCENE_NODE_H