#include "../include/Icarus/Vector3D.hpp"

#include <math.h>



Vector3D::Vector3D(double x, double y, double z) {
	this->x = x;
	this->y = y;
	this->z = z;
};



double Vector3D::getX() { return this->x; };
void Vector3D::setX(double x) { this->x = x; };

double Vector3D::getY() { return this->y; };
void Vector3D::setY(double y) { this->y = y; };

double Vector3D::getZ() { return this->z; };
void Vector3D::setZ(double z) { this->z = z; };



double Vector3D::length() {
	return (sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z)));
};

void Vector3D::rotate(double alpha, double beta, double gamma) {
	Vector3D tmp;

	// Rotation on Z-Axis (gamma angle)
	tmp = *this;
	this->x = tmp.getX() * cos(gamma) - tmp.getY() * sin(gamma);
	this->y = tmp.getX() * sin(gamma) + tmp.getY() * cos(gamma);

	// Rotation on Y-Axis (beta angle)
	tmp = *this;
	this->x = tmp.getX() * cos(beta) + tmp.getZ() * sin(beta);
	this->z = tmp.getZ() * cos(beta) - tmp.getX() * sin(beta);

	// Rotation on X-Axis (alpha angle)
	tmp = *this;
	this->y = tmp.getY() * cos(alpha) - tmp.getZ() * sin(alpha);
	this->z = tmp.getY() * sin(alpha) + tmp.getZ() * cos(alpha);
};

void Vector3D::rotate(Vector3D angles) {
	this->rotate(angles.getX(), angles.getY(), angles.getZ());
};



void Vector3D::bound(double lower, double upper) {
	double period = upper - lower;

	while((this->x < lower) || (this->x > upper) || 
		(this->y < lower) || (this->y > upper) || 
		(this->z < lower) || (this->z > upper))
	{
		if (this->x < lower)
			this->x += period;
		if (this->x > upper)
			this->x -= period;
		
		if (this->y < lower)
			this->y += period;
		if (this->y > upper)
			this->y -= period;
		
		if (this->z < lower)
			this->z += period;
		if (this->z > upper)
			this->z -= period;
	}
};



// Operators overload
Vector3D& Vector3D::operator+=(const Vector3D& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;

	return *this;
};
Vector3D Vector3D::operator+(Vector3D lhs, const Vector3D& rhs) {
	lhs += rhs;
	return lhs;
};

Vector3D& Vector3D::operator-=(const Vector3D& rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;

	return *this;
};
Vector3D Vector3D::operator-(Vector3D lhs, const Vector3D& rhs) {
	lhs -= rhs;
	return lhs;
};

Vector3D& Vector3D::operator*=(const double rhs) {
	this->x *= rhs;
	this->y *= rhs;
	this->z *= rhs;

	return *this;
};
Vector3D Vector3D::operator*(Vector3D lhs, const double rhs) {
	lhs *= rhs;
	return lhs;
};

Vector3D& Vector3D::operator/=(const double rhs) {
	this->x /= rhs;
	this->y /= rhs;
	this->z /= rhs;

	return *this;
};
Vector3D Vector3D::operator/(Vector3D lhs, const double rhs) {
	lhs /= rhs;
	return lhs;
};

bool Vector3D::operator==(const Vector3D& lhs, const Vector3D& rhs) {
	return ((lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY()) && (lhs.getZ() == rhs.getZ()));
};
bool Vector3D::operator!=(const Vector3D& lhs, const Vector3D& rhs) { return !(lhs == rhs); };