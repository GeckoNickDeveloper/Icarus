#ifndef SENSORS_H
#define SENSORS_H

#include <mpu6050.h>
#include <bh1750.h>
#include <driver/i2c.h>

#include "shared.h"

#define TAG_SENSORS "Icarus Sensors"

extern void icarus_init_sensors();

extern vector3d_t icarus_get_acceleration();
extern vector3d_t icarus_get_rotation();
extern float icarus_get_temperature();
extern float icarus_get_luminosity();

extern float icarus_get_proximity();

// Thread worker
//extern void* icarus_sensor_worker(void* args);
//extern void* icarus_proximity_worker(void* args);

#endif //! SENSORS_H