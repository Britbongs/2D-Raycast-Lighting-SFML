#include "stdafx.hpp"

#ifndef PLAYABLE_BUILD 
#include <vld.h> //Visual Leak Detector include
#else
#include <Windows.h>
#endif 

#include "Game\Game.hpp"


#ifndef PLAYABLE_BUILD
int main(void)
#else 
// Winmain to hide console
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
#endif
{
	
	Game GameInstance;

	if (!GameInstance.InitGame())
	{
		GameInstance.CleanUpGame();
		return(EXIT_FAILURE);
	}

	GameInstance.RunGame();

	GameInstance.CleanUpGame();
	
	system("pause");

	return EXIT_SUCCESS;
}
