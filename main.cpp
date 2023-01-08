#include <SFML/Graphics.hpp>
#include "Game.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main()
#endif	
{
	Game game;

	while (game.IsRunning())
	{
		game.Loop();
	}

	return 0;
}







