#pragma once
#include <SFML/Graphics.hpp>
#include "Vector.h"
#include <string>

class Button {
private:
	sf::Vector2f rectSize;	// button rectangle size: width | height
	sf::Vector2f position;		// position on screen

	sf::RenderWindow* window;	// rendering window

	std::string buttonText;	// text on the button
	sf::Font font;			// text font
	int fontSize;			// font size
	sf::Color colorText;	// text color
	sf::Color colorRect;	// rect color

	int getFontSizeFromRectSize(int charCount);		// calculate font size
public:
	Button(sf::RenderWindow* window);	// init

	void draw();	// draw button

	void setSize(Vector newSize);		// set new size
	void setPosition(Vector newPosition);	// set new position
	void setFont(sf::Font newFont);		// set new font
	void setColor(sf::Color newColorText, sf::Color newColorRect);	// set new colors
	void setButtonText(std::string newText);	// set new text

	bool checkPressed(Vector mousePos);		// check mouse pressed
};