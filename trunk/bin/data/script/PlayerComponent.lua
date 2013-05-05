debug("-----------------------------------------------------------------")
debug("compile PlayerComponent.lua")

-- declare player state const
k_playerStateNone		= 0		
k_playerStateStand 		= (k_playerStateNone+1)
k_playerStateRun 		= (k_playerStateStand+1)
k_playerStateFlipTurn 	= (k_playerStateRun+1)
k_playerStateRunFast 	= (k_playerStateFlipTurn+1)

k_playerUpBodyNone		= 0
k_playerUpBodyAimMC		= (k_playerUpBodyNone+1)
k_playerUpBodyShootMC	= (k_playerUpBodyAimMC+1)
k_playerUpBodyReloadMC	= (k_playerUpBodyShootMC+1)

k_playerSubStateInit	= 0
k_playerSubStateUpdate	= (k_playerSubStateInit+1)
k_playerSubStateEnd		= (k_playerSubStateUpdate+1)


-- declare player animation const
k_playerAnimIdle				= "TP_Idle"

k_playerAnimShootMachineGuns	= "TP_Shoot_MachineGuns"
k_playerAnimReloadMachineGuns	= "TP_Reload_MachineGuns"

k_playerAnimRunForward			= "TP_RunFront"
k_playerAnimRunBackward			= "TP_RunBack"
k_playerAnimRunLeft				= "TP_RunLeft"
k_playerAnimRunRight			= "TP_RunRight"
k_playerAnimRunForwardLeft		= "TP_RunFrontLeft"
k_playerAnimRunForwardRight		= "TP_RunFrontRight"
k_playerAnimRunBackwardLeft		= "TP_RunBackLeft"
k_playerAnimRunBackwardRight	= "TP_RunBackRight"

k_playerAnimRunFastForward		= "TP_RunSprint_Front"

local k_playerRunAnims = {
		k_playerAnimRunForward,
		k_playerAnimRunBackward,
		k_playerAnimRunLeft,
		k_playerAnimRunRight,
		k_playerAnimRunForwardLeft,
		k_playerAnimRunForwardRight,
		k_playerAnimRunBackwardLeft,
		k_playerAnimRunBackwardRight
}

k_animRunRotateSpeed			= 0.4
k_animStandToRunBlendSpeed		= 0.003
k_animStandToRunFastBlendSpeed	= 0.002
k_animRunToStandBlendSpeed		= 0.003
k_animFipTurnBlendSpeed			= 0.005

k_runSpeed						= 0.3
k_runFastSpeed					= 0.5

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
	newObj.m_weaponComp		= getWeaponComponent(gameObj)
	
	-- player state
	newObj.m_playerState		= k_playerStateStand
	newObj.m_playerSubState		= k_playerSubStateInit
	newObj.m_nextPlayerState	= k_playerStateNone
	
	-- upbody state
	newObj.m_playerUpBodyState		= k_playerUpBodyAimMC
	newObj.m_playerUpBodySubState	= k_playerSubStateInit
	newObj.m_nextUpbodyState		= k_playerUpBodyNone	
	
	-- input
	newObj.m_inputRun 			= 0
	newObj.m_inputRunStrength 	= 0.0
	newObj.m_inputRunRotate 	= 0.0
	newObj.m_inputShoot 		= 0
	newObj.m_inputReload		= 0	
	
	-- animation
	newObj.m_runFactor			= 0.0
	newObj.m_runFastFactor		= 0.0
	
	-- run params
	newObj.m_runVector			= irr.core.vector3d()
	newObj.m_currentRunVector	= irr.core.vector3d()
	
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
	
	-- up body
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
	self.m_nextUpbodyState 			= upbodyState
	self.m_playerUpBodySubState		= k_playerSubStateEnd
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
		self.m_playerUpBodyState	= self.m_nextUpbodyState
		self.m_playerUpBodySubState	= k_playerSubStateInit
		self.m_nextUpbodyState		= k_playerUpBodyNone
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
		[k_playerStateFlipTurn]	= function() self:updatePlayerStateFlipTurn(timeStep) end,
		[k_playerStateRunFast]	= function() self:updatePlayerStateRunFast(timeStep) end,
	}
				
	local stateUpBodyUpdate = {
		[k_playerUpBodyAimMC]		= function() self:updatePlayerUpBodyAimMC(timeStep)	end,
		[k_playerUpBodyShootMC]		= function() self:updatePlayerUpBodyShootMC(timeStep) end,
		[k_playerUpBodyReloadMC]	= function() self:updatePlayerUpBodyReloadMC(timeStep) end,
	}
	
	-- update state
	stateUpdate[self.m_playerState]()
	
	-- update upbody state
	stateUpBodyUpdate[self.m_playerUpBodyState]()
	
	-- if run fast, the main character do not shoot or reload the weapon
	if self.m_playerState == k_playerStateRunFast and self.m_playerUpBodyState ~= k_playerUpBodyAimMC then
		self:setUpbodyState(k_playerUpBodyAimMC)
	end
	
end

-----------------------------------------------------------
-- player state implement 
-----------------------------------------------------------

-- updatePlayerStateStand
-- Update when player not moving
function CPlayerComponent:updatePlayerStateStand(timeStep)
	
	if self.m_playerSubState == k_playerSubStateInit then
		-- init state
		-- set anim on layer 0
		setColladaAnimation(self.m_collada, k_playerAnimIdle, 0, true, 0)
		setColladaAnimWeight(self.m_collada, 1, 0, 0)			
		
		-- disable multi move anim
		for i=1,8 do 			
			colladaEnableAnimTrackChannel(self.m_collada, i, false, 0)
		end			
		
		self.m_playerSubState = k_playerSubStateUpdate
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
		return
	end
	 
	-- rotate character	
	if self.m_needRotateCharacter == true then
		self.m_needRotateCharacter = (self:rotatePlayerToFront(0.6) == false)
	end
	
	-- set run vector
	local frontx, fronty, frontz = getObjectFront(self.m_gameObject)	
	self.m_currentRunVector = irr.core.vector3d(frontx, fronty, frontz)
	self.m_runVector = irr.core.vector3d(frontx, fronty, frontz)
	
	-- check input
	if self.m_inputRun == 1 then
		self:setPlayerState(k_playerStateRun)
	end
end

-- updatePlayerStateRun
-- Update when player running
function CPlayerComponent:updatePlayerStateRun(timeStep)	
		
	if self.m_playerSubState == k_playerSubStateInit then		
		-- track 0
		setColladaAnimation(self.m_collada, k_playerAnimIdle, 0, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 0, true, 0)		
				
		for i=1,8 do
			setColladaAnimation(self.m_collada, k_playerRunAnims[i], i, true, 0)
			colladaEnableAnimTrackChannel(self.m_collada, i, true, 0)
		end
				
		-- default run blend
		runBlend = self:calcRunAnimationBlend(0)
		
		self.m_playerSubState = k_playerSubStateUpdate
		
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
		return
	end
	
	-- update run vector
	local frontx, fronty, frontz = getObjectFront(self.m_gameObject)
	frontVector = irr.core.vector3d(frontx, fronty, frontz)
	self.m_runVector = irr.core.vector3d(frontx, fronty, frontz)
		
	local q = irr.core.quaternion()
	q:fromAngleAxis( k_degToRad*self.m_inputRunRotate, irr.core.vector3d(0,1,0) )
	local mat = q:getMatrix()
	mat:rotateVect(self.m_runVector);
	self.m_runVector:normalize();		

	-- flip turn
	local angle = math.abs(CVectorUtil.getAngle(self.m_runVector, self.m_currentRunVector))
	if ( angle > 160 ) then
		self:setPlayerState(k_playerStateFlipTurn)
	end
		
	-- update current run vector
	local ret = CVectorUtil.turnToDir(self.m_currentRunVector, self.m_runVector, k_animRunRotateSpeed)
	self.m_currentRunVector = irr.core.vector3d(ret[2])
			
	-- if have input
	if self.m_inputRun == 1 then		
		angle = -CVectorUtil.getAngle(self.m_currentRunVector, frontVector)
		runBlend = self:calcRunAnimationBlend(angle)
	end
		
	-- set weight of run anim	
	local runFactor = self.m_runFactor
	setColladaAnimWeight(self.m_collada, 1 - runFactor, 0, 0)
	for id,weight in ipairs(runBlend) do	
		setColladaAnimWeight(self.m_collada, weight*runFactor, id, 0)
	end	
	
	-- sync animation
	colladaSynchronizedAnim(self.m_collada, 1.0, 0)
	
	-- inc run factor
	if self.m_inputRun == 1 then	
		self.m_runFactor = self.m_runFactor + timeStep*k_animStandToRunBlendSpeed
	else
		self.m_runFactor = self.m_runFactor - timeStep*k_animRunToStandBlendSpeed
	end
	
	-- clamp from 0 to 1
	if self.m_runFactor >= 1.0 then
		self.m_runFactor = 1.0
	elseif self.m_runFactor <= 0.0 then
		self.m_runFactor = 0.0
		self:setPlayerState(k_playerStateStand)
	end		
	
	-- rotate character	
	if self.m_needRotateCharacter == true then
		self.m_needRotateCharacter = (self:rotatePlayerToFront(0.6) == false)
	end		
	
	-- move character
	local posX, posY, posZ = getObjectPosition(self.m_gameObject)
	posX = posX + self.m_runFactor*self.m_currentRunVector.X*timeStep*k_runSpeed
	posY = posY + self.m_runFactor*self.m_currentRunVector.Y*timeStep*k_runSpeed
	posZ = posZ + self.m_runFactor*self.m_currentRunVector.Z*timeStep*k_runSpeed	
	
	setObjectPosition(self.m_gameObject, posX, posY, posZ)
	
	-- change to state run fast
	if self.m_inputRunStrength > 1.0 then
		self:setPlayerState(k_playerStateRunFast)
	end
	
end

function CPlayerComponent:updatePlayerStateRunFast(timeStep)

	if self.m_playerSubState == k_playerSubStateInit then						
		-- default run blend
		runBlend = self:calcRunAnimationBlend(0)

		setColladaAnimation(self.m_collada, k_playerAnimRunFastForward, 9, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 9, true, 0)
		
		self.m_runFastFactor = 0.0		
		self.m_playerSubState = k_playerSubStateUpdate		
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
		return
	end
	
	-- update run vector
	local currentCamera = getActiveCamera()
		
	local x1,y1,z1, x2,y2,z2	= getCameraRay(currentCamera)	
	local posx, posy, posz 		= getObjectPosition(self.m_gameObject)
	
	local frontVector = irr.core.vector3d(x2 - posx, y2 - posy, z2 - posz)
	frontVector.Y = 0
	frontVector:normalize()
		
	self.m_runVector = irr.core.vector3d(frontVector)
		
	local q = irr.core.quaternion()
	q:fromAngleAxis( k_degToRad*self.m_inputRunRotate, irr.core.vector3d(0,1,0) )
	local mat = q:getMatrix()
	mat:rotateVect(self.m_runVector);
	self.m_runVector:normalize();		

	-- flip turn
	local angle = math.abs(CVectorUtil.getAngle(self.m_runVector, self.m_currentRunVector))
	if ( angle > 160 ) then
		-- todo late		
	end
		
	-- update current run vector
	local ret = CVectorUtil.turnToDir(self.m_currentRunVector, self.m_runVector, k_animRunRotateSpeed)
	self.m_currentRunVector = irr.core.vector3d(ret[2])
			
	-- if have input
	if self.m_inputRun == 1 then
		angle = -CVectorUtil.getAngle(self.m_currentRunVector, frontVector)
		runBlend = self:calcRunAnimationBlend(angle)
	end
		
	-- set weight of run anim
	local runFactor 	= self.m_runFactor*(1.0 - self.m_runFastFactor)
	local runFastFactor = self.m_runFactor*self.m_runFastFactor
	
	setColladaAnimWeight(self.m_collada, 1 - self.m_runFactor, 0, 0)
	for id,weight in ipairs(runBlend) do
		setColladaAnimWeight(self.m_collada, weight*runFactor, id, 0)
	end	
	setColladaAnimWeight(self.m_collada, runFastFactor, 9, 0)
		
	
	-- sync animation
	colladaSynchronizedAnim(self.m_collada, 1.0, 0)
	
	-- inc run factor
	if self.m_inputRun == 1 then	
		self.m_runFactor = self.m_runFactor + timeStep*k_animStandToRunBlendSpeed
		
		-- inc run fast factor
		if self.m_inputRunStrength > 1.0 then
			self.m_runFastFactor = self.m_runFastFactor + timeStep*k_animStandToRunFastBlendSpeed
		else
			self.m_runFastFactor = self.m_runFastFactor - timeStep*k_animStandToRunFastBlendSpeed
		end
		
	else
		self.m_runFactor = self.m_runFactor - timeStep*k_animRunToStandBlendSpeed
		self.m_runFastFactor = self.m_runFastFactor - timeStep*k_animStandToRunFastBlendSpeed
	end
	
	-- clamp from 0 to 1
	if self.m_runFactor >= 1.0 then
		self.m_runFactor = 1.0	
	elseif self.m_runFactor <= 0.0 then
		self.m_runFactor = 0.0
		self:setPlayerState(k_playerStateStand)
		return
	end
	
	if self.m_runFastFactor >= 1.0 then
		self.m_runFastFactor = 1.0
	elseif self.m_runFastFactor <= 0.0 then
		self.m_runFastFactor = 0.0
		self:setPlayerState(k_playerStateRun)
		return
	end
	
	-- rotate character to move
	if self.m_inputRunStrength > 1.0 then
		self:rotatePlayerToVector(self.m_currentRunVector, 0.6)
	else
		self:rotatePlayerToFront(0.6)
	end
		
	-- move character
	local posX, posY, posZ = getObjectPosition(self.m_gameObject)
	posX = posX + self.m_runFactor*self.m_currentRunVector.X*timeStep*k_runFastSpeed
	posY = posY + self.m_runFactor*self.m_currentRunVector.Y*timeStep*k_runFastSpeed
	posZ = posZ + self.m_runFactor*self.m_currentRunVector.Z*timeStep*k_runFastSpeed	
	
	setObjectPosition(self.m_gameObject, posX, posY, posZ)
		
end

-- updatePlayerStateFlipTurn
-- update when player force change the run turn
function CPlayerComponent:updatePlayerStateFlipTurn(timeStep) 
	if self.m_playerSubState == k_playerSubStateInit then		
		-- track 0
		setColladaAnimation(self.m_collada, k_playerAnimIdle, 0, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 0, true, 0)		
				
		for i=1,8 do
			setColladaAnimation(self.m_collada, k_playerRunAnims[i], i, true, 0)
			colladaEnableAnimTrackChannel(self.m_collada, i, true, 0)
		end		
		
		self.m_playerSubState = k_playerSubStateUpdate
		
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
		return
	end
	
		
	-- update run vector
	local frontx, fronty, frontz = getObjectFront(self.m_gameObject)
	frontVector = irr.core.vector3d(frontx, fronty, frontz)	
			
	-- if have input
	angle = -CVectorUtil.getAngle(self.m_currentRunVector, frontVector)
	runBlend = self:calcRunAnimationBlend(angle)
	
	-- set weight of run anim
	local runFactor = self.m_runFactor
	setColladaAnimWeight(self.m_collada, 1 - runFactor, 0, 0)
	for id,weight in ipairs(runBlend) do
		setColladaAnimWeight(self.m_collada, weight*runFactor, id, 0)		
	end
	
	-- sync animation
	colladaSynchronizedAnim(self.m_collada, 1.0, 0)			
	
	-- stop run animation
	self.m_runFactor = self.m_runFactor - timeStep*k_animFipTurnBlendSpeed
	if self.m_runFactor <= 0 then
		self.m_runFactor = 0
		
		-- call run vector
		self.m_runVector = irr.core.vector3d(frontx, fronty, frontz)		
		local q = irr.core.quaternion()
		q:fromAngleAxis( k_degToRad*self.m_inputRunRotate, irr.core.vector3d(0,1,0) )
		local mat = q:getMatrix()
		mat:rotateVect(self.m_runVector);
		self.m_runVector:normalize();
	
		-- force turn
		self.m_currentRunVector = self.m_runVector
		
		-- return state run
		self:setPlayerState(k_playerStateRun)
	end
end

-----------------------------------------------------------
-- upbody state implement 
-----------------------------------------------------------


-- updatePlayerUpBodyAim
-- Update aim machine gun
function CPlayerComponent:updatePlayerUpBodyAimMC(timeStep)
	
	if self.m_playerUpBodySubState == k_playerSubStateInit then		
		self.m_playerUpBodySubState = k_playerSubStateUpdate		
	elseif self.m_playerUpBodySubState == k_playerSubStateEnd then
		self:doNextUpBodyState()
		return
	end
		
	self.m_needRotateCharacter = true
	
	-- if player is run fast, player can not reload or shoot
	if self.m_playerState ~= k_playerStateRunFast then	
		if self.m_inputReload == 1 then		
			self:setUpbodyState(k_playerUpBodyReloadMC)
		elseif self.m_inputShoot == 1 then		
			self:setUpbodyState(k_playerUpBodyShootMC)		
		end			
	end
	
	-- do not shoot the gun
	shootActiveWeapon(self.m_weaponComp, false)	
end

-- updatePlayerUpBodyShootMC
-- update state when shoot machine gun
function CPlayerComponent:updatePlayerUpBodyShootMC(timeStep)
	
	if self.m_playerUpBodySubState == k_playerSubStateInit then		
		self.m_playerUpBodySubState = k_playerSubStateUpdate		
	elseif self.m_playerUpBodySubState == k_playerSubStateEnd then		
		self:doNextUpBodyState()
		return
	end
		
	self.m_needRotateCharacter = true
	
	if self.m_inputShoot == 1 then
		-- todo shoot
		shootActiveWeapon(self.m_weaponComp, true)
		
		-- reload weapon (if bullet count = 0)
		if needReloadActiveWeapon(self.m_weaponComp) == true then			
			self:setUpbodyState(k_playerUpBodyReloadMC)			
		end
		
	elseif self.m_inputReload == 1 then
		self:setUpbodyState(k_playerUpBodyReloadMC)
	else
		self:setUpbodyState(k_playerUpBodyAimMC)
	end
	
end

-- updatePlayerUpBodyReloadMC
-- update state when reload machine gun
function CPlayerComponent:updatePlayerUpBodyReloadMC(timeStep)
	
	if self.m_playerUpBodySubState == k_playerSubStateInit then		
		
		-- begin reload the gun
		reloadActiveWeapon(self.m_weaponComp, true)
		
		-- active layer 2 of reload animation
		---------------------------------------------------------				
		setColladaAnimationLayer(self.m_collada, self.m_allUpbodyNodes, 1)
		enableColladaAnimationLayer(self.m_collada, 1, true)
		setSceneNodeIsJoinAnimLayer(self.m_collada, "Bip01_Spine1-node", true, true)		
		
		-- set animation reload		
		setColladaAnimation(self.m_collada, k_playerAnimReloadMachineGuns, 0, true, 1)
		setColladaAnimLoop(self.m_collada, false, 0, 1)
		
		self.m_playerUpBodySubState = k_playerSubStateUpdate
		return
		
	elseif self.m_playerUpBodySubState == k_playerSubStateEnd then
	
		-- disable layer 2
		setColladaAnimationLayer(self.m_collada, self.m_allUpbodyNodes, 0)
		enableColladaAnimationLayer(self.m_collada, 1, false)
		
		self:doNextUpBodyState()
		return
	end
		
	self.m_needRotateCharacter = true
	
	if isColladaEndAnimation(self.m_collada, 0, 1) == true then
		-- end reload the gun
		reloadActiveWeapon(self.m_weaponComp, false)
	
		-- change to aim state
		self:setUpbodyState(k_playerUpBodyAimMC)
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
	local x1,y1,z1, x2,y2,z2 		= getCameraRay(currentCamera)	
	local posx, posy, posz 			= getObjectPosition(self.m_gameObject)
	
	local aimPos = irr.core.vector3d(x2 - posx, y2 - posy, z2 - posz)
	aimPos.Y = 0
	aimPos:normalize()

	local v0 = irr.core.vector3d(frontx, fronty, frontz)
	ret = CVectorUtil.turnToDir(v0, aimPos, rotStep)
	
	local lookAt = ret[2];
	setObjectLookAtPos(self.m_gameObject, lookAt.X, lookAt.Y, lookAt.Z )
	
	return ret[1]
end

-- rotatePlayerToVector
-- rotate player to vector
function CPlayerComponent:rotatePlayerToVector(vector, rotStep)
	vector:normalize()
	
	local frontx, fronty, frontz = getObjectFront(self.m_gameObject)
	local v0 = irr.core.vector3d(frontx, fronty, frontz)
	ret = CVectorUtil.turnToDir(v0, vector, rotStep)
	
	local lookAt = ret[2];
	setObjectLookAtPos(self.m_gameObject, lookAt.X, lookAt.Y, lookAt.Z )	
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

-- calcRunAnimationBlend
-- calc animation
function CPlayerComponent:calcRunAnimationBlend(rot)
	local forward = 0.0
	local backward = 0.0
	local left = 0.0
	local right = 0.0
    local forwardleft = 0.0
	local forwardright = 0.0
	local backwardleft = 0.0
	local backwardright = 0.0
	
	
	-- 8 MOVE ANIMATION	
	if -90.0 <= rot and rot < 90.0 then		
		if 0.0 <= rot and rot < 45.0 then
			-- forward & forwardleft
			forwardleft = rot/45.0
			forward = 1.0 - forwardleft					
		elseif 45.0 <= rot and rot < 90.0 then			
			-- left & forwardleft
			left = (rot - 45.0)/45.0
			forwardleft = 1.0 - left
		elseif -45.0 <= rot and rot < 0.0 then
			-- forward & forwardright
			forwardright = rot/-45.0
			forward = 1.0 - forwardright
		else		
			-- -90 <= rot and rot < -45
			-- right & forwardright
			right = (rot + 45.0)/(-45.0)
			forwardright = 1.0 - right
		end
		
	else
		if 90.0 <= rot and rot < 135.0 then		
			-- left & backwardleft
			backwardleft = (rot-90)/45.0
			left = 1.0 - backwardleft
		elseif 135.0 <= rot and rot < 180.0 then
			-- backward & backwardleft
			backward = (rot - 135.0)/45.0
			backwardleft = 1.0 - backward
		elseif -90.0 >= rot and rot > -135 then		
			-- right & backwardright
			backwardright = (rot+90)/(-45.0)
			right = 1.0 - backwardright
		else		
			-- -135 >= rot & rot > -180
			-- backward & backwardright
			backward = (rot + 135)/(-45.0)
			backwardright = 1.0 - backward
		end
	end
	
	return {forward, backward, left, right, forwardleft, forwardright, backwardleft, backwardright}
	
	--[[
	-- 4 MOVE ANIMATION
	if -90.0 <= rot and rot <= 90.0 then
		-- move forward		
		backward = 0.0;
		if rot <= 0.0 and rot <= 90.0 then		
			-- right
			left = 0.0;
            
			local fixAngle	= k_degToRad*math.mod(rot + 90.0, 360.0);
			local dForward	= math.abs(math.sin(fixAngle));
            
			-- we have sin2 + cos2 = 1
            -- it mean dforward2 + dright2 = 1.0
            forward = dForward*dForward;
			right   = 1.0 - forward;		
		else		
			-- left
			right = 0.0;
            
			local fixAngle	= k_degToRad*math.mod(rot + 90.0, 360.0);
			local dForward	= math.abs(math.sin(fixAngle));
			
            forward = dForward*dForward;
            left = 1.0 - forward;
		end
	else	
		-- move back
		forward = 0.0;
		if 90.0 <= rot and rot <= 180.0 then
			-- left
			local right = 0.0;
            
			local fixAngle	= k_degToRad*math.mod(rot + 90.0, 360.0);
			local dBackward	= math.abs(math.sin(fixAngle));			
            
            backward    = dBackward*dBackward;
            left        = 1.0- backward;		
		else		
			-- right
			left = 0.0;
            
			local fixAngle	= k_degToRad*math.mod(rot + 90.0, 360.0);
			local dBackward	= math.abs(math.sin(fixAngle));
			
            backward    = dBackward*dBackward;
            right       = 1.0- backward;
		end
	end
    
	return {forward, backward, left, right}
	--]]
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