#pragma once
#include "Shape.h"

class Circle : virtual public Shape {
public:
	Circle(sf::RenderWindow* window) { this->window = window; }
	Circle(sf::RenderWindow* window, float radius, Vector position, sf::Color color) {
		this->window = window;
		this->radius = radius;
		this->position = position;
		this->color = color;
	}
	~Circle() {
		/*delete& position;
		delete& color;*/
	}

	Circle(const Circle& other){// : Shape(other) {
		window = other.window;
		position = other.position;
		color = other.color;
		rotation = other.rotation;
		radius = other.radius;
	}

	float radius = 10;

	void draw() {
		sf::CircleShape shape(radius);
		shape.setPosition(position[0]-radius, position[1]-radius);
		shape.setFillColor(color);
		shape.setRotation(rotation);
		window->draw(shape);
	}
};