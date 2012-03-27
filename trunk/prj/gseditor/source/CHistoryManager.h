#ifndef _HISTORYMANAGER_
#define _HISTORYMANAGER_

#include "stdafx.h"
#include "CGameObject.h"
#include "CSerializable.h"

class CHistoryManager: public uiSingleton<CHistoryManager>
{
protected:
	enum ActionType
	{
		CreateObject = 0,
		DeleteObject,
		ModifyObject
	};
		

	typedef struct tagAction
	{
		ActionType			actionType;
		CSerializable		object1;
		CSerializable		object2;
	}SAction;

	typedef vector<SAction>	HistoryAction;

	// undo action
	vector<HistoryAction>	m_undoAction;
	vector<HistoryAction>	m_redoAction;

	HistoryAction			m_currentAction;
	SAction					m_modifyAction;

	bool					m_disable;
	bool					m_begin;
protected:
	bool doCreate(	SAction* action, bool redo = false );
	bool doDelete(	SAction* action, bool redo = false );
	bool doModify(	SAction* action, bool redo = false );
public:
	CHistoryManager();
	virtual ~CHistoryManager();

	bool undo();
	bool redo();

	void beginHistory();
	void endHistory();

	void addHistoryCreateObj( CGameObject* pObj );
	void addHistoryDeleteObj( CGameObject* pObj );
	void addHistoryBeginModifyObj(	CGameObject* pObj );
	void addHistoryEndModifyObj(	CGameObject* pObj );

	void clearAll();
};

#endif