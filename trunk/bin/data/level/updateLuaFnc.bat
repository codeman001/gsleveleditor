set NPPPATH=C:\Program Files\Notepad++\plugins\APIs
set LUACODEDEF="..\..\..\tools\luaCodeDefine\Release\luaCodeDefine.exe"
%LUACODEDEF% luaFunction.xml lua.xml
copy lua.xml "%NPPPATH%\lua.xml"