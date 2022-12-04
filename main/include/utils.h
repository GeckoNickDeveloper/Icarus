#ifndef UTILS_H
#define UTILS_H

#include "shared.h"
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Constants
#define PI 3.14159265359
#define G 9.80665			// m/s^2
#define SOUND_SPEED 343		// m/s

// Trigonometry
extern float rad2deg(float angle);
extern float deg2rad(float angle);

// Vector3D
extern vector3d_t icarus_add(vector3d_t a, vector3d_t b);
extern vector3d_t icarus_subtract(vector3d_t a, vector3d_t b);
extern vector3d_t icarus_multiply(vector3d_t a, float b);
extern vector3d_t icarus_divide(vector3d_t a, float b);

extern float icarus_length(vector3d_t src);
extern vector3d_t icarus_rotate(vector3d_t source, float alpha, float beta, float gamma);

// Compare
//extern bool icarus_compare_commands(command_t a, command_t b);
//extern bool icarus_compare_vectors(vector3d_t a, vector3d_t b);
extern bool icarus_equals_commands(command_t a, command_t b);
extern bool icarus_equals_vectors(vector3d_t a, vector3d_t b);

// Timestamp
extern void print_timestamp();

// Delays
extern void icarus_delay(long millis);

#endif //! UTILS_H