#include "stdafx.h"

#if IN_DEVELOPMENT_BUILD 
#include <vld.h> //Visual Leak Detector include1
#endif 

#include "Game\Game.hpp"

int main(int argc, char* argv[], char* envp[])
{
	Game GameObj;

	if (!GameObj.InitGame())
	{
		return(-1);
	}
	
	GameObj.RunGame();

	GameObj.CleanUpGame();
	
	return 0;
}
