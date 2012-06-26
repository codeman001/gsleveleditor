function triggerInitLevel_always(triggerID)
	local cameraID = getObjectByName("camMain")	
	setLevelCamera(cameraID)
	disableObject(triggerID)
	debug("triggerInitLevel_always")
end