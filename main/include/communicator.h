#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <mqtt_client.h>
#include <esp_err.h>
#include <esp_log.h>

#include "../include/shared.h"

#define TAG_COMMUNICATION "Icarus Communication"
#define TAG_MQTT "Icarus MQTT"

extern void icarus_mqtt_setup();

extern void icarus_publish_telemetry(telemetry_t current);
extern void icarus_publish_luminosity(float current);
extern void icarus_publish_terrain(float current);

#endif //! COMMUNICATOR_H