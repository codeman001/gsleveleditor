// gsserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CMultiplayerManager.h" 

void runServer()
{
	CMultiplayerManager *mpManager = new CMultiplayerManager(true, false);

	if ( mpManager->init() == false )
	{
		printf("Init multiplayer error! \n");
		delete mpManager;
		return;
	}

	printf("Start server !!!\n");

	while (1)
	{
		mpManager->update();
		Sleep(1);
	}

	delete mpManager;
}

void runTestClient()
{
	CMultiplayerManager *mpManager = new CMultiplayerManager(false, false);

	if ( mpManager->init() == false )
	{
		printf("Init client multiplayer error! \n");
		delete mpManager;
		return;
	}

	printf("Start client!!!\n");

	while (1)
	{
		mpManager->sendDiscoveryPacket();
		Sleep(1);
	}

	delete mpManager;
}

int main(int argc, char* argv[])
{
	bool isClient = true;

	if ( argc == 2 )
	{
		if ( strcmp(argv[1],"server") == 0 )
			isClient = false;
	}

	if ( isClient )
		runTestClient();
	else
		runServer();
	
	return 0;
}

