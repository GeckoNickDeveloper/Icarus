#include "../include/Icarus/Network.hpp"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"



Network::Network() {
	this->state = NetworkState::NETWORK_DISCONNECTED;

	esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");

	//=============================

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    this->initCfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&(this->initCfg)));

    /*esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &Network::eventHelper,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &Network::eventHelper,
                                                        NULL,
                                                        &instance_got_ip));*/

	strcpy((char*)(this->cfg.sta.ssid), EXAMPLE_ESP_WIFI_SSID);
	strcpy((char*)(this->cfg.sta.password), EXAMPLE_ESP_WIFI_PASS);
	this->cfg.sta.threshold.authmode = WIFI_AUTH_OPEN;//ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD;
	//this->cfg.sta.sae_pwe_h2e = WPA3_SAE_PWE_BOTH;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &(this->cfg)) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(this->TAG, "wifi_init_sta finished.");

	ESP_LOGW(this->TAG, EXAMPLE_ESP_WIFI_SSID);
	ESP_LOGW(this->TAG, EXAMPLE_ESP_WIFI_PASS);

	this->connect();
};

bool Network::connected() {
	return (this->state == NetworkState::NETWORK_CONNECTED);
};

void Network::connect() {
	esp_err_t err = esp_wifi_connect();
	if (err == ESP_OK) {
		this->state = NetworkState::NETWORK_CONNECTED;
		ESP_LOGW(this->TAG, "Connection");

	} else {
		ESP_LOGE(this->TAG, "Connection failed");
	}
};

void Network::disconnect() {
	esp_err_t err = esp_wifi_disconnect();
	if (err == ESP_OK) {
		this->state = NetworkState::NETWORK_DISCONNECTED;
		ESP_LOGW(this->TAG, "Disconnection");
	}
	else {
		ESP_LOGE(this->TAG, "Disconnection failed");
	}	
};
