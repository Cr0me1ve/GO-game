#include "Vector.h"

Vector::Vector() {
	x = 0;
	y = 0;
}

Vector::Vector(int x, int y) {
	this->x = x;
	this->y = y;
}

Vector::Vector(const sf::Vector2i v2i) {
	this->x = v2i.x;
	this->y = v2i.y;
}

Vector Vector::operator-(const Vector& other) const {
	return Vector(x - other.x, y - other.y);
}

bool Vector::operator==(const Vector& other) const {
	if (x != other.x || y != other.y)
		return false;
	return true;
}

Vector& Vector::operator=(const Vector& other) {
	x = other.x;
	y = other.y;
	return *this;
}

Vector::Vector(const Vector& vec) {
	x = vec.x;
	y = vec.y;
}

void Vector::add(const Vector* other) {
	x += other->x;
	y += other->y;
}

void Vector::subtract(const Vector* other) {
	x -= other->x;
	y -= other->y;
}

std::string Vector::toString() const {
	return std::to_string(x) + " " + std::to_string(y);
}

int Vector::distanceSquared() const {
	return x * x + y * y;
}

Vector Vector::operator+(const Vector& other) const {
	return Vector(x + other.x, y + other.y);
}