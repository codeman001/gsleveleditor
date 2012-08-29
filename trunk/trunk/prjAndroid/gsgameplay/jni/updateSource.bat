rem @echo off
set VC_TOOL=..\..\..\tools\vcProjExport\Release\VCPrjExport.exe

rem %VC_TOOL% prj_lua.vcproj source_liblua.mak
rem %VC_TOOL% prj_freetype.vcproj source_libfreetype.mak
%VC_TOOL% prj_gameswf.vcproj source_gameswf.mak
rem %VC_TOOL% prj_irrlicht.vcproj source_irlicht.mak
rem %VC_TOOL% prj_irrlichtEx.vcproj source_irlichtex.mak
rem %VC_TOOL% prj_gsgameplay.vcproj source_gameplay.mak
