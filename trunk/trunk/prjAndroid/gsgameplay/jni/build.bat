@echo on

@set BASHPATH="C:\DevTools\cygwin\bin\bash.exe"
@set PROJECTDIR="/cygdrive/e/ProjectC/gseditor/trunk/prjAndroid/gsgameplay/jni"
@set NDKDIR="/cygdrive/e/Android/android-ndk-r6/ndk-build"

%BASHPATH% --login -c "cd %PROJECTDIR% && %NDKDIR%

@pause: