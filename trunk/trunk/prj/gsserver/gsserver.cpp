// gsserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CMultiplayerManager.h" 

int _tmain(int argc, _TCHAR* argv[])
{
	CMultiplayerManager *mpManager = new CMultiplayerManager(true, false);

	if ( mpManager->init() == false )
		printf("Init multiplayer error! \n");

	while (1)
	{
		mpManager->update();
		Sleep(1);
	}

	delete mpManager;
	return 0;
}

