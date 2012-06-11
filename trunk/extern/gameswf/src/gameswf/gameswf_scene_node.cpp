#include "stdafx.h"
#include "gameswf/gameswf_scene_node.h"
#include "gameswf/gameswf_character.h"
#include "gameswf/gameswf_sprite.h"
#include "gameswf/gameswf_root.h"

#ifdef _GLITCH

namespace gameswf
{
	const char* scene_node::NODE_NAME = "gameswf_render_node";

	template < typename T >
	void collect(const glitch::u32* indices,
						glitch::u32 count,
						const glitch::video::SVertexStream& stream,
						point* uvs)
	{
		glitch::video::SVertexStream::SMapBuffer<const glitch::core::vector2d<T> > uv(stream, glitch::video::EBMA_READ_CLIENT);
		if (indices)
		{
			for(glitch::u32 i = 0; i < count; i++, indices += 3)
			{
#ifndef GLITCH_USE_RIGHT_HAND_CONVENTION
				*(uvs++) = point((float)uv[indices[0]].X, (float)uv[indices[0]].Y);
				*(uvs++) = point((float)uv[indices[1]].X, (float)uv[indices[1]].Y);
				*(uvs++) = point((float)uv[indices[2]].X, (float)uv[indices[2]].Y);
#else
				*(uvs++) = point((float)uv[indices[2]].X, (float)uv[indices[2]].Y);
				*(uvs++) = point((float)uv[indices[1]].X, (float)uv[indices[1]].Y);
				*(uvs++) = point((float)uv[indices[0]].X, (float)uv[indices[0]].Y);
#endif // GLITCH_USE_RIGHT_HAND_CONVENTION
			}
		}
		else
		{
			for(glitch::u32 i = 0; i < count; i += 3)
			{
#ifndef GLITCH_USE_RIGHT_HAND_CONVENTION
				*(uvs++) = point((float)uv[i].X, (float)uv[i].Y);
				*(uvs++) = point((float)uv[i+1].X, (float)uv[i+1].Y);
				*(uvs++) = point((float)uv[i+2].X, (float)uv[i+2].Y);
#else
				*(uvs++) = point((float)uv[i+2].X, (float)uv[i+2].Y);
				*(uvs++) = point((float)uv[i+1].X, (float)uv[i+1].Y);
				*(uvs++) = point((float)uv[i].X, (float)uv[i].Y);
#endif // GLITCH_USE_RIGHT_HAND_CONVENTION
			}
		}
	}

	scene_node::scene_node(player* p, glitch::scene::ISceneNode* node, glitch::core::dimension2d<glitch::s32> texture_size) :
		m_node(node),
		m_id_texture((glitch::u16)-1),
		m_top_left_index(0),
		m_bottom_left_index(0),
		m_top_right_index(0),
		m_bottom_right_index(0),
		m_player(p)
	{
		setName(NODE_NAME);
		m_node->grab();

		glitch::scene::CMeshSceneNode* mesh_node = (glitch::scene::CMeshSceneNode*)m_node;

		init_corners();
		
		// create texture/render target
		glitch::video::IVideoDriver* driver = p->m_context->m_video_driver;

		// disable mipmaps for textures
		const bool mipmaps = driver->getOption(glitch::video::EVDO_CREATE_TEXTURE_MIPMAPS);
		driver->setOption(glitch::video::EVDO_CREATE_TEXTURE_MIPMAPS, false);

#if GLITCH_REVISION >= GAMESWF_PIXEL_FORMAT_REV
		m_texture = driver->getTextureManager()->addTexture(texture_size, node->getUID(), glitch::video::EPF_R8G8B8A8);
#else
		m_texture = driver->getTextureManager()->addTexture(texture_size, node->getUID(), glitch::video::ECF_R8G8B8A8);
#endif

		// restore mipmaps
		driver->setOption(glitch::video::EVDO_CREATE_TEXTURE_MIPMAPS, mipmaps);

		m_target = driver->createRenderTarget(m_texture);

		glitch::video::CMaterialPtr materialPtr = mesh_node->getMesh()->getMaterial(0);
		if(materialPtr != NULL)
		{
			glitch::video::CMaterialRendererPtr renderer = materialPtr->getMaterialRenderer();
			m_id_texture = renderer->getParameterID(glitch::video::ESPT_TEXTURE);

			if(m_id_texture != (glitch::u16)-1)
				materialPtr->setParameter(m_id_texture, m_texture);
		}
		//mesh_node->getMesh()->getMeshBuffer(0)->

		reset_last_point();
	}

	void scene_node::update_inverse_transform()
	{
		point screen_pt = m_player->get_root()->m_mouse_screen_pos;

		if(screen_pt.m_x != m_last_point.m_x || screen_pt.m_y != m_last_point.m_y || m_node->getAbsoluteTransformation() != m_last_node_transform)
		{
			glitch::video::IVideoDriver* driver = m_player->m_context->m_video_driver;
			glitch::scene::ISceneCollisionManager* manager = driver->getDevice()->getSceneManager()->getSceneCollisionManager();

			glitch::core::line3d<glitch::f32> ray = manager->getRayFromScreenCoordinates(glitch::core::position2d<glitch::s32>((int)screen_pt.m_x, (int)screen_pt.m_y));
			point uv;
			if(get_collision_uv(ray, uv))
			{
				//printf("collistion uv=%f,%f\n", uv.m_x, uv.m_y);

				//uv.m_x = fmin(fmax(uv.m_x, 0), 1.0f);
				//uv.m_y = fmin(fmax(uv.m_y, 0), 1.0f);
				m_world_point = point(uv.m_x * m_player->get_root()->m_viewport_width, uv.m_y * m_player->get_root()->m_viewport_height);
				m_player->get_root()->screen_to_logical(m_world_point);
			}
			else
			{
				m_world_point = point(-100000, -100000);
			}
		}

		m_last_node_transform = m_node->getAbsoluteTransformation();
		m_last_point = screen_pt;
	}

	void scene_node::get_world_mouse(int& x, int& y)
	{
		update_inverse_transform();

		x = (int)m_world_point.m_x;
		y = (int)m_world_point.m_y;
	}

	void scene_node::get_local_mouse(character* ch, float& x, float& y)
	{
		update_inverse_transform();

		point local_point = m_world_point;
		local_point.m_x = PIXELS_TO_TWIPS(local_point.m_x);
		local_point.m_y = PIXELS_TO_TWIPS(local_point.m_y);
		point temp = local_point;
		if(ch->get_parent() != NULL)
			ch->get_parent()->get_world_matrix().transform_by_inverse(&temp, local_point);

		//printf("collistion at %d,%d\n", (int)TWIPS_TO_PIXELS(temp.m_x), (int)TWIPS_TO_PIXELS(temp.m_y));
		x = temp.m_x;
		y = temp.m_y;
	}

	void scene_node::init_corners()
	{
		glitch::scene::CMeshSceneNode* mesh_node = (glitch::scene::CMeshSceneNode*)m_node;
		glitch::scene::CTriangleSelector selector(mesh_node->getMesh(), NULL);

		glitch::s32 count = 0;
		selector.getTriangles(m_triangles, 2, count);

		glitch::s32 offset = 0;
		for(glitch::s32 i = 0; i < count; i++)
		{
			m_vertices[offset++] = m_triangles[i].pointA;
			m_vertices[offset++] = m_triangles[i].pointB;
			m_vertices[offset++] = m_triangles[i].pointC;
		}

		glitch::u32 uv_count = (glitch::u32)count * 3;
		point uvs[6];
		collect_uvs(mesh_node->getMesh(), uvs, uv_count);

		for(glitch::u32 i = 0; i < uv_count; i++)
		{
			if(uvs[i].m_x < 0.5f)
			{
				if(uvs[i].m_y > 0.5f)
					m_top_left_index = i;
				else
					m_bottom_left_index = i;
			}
			else
			{
				if(uvs[i].m_y > 0.5f)
					m_top_right_index = i;
				else
					m_bottom_right_index = i;
			}
		}
	}

	void scene_node::collect_uvs(const glitch::scene::IMeshConstPtr& mesh, point* uvs, unsigned int count)
	{
		if(mesh->getMeshBufferCount() == 0)
			return;
		
		const glitch::scene::CMeshBuffer* buf = mesh->getMeshBuffer(0).get();
		count = imin(buf->getIndexCount(), count);

		const glitch::video::CVertexStreams* streams = buf->getVertexStreams().get();

		if(streams->getTextureUnitCount() == 0)
			return;

		const glitch::video::SVertexStream& uvStream = streams->getTexCoordStream(0);
		const glitch::video::CPrimitiveStream& prims = buf->getPrimitiveStream();
		const glitch::u8* indices = NULL;
		if (prims.getIndexBuffer())
		{
			indices = (const glitch::u8*)prims.getIndexBuffer()->map(glitch::video::EBMA_READ_CLIENT);
		}

		if(indices != NULL)
		{
			glitch::u32 index_size = glitch::video::getIndexTypeSize(prims.getIndexType());
			glitch::u32 index_array[6];
			assert(count <= 6);
			for(glitch::u32 i = 0; i < count; i++, indices += index_size)
			{
				switch(index_size)
				{
				case 1: index_array[i] = *indices; break;
				case 2: index_array[i] = *(glitch::u16*)indices; break;
				case 4: index_array[i] = *(glitch::u32*)indices; break;
				default: assert(false); break;
				}
			}

			if(uvStream.getArraySize() == 2)
			{
				switch(uvStream.getValueType())
				{
				case glitch::video::EVAVT_BYTE:
					collect<glitch::s8>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_UBYTE:
					collect<glitch::u8>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_SHORT:
					collect<glitch::s16>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_USHORT:
					collect<glitch::u16>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_INT:
					collect<glitch::s32>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_UINT:
					collect<glitch::u32>(index_array, count/3, uvStream, uvs);
					break;
				case glitch::video::EVAVT_FLOAT:
					collect<glitch::f32>(index_array, count/3, uvStream, uvs);
					break;
				}

				glitch::core::vector3df scale = streams->getTexCoordScale(0);
				glitch::core::vector3df offset = streams->getTexCoordOffset(0);
				
				if(scale.X != 0.0f && scale.Y != 0.0f)
				{
					for(glitch::u32 i = 0; i < count; i++)
					{
						uvs[i].m_x = (uvs[i].m_x * scale.X) + offset.X;
						uvs[i].m_y = (uvs[i].m_y * scale.Y) + offset.Y;
					}
				}
			}
			else
			{
				assert(false);
			}

			prims.getIndexBuffer()->unmap();
		}
	}

	bool scene_node::get_collision_uv(glitch::core::line3d<glitch::f32>& ray, point& uv)
	{
		glitch::core::matrix4 mat = m_node->getAbsoluteTransformation();
		glitch::core::triangle3df world_triangles[2];
		for(int i = 0; i < 2; ++i)
		{
			mat.transformVect(world_triangles[i].pointA, m_triangles[i].pointA);
			mat.transformVect(world_triangles[i].pointB, m_triangles[i].pointB);
			mat.transformVect(world_triangles[i].pointC, m_triangles[i].pointC);
		}

		glitch::core::vector3df linevect = ray.getVector().normalize();
		glitch::core::vector3df intersection;
		glitch::f32 nearest = FLT_MAX;
		bool found = false;
		const glitch::f32 raylength = ray.getLengthSQ();
		const glitch::f32 minX = glitch::core::min_(ray.start.getX(), ray.end.getX());
		const glitch::f32 maxX = glitch::core::max_(ray.start.getX(), ray.end.getX());
		const glitch::f32 minY = glitch::core::min_(ray.start.getY(), ray.end.getY());
		const glitch::f32 maxY = glitch::core::max_(ray.start.getY(), ray.end.getY());
		const glitch::f32 minZ = glitch::core::min_(ray.start.getZ(), ray.end.getZ());
		const glitch::f32 maxZ = glitch::core::max_(ray.start.getZ(), ray.end.getZ());

		for(int i = 0; i < 2; ++i)
		{
			const glitch::core::triangle3df & triangle = world_triangles[i];

			if(minX > triangle.pointA.getX() && minX > triangle.pointB.getX() && minX > triangle.pointC.getX())
				continue;
			if(maxX < triangle.pointA.getX() && maxX < triangle.pointB.getX() && maxX < triangle.pointC.getX())
				continue;
			if(minY > triangle.pointA.getY() && minY > triangle.pointB.getY() && minY > triangle.pointC.getY())
				continue;
			if(maxY < triangle.pointA.getY() && maxY < triangle.pointB.getY() && maxY < triangle.pointC.getY())
				continue;
			if(minZ > triangle.pointA.getZ() && minZ > triangle.pointB.getZ() && minZ > triangle.pointC.getZ())
				continue;
			if(maxZ < triangle.pointA.getZ() && maxZ < triangle.pointB.getZ() && maxZ < triangle.pointC.getZ())
				continue;

			if(ray.start.getDistanceFromSQ(triangle.pointA) >= nearest &&
				ray.start.getDistanceFromSQ(triangle.pointB) >= nearest &&
				ray.start.getDistanceFromSQ(triangle.pointC) >= nearest)
				continue;

			if (triangle.getIntersectionWithLine(ray.start, linevect, intersection))
			{
				const float tmp = intersection.getDistanceFromSQ(ray.start);
				const float tmp2 = intersection.getDistanceFromSQ(ray.end);

				if (tmp < raylength && tmp2 < raylength && tmp < nearest)
				{
					nearest = tmp;
					found = true;
				}
			}
		}

		if(found)
		{
			mat.makeInverse();
			mat.transformVect(intersection);

			glitch::core::vector3df horz(	m_vertices[m_bottom_right_index].X - m_vertices[m_bottom_left_index].X,
											m_vertices[m_bottom_right_index].Y - m_vertices[m_bottom_left_index].Y,
											m_vertices[m_bottom_right_index].Z - m_vertices[m_bottom_left_index].Z);

			glitch::core::vector3df vert(	m_vertices[m_top_left_index].X - m_vertices[m_bottom_left_index].X,
											m_vertices[m_top_left_index].Y - m_vertices[m_bottom_left_index].Y,
											m_vertices[m_top_left_index].Z - m_vertices[m_bottom_left_index].Z);

			glitch::core::vector3df inter(	intersection.X - m_vertices[m_bottom_left_index].X,
											intersection.Y - m_vertices[m_bottom_left_index].Y,
											intersection.Z - m_vertices[m_bottom_left_index].Z);

			glitch::f32 projx = inter.dotProduct(horz) / horz.getLengthSQ();
			glitch::f32 projy = inter.dotProduct(vert) / vert.getLengthSQ();
			uv.m_x = projx;
			uv.m_y = projy;
		}

		return found;;
	}

	void scene_node::build_dlist(character* ch)
	{
		if(ch->get_visible() && ch->get_world_cxform().m_[3][0] >= 0.0f)
		{
			if(ch->m_custom != NULL && ch->m_custom->m_scene_node == this)
				m_dlist.push_back(ch);

			if(ch->is(AS_SPRITE))
			{
				sprite_instance* sprite = static_cast<sprite_instance*>(ch);
				for(int i = 0; i < sprite->m_display_list.size(); i++)
					build_dlist(sprite->m_display_list.get_character(i));
			}
		}
	}

	bool scene_node::onRegisterSceneNode()
	{
		bool visible = false;
		for(int i = 0; i < m_characters.size(); i++)
			visible |= m_characters[i]->get_visible();

		if(!visible)
			return false;

		m_dlist.clear();
		if(m_characters.size() == 1)
			m_dlist = m_characters;
		else
			build_dlist(m_player->get_root()->get_root_movie());

		glitch::video::IVideoDriver* driver = m_player->m_context->m_video_driver;

		driver->pushRenderTarget(m_target);
		driver->clearBuffers(glitch::video::EFB_COLOR);

		// render display list
		for(int i = 0; i < m_dlist.size(); i++)
		{
			character* ch = m_dlist[i];
			scene_node* old_scene_node = ch->m_custom->m_scene_node;
			ch->m_custom->m_scene_node = NULL;
			//const matrix* old_mat = ch->m_matrix;
			//ch->set_const_matrix(&matrix::identity);

			ch->get_root()->begin_display();
			ch->display();
			ch->get_root()->end_display();

			ch->m_custom->m_scene_node = old_scene_node;
			//ch->set_const_matrix(old_mat);
		}

		driver->popRenderTarget();
		
		return true;
	}

}; // namespace gameswf
#endif
