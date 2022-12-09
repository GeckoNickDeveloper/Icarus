#include "../include/workers.h"

#include <esp_timer.h>

#define TAG "Icarus Workers"

// Sensors
void* icarus_sensor_worker(void* args) {
	int dt = 1000 / CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY;
	unsigned long current_cycle;

	int i=0;

	unsigned long prev = 0;
	unsigned long now;
	float delta;
	vector3d_t acc;
	vector3d_t gyro;
	telemetry_t tlm;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		tlm = icarus_get_shared_telemetry();
		
		//acc = smooth_acc(icarus_get_acceleration());
		gyro = smooth_gyro(icarus_remove_gyro_offset(icarus_get_rotation()));
		now =  icarus_micros(); // microsecons

		if (prev == 0)
			delta = 0.0;
		else
			delta = (float) (now - prev) / 1000000.0; // micros to sec
		prev = now;

		// error free gyro/acc
		

		// Orientation update
		tlm.orientation = icarus_add(tlm.orientation, icarus_multiply(gyro, delta));
		
		// Linear acceleration
		//acc = icarus_rotate(acc, tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
		//acc = icarus_subtract(acc, gravity);
		
		// Moto unif. acc.
		//tlm.velocity =	icarus_add(tlm.velocity, icarus_multiply(acc, delta));
		//tlm.position =	icarus_add(tlm.position,		// x(t) = x +
		//					icarus_add(tlm.velocity,	// V * t +
		//						icarus_multiply(acc, 0.5 * delta * delta))); // 0.5 * a * t^2

		icarus_set_shared_telemetry(tlm);

		// LOG
		if ((i % (CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY * 1)) == 0)
			ESP_LOGE(TAG_SENSORS, "Orientation [%f, %f, %f]", rad2deg(tlm.orientation.x), rad2deg(tlm.orientation.y), rad2deg(tlm.orientation.z));
		i++;

		// ========== END

		// Speed limiter to stick with sample rate
		icarus_delay(dt - (icarus_millis() - current_cycle));
	}
	
	return NULL;
};

// TODO handle timeout
void* icarus_proximity_worker(void* args) {
	unsigned long dt = 1000 / 1; // Temporary
	unsigned long current_cycle;

	float distance;
	long long prev = 0;
	float delta;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		prev = icarus_micros();
		gpio_set_level(CONFIG_ICARUS_TERRAIN_TRIGGER, 1);
		while(gpio_get_level(CONFIG_ICARUS_TERRAIN_ECHO));
		delta = (float) (icarus_micros() - prev) / 1000000.0;

		distance = SOUND_SPEED * delta * 0.5;

		ESP_LOGI(TAG_SENSORS, "Distance [%f]", distance);
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt - (icarus_millis() - current_cycle));
	}
	
	return NULL;
};


// Communication
void* icarus_communication_worker(void* args) {
	unsigned long dt = 1000 / CONFIG_ICARUS_COMMUNICATION_FREQUENCY;
	unsigned long current_cycle;

	telemetry_t tlm;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		tlm = icarus_get_shared_telemetry();

		icarus_publish_telemetry(tlm);
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt - (icarus_millis() - current_cycle));
	}

	return NULL;
};

// Actuator
void* icarus_actuator_worker(void* args) {
	unsigned long dt = 1000 / CONFIG_ICARUS_ACTUATOR_FREQUENCY;
	unsigned long current_cycle;

	command_t cmd;
	command_t prev_cmd;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		cmd = icarus_get_shared_command();

		if (!icarus_equals_commands(cmd, prev_cmd))
			icarus_apply_command(cmd);
	
		prev_cmd = cmd;
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt - (icarus_millis() - current_cycle));
	}
	
	return NULL;
}