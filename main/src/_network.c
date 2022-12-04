#include "../include/network.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "Icarus Network";

#if CONFIG_ICARUS_WIFI_AUTH_OPEN
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ICARUS_WIFI_AUTH_WEP
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ICARUS_WIFI_AUTH_WPA_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ICARUS_WIFI_AUTH_WPA2_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ICARUS_WIFI_AUTH_WPA_WPA2_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ICARUS_WIFI_AUTH_WPA3_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ICARUS_WIFI_AUTH_WPA2_WPA3_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ICARUS_WIFI_AUTH_WAPI_PSK
#define ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif


static EventGroupHandle_t event_group;
static int retries = 0;

void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	//ESP_LOGI(TAG, "Network event handler");
	//
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		ESP_LOGW(TAG, "Disconnected");
		if (retries < CONFIG_ICARUS_MAXIMUM_RETRY) {
			esp_wifi_connect();
			retries++;
			ESP_LOGW(TAG, "Retry to connect to the AP");
		} else {
			xEventGroupSetBits(event_group, WIFI_FAIL_BIT);
		}
		ESP_LOGW(TAG,"Connection to the AP failed");
	} else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
		ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "got IP [" IPSTR "]", IP2STR(&event->ip_info.ip));
		retries = 0;
		xEventGroupSetBits(event_group, WIFI_CONNECTED_BIT);
	}
};

void icarus_network_setup() {
	// NVS Init
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	// Actual WiFi Init	
	event_group = xEventGroupCreate();

	ESP_ERROR_CHECK(esp_netif_init());

	ESP_ERROR_CHECK(esp_event_loop_create_default());
	esp_netif_create_default_wifi_sta();

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	ESP_ERROR_CHECK(esp_wifi_init(&cfg));

	esp_event_handler_instance_t instance_any_id;
	esp_event_handler_instance_t instance_got_ip;
	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
														ESP_EVENT_ANY_ID,
														&event_handler,
														NULL,
														&instance_any_id));
	ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
														IP_EVENT_STA_GOT_IP,
														&event_handler,
														NULL,
														&instance_got_ip));

	wifi_config_t wifi_config = {
		.sta = {
			.ssid = CONFIG_ICARUS_WIFI_SSID,
			.password = CONFIG_ICARUS_WIFI_PASSWORD,
			/* Authmode threshold resets to WPA2 as default if password matches WPA2 standards (pasword len => 8).
				* If you want to connect the device to deprecated WEP/WPA networks, Please set the threshold value
				* to WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK and set the password with length and format matching to
			* WIFI_AUTH_WEP/WIFI_AUTH_WPA_PSK standards.
				*/
			.threshold.authmode = ICARUS_WIFI_SCAN_AUTH_MODE_THRESHOLD,
			.sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
		},
	};
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
	ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
	ESP_ERROR_CHECK(esp_wifi_start() );

	ESP_LOGI(TAG, "wifi_init_sta finished.");

	/* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
		* number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
	EventBits_t bits = xEventGroupWaitBits(event_group,
			WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);

	/* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
		* happened. */
	if (bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
					CONFIG_ICARUS_WIFI_SSID, CONFIG_ICARUS_WIFI_PASSWORD);
	} else if (bits & WIFI_FAIL_BIT) {
		ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
					CONFIG_ICARUS_WIFI_SSID, CONFIG_ICARUS_WIFI_PASSWORD);
	} else {
		ESP_LOGE(TAG, "UNEXPECTED EVENT");
	}
};