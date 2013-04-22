debug("-----------------------------------------------------------------")
debug("compile PlayerComponent.lua")

-- declare player state const
k_playerStateNone		= 0		
k_playerStateStand 		= (k_playerStateNone+1)
k_playerStateRun 		= (k_playerStateStand+1)

k_playerUpBodyNone		= 0
k_playerUpBodyAim		= (k_playerUpBodyNone+1)
k_playerUpBodyShoot		= (k_playerUpBodyAim+1)
k_playerUpBodyReload	= (k_playerUpBodyShoot+1)

k_playerSubStateInit	= 0
k_playerSubStateUpdate	= (k_playerSubStateInit+1)
k_playerSubStateEnd		= (k_playerSubStateUpdate+1)


-- declare player animation const
k_playerAnimShootMachineGuns	= "TP_Shoot_MachineGuns"
k_playerAnimRunForward			= "TP_RunFront"
k_playerAnimRunBackward			= "TP_RunBack"
k_playerAnimRunLeft				= "TP_RunLeft"
k_playerAnimRunRight			= "TP_RunRight"


-- class CPlayerComponent
CPlayerComponent = {}
CPlayerComponent.__index = CPlayerComponent

-- list of all players
CPlayerComponentList = {}

-- class CPlayerComponent
-- constructor
function CPlayerComponent.create(gameObj)
	local newObj = {}
	setmetatable(newObj, CPlayerComponent)
	
	debug("CPlayerComponent init")
	
	-- game object
	newObj.m_gameObject 	= gameObj
	newObj.m_collada		= getObjectCollada(gameObj)	
	newObj.m_playerComp		= getPlayerComponent(gameObj)
	
	-- player state
	newObj.m_playerState		= k_playerStateStand
	newObj.m_playerSubState		= k_playerSubStateInit
	newObj.m_nextPlayerState	= k_playerStateNone
	
	-- upbody state
	newObj.m_playerUpState		= k_playerUpBodyAim
	newObj.m_playerSubUpState	= k_playerSubStateInit
	newObj.m_nextUpbodyState	= k_playerUpBodyNone	
	
	-- input
	newObj.m_inputRun 			= 0
	newObj.m_inputRunStrength 	= 0.0
	newObj.m_inputRunRotate 	= 0.0
	newObj.m_inputShoot 		= 0
	newObj.m_inputReload		= 0
	
	-- spine
	newObj.m_spineRotX				= 0.0
	newObj.m_needRotateCharacter	= false
	
	-- get list scene node
	newObj.m_allSceneNodes		= {}
	newObj.m_allUpbodyNodes		= {}
	newObj.m_allFootNodes		= {}
	
	---------------------------------------------------------
	-- add root node
	table.insert(newObj.m_allSceneNodes, getColladaSceneNode(newObj.m_collada, "BoneRoot"))		
	local allChildSceneNode = getChildsOfColladaSceneNode(newObj.m_collada, "BoneRoot")
	for id,node in ipairs(allChildSceneNode) do
		table.insert(newObj.m_allSceneNodes, node)		
	end		
	debug("Total bone nodes of player: " .. table.getn(newObj.m_allSceneNodes) )
	
	---------------------------------------------------------
	-- add upbody node
	table.insert(newObj.m_allUpbodyNodes, getColladaSceneNode(newObj.m_collada, "Bip01_Spine1-node"))
	local allUpBodySceneNode = getChildsOfColladaSceneNode(newObj.m_collada, "Bip01_Spine1-node")
	for id,node in ipairs(allUpBodySceneNode) do
		table.insert(newObj.m_allUpbodyNodes, node)
	end	
	debug("Total upbody nodes of player: " .. table.getn(newObj.m_allUpbodyNodes) )
	
	---------------------------------------------------------
	-- add foot node
	for id,node in ipairs(newObj.m_allSceneNodes) do
		local isFootNode = true
		
		-- find in upbody nodes
		for idUp,upNode in ipairs(newObj.m_allUpbodyNodes) do
			if node == upNode then
				isFootNode = false
				break
			end
		end
		
		-- add to upbody node
		if isFootNode == true then
			table.insert(newObj.m_allFootNodes, node)			
		end		
	end
	debug("Total foot nodes of player: " .. table.getn(newObj.m_allFootNodes) )
	
	---------------------------------------------------------
	-- setup animation lauer
	setColladaAnimationLayer(newObj.m_collada, newObj.m_allFootNodes, 0)
	setColladaAnimationLayer(newObj.m_collada, newObj.m_allUpbodyNodes, 1)
	
	enableColladaAnimationLayer(newObj.m_collada, 0, true)
	enableColladaAnimationLayer(newObj.m_collada, 1, true)
	
	setSceneNodeIsJoinAnimLayer(newObj.m_collada, "Bip01_Spine1-node", true, false, false, false)
	applyModifyPlayerBoneTransform(newObj.m_playerComp,"Bip01_Spine1-node", "updatePlayerComponentBoneTransformCallback");
	
	return newObj;
end

-----------------------------------------------------------
-- common function implement 
-----------------------------------------------------------

-- setPlayerState
-- change state of player
function CPlayerComponent:setPlayerState( playerState )
	self.m_nextPlayerState 		= playerState
	self.m_playerSubState		= k_playerSubStateEnd
end

-- setUpbodyState
-- change state of player
function CPlayerComponent:setUpbodyState( upbodyState )
	self.m_nextUpbodyState 		= upbodyState
	self.m_playerSubUpState		= k_playerSubStateEnd
end

-- doNextState
-- force change state
function CPlayerComponent:doNextState()
	if self.m_nextPlayerState ~= k_playerStateNone then
		self.m_playerState 		= self.m_nextPlayerState
		self.m_playerSubState 	= k_playerSubStateInit
		self.m_nextPlayerState	= k_playerStateNone
	end
end

-- doNextUpBodyState
-- force change upbody state
function CPlayerComponent:doNextUpBodyState()
	if self.m_nextUpbodyState ~= k_playerUpBodyNone then
		newObj.m_playerUpState		= self.m_nextUpbodyState
		newObj.m_playerSubUpState	= k_playerSubStateInit
		newObj.m_nextUpbodyState	= k_playerUpBodyNone
	end
end




-----------------------------------------------------------
-- update state implement 
-----------------------------------------------------------

-- updateState
-- update frame by frame
function CPlayerComponent:updateState(timeStep)

	local stateUpdate = {
		[k_playerStateStand] 	= function() self:updatePlayerStateStand(timeStep)	end,
		[k_playerStateRun]		= function() self:updatePlayerStateRun(timeStep) 	end,
	}
				
	local stateUpBodyUpdate = {
		[k_playerUpBodyAim]		= function() self:updatePlayerUpBodyAim(timeStep)	end,
		[k_playerUpBodyShoot]	= function() end,
		[k_playerUpBodyReload]	= function() end,
	}
	
	-- update state
	stateUpdate[self.m_playerState]();
	
	-- update upbody state
	stateUpBodyUpdate[self.m_playerUpState]();
	
end

-- updatePlayerStateStand
-- Update when player not moving
function CPlayerComponent:updatePlayerStateStand(timeStep)
	
	if self.m_playerSubState == k_playerSubStateInit then
		-- init state
		-- set anim on layer 0
		setColladaAnimation(self.m_collada, k_playerAnimShootMachineGuns, 0, true, 0)
		pauseColladaAnimAtFrame(self.m_collada, 0, 0, 0)
		
		self.m_playerSubState = k_playerSubStateUpdate
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self.doNextState()
	end
	 
	-- rotate character	
	if self.m_needRotateCharacter == true then
		self.m_needRotateCharacter = (self:rotatePlayerToFront(6.0) == false)
	end
	
	-- check input
end

-- updatePlayerStateRun
-- Update when player running
function CPlayerComponent:updatePlayerStateRun(timeStep)
	
end


-- updatePlayerUpBodyAim
-- Update aim
function CPlayerComponent:updatePlayerUpBodyAim(timeStep)

	if self.m_playerSubUpState == k_playerSubStateInit then
		-- init state
		-- set anim on layer 1
		setColladaAnimation(self.m_collada, k_playerAnimShootMachineGuns, 0, true, 1)
		pauseColladaAnimAtFrame(self.m_collada, 0, 0, 1)
		
		self.m_playerSubUpState = k_playerSubStateUpdate
	elseif self.m_playerSubUpState == k_playerSubStateEnd then
		-- end state
		self.doNextUpBodyState()
	end

	-- rotate spine
	local angle = self:getAngleFromPlayerFrontToCameraView()
	self.m_spineRotX = -angle
		
	-- rotate character if the angle is too large
	if math.abs(angle) >= 45 then
		self.m_needRotateCharacter = true
	end
	
end

-----------------------------------------------------------
-- player support function implement
-----------------------------------------------------------

-- rotatePlayerToFront
-- Rotate player to front of camera
function CPlayerComponent:rotatePlayerToFront(rotStep)
	local currentCamera = getActiveCamera()
	
	local frontx, fronty, frontz	= getObjectFront(self.m_gameObject)
	local x1,x2,x3, y1,y2,y3 		= getCameraRay(currentCamera)
	local colx, coly, colz 			= getLevelCollision(x1,x2,x3, y1,y2,y3)
	local posx, posy, posz 			= getObjectPosition(self.m_gameObject)
	
	local aimPos = irr.core.vector3d(colx - posx, coly - posy, colz - posz)
	aimPos.Y = 0
	aimPos:normalize()

	local v0 = irr.core.vector3d(frontx, fronty, frontz)
	ret = CVectorUtil.turnToDir(v0, aimPos, rotStep)
	
	local lookAt = ret[2];
	setObjectLookAtPos(self.m_gameObject, lookAt.X, lookAt.Y, lookAt.Z )
	
	return ret[1]
end

-- getAngleFromPlayerFrontToCameraView
-- get angle from camera view & player front
function CPlayerComponent:getAngleFromPlayerFrontToCameraView()
	local currentCamera = getActiveCamera()
	local frontx, fronty, frontz	= getObjectFront(self.m_gameObject)
	local x1,x2,x3, y1,y2,y3 		= getCameraRay(currentCamera)
	local colx, coly, colz 			= getLevelCollision(x1,x2,x3, y1,y2,y3)
	local posx, posy, posz 			= getObjectPosition(self.m_gameObject)
	
	local cameraFront = irr.core.vector3d(colx - posx, coly - posy, colz - posz)
	cameraFront.Y = 0
	cameraFront:normalize()
	
	local playerFront = irr.core.vector3d(frontx, fronty, frontz)
	
	return CVectorUtil.getAngle(playerFront, cameraFront)	
end

-----------------------------------------------------------
-- player bone callback implement
-----------------------------------------------------------

-- boneTransformCallback
-- call when apply modify bone transform
function CPlayerComponent:boneTransformCallback(boneName)
	-- new quanternion
	local q = irr.core.quaternion()
	
	if boneName == "Bip01_Spine1-node" then		
		-- rot x
		q:fromAngleAxis(k_degToRad*self.m_spineRotX, irr.core.vector3d(1,0,0));
	end
	
	return q
end

-----------------------------------------------------------
-- C/C++ call interface 
-----------------------------------------------------------

-- createPlayerComponent
-- init new component
function createPlayerComponent(objectName, gameObj)
	local obj = CPlayerComponent.create(gameObj)
	CPlayerComponentList[objectName] = obj;
end

-- releasePlayerComponent
-- release player
function releasePlayerComponent(objectName)
	CPlayerComponentList[objectName] = nil
end

-- updatePlayerComponent
-- update frame by frame
function updatePlayerComponent(objectName, timeStep)
	if CPlayerComponentList[objectName] ~= nil then
		CPlayerComponentList[objectName]:updateState(timeStep)
	end
end

-- updatePlayerComponentInput
-- update move input
function updatePlayerComponentInput(objectName, isRun, runStrength, runRotate, shoot, reload)
	if CPlayerComponentList[objectName] ~= nil then
		local playerComp = CPlayerComponentList[objectName]
		
		-- update input
		playerComp.m_inputRun 			= isRun
		playerComp.m_inputRunStrength 	= runStrength
		playerComp.m_inputRunRotate 	= runRotate
		playerComp.m_inputShoot 		= shoot
		playerComp.m_inputReload		= reload
		
	end
end

-- updatePlayerComponentBoneTransformCallback
-- update when bone transform
function updatePlayerComponentBoneTransformCallback(objectName, boneName)
	-- implement bone transform callback
	if CPlayerComponentList[objectName] ~= nil then
		return CPlayerComponentList[objectName]:boneTransformCallback(boneName)
	end		
	return irr.core.quaternion()
end

debug("-----------------------------------------------------------------\n\n")