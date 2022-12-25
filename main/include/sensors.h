#ifndef SENSORS_H
#define SENSORS_H

#include "mpu6050.h"
//#include <mpu6050.h>
#include <bh1750.h>
#include <driver/i2c.h>

#include "shared.h"

#define TAG_SENSORS "Icarus Sensors"

extern void icarus_init_sensors();

// MPU6050 wrapped
extern vector3d_t icarus_get_acceleration();
extern vector3d_t icarus_get_rotation();
extern float icarus_get_temperature();

// MPU6050 filtered
//extern vector3d_t icarus_get_linear_rotation();
extern vector3d_t icarus_get_linear_acceleration();
extern vector3d_t icarus_extract_gravity(vector3d_t acc);
extern vector3d_t icarus_get_orientation(vector3d_t orientation);
extern void icarus__get_orientation(vector3d_t* orientation);

// MPU6050 Aux
extern vector3d_t icarus_get_gyro_offset();

// BH1750 wrapped
extern float icarus_get_luminosity();


// SR04
extern float icarus_get_proximity();


#endif //! SENSORS_H