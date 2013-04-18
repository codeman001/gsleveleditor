debug("-----------------------------------------------------------------")
debug("compile levelMainMenu.lua")

function triggerInitLevel_enable(triggerID)
	-- set main camera
	local cameraID = getObjectByName("CamMain")
	setLevelCamera(cameraID)
	setCameraFarValue(cameraID, 10000)
	
	-- camera move to waypoint
	local wayPointID =  getObjectByName("CameraInitWayPoint")
	setAnimatorMoveToWayPoint(cameraID, wayPointID, 0.03, true)
	
	-- set skybox
	local levelObjectID = getObjectByName("LevelMainMenu")
	local colladaID = getObjectCollada(levelObjectID)
	
	setSceneNodeIsSkydome(colladaID,"Object01-node")
	
	-- default ambient light
	setLevelAmbientLight(50,50,50)
end

function triggerSkybox_always(triggerID)
	-- get current camera pos
	local x,y,z = getCurrentCameraPosition()
	
	-- set skybox position
	local levelObjectID = getObjectByName("LevelMainMenu")
	local colladaID = getObjectCollada(levelObjectID)
	
	setSceneNodePosition(colladaID,"Object01-node",x,y,z)
end

debug("-----------------------------------------------------------------\n\n")