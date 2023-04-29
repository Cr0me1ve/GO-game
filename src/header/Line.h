#pragma once
#include "Shape.h"
#include <iostream>

class Line : virtual public Shape {
public:
	Line() : Shape() {}
	Line(sf::RenderWindow* window) : Shape() { this->window = window; }
	Line(sf::RenderWindow* window, float length, float width, Vector position, float rotation, sf::Color color) : Shape() {
		this->window = window;
		this->length = length;
		this->width  = width;
		this->position = position;
		this->rotation = rotation;
		this->color = color;
	}

	~Line() {}

	Line(const Line& other) : Shape(other) {
		this->length = length;
		this->width = width;
	}

	float length = 10;
	float width = 2;

	void draw() {
		sf::RectangleShape shape(sf::Vector2f(length, width));
		shape.setPosition(position[0], position[1]);
		shape.setFillColor(color);
		shape.setRotation(rotation);
		window->draw(shape);
	}
};