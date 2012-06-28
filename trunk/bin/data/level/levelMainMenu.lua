debug("-----------------------------------------------------------------")
debug("compile levelMainMenu.lua")

function triggerInitLevel_always(triggerID)
	local cameraID = getObjectByName("camMain")
	setLevelCamera(cameraID)
	disableObject(triggerID)
end

debug("-----------------------------------------------------------------\n\n")