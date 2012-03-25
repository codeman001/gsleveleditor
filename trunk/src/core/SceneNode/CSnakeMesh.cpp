#include "stdafx.h"
#include "CSnakeMesh.h"

CSnakeMesh::CSnakeMesh()
{
	m_meshBuffer = new CDynamicMeshBuffer(video::EVT_STANDARD, video::EIT_32BIT);
	m_meshBuffer->setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_VERTEX);
	m_meshBuffer->setHardwareMappingHint(scene::EHM_DYNAMIC, scene::EBT_INDEX);	

	addMeshBuffer( m_meshBuffer );
	
	m_dirty = true;

	m_segRadius		= 20.0f;		
	m_cylinderSeg	= 10;
		
	m_meshBuffer->getMaterial().Wireframe = true;
	m_meshBuffer->getMaterial().Lighting = false;
	m_meshBuffer->getMaterial().MaterialType = video::EMT_SOLID;
}

CSnakeMesh::~CSnakeMesh()
{
	m_meshBuffer->drop();
}

void CSnakeMesh::skinSnakeMesh()
{
	if ( m_dirty )
	{
		const float fTwoTimesPI = core::PI * 2.0f;

		// num seg
		m_cylinderLenSeg = m_arrayBone->size();

		// calc num vertex
		int numVertex = m_cylinderSeg * m_cylinderLenSeg;		
		m_meshBuffer->getVertexBuffer().set_used( numVertex );
		
		// get vertex buffer
		S3DVertex*	pBuffer = m_meshBuffer->getVertexBuffer().pointer();
		S3DVertex*	pVertex = pBuffer;		

		// transform
		core::matrix4	mat, matTranslate, matRotate;		
		core::vector3df	*v;

		float angle = (float)(core::vector2df( 1.0f, 1.0f ).getAngleTrig());

		list<core::vector3df>::iterator it = m_arrayBone->begin(), end = m_arrayBone->end();

		float r = 0.0f;
		float ty = 0.0f;
		float length = 0.0f;

		int id = 0;

		while ( it != end )
		{			
			core::vector3df& point = (*it);
			it++;

			if ( it != end )
			{
				core::vector3df& nextPoint = (*it);
				core::vector2df	rotate(nextPoint.Z - point.Z, nextPoint.X - point.X);
				r = (float)rotate.getAngleTrig();

				length = rotate.getLength()/50.0f;
			}

			// set matrix transform			
			matRotate.setRotationDegrees( core::vector3df( 0, r, 0 )  );
			matTranslate.setTranslation	( core::vector3df( point.X, point.Y, point.Z ) );
			
			// calc matrix
			mat = matTranslate * matRotate;

			float radius = m_segRadius;
			
			for( int i = 0; i < m_cylinderSeg; ++i )
			{
				float fTheta = (fTwoTimesPI * i) / (m_cylinderSeg - 1) - core::PI;				

				v = (core::vector3df*)pVertex;
				v->X = sinf(fTheta) * radius;
				v->Y = cosf(fTheta) * radius;
				v->Z = 0;

				pVertex->TCoords.X = i/(float)(m_cylinderSeg-1);
				pVertex->TCoords.Y = ty;
				
				mat.transformVect( *v );			

				pVertex++;
			}	

			ty = ty + length;
			id++;
		}			

		int nRow	= m_cylinderLenSeg - 1;
		int numPoly = nRow * ( m_cylinderSeg * 2 - 2 );
		
		int vertexID = 0;

		m_meshBuffer->getIndexBuffer().set_used( numPoly * 3 );
		u32 *indexBuffer = (u32*)m_meshBuffer->getIndexBuffer().pointer();

		u32 *p = indexBuffer;

		for (int i = 0, n = numPoly/2; i < n; i++ )
		{
			*p = vertexID + 1;						p++;
			*p = vertexID;							p++;
			*p = vertexID + m_cylinderSeg;			p++;

			*p = vertexID + 1;						p++;
			*p = vertexID + m_cylinderSeg;			p++;
			*p = vertexID + m_cylinderSeg + 1;		p++;
			
			vertexID++;

			// next row
			if ( i != 0 && (i + 1)%(m_cylinderSeg-1) == 0 )
				vertexID++;
		}

		m_meshBuffer->recalculateBoundingBox();

		recalculateBoundingBox();
		m_dirty = true;
	}
}