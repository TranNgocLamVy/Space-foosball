#pragma once

#include <cmath>
#include <string>

class Vector2 {
public:
    float x, y;

    // Constructors
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    // Arithmetic operators
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(const Vector2& other) const {
        return Vector2(x * other.x, y * other.y);
    }

    Vector2 operator/(const Vector2& other) const {
        return Vector2(x / other.x, y / other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }

    // Compound assignment operators
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator*=(const Vector2& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vector2& operator/=(const Vector2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    // Comparison operators
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }

    // Magnitude (length) of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector (make its length 1)
    void normalized() {
        float mag = magnitude();
		if (mag != 0) {
			x /= mag;
			y /= mag;
		}
    }

	// Limit the magnitude of the vector
	void clamp(float min, float max) {
		float mag = magnitude();
		if (mag < min) {
			normalized();
			x *= min;
			y *= min;
		}
		else if (mag > max) {
			normalized();
			x *= max;
			y *= max;
		}
	}

    // Dot product
    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    // Distance between two vectors
    float distance(const Vector2& other) const {
        return (*this - other).magnitude();
    }

    // Angle between two vectors in radians
    float angle(const Vector2& other) const {
        return std::acos(dot(other) / (magnitude() * other.magnitude()));
    }

    // Rotate the vector by a given angle (in radians)
    Vector2 rotated(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vector2(x * cosA - y * sinA, x * sinA + y * cosA);
    }

    // String representation of the vector
    std::string toString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};