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

	ESP_LOGI(TAG_SENSORS, "LOG START");

	printf("X, Y, Z\r\n%f, %f, %f\r\n", 0.0, 0.0, 0.0);

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		tlm = icarus_get_shared_telemetry();
		
		acc =	approx(
					smooth_acc(
						icarus_get_linear_acceleration()
					),
					CONFIG_ICARUS_APPROXIMATION_DIGITS
				);
		//acc = approx(smooth_acc(icarus_get_linear_acceleration()), CONFIG_ICARUS_APPROXIMATION_DIGITS);
		gyro =	approx(
					smooth_gyro(
						icarus_subtract(
							icarus_get_rotation(),
							icarus_get_gyro_offset()
						)
					),
					CONFIG_ICARUS_APPROXIMATION_DIGITS
				);

		//gyro = approx(smooth_gyro(icarus_get_linear_rotation()), CONFIG_ICARUS_APPROXIMATION_DIGITS);
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
		tlm.velocity =	icarus_add(tlm.velocity, icarus_multiply(acc, delta));
		//tlm.position =	icarus_add(tlm.position,		// x(t) = x +
		//					icarus_add(tlm.velocity,	// V * t +
		//						icarus_multiply(acc, 0.5 * delta * delta))); // 0.5 * a * t^2

		icarus_set_shared_telemetry(tlm);

		// LOG
		if ((i % (CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY * 601)) == 0) // 10m logs (before)
			ESP_LOGI(TAG_SENSORS, "LOG END");
		else if ((i % (CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY * 1)) == 0) {
			//ESP_LOGI(TAG_SENSORS, "Orientation [%f, %f, %f]", rad2deg(tlm.orientation.x), rad2deg(tlm.orientation.y), rad2deg(tlm.orientation.z));
			//ESP_LOGW(TAG_SENSORS, "Vel [%f, %f, %f]", tlm.velocity.x, tlm.velocity.y, tlm.velocity.z);
			//ESP_LOGE(TAG_SENSORS, "Acc [%f, %f, %f]", acc.x, acc.y, acc.z);
			printf("%f, %f, %f\r\n", (tlm.orientation.x), (tlm.orientation.y), (tlm.orientation.z));
			//printf("%f, %f, %f\r\n", rad2deg(tlm.orientation.x), rad2deg(tlm.orientation.y), rad2deg(tlm.orientation.z));
		}
		//ESP_LOGW(TAG_SENSORS, "Gyro [%f, %f, %f]", gyro.x, gyro.y, gyro.z);

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
		distance = icarus_get_proximity();

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