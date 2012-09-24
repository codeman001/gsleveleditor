LOCAL_C_INCLUDES := ../../../src \
../../../../extern/irrogles/include \
../../../../extern/irrogles/source/Irrlicht \
../../../src/core \
../../../src/core/Animators \
../../../src/core/Component \
../../../src/core/GameObject \
../../../src/core/ObjectTemplate \
../../../src/core/SceneNode \
../../../src/core/Network \
../../../src/core/Serializable \
../../../../extern/spark/include \
../../../../extern/gameswf \
../../../../extern/freetype/include \
../../../../extern/lua/src \
../../../../extern/vld/include 

LOCAL_CFLAGS := -DGSGAMEPLAY \
-DANDROID 

LOCAL_SRC_FILES := ../../../src/core/Animators/CGameCameraFollowAnimator.cpp \
../../../src/core/Animators/CGameGSCameraAnimator.cpp \
../../../src/core/Component/CAnimMeshComponent.cpp \
../../../src/core/Component/CBillboardComponent.cpp \
../../../src/core/Component/CBoxObjectComponent.cpp \
../../../src/core/Component/CColladaMeshComponent.cpp \
../../../src/core/Component/CComponentFactory.cpp \
../../../src/core/Component/CEllipsoidCollisionComponent.cpp \
../../../src/core/Component/CGrassComponent.cpp \
../../../src/core/Component/CLightingComponent.cpp \
../../../src/core/Component/CObjectCollisionComponent.cpp \
../../../src/core/Component/CObjectPropertyComponent.cpp \
../../../src/core/Component/CObjectTransformComponent.cpp \
../../../src/core/Component/CParticleComponent.cpp \
../../../src/core/Component/CShadowComponent.cpp \
../../../src/core/Component/CSkyboxComponent.cpp \
../../../src/core/Component/CStaticMeshComponent.cpp \
../../../src/core/Component/CTerrainComponent.cpp \
../../../src/core/Component/CWaterComponent.cpp \
../../../src/core/Component/IObjectComponent.cpp \
../../../src/core/GameObject/CGameCamera.cpp \
../../../src/core/GameObject/CGameObject.cpp \
../../../src/core/GameObject/CLightObject.cpp \
../../../src/core/GameObject/CTrigger.cpp \
../../../src/core/GameObject/CWayPoint.cpp \
../../../src/core/GameObject/CZone.cpp \
../../../src/core/ObjectTemplate/CObjectTemplate.cpp \
../../../src/core/ObjectTemplate/CObjTemplateFactory.cpp \
../../../src/core/SceneNode/CGameAnimatedMeshSceneNode.cpp \
../../../src/core/SceneNode/CGameBillboardSceneNode.cpp \
../../../src/core/SceneNode/CGameBoxSceneNode.cpp \
../../../src/core/SceneNode/CGameColladaSceneNode.cpp \
../../../src/core/SceneNode/CGameContainerSceneNode.cpp \
../../../src/core/SceneNode/CGameDebugSceneNode.cpp \
../../../src/core/SceneNode/CGameGrassSceneNode.cpp \
../../../src/core/SceneNode/CGameMeshSceneNode.cpp \
../../../src/core/SceneNode/CGameOxySceneNode.cpp \
../../../src/core/SceneNode/CGameParticleContainerSceneNode.cpp \
../../../src/core/SceneNode/CGameWaterSceneNode.cpp \
../../../src/core/Serializable/CBinaryUtils.cpp \
../../../src/core/Serializable/CSerializable.cpp \
../../../src/core/Network/CComms.cpp \
../../../src/core/Network/CDataPacket.cpp \
../../../src/core/Network/CMultiplayerManager.cpp \
../../../src/gameComponent/CGameComponent.cpp \
../../../src/gameComponent/CInventoryComponent.cpp \
../../../src/gameComponent/CPlayerComponent.cpp \
../../../src/gameComponent/CWeaponComponent.cpp \
../../../src/gameDebug/CGameDebug.cpp \
../../../src/gameState/CGameState.cpp \
../../../src/gameState/CGameStateManager.cpp \
../../../src/gameState/CStateGameLoading.cpp \
../../../src/gameState/CStateGameplay.cpp \
../../../src/gameState/CStateInit.cpp \
../../../src/gameState/CStateMainMenu.cpp \
../../../src/script/CScriptManager.cpp \
../../../src/swfUI/CGameUI.cpp \
../../../src/swfUI/CMenuFx.cpp \
../../../src/swfUI/CMenuFxObj.cpp \
../../../src/IViewImp.cpp \
../../../src/gameLevel/CGameLevel.cpp \
../../../src/gameLevel/LevelScript.cpp \
../../../src/gameControl/CDpad.cpp \
../../../src/gameControl/CGameControl.cpp \
../../../src/gameControl/CTouchManager.cpp \
../native/AppInterface.cpp \
../../../src/CApplication.cpp \
../../../src/stdafx.cpp \
../native/NativeInterface.c 

