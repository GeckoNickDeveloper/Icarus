#include "../include/communicator.h"
#include "../include/utils.h"
#include <string.h>

static esp_mqtt_client_handle_t client;

void icarus_mqtt_callback_selector(esp_mqtt_event_handle_t event) {
	// Extract topic from event
	char* topic = (char*) calloc(event->topic_len + 1, sizeof(char));
	strncpy(topic, event->topic, event->topic_len);

	// Print the topic
	ESP_LOGI(TAG_COMMUNICATION, "Topic [%s]", topic);

	// Actual selector
	if (strcmp(topic, "/icarus/command") == 0) {
		command_t cmd;
		memcpy(&cmd, (void*) event->data, sizeof(command_t));
		icarus_set_shared_command(cmd);

#if CONFIG_ICARUS_DEBUG_SYNC_TIME_ENABLED
		icarus_system_time_print_now();
#endif
		ESP_LOGW(TAG_COMMUNICATION, "%c%c%c%c%c", cmd.pitch, cmd.roll, cmd.yaw, cmd.throttle, cmd.aux);
		//ESP_LOGW(TAG_COMMUNICATION, "%d %d %d %d %d", cmd.pitch, cmd.roll, cmd.yaw, cmd.throttle, cmd.aux);
	}

	free(topic);
};

void icarus_mqtt_handler(void* args, esp_event_base_t base, int32_t event_id, void *event_data) {
	ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
	esp_mqtt_event_handle_t event = event_data;
	//esp_mqtt_client_handle_t client = event->client;
		
	int msg_id;
	switch ((esp_mqtt_event_id_t) event_id) {
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
			//msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
			//ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

			msg_id = esp_mqtt_client_subscribe(client, "/icarus/command", 0);
			msg_id = esp_mqtt_client_subscribe(client, "/icarus/#", 0);
			//msg_id = esp_mqtt_client_subscribe(client, "/icarus/echo", 0);
			ESP_LOGI(TAG_MQTT, "sent subscribe successful, msg_id=%d", msg_id);

			//msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
			//ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

			//msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
			//ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
			break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
			break;

		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

			msg_id = esp_mqtt_client_publish(client, "/icarus/echo", "data", 0, 0, 0);
			//ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
			break;
		case MQTT_EVENT_UNSUBSCRIBED:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
			break;
		case MQTT_EVENT_PUBLISHED:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
			break;
		case MQTT_EVENT_DATA:
			ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");

			icarus_mqtt_callback_selector(event);
//			 
// 
//			 if (strncmp(event->topic, "/icarus/echo", strlen("/icarus/echo")) != 0) {
//			 	msg_id = esp_mqtt_client_publish(client, "/icarus/echo", event->data, event->data_len, 0, 0);
//			 	printf("ECHO\r\n");
//			 }
// 
//			 printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
//			printf("DATA=%.*s\r\n", event->data_len, event->data);
			break;
		case MQTT_EVENT_ERROR:
			ESP_LOGE(TAG_MQTT, "MQTT_EVENT_ERROR");
			//if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
			//	log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
			//	log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
			//	log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
			//	ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
			//}
			break;
		default:
			ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
			break;
	}
};

void icarus_mqtt_setup() {
	esp_mqtt_client_config_t cfg = {
        .broker.address.uri = CONFIG_ICARUS_MQTT_BROKER_URL,
		.broker.address.port = CONFIG_ICARUS_MQTT_BROKER_PORT,
#if CONFIG_ICARUS_MQTT_USES_AUTHENTICATION
		.credentials.username = CONFIG_ICARUS_MQTT_AUTH_USERNAME,
		.credentials.authentication.password = CONFIG_ICARUS_MQTT_AUTH_PASSWORD,
#endif
    };

	client = esp_mqtt_client_init(&cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, icarus_mqtt_handler, NULL);
    esp_mqtt_client_start(client);
};



void icarus_publish_telemetry(telemetry_t current) {
	//int esp_mqtt_client_publish(esp_mqtt_client_handle_t client, const char *topic, const char *data, int len, int qos, int retain)
	int err = esp_mqtt_client_publish(client, "/icarus/telemetry", (char*) &current, sizeof(telemetry_t), 0, 0);

	if (err == -1)
		ESP_LOGE(TAG_COMMUNICATION, "Telemetry not send due to disconnected client");	
};

void icarus_publish_luminosity(float current) {
	int err = esp_mqtt_client_publish(client, "/icarus/luminosity", (char*) &current, sizeof(float), 0, 0);

	if (err == -1)
		ESP_LOGE(TAG_COMMUNICATION, "Luminosity not send due to disconnected client");	
};

void icarus_publish_terrain(float current) {
	int err = esp_mqtt_client_publish(client, "/icarus/terrain", (char*) &current, sizeof(float), 0, 0);

	if (err == -1)
		ESP_LOGE(TAG_COMMUNICATION, "Terrain not send due to disconnected client");	
};
