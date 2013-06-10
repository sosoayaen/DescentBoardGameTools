-- ╪сть LuaCOM
local luacom = require "luacom"

-- First, we get a luacom object using LuaCOM API
local excel = luacom.CreateObject("Excel.Application")
print(excel);
print(luacom.GetType(excel));
--assert(luacom.GetType(excel) == "LuaCOM")
-- now we get one from a method call
--sheets = excel.Sheets
--assert(luacom.GetType(sheets) == "LuaCOM")
