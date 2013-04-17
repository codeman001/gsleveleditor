debug("-----------------------------------------------------------------")
debug("compile PlayerComponent.lua")

CPlayerComponent = {}
CPlayerComponent.__index = CPlayerComponent
CPlayerComponentList = {}

function CPlayerComponent.create(gameObj)
	local newObj = {}
	setmetatable(newObj, CPlayerComponent)
	
	newObj.m_gameObject 	= gameObj;
	newObj.m_playerState	= 0;
	newObj.m_playerUpState	= 0;
	
	return newObj;
end

function CPlayerComponent:updateState(timeStep)
	debug( 'CPlayerComponent::updateState object: ' .. self.m_gameObject .. ' + ' .. self.m_playerState .. ' + ' .. self.m_playerUpState )
end

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

debug("-----------------------------------------------------------------\n\n")