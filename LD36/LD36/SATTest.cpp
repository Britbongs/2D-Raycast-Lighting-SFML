#include "stdafx.h"
#include <iostream> 

using namespace std;

int main(int argc, char* argv[], char* envp[])
{
	RenderWindow RWindow(VideoMode(1024, 720), "SATTest", Style::Close);

	while (RWindow.isOpen())
	{
		Event Evnt;

		while (RWindow.pollEvent(Evnt))
		{
			if (Evnt.type == Event::EventType::Closed)
			{
				RWindow.close();
			}
		}

		RWindow.clear();

		RWindow.display();

	}


	return 0;
}
