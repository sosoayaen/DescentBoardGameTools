-- module(package.seeall, "DataHandle");

-- 先暂时载入默认的怪物属性的表
local Monsters = require "property.monsters"
local Heroes = require "property.heroes"
DataHandle = 
{
	GetDataTblFromID = function(nID, nPlayers)
		-- 默认返回空表
		return {};
	end
}

local GetObject = function(nID)
	nID = tonumber(nID);
	if nID >= 1000 and nID < 3000 then
		return Monsters;
	elseif nID >=3000 and nID < 4000 then
		return Heroes;
	end
end

DataHandle.GetDataTblFromID = function(nID, nPlayers)
	local obj = GetObject(nID);
	if not obj then return nil end
	return obj.GetData(nID, nPlayers);
end

-- local t = DataHandle.GetDataTblFromID('2000', 2);

-- local print_r = nil;
-- print_r = function(k, v)
-- 	if type(v) == 'table' then
-- 		table.foreach(v, print_r)
-- 	else
-- 		print(k, v);
-- 	end
-- end

-- print_r(nil, t);

return DataHandle;