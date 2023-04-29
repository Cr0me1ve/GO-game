#include "Button.h"
#include <SFML/Graphics.hpp>

int Button::getFontSizeFromRectSize(int charCount) {
	float ratioHWFont = 1.681f;		// font ratio height/width
	int letterSpacingsCount = charCount - 1;	// quantity of spacings btw letters

	int result = (rectSize.x - letterSpacingsCount) / (charCount / ratioHWFont);
	result -= result / 6.9f;	// letter spacing depends on font size, it is approximate ratio

	if (result < rectSize.y)
		return result;
	else
		return rectSize.y;
}

Button::Button(sf::RenderWindow* window):position(0, 0),colorText(sf::Color::Black),colorRect(sf::Color::White),buttonText() {
	this->window = window;
	fontSize = 12;	// default size
}

void Button::draw() {
	fontSize = getFontSizeFromRectSize(buttonText.length());

	int heightCorrction = -fontSize / 5;	// text position correction
	int widthCorrction = -fontSize / 15;

	sf::Text text;	// button text
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(colorText);
	text.setPosition(position + sf::Vector2f(widthCorrction, heightCorrction) + sf::Vector2f(fontSize/5, (rectSize.y - fontSize) / 2));
	text.setString(buttonText);

	sf::RectangleShape rect;	// button rectangle
	rect.setSize(rectSize);
	rect.setFillColor(colorRect);
	rect.setPosition(position);

	window->draw(rect);		// draw
	window->draw(text);
}

void Button::setSize(Vector newSize) {
	rectSize.x = newSize.x;
	rectSize.y = newSize.y;
}

void Button::setPosition(Vector newPosition) {
	position.x = newPosition.x;
	position.y = newPosition.y;
}

void Button::setFont(sf::Font newFont) {
	font = newFont;
}

void Button::setColor(sf::Color newColorText, sf::Color newColorRect) {
	colorText = newColorText;
	colorRect = newColorRect;
}

void Button::setButtonText(std::string newText) {
	buttonText = newText;
}

bool Button::checkPressed(Vector mousePos) {
	if (mousePos.x >= position.x && mousePos.x <= position.x + rectSize.x && mousePos.y >= position.y && mousePos.y <= position.y + rectSize.y)
		return true;
	return false;
}
