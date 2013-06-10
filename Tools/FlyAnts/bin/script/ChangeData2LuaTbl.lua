local file = io.open("OverLordCard.txt", "r+b");
local OLC = {};

if file then
	local keytbl = {};	-- 键值表，从数据文件的第一行读取
	
	local data = file:read("*l");
	local idx = 1;
	while(data) do
		-- print(data);
		local cardPropertyTbl = {};
		
		local idxKey = 1;
		for val in string.gmatch(data, "([^\t]+)") do
			if string.find(val, '\r') then
				val = string.gsub(val, '\r', '');
			end
			if string.find(val, '\n') then
				val = string.gsub(val, '\n', '');
			end
			
			if idx == 1 then
				table.insert(keytbl, val);
			else
				cardPropertyTbl[keytbl[idxKey]] = val;
-- 				print(keytbl[idxKey], cardPropertyTbl[keytbl[idxKey]]);
-- 				print(#val, val);
				idxKey = idxKey + 1;
			end
		end

		if idx > 1 then
-- 			print(cardPropertyTbl[keytbl[1]])
			OLC[cardPropertyTbl[keytbl[1]]] = cardPropertyTbl;
		end
		
		idx = idx + 1;
		
-- 		if idx > 2 then
-- 			break;
-- 		end

		data = file:read("*l");
	end
	
	file:close();
end

-- table.foreach(OLC, print);

local dump = nil;

dump = function(o)
	if type(o) == 'table' then
		local s = '{ '
		for k, v in pairs(o) do
			if type(k) ~= 'number' then k = string.format('"%s"', tostring(k)) end
			s = string.format('%s[%s] = %s, ', s, k, dump(v));
		end
		return s .. '} '
	else
		return string.format('"%s"', tostring(o))
	end
end

print(dump(OLC))