#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "../include/shared.h"
// Palesemente rubato
#include "../include/iot_servo.h"



extern void icarus_init_actuator();

// Actuator
extern void icarus_apply_command(command_t cmd);

// Thread worker
extern void* icarus_actuator_worker(void* args);

#endif //! ACTUATOR_H