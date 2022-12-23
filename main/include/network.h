#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

#include <esp_modem_api.h>
#include <esp_modem_c_api_types.h>
#include <esp_modem_config.h>
#include <esp_modem_dce_config.h>

#include <esp_netif.h>
#include <esp_netif_ppp.h>
#include <esp_event.h>

#include <esp_log.h>
#include <esp_err.h>

#include <driver/gpio.h>



extern void icarus_network_setup();

#endif //! NETWORK_H