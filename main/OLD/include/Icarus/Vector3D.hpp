#ifndef VECTOR3D_HPP
#define VECTOR3D_HPP

/**
 * @class Vector3D
 * 
 * @details 3-Dimensional vector rappresentation 
 */
class Vector3D {
private:
	double x;
	double y;
	double z;

public:
	/**
	 * @brief Construct a new Vector3D object
	 * 
	 * @param x New vector X value
	 * @param y New vector Y value
	 * @param z New vector Z value
	 */
	Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);

	/**
	 * @brief Getter of X
	 * 
	 * @return double x
	 */
	double getX();
	/**
	 * @brief Setter of X
	 */
	void setX(double x);

	/**
	 * @brief Getter of Y
	 * 
	 * @return double y
	 */
	double getY();
	/**
	 * @brief Setter of Y
	 */
	void setY(double y);

	/**
	 * @brief Getter of Z
	 * 
	 * @return double z
	 */
	double getZ();
	/**
	 * @brief Setter of Z
	 */
	void setZ(double z);


	/**
	 * @brief Calculate the length of the vector
	 * 
	 * @return double The length of the vector
	 */
	double length();
	/**
	 * @brief Rotate the vector alongside each axis according to specified angles
	 * 
	 * @param alpha X-axis angle
	 * @param beta Y-axis angle
	 * @param gamma Z-axis angle
	 */
	void rotate(double alpha, double beta, double gamma);
	/**
	 * @brief Rotate the vector alongside each axis according to specified angles
	 * 
	 * @param angles Vector3D containing the three axis (x => alpha, y => beta, z => gamma)
	 */
	void rotate(Vector3D angles);

	/**
	 * @brief 
	 * 
	 * @param lower 
	 * @param upper 
	 * @return Vector3D Bounded vector
	 */
	void bound(double lower, double upper);

	// Operators overload
	Vector3D& operator+=(const Vector3D& rhs);
	friend Vector3D operator+(Vector3D lhs, const Vector3D& rhs);
	
	Vector3D& operator-=(const Vector3D& rhs);
	friend Vector3D operator-(Vector3D lhs, const Vector3D& rhs);

	Vector3D& operator*=(const double rhs);
	friend Vector3D operator*(Vector3D lhs, const double rhs);
	
	Vector3D& operator/=(const double rhs);
	friend Vector3D operator/(Vector3D lhs, const double rhs);

	bool operator==(const Vector3D& lhs, const Vector3D& rhs);
	bool operator!=(const Vector3D& lhs, const Vector3D& rhs);
};

#endif //! VECTOR3D_HPP