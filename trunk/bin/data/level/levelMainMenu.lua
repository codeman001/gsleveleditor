debug("-----------------------------------------------------------------")
debug("compile levelMainMenu.lua")

function triggerInitLevel_enable(triggerID)
	-- set main camera
	local cameraID = getObjectByName("camMain")
	setLevelCamera(cameraID)
	setCameraFarValue(cameraID, 10000)
	
	-- camera move to waypoint
	local wayPointID =  getObjectByName("cameraInitWayPoint")
	setAnimatorMoveToWayPoint(cameraID, wayPointID, 0.03, true)
	
	-- set skybox
	local levelObjectID = getObjectByName("levelMainMenu")
	setSceneNodeIsSkydome(levelObjectID,"Object01-node")
	
	-- default ambient light
	setLevelAmbientLight(50,50,50)
end

function triggerSkybox_always(triggerID)
	-- get current camera pos
	x,y,z = getCurrentCameraPosition()
	
	-- set skybox position
	local levelObjectID = getObjectByName("levelMainMenu")
	setSceneNodePosition(levelObjectID,"Object01-node",x,y,z)
end

debug("-----------------------------------------------------------------\n\n")