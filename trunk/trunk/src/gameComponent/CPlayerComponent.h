#ifndef _CPLAYERCOMPONENT_H_
#define _CPLAYERCOMPONENT_H_

#include "IObjectComponent.h"
#include "CColladaMeshComponent.h"
#include "CGameComponent.h"


class CPlayerComponent: public IObjectComponent
{
public:
	struct SPlayerState
	{
		int				stateID;
		std::string		beginAnimation;
		std::string		animation;
		std::string		endAnimation;
		float			moveSpeed;
	};

	enum EPlayerState
	{
		PlayerIdle = 0,
		PlayerWalk,
		PlayerRun,
		NumStateCount
	};

protected:
	vector<SPlayerState>	m_playerState;	
	EPlayerState			m_state;
public:
	CPlayerComponent(CGameObject* obj);
	virtual ~CPlayerComponent();

	// init
	// run when init object
	virtual void initComponent();

	// update
	// run when update per frame
	virtual void updateComponent();

	// saveData
	// save data to serializable
	virtual void saveData( CSerializable* pObj );

	// loadData
	// load data to serializable
	virtual void loadData( CSerializable* pObj );

public:
		

};

#endif