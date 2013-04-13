#include "stdafx.h"
#include "CBaseGameMatch.h"

#include "gameComponent/CPlayerComponent.h"
#include "gameComponent/CSpawnPointComponent.h"

#include "gameLevel/CGameLevel.h"
#include "gameState/CGameStateManager.h"
#include "gameState/CStateIGM.h"

CBaseGameMatch::CBaseGameMatch(IGameMatch::EGameMatchType type)
	:IGameMatch(type)
{
}

CBaseGameMatch::~CBaseGameMatch()
{
}

// initUI
// init ui
void CBaseGameMatch::initUI(CMenuFxObj menufx, CUICanvas* rootWidget)
{
	IGameMatch::initUI(menufx, rootWidget);

	// create button
	m_btnPause = new CUIButton("pause", m_rootWidget, m_menufx.findObj("mcBtnPause"));
}


// update
// update match
void CBaseGameMatch::update()
{
}

// onEvent
// process event
void CBaseGameMatch::onEvent(const SEvent& event)
{
	if ( event.GameEvent.EventID == (s32)EvtButtonPress )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);			
	}
	else if ( event.GameEvent.EventID == (s32)EvtButtonRelease )
	{
		SEventButtonData *buttonEvent = ((SEventButtonData*)event.GameEvent.EventData);
		if ( buttonEvent->data == m_btnPause )
			CGameStateMgr::getInstance()->pushState( new CStateIGM() );
	}
}

// spawnPlayer
// create a player
CGameObject* CBaseGameMatch::spawn3rdPlayer()
{
	CGameLevel *currentLevel = CGameLevel::getCurrentLevel();
	if ( currentLevel == NULL )
		return NULL;

	CZone *zone = currentLevel->getActiveZone();

	// create player object
	CGameObject *obj = zone->createObjectWithNoData(L"Player");

	// set character id
	CPlayerComponent *playerCmp = (CPlayerComponent*)obj->getComponent(CGameComponent::PlayerComponent);
	playerCmp->setCharacterID(0);

	// load model + anim
	obj->initComponent();

	// set position
	core::vector3df rot, pos;
	CGameObject *spawnPointMgr = zone->getSpawnPointManager();
	CSpawnPointMgrComponent *spawnComp = (CSpawnPointMgrComponent*)spawnPointMgr->getComponent(CGameComponent::SpawnPointMgrComponent);

	spawnComp->getRandomPosition(-1, pos, rot);
	obj->setPosition(pos);
	obj->setRotation(rot);

	// create camera
	CGameCamera* cam = zone->createCamera();
	cam->setFollowObjectCamera(obj, 300);
	currentLevel->setCamera(cam);

	core::vector3df defaultCamLook(0,0,1);
	core::vector3df playerFront = obj->getFront();
	float angle = playerCmp->getAngle(defaultCamLook, playerFront);
	cam->setFollowRotate(0, angle);

	return obj;
}