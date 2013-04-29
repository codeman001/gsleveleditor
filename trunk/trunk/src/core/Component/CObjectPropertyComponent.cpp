#include "stdafx.h"
#include "IView.h"
#include "CObjectPropertyComponent.h"

#include "CAnimMeshComponent.h"
#include "CStaticMeshComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameContainerSceneNode.h"

CObjectPropertyComponent::CObjectPropertyComponent(CGameObject *owner)
	:IObjectComponent( owner, IObjectComponent::ObjectProperty )
{
	m_enableDirectionLight	= true;
	m_enablePointLight		= true;
}
	
CObjectPropertyComponent::~CObjectPropertyComponent()
{
}

// init
// run when init object
void CObjectPropertyComponent::initComponent()
{
	if ( m_gameObject == NULL && m_gameObject->getSceneNode() == NULL )
		return; 


	//////////////////////////////////////////////////////////////
	// enable/disable point light & direction light
	//////////////////////////////////////////////////////////////

	ISceneNode		*node	= m_gameObject->getSceneNode();
	
	std::vector<ISceneNode*>		listNodesHaveMesh;

	if ( m_gameObject->getComponent( IObjectComponent::StaticMesh ) != NULL )
	{		
		listNodesHaveMesh.push_back( node );
	}
	else if ( m_gameObject->getComponent( IObjectComponent::AnimMesh ) != NULL )
	{		
		listNodesHaveMesh.push_back( node );
	}
	else if ( m_gameObject->getComponent( IObjectComponent::ColladaMesh ) != NULL )
	{
		CColladaMeshComponent *comp = (CColladaMeshComponent*)m_gameObject->getComponent( IObjectComponent::ColladaMesh );		
		CGameColladaContainerSceneNode* colladaNode = (CGameColladaContainerSceneNode*)comp->getColladaNode();
		
		if ( colladaNode )
		{
			std::vector<ISceneNode*>* listMeshNode = colladaNode->getBoundingMeshNode();
			for ( int i = 0, n = (int)listMeshNode->size(); i < n; i++ )
				listNodesHaveMesh.push_back( listMeshNode->at(i) );
		}
	}

	for ( int i = 0, n = (int)listNodesHaveMesh.size(); i < n; i++ )
	{
		ISceneNode* node = listNodesHaveMesh[i];
		node->enablePointLight(m_enablePointLight);
		node->enableDirectionLight(m_enableDirectionLight);
	}

	//////////////////////////////////////////////////////////////
}

// update
// run when update per frame
void CObjectPropertyComponent::updateComponent()
{
}

// saveData
// save data to serializable
void CObjectPropertyComponent::saveData( CSerializable* pObj )
{
	pObj->addGroup( IObjectComponent::s_compType[ m_componentID ] );

	pObj->addBool("enableDirectionLight",	m_enableDirectionLight, true);
	pObj->addBool("enablePointLight",		m_enablePointLight, true);
}

// loadData
// load data to serializable
void CObjectPropertyComponent::loadData( CSerializable* pObj )
{
	pObj->nextRecord();

	m_enableDirectionLight	= pObj->readBool();
	m_enablePointLight		= pObj->readBool();
}