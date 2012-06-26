#ifndef _CTRIGGER_H
#define _CTRIGGER_H

#include "CGameObject.h"

class CTrigger: public CGameObject
{
protected:
	float m_boxSize;

	std::string m_scriptFile;
	std::string m_onAlways;
	std::string	m_onWhileIn;
	std::string	m_onWhileOut;
	std::string m_onEnter;
	std::string	m_onExit;
	std::string m_onEnable;
	std::string m_onDisable;

	bool m_enableState;	
public:
	CTrigger();

	CTrigger(CGameObject *parent);

	virtual ~CTrigger();

	// saveData
	// save data to serializable ( use for save in game .sav )
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable ( use for load in game .sav )
	virtual void loadData( CSerializable* pObj );

	// getData
	// get basic data to serializable
	virtual void getData( CSerializable* pObj );

	// updateData
	// update data
	virtual void updateData( CSerializable* pObj );

	// updateObject
	// update object by frame...
	virtual void updateObject();	

	// property declare
	const std::string& getScripFile()
	{
		return m_scriptFile;
	}

	const std::string& getAllwayFunc()
	{
		return m_onAlways;
	}

	const std::string& getWhileInFunc()
	{	
		return m_onWhileIn;
	}

	const std::string& getWhileOutFunc()
	{
		return m_onWhileOut;
	}

	const std::string& getOnEnterFunc()
	{
		return m_onEnter;
	}

	const std::string& getOnExitFunc()
	{
		return m_onExit;
	}

	const std::string& getOnEnableFunc()
	{
		return m_onEnable;
	}

	const std::string& getOnDisableFunc()
	{
		return m_onDisable;
	}
	// end property
protected:

	// runFunction
	// run script func
	void runFunction(const string& funcName, int id1, int id2, bool useID2);
};

#endif