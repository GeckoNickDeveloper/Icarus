#ifndef COMMUNICATOR_HPP
#define COMMUNICATOR_HPP

#include <string>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
//#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

class Communicator {
private:
	std::string broker;					// (mqtt)
	esp_mqtt_client_config_t cfg;		// (mqtt)
	esp_mqtt_client_handle_t client;	// (mqtt)

	const char* TAG = "Icarus MQTT";

	void* task();
	void mqttHandler(esp_event_base_t base, int32_t event_id, void *event_data);
public:
	Communicator(std::string broker);
	
	static void* taskHelper(void* ctx);
	static void mqttHelper(void* ctx, esp_event_base_t base, int32_t event_id, void *event_data);
};

#endif //! COMMUNICATOR_HPP