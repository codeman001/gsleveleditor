debug("-----------------------------------------------------------------")
debug("compile PlayerComponent.lua")

-- declare player state const
k_playerStateNone		= 0		
k_playerStateStand 		= (k_playerStateNone+1)
k_playerStateRun 		= (k_playerStateStand+1)
k_playerStateFlipTurn 	= (k_playerStateRun+1)

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

k_animRunRotateSpeed			= 0.4
k_animStandToRunBlendSpeed		= 0.003
k_animRunToStandBlendSpeed		= 0.003
k_animFipTurnBlendSpeed			= 0.005

k_runSpeed						= 0.4


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
	
	-- animation
	newObj.m_runFactor			= 0.0	
	
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
	
	-- head
	--table.insert(newObj.m_allUpbodyNodes, getColladaSceneNode(newObj.m_collada, "Bip01_Head-node"))
	--local allUpBodySceneNode = getChildsOfColladaSceneNode(newObj.m_collada, "Bip01_Head-node")
	--for id,node in ipairs(allUpBodySceneNode) do
	--	table.insert(newObj.m_allUpbodyNodes, node)
	--end	
	-- right hand
	table.insert(newObj.m_allUpbodyNodes, getColladaSceneNode(newObj.m_collada, "Bip01_R_Clavicle-node"))	
	local allUpBodySceneNode = getChildsOfColladaSceneNode(newObj.m_collada, "Bip01_R_Clavicle-node")
	for id,node in ipairs(allUpBodySceneNode) do
		table.insert(newObj.m_allUpbodyNodes, node)
	end	
	-- left hand
	table.insert(newObj.m_allUpbodyNodes, getColladaSceneNode(newObj.m_collada, "Bip01_L_Clavicle-node"))	
	local allUpBodySceneNode = getChildsOfColladaSceneNode(newObj.m_collada, "Bip01_L_Clavicle-node")
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
	
	--setSceneNodeIsJoinAnimLayer(newObj.m_collada, "Bip01_Head-node", true, false)
	setSceneNodeIsJoinAnimLayer(newObj.m_collada, "Bip01_R_Clavicle-node", true, true)
	setSceneNodeIsJoinAnimLayer(newObj.m_collada, "Bip01_L_Clavicle-node", true, true)
	
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
		[k_playerStateFlipTurn]	= function() self:updatePlayerStateFlipTurn(timeStep) end,
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

-----------------------------------------------------------
-- player state implement 
-----------------------------------------------------------

-- updatePlayerStateStand
-- Update when player not moving
function CPlayerComponent:updatePlayerStateStand(timeStep)
	
	if self.m_playerSubState == k_playerSubStateInit then
		-- init state
		-- set anim on layer 0
		setColladaAnimation(self.m_collada, k_playerAnimShootMachineGuns, 0, true, 0)
		setColladaAnimWeight(self.m_collada, 1, 0, 0)
		
		pauseColladaAnimAtFrame(self.m_collada, 0, 0, 0)
		
		-- disable multi anim
		colladaEnableAnimTrackChannel(self.m_collada, 1, false, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 2, false, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 3, false, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 4, false, 0)
		
		self.m_playerSubState = k_playerSubStateUpdate
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
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
		setColladaAnimation(self.m_collada, k_playerAnimShootMachineGuns, 0, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 0, true, 0)
		pauseColladaAnimAtFrame(self.m_collada, 0, 0, 0)
				
		-- track 1
		setColladaAnimation(self.m_collada, k_playerAnimRunForward, 1, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 1, true, 0)
		
		-- track 2
		setColladaAnimation(self.m_collada, k_playerAnimRunBackward, 2, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 2, true, 0)
		
		-- track 3
		setColladaAnimation(self.m_collada, k_playerAnimRunLeft, 3, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 3, true, 0)
		
		-- track 4
		setColladaAnimation(self.m_collada, k_playerAnimRunRight, 4, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 4, true, 0)
		
		-- default run blend
		runBlend = self:calcRunAnimationBlend(0)
		
		self.m_playerSubState = k_playerSubStateUpdate
		
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
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
	
end

-- updatePlayerStateFlipTurn
-- update when player force change the run turn
function CPlayerComponent:updatePlayerStateFlipTurn(timeStep) 
	if self.m_playerSubState == k_playerSubStateInit then		
		-- track 0
		setColladaAnimation(self.m_collada, k_playerAnimShootMachineGuns, 0, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 0, true, 0)
		pauseColladaAnimAtFrame(self.m_collada, 0, 0, 0)
				
		-- track 1
		setColladaAnimation(self.m_collada, k_playerAnimRunForward, 1, true, 0)
		colladaEnableAnimTrackChannel(self.m_collada, 1, true, 0)
		
		-- track 2
		setColladaAnimation(self.m_collada, k_playerAnimRunBackward, 2, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 2, true, 0)
		
		-- track 3
		setColladaAnimation(self.m_collada, k_playerAnimRunLeft, 3, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 3, true, 0)
		
		-- track 4
		setColladaAnimation(self.m_collada, k_playerAnimRunRight, 4, true, 0)		
		colladaEnableAnimTrackChannel(self.m_collada, 4, true, 0)		
		
		self.m_playerSubState = k_playerSubStateUpdate
		
	elseif self.m_playerSubState == k_playerSubStateEnd then
		-- end state
		self:doNextState()
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
-- Update aim
function CPlayerComponent:updatePlayerUpBodyAim(timeStep)
	self.m_needRotateCharacter = true
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

-- calcRunAnimationBlend
-- calc animation
function CPlayerComponent:calcRunAnimationBlend(rot)
	local forward = 0.0
	local backward = 0.0
	local left = 0.0
	local right = 0.0
    
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