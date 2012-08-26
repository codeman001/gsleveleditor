rem @echo off
set VC_TOOL=..\..\..\..\Tools\VCPrjExport.exe

%VC_TOOL% freetype_android.vcproj source_libfreetype.mak
%VC_TOOL% libcurl_android.vcproj source_liburl.mak
%VC_TOOL% toado_android.vcproj source_toado.mak
