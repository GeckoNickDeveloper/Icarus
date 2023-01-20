#ifndef ACTUATOR_H
#define ACTUATOR_H

#include "../include/shared.h"
// Palesemente rubato
#include "../include/iot_servo.h"

#define TAG_ACTUATOR "Icarus Actuator"

extern void icarus_init_actuator();

extern void icarus_apply_command(const command_t prev, command_t cmd);

#endif //! ACTUATOR_H