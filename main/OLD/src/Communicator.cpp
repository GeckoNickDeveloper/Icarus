#include "../include/Icarus/Communicator.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

Communicator::Communicator(std::string broker) {
	this->broker = broker;
	this->cfg.broker.address.uri = broker.c_str();
	
	ESP_LOGE(this->TAG, "FYCK THE WHAT");

	//this->client = esp_mqtt_client_init(&(this->cfg));
    
	//esp_err_t err = esp_mqtt_client_register_event(this->client, MQTT_EVENT_ANY, Communicator::mqttHelper, this);
    //if (err != ESP_OK)
	//	ESP_LOGE(this->TAG, "WTF");
	//esp_mqtt_client_start(this->client);
};


//! @todo implement
void* Communicator::task() {
	printf("TODO TASK\n");

	while (1) {
		printf("broker: %s\n", this->broker.c_str());
		vTaskDelay(2000 / portTICK_PERIOD_MS);
	};

	return nullptr;
};

//! @todo implement
void Communicator::mqttHandler(esp_event_base_t base, int32_t event_id, void *event_data) {
	printf("TODO HANDLER\n");

	/*ESP_LOGD(this->TAG, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
	esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
	//esp_mqtt_client_handle_t client = event->client;
	int msg_id;
	switch ((esp_mqtt_event_id_t) event_id) {
	case MQTT_EVENT_CONNECTED:
		ESP_LOGI(this->TAG, "MQTT_EVENT_CONNECTED");
		msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
		ESP_LOGI(this->TAG, "sent publish successful, msg_id=%d", msg_id);

		msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
		ESP_LOGI(this->TAG, "sent subscribe successful, msg_id=%d", msg_id);

		msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
		ESP_LOGI(this->TAG, "sent subscribe successful, msg_id=%d", msg_id);

		msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
		ESP_LOGI(this->TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
		break;
	case MQTT_EVENT_DISCONNECTED:
		ESP_LOGI(this->TAG, "MQTT_EVENT_DISCONNECTED");
		break;

	case MQTT_EVENT_SUBSCRIBED:
		ESP_LOGI(this->TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
		msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
		ESP_LOGI(this->TAG, "sent publish successful, msg_id=%d", msg_id);
		break;
	case MQTT_EVENT_UNSUBSCRIBED:
		ESP_LOGI(this->TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_PUBLISHED:
		ESP_LOGI(this->TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
		break;
	case MQTT_EVENT_DATA:
		ESP_LOGI(this->TAG, "MQTT_EVENT_DATA");
		printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
		printf("DATA=%.*s\r\n", event->data_len, event->data);
		break;
	case MQTT_EVENT_ERROR:
		ESP_LOGI(this->TAG, "MQTT_EVENT_ERROR");
		*if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
			log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
			log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
			log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
			ESP_LOGI(this->TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

		}*
		break;
	default:
		ESP_LOGI(this->TAG, "Other event id:%d", event->event_id);
		break;
	}*/
};



void* Communicator::taskHelper(void* ctx) {
	return ((Communicator*) ctx)->task();
};

//! @todo implement
void Communicator::mqttHelper(void* ctx, esp_event_base_t base, int32_t event_id, void *event_data) {
	((Communicator*) ctx)->mqttHandler(base, event_id, event_data);
};