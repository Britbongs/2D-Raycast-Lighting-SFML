#include "stdafx.h"

#ifndef PLAYABLE_BUILD 
#include <vld.h> //Visual Leak Detector include1
#endif 

#include "Game\Game.hpp"

#ifndef PLAYABLE_BUILD
int main(void)
{
	Game GameInstance;

	if (!GameInstance.InitGame())
	{
		return(-1);
	}

	GameInstance.RunGame();

	GameInstance.CleanUpGame();

	system("pause");

	return 0;
}
#endif

#ifdef PLAYABLE_BUILD 
#include <Windows.h>

// Winmain to hide console
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)

{
	Game GameInstance;

	if (!GameInstance.InitGame())
	{
		return EXIT_FAILURE;
	}

	GameInstance.RunGame();

	GameInstance.CleanUpGame();

	return EXIT_SUCCESS;
}

#endif