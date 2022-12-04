#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <mqtt_client.h>
#include <esp_err.h>
#include <esp_log.h>

#include "../include/shared.h"

extern void icarus_mqtt_setup();

extern void icarus_publish_telemetry(telemetry_t current);

// Thread worker
extern void* icarus_communication_worker(void* args);

#endif //! COMMUNICATOR_H