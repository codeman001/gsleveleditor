CC = g++
RR = windres
PROG = uiClass.DLL

SOURCE			=   uiClass.cpp \
uiObject.cpp \
uiApplication.cpp \
uiWindowEvent.cpp \
uiWindowFunc.cpp \
uiForm.cpp \
uiDialog.cpp \
uiDock.cpp \
uiLockLayout.cpp \
uiGraphicsObject.cpp \
uiGraphics.cpp \
uiFont.cpp \
uiPen.cpp \
uiBrush.cpp \
uiColor.cpp \
uiBitmap.cpp \
uiIcon.cpp \
uiCursor.cpp \
uiKeyEvent.cpp \
uiSizeEvent.cpp \
uiCharEvent.cpp \
uiMouseEvent.cpp \
uiEventCallback.cpp \
uiMenu.cpp \
uiMenuItem.cpp \
uiControl.cpp \
uiButton.cpp \
uiCheckbox.cpp \
uiComboBox.cpp \
uiEdit.cpp \
uiGroupbox.cpp \
uiListView.cpp \
uiRadioButton.cpp \
uiSplitContainer.cpp \
uiStatic.cpp \
uiProgressBar.cpp \
uiListBox.cpp \
uiAbstractList.cpp \
uiTreeView.cpp \
uiRebar.cpp \
uiToolbar.cpp \
uiTabControl.cpp \
uiToolTip.cpp \
uiStatusBar.cpp \
uiRebarWindow.cpp \
uiKeyAccel.cpp \
uiTabController.cpp \




RESOURCEOUT	= resource.o
RESOURCEPATH = uiClass.rc

UNICODE         = -DUNICODE -D_UNICODE
CFLAGS			= -DTIXML_USE_STL $(UNICODE)
DFLAGS			= -Wl,--subsystem,windows
LIBS			= -lcomdlg32 -lcomctl32 -lgdi32 -lole32 -loleacc -lshell32 -lshlwapi

all: Compile

Compile:
	$(CC) $(DFLAGS) $(CFLAGS) -shared $(SOURCE) $(RESOURCEOUT) -o $(PROG) $(LIBS) 

Resource:
	$(RR) --input=$(RESOURCEPATH) --output=$(RESOURCEOUT) --input-format=rc --output-format=coff
