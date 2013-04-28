del _common.zip
del _textureTGA.zip
del _texturePVR.zip
del _scene.zip

..\tools\7z\7z.exe a -r _common.zip *.css
..\tools\7z\7z.exe a -r _common.zip *.fsh
..\tools\7z\7z.exe a -r _common.zip *.vsh
..\tools\7z\7z.exe a -r _common.zip *.swf
..\tools\7z\7z.exe a -r _common.zip *.lv
..\tools\7z\7z.exe a -r _common.zip *.lua
..\tools\7z\7z.exe a -r _common.zip *.png
..\tools\7z\7z.exe a -r _common.zip *.xml

..\tools\7z\7z.exe a -r _textureTGA.zip *.tga
..\tools\7z\7z.exe a -r _texturePVR.zip *.pvr

..\tools\7z\7z.exe a -r _scene.zip *.scene
..\tools\7z\7z.exe a -r _scene.zip *.anim

