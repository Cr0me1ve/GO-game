#pragma once
#include <SFML/Graphics.hpp>
#include "Vector.h"

class Shape {
public:
	Shape() :position(0, 0), color(0, 0, 0) {}
	Shape(sf::RenderWindow* window) :position(0, 0), color(0, 0, 0) { this->window = window; }
	~Shape() {}

	Shape(const Shape& other) {
		window = other.window;
		position = other.position;
		color = other.color;
		rotation = other.rotation;
	}

	sf::RenderWindow* window;
	Vector position;
	float rotation = 0.f;
	sf::Color color;

	virtual void draw() {};

	void setColor(int r, int g, int b) {
		color = sf::Color(r, g, b);
	}
};