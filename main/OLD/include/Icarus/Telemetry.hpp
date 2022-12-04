#ifndef TELEMETRY_HPP
#define TELEMETRY_HPP

#include "Vector3D.hpp"

struct Telemetry {
	Vector3D position;
	Vector3D velocity;
	Vector3D rotation;
};

#endif //! TELEMETRY_HPP