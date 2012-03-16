#include "stdafx.h"
#include "CObjectTemplate.h"

CObjectTemplate::CObjectTemplate()
{
	m_templateName = L"";

#ifdef GSEDITOR
	m_numObjRef = 0;
	m_isDirty	= false;
#endif

}

CObjectTemplate::~CObjectTemplate()
{
}

// containComponent
// check the obj template has the componentID
bool CObjectTemplate::containComponent( char *lpName )
{	
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

// getComponent
// get component
CSerializable * CObjectTemplate::getComponent( char* comName )
{
	int n = (int)m_objectComponents.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &m_objectComponents[i];

		// found this component
		if ( strcmp( comName, p->getAllRecord()->front().name ) == 0 )
			return p;
	}

	return NULL;
}

// addComponent
// add component to object
bool CObjectTemplate::addComponent( char *lpName )
{
	if ( containComponent( lpName ) == true )
		return false;

#ifdef GSEDITOR
	m_isDirty = true;
#endif

	// add this component to template
	int n = CComponentFactory::s_compTemplate.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &CComponentFactory::s_compTemplate[i];
		if ( strcmp( lpName, p->getAllRecord()->front().name ) == 0 )
			m_objectComponents.push_back( *p );
	}
	sortComponentByName();

	return true;
}

// removeComponent
// remove component from object
bool CObjectTemplate::removeComponent( char *lpName )
{

	int n = (int)m_objectComponents.size();
	for ( int i = 0; i < n; i++ )
	{
		CSerializable *p = &m_objectComponents[i];

		// found this component
		if ( strcmp( lpName, p->getAllRecord()->front().name ) == 0 )
		{
			m_objectComponents.erase( m_objectComponents.begin() + i );
			sortComponentByName();

#ifdef GSEDITOR
			m_isDirty = true;
#endif

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