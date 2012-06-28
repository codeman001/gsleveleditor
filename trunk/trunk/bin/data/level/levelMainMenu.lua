debug("-----------------------------------------------------------------")
debug("compile levelMainMenu.lua")

function triggerInitLevel_always(triggerID)
	-- set main camera
	local cameraID = getObjectByName("camMain")
	setLevelCamera(cameraID)
	
	-- move to waypoint
	local wayPointID =  getObjectByName("cameraInitWayPoint")
	setAnimatorMoveToWayPoint(cameraID, wayPointID, 0.03, true)
	
	-- disable trigger
	disableObject(triggerID)
end

debug("-----------------------------------------------------------------\n\n")