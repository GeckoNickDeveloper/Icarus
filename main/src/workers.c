#include "../include/workers.h"

#include <esp_timer.h>

#define TAG "Icarus Workers"

// Sensors
void* icarus_sensor_worker(void* args) {
	long long prev = 0;
	long long now;
	float delta;
	vector3d_t acc;
	vector3d_t gyro;
	telemetry_t tlm;

	while(1) {
		tlm = icarus_get_shared_telemetry();
		
		acc = smooth_acc(icarus_get_acceleration());
		gyro = smooth_gyro(icarus_get_rotation());
		now =  esp_timer_get_time(); // microsecons

		if (prev == 0)
			delta = 0.0;
		else
			delta = (float) (now - prev) / 1000000.0; // micros to sec

		// error free gyro/acc

		// Orientation update
		//tlm.orientation = icarus_add(tlm.orientation, icarus_multiply(gyro, delta));
		
		// Linear acceleration
		ESP_LOGW(TAG_SENSORS, "SMOOTH [%f, %f, %f]", acc.x, acc.y, acc.z);
		acc = icarus_rotate(acc, tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
		//acc = icarus_subtract(acc, gravity);
		ESP_LOGI(TAG_SENSORS, "LINEAR [%f, %f, %f]", acc.x, acc.y, acc.z);

		// Moto unif. acc.
		tlm.velocity =	icarus_add(tlm.velocity, icarus_multiply(acc, delta));
		tlm.position =	icarus_add(tlm.position,		// x(t) = x +
							icarus_add(tlm.velocity,	// V * t +
								icarus_multiply(acc, 0.5 * delta * delta))); // 0.5 * a * t^2

		icarus_set_shared_telemetry(tlm);
		prev = now;

		ESP_LOGE(TAG_SENSORS, "POSITION [%f, %f, %f]", tlm.position.x, tlm.position.y, tlm.position.z);
		ESP_LOGE(TAG_SENSORS, "VELOCITY [%f, %f, %f]", tlm.velocity.x, tlm.velocity.y, tlm.velocity.z);
		ESP_LOGE(TAG_SENSORS, "ORIENTATION [%f, %f, %f]\r\n", tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
	}
	
	return NULL;
};


void* icarus_proximity_worker(void* args) {
	float distance;
	long long prev = 0;
	float delta;

	while(1) {
		prev = esp_timer_get_time();
		gpio_set_level(CONFIG_ICARUS_TERRAIN_TRIGGER, 1);
		while(gpio_get_level(CONFIG_ICARUS_TERRAIN_ECHO));
		delta = (float) (esp_timer_get_time() - prev) / 1000000.0;

		distance = SOUND_SPEED * delta * 0.5;

		ESP_LOGI(TAG_SENSORS, "Distance [%f]", distance);
	}
	
	return NULL;
};


// Communication
void* icarus_communication_worker(void* args) {
	telemetry_t tlm;

	while(1) {
		tlm = icarus_get_shared_telemetry();

		icarus_publish_telemetry(tlm);
		icarus_delay(1000);
	}

	return NULL;
};

// Actuator
void* icarus_actuator_worker(void* args) {
	command_t cmd;
	command_t prev_cmd;
	int i = 0;

	while(1) {
		cmd = icarus_get_shared_command();

		if (i % 2 == 0)
			cmd.pitch = 0x01;
		else
			cmd.pitch = 0x00;

		if (!icarus_equals_commands(cmd, prev_cmd))
			icarus_apply_command(cmd);
	
		prev_cmd = cmd;

		icarus_delay(3000);
		i++;
	}
	
	return NULL;
}