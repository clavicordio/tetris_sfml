/*****************************************************************************************
/* File: IO.h
/* Desc: Class for handling input & drawing, it uses SFML for the rendering. Change the methods of this class
/*       in order to use a different renderer
/*****************************************************************************************/

#ifndef _IO_
#define _IO_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <string>

class IO
{
public:
	enum Color { eBlack, eRed, eGreen, eBlue, eCyan, eMagenta, eYellow, eWhite }; // Colors
	enum Key { eKeyNone, eKeyRight, eKeyLeft, eKeyUp, eKeyDown, eKeyRotate, eKeyDrop, eKeyEscape };
	enum EventType { eEventNone, eGameClosed, eKeyPressed };

	struct Event 
	{
		EventType type;
		Key key;
	};

	IO();

	void DrawRectangle(int x1, int y1, int x2, int y2, Color color);
	void DrawText(int x, int y, std::string text_to_draw, int size, Color color);

	void ClearScreen ();
	void UpdateScreen ();
	void CloseWindow();

	bool WindowIsOpen() const;
	int GetScreenWidth() const;
	int GetScreenHeight() const;

	void ClockReset();
	int ClockGetElapsedTimeMS() const;

	bool PollEvent(Event* event);

private:
	std::unique_ptr<sf::RenderWindow> window_;
	sf::Clock clock_;
	sf::Font font_;

	static sf::Color GetColor(IO::Color color);
	static sf::Keyboard::Key GetKey(IO::Key key);
	static IO::Event GetEvent(sf::Event);
};

#endif // _IO_
