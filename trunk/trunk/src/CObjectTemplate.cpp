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

// containComponent
// check the obj template has the componentID
bool CObjectTemplate::containComponent( int comID )
{	
	const char *lpName = CComponentFactory::getComponentName( comID );
	if ( lpName == NULL )
		return false;

	int n = (int)m_objectComponents.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &m_objectComponents[i];

		// found this component
		if ( strcmp( lpName, p->getAllRecord()->front().name ) == 0 )
			return true;
	}

	return false;
}

// addComponent
// add component to object
bool CObjectTemplate::addComponent( int comID )
{
	if ( containComponent( comID ) == true )
		return false;
	
	// add this component to template
	m_objectComponents.push_back( CComponentFactory::s_compTemplate[comID] );
	sortComponentByName();

	return true;
}

// removeComponent
// remove component from object
bool CObjectTemplate::removeComponent( int comID )
{
	const char *lpName = CComponentFactory::getComponentName( comID );
	if ( lpName == NULL )
		return false;

	int n = (int)m_objectComponents.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &m_objectComponents[i];

		// found this component
		if ( strcmp( lpName, p->getAllRecord()->front().name ) == 0 )
		{
			m_objectComponents.erase( m_objectComponents.begin() + i );
			sortComponentByName();
			return true;
		}
	}
	return false;
}

// sortComponentByName
// sort component on the list
void CObjectTemplate::sortComponentByName()
{
	int len = (int)m_objectComponents.size();	 
		
	for (int i = 1; i <= len - 1; i++)
	{
		CSerializable obj = m_objectComponents[i];
		char *val = obj.getAllRecord()->front().name;

		int j = i - 1;
		int done = 0;

		do
		{
			char *val1 = m_objectComponents[j].getAllRecord()->front().name;

			if ( strcmp(val1, val) > 0 )
			{
				m_objectComponents[j + 1] = m_objectComponents[j];

				j = j - 1;
				if ( j < 0 )
					done = 1;
			}
			else
				done = 1;
		}
		while (done == 0);
		
		m_objectComponents[j + 1] = obj;		
	}
}