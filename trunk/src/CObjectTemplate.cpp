#include "stdafx.h"
#include "CObjectTemplate.h"

CObjectTemplate::CObjectTemplate()
{
	m_templateName = L"";
	m_objectTemplateID = -1;
}

CObjectTemplate::~CObjectTemplate()
{
}

// addComponent
// add component to object
bool CObjectTemplate::addComponent( int comID )
{
	// check if this component has added
	int nComs = m_objectComponents.size();
	for ( int i = 0; i < nComs; i++ )
	{
		if ( m_objectComponents[i] == comID )
			return false;
	}

	// add to object
	m_objectComponents.push_back( comID );

	// sort list component
	std::sort( m_objectComponents.begin(), m_objectComponents.end() );
	return true;
}

// removeComponent
// remove component from object
bool CObjectTemplate::removeComponent( int comID )
{
	// check if this component has added
	int nComs = m_objectComponents.size();
	for ( int i = 0; i < nComs; i++ )
	{
		if ( m_objectComponents[i] == comID )
		{
			m_objectComponents.erase( m_objectComponents.begin() + i );

			// sort list component
			std::sort( m_objectComponents.begin(), m_objectComponents.end() );

			return true;
		}
	}

	return false;
}