#ifndef WORKERS_H
#define WORKERS_H

#include "shared.h"
#include "actuator.h"
#include "communicator.h"
#include "network.h"
#include "sensors.h"
#include "utils.h"
#include "queue.h"
#include "smoother.h"

#define TAG_WORKERS "Icarus Workers"

// Sensors
extern void* icarus_mpu6050_worker(void* args);
extern void* icarus_bh1750_worker(void* args);
extern void* icarus_sr04_worker(void* args);

// Communication
extern void* icarus_communication_worker(void* args);

// Actuator
extern void* icarus_actuator_worker(void* args);

// Pilot
extern void* icarus_pilot_worker(void* args);

#endif //! WORKERS_H