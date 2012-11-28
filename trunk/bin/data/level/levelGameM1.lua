debug("-----------------------------------------------------------------")
debug("compile levelGameM1.lua")

function triggerInitLevel_enable(triggerID)
	-- setting player
	local playerID = getObjectByName("player")
	clearObjectLod( playerID )	
		
	-- setting camera
	local cameraID = getObjectByName("playerCamera")
	setLevelCamera(cameraID)
	setCameraFarValue(cameraID, 10000)
	setCameraFollowObject(cameraID, playerID, 350)
	
	-- default ambient light
	setLevelAmbientLight(100,100,100)	
end

debug("-----------------------------------------------------------------\n\n")