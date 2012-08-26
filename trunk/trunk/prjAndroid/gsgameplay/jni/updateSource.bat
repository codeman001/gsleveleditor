rem @echo off
set VC_TOOL=..\..\..\tools\vcProjExport\Release\VCPrjExport.exe

%VC_TOOL% prj_lua.vcproj source_liblua.mak
%VC_TOOL% prj_freetype.vcproj source_libfreetype.mak
%VC_TOOL% prj_gameswf.vcproj source_gameswf.mak
%VC_TOOL% prj_irrlicht.vcproj source_irlicht.mak
