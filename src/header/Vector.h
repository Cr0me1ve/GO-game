#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

class Vector {
public:
	int x;
	int y;

	Vector();
	~Vector() {}

	Vector(int x, int y);
	Vector(const sf::Vector2i v2i);

	Vector copy() { return Vector(x, y); };

	Vector operator+(const Vector& other) const;
	Vector operator-(const Vector& other) const;
	Vector& operator=(const Vector& other);
	Vector(const Vector&);
	int& operator[](int n) { if (n == 0) return x; return y; }
	bool operator==(const Vector& other) const;

	void add(const Vector* other);		// increase vector with other
	void subtract(const Vector* other);	// decrease vector with other

	int& get(const int n) { if (n == 0) return x; return y; }

	int distanceSquared() const;		// squared distance of the vector
	std::string toString() const;		// convert to string
};