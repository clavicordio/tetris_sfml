/*****************************************************************************************
/* File: IO.cpp
/* Desc: Class for handling inpung & drawing, it uses SDL for the rendering. Change the methods 
/* of this class in order to use a different renderer
/*****************************************************************************************/

#include "IO.h"
#include <assert.h>
#include <stdexcept>
#include <iostream>
#include "Resources.h" // binary resource data

IO::IO() 
{
	this->window_ = std::make_unique<sf::RenderWindow>(sf::VideoMode(640, 480), "SUPER MEGA TETRIS");
	this->window_->setVerticalSyncEnabled(true);
	this->window_->setFramerateLimit(30);

	this->clock_ = sf::Clock();
	this->ClockReset();

	//if (!this->font_.loadFromFile("joystix.ttf"))
	if (!this->font_.loadFromMemory(Resources::joystix_ttf, Resources::joystix_ttf_len))
		throw std::runtime_error("Can't load font.");
}

void IO::ClearScreen() 
{
	this->window_->clear();
}


sf::Color IO::GetColor(Color color)
{
	switch (color) 
	{
	case eBlack:	return sf::Color::Black;
	case eRed:		return sf::Color::Red;
	case eGreen:	return sf::Color::Green;
	case eBlue:		return sf::Color::Blue;
	case eCyan:		return sf::Color::Cyan;
	case eMagenta:	return sf::Color::Magenta;
	case eYellow:	return sf::Color::Yellow;
	case eWhite:	return sf::Color::White;
	default:		assert(false);
	}
}

sf::Keyboard::Key IO::GetKey(Key key)
{
	switch (key) 
	{
	case eKeyLeft:		return sf::Keyboard::Left;
	case eKeyRight:		return sf::Keyboard::Right;
	case eKeyDown:		return sf::Keyboard::Down;
	case eKeyUp:		return sf::Keyboard::Up;
	case eKeyRotate:	return sf::Keyboard::Z;
	case eKeyDrop:		return sf::Keyboard::X;
	case eKeyEscape:	return sf::Keyboard::Escape;
	default:			assert(false);
	}
}

/*
======================================
Draw a rectangle of a given color

Parameters:
>> x1, y1: 		Upper left corner of the rectangle
>> x2, y2: 		Lower right corner of the rectangle
>> color		Rectangle color
======================================
*/
void IO::DrawRectangle (int x1, int y1, int x2, int y2, Color color)
{
	sf::RectangleShape shape(sf::Vector2f((float)(x2 - x1), (float)(y2 - y1)));
	shape.setPosition(sf::Vector2f((float)x1, (float)y1));
	shape.setFillColor(IO::GetColor(color));
	this->window_->draw(shape);
}

void IO::DrawText(int x, int y, std::string text_to_draw, int size, Color color)
{
	sf::Text text;
	text.setFont(this->font_);
	text.setString(text_to_draw);
	text.setCharacterSize(size);
	text.setFillColor(IO::GetColor(color));
	text.setPosition(sf::Vector2f(x, y));
	this->window_->draw(text);
}

int IO::GetScreenWidth() const
{
	return this->window_->getSize().x;
}

int IO::GetScreenHeight() const
{
	return this->window_->getSize().y;
}

void IO::UpdateScreen()
{
	this->window_->display();
}

void IO::CloseWindow()
{
	this->window_->close();
}

bool IO::WindowIsOpen() const
{
	return this->window_->isOpen();
}

void IO::ClockReset() 
{
	this->clock_.restart();
}

int IO::ClockGetElapsedTimeMS() const
{
	return this->clock_.getElapsedTime().asMilliseconds();
}

bool IO::PollEvent(IO::Event* event) 
{
	sf::Event sf_event;
	bool res = this->window_->pollEvent(sf_event);
	*event = IO::GetEvent(sf_event);
	return res;
}

IO::Event IO::GetEvent(sf::Event event)
{
	if (event.type == sf::Event::Closed)
	{
		return IO::Event{ IO::EventType::eGameClosed };
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		IO::Key key;
		switch (event.key.code)
		{
		case sf::Keyboard::Left:	key = IO::eKeyLeft;		break;
		case sf::Keyboard::Right:	key = IO::eKeyRight;	break;
		case sf::Keyboard::Down:	key = IO::eKeyDown;		break;
		case sf::Keyboard::Up:		key = IO::eKeyUp;		break;
		case sf::Keyboard::Z:		key = IO::eKeyRotate;	break;
		case sf::Keyboard::X:		key = IO::eKeyDrop;		break;
		case sf::Keyboard::Escape:	key = IO::eKeyEscape;	break;
		default:					return IO::Event{ IO::EventType::eEventNone };
		}
		assert(IO::GetKey(key) == event.key.code);

		return IO::Event{ IO::EventType::eKeyPressed, key };
	}

	return IO::Event{ IO::EventType::eEventNone };
}
