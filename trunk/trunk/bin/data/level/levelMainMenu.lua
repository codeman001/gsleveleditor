debug("-----------------------------------------------------------------")
debug("compile levelMainMenu.lua")

function triggerInitLevel_enable(triggerID)
	-- set main camera
	local cameraID = getObjectByName("camMain")
	setLevelCamera(cameraID)
	setCameraFarValue(cameraID, 10000)
	
	-- move to waypoint
	local wayPointID =  getObjectByName("cameraInitWayPoint")
	setAnimatorMoveToWayPoint(cameraID, wayPointID, 0.03, true)
	
	-- disable trigger
	-- disableObject(triggerID)
end

function triggerInitLevel_always(triggerID)
	--local cameraID = getObjectByName("camMain")
	--x,y,z = getObjectPosition(csameraID)
	--debug("Camera pos: "..x..","..y..","..z)
end

debug("-----------------------------------------------------------------\n\n")