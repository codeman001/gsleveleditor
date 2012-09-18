debug("-----------------------------------------------------------------")
debug("compile levelGameM1.lua")

function triggerInitLevel_enable(triggerID)
	-- setting player
	playerID = getObjectByName("player")
	clearObjectLod( playerID )	
	--addObjectLod( playerID, 3000 , "Marin-node" )
	--addObjectLod( playerID, 6000 , "Marin_lod1-node" )
	--addObjectLod( playerID, 9000 , "Marin_lod2-node" )
	
	-- setting camera
	local cameraID = getObjectByName("playerCamera")
	setLevelCamera(cameraID)
	setCameraFarValue(cameraID, 10000)
	setCameraFollowObject(cameraID, playerID, 350)
	
	-- default ambient light
	setLevelAmbientLight(100,100,100)
	
	-- add shotgun to inventory
	addItemToInventory( playerID, "weaponShotGun" )
	setActiveItemOnInventory( playerID, "weaponShotGun" )
end

debug("-----------------------------------------------------------------\n\n")