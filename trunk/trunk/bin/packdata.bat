delete _common.zip
delete _texture.zip
delete _scene.zip

..\tools\7z\7z.exe a -r _common.zip *.css
..\tools\7z\7z.exe a -r _common.zip *.fsh
..\tools\7z\7z.exe a -r _common.zip *.vsh
..\tools\7z\7z.exe a -r _common.zip *.swf
..\tools\7z\7z.exe a -r _common.zip *.lv
..\tools\7z\7z.exe a -r _common.zip *.lua
..\tools\7z\7z.exe a -r _common.zip *.png

..\tools\7z\7z.exe a -r _texture.zip *.tga
..\tools\7z\7z.exe a -r _texture.zip *.pvr

..\tools\7z\7z.exe a -r _scene.zip *.scene
..\tools\7z\7z.exe a -r _scene.zip *.anim

