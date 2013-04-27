debug("-----------------------------------------------------------------")
debug("compile VectorUtils.lua")

-- declare const
k_realPrecision = 0.000001
k_PI = 3.14159265359
k_degToRad = k_PI/180.0
k_radToDeg = 180.0/k_PI

CVectorUtil = {}
CVectorUtil.__index = CVectorUtil

function CVectorUtil.turnToDir(dir, turnTo, speed)
	-- rotate front vec
	local f = speed*getTimeStep();	
	
	if ( f >= math.abs( CVectorUtil.getAngle(dir, turnTo)) ) then		
		dir = turnTo;
        return {true, turnTo};
	end
    
	dir = CVectorUtil.interpolateTurnToDir( dir, turnTo, f );
    
	local rot = math.abs(CVectorUtil.getAngle(dir, turnTo));
    if ( rot <= speed ) then    
        dir = turnTo;
        return {true, turnTo};
    end
    
	return {false, dir};
end


function CVectorUtil.interpolateTurnToDir(turnFrom, turnTo, f)
	-- calc turn Direction    
    local normal = turnTo:crossProduct(turnFrom)
    normal:normalize()
    
    local q = irr.core.quaternion()
	q:fromAngleAxis( k_degToRad*f, normal)
    
	-- rotate result vector
	local dir = irr.core.vector3d(turnFrom)
	local mat = q:getMatrix()
	mat:rotateVect(dir)
	dir:normalize()
    
	return dir
end

function CVectorUtil.getAngle(v1, v2)
	local normal = v2:crossProduct(v1)
	local angleVec = v2:dotProduct(v1)
	
	angleVec = CVectorUtil.fixAngle(angleVec)
	angleVec = k_radToDeg*math.acos(angleVec)
    
	if normal.Y < 0 then
		angleVec = -angleVec;
	end
	
	return angleVec;
end

function CVectorUtil.realIsZero(x)
	return math.abs(x) < k_realPrecision
end

function CVectorUtil.realIsEqual(a,b)
	return CVectorUtil.realIsZero(a-b)
end

function CVectorUtil.fixAngle(f)
	if (CVectorUtil.realIsEqual(f,1) == true) then
		return 1
	elseif ( CVectorUtil.realIsEqual(f,-1) == true ) then
		return -1
	end
	
	return f;
end

debug("-----------------------------------------------------------------\n\n")