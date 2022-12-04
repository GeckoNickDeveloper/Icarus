#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <esp_system.h>
#include <esp_event.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <esp_err.h>
#include <esp_log.h>

extern void icarus_network_setup();

#endif //! NETWORK_H