#include "../include/workers.h"

#include <esp_timer.h>

#define TAG "Icarus Workers"

// Sensors
void* icarus_mpu6050_worker(void* args) {
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_MPU6050_SAMPLING_FREQUENCY;
	unsigned long dt_ms = icarus_sec_to_millis(dt);
	unsigned long current_cycle;

	int i=0;

	vector3d_t acc;
	vector3d_t gyro;
	telemetry_t tlm;

	//ESP_LOGI(TAG_SENSORS, "LOG START");

	printf("X, Y, Z\r\n%f, %f, %f\r\n", 0.0, 0.0, 0.0);

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		tlm = icarus_get_shared_telemetry();
		
		acc =	approx(
					smooth_acc(
						icarus_get_acceleration()
					),
					CONFIG_ICARUS_SMOOTHING_APPROXIMATION_DIGITS
				);
		
		gyro =	approx(
					smooth_gyro(
						icarus_subtract(
							icarus_get_rotation(),
							icarus_get_gyro_bias()
						)
					),
					CONFIG_ICARUS_SMOOTHING_APPROXIMATION_DIGITS
				);

		
		// Orientation update
		tlm.orientation = icarus_add(tlm.orientation, icarus_multiply(gyro, dt));
		tlm.orientation = icarus_bound(tlm.orientation, 0, 2 * PI);
		//tlm.orientation = icarus_get_orientation(tlm.orientation);

		// Linear acceleration
		acc = icarus_rotate(acc, tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
		acc = icarus_subtract(acc, icarus_get_gravity());
		
		// Moto unif. acc.
		tlm.position =
			icarus_add(tlm.position,		// x(t) = x +
				icarus_add(			
					icarus_multiply(tlm.velocity, dt),	// V * t +
					icarus_multiply(acc, 0.5 * dt * dt))	// 0.5 * a * t^2
				);
							
		tlm.velocity =	
			icarus_add(tlm.velocity,
				icarus_multiply(acc, dt));	// V(t) = V + a * dt

		icarus_set_shared_telemetry(tlm);

		// LOG
		if (((i % (CONFIG_ICARUS_SENSOR_MPU6050_SAMPLING_FREQUENCY * 609)) == 0) && (i != 0)) // 10m logs (before)
			ESP_LOGI(TAG_SENSORS, "LOG END");
		else if ((i % (CONFIG_ICARUS_SENSOR_MPU6050_SAMPLING_FREQUENCY)) == 0) { // every second
			//ESP_LOGI(TAG_SENSORS, "Orientation [%f, %f, %f]", rad2deg(tlm.orientation.x), rad2deg(tlm.orientation.y), rad2deg(tlm.orientation.z));
			//ESP_LOGW(TAG_SENSORS, "Vel [%f, %f, %f]", tlm.velocity.x, tlm.velocity.y, tlm.velocity.z);
			//ESP_LOGE(TAG_SENSORS, "Acc [%f, %f, %f]", acc.x, acc.y, acc.z);
			//ESP_LOGE(TAG_SENSORS, "Gravity [%f]", icarus_length(icarus_get_gravity()));
			//vector3d_t g = icarus_get_gravity();
			//printf("%f, %f, %f\r\n", (tlm.orientation.x), (tlm.orientation.y), (tlm.orientation.z));
			//printf("%f, %f, %f\r\n", rad2deg(tlm.orientation.x), rad2deg(tlm.orientation.y), rad2deg(tlm.orientation.z));
		}

		i++;

		// ========== END

		// Speed limiter to stick with sample rate
		icarus_delay(dt_ms - (icarus_millis() - current_cycle));
	}
	
	return NULL;
};

void* icarus_bh1750_worker(void* args) {
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_BH1750_SAMPLING_FREQUENCY; // Temporary
	unsigned long dt_ms = icarus_sec_to_millis(dt);
	unsigned long current_cycle;

	float lux;
	long long prev = 0;
	float delta;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		lux = icarus_get_luminosity();
		icarus_set_shared_luminosity(lux);

		//ESP_LOGI(TAG_SENSORS, "Luminosity [%f]", lux);
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt_ms - (icarus_millis() - current_cycle));
	}
	
	return NULL;
};

// TODO handle timeout
void* icarus_sr04_worker(void* args) {
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_SR04_SAMPLING_FREQUENCY; // Temporary
	unsigned long dt_ms = icarus_sec_to_millis(dt);
	unsigned long current_cycle;

	float distance;
	long long prev = 0;
	float delta;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		distance = icarus_get_proximity();
		icarus_set_shared_terrain(distance);

		ESP_LOGI(TAG_SENSORS, "Distance [%f]", distance);
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt_ms - (icarus_millis() - current_cycle));
	}
	
	return NULL;
};


// Communication
void* icarus_communication_worker(void* args) {
	float dt = CONFIG_ICARUS_COMMUNICATION_TRANSMITION_RATE;
	unsigned long dt_ms = icarus_sec_to_millis(dt);
	unsigned long current_cycle;

	telemetry_t tlm;
	float luminosity;
	float terrain;

	while(1) {

		// ========== START
		tlm = icarus_get_shared_telemetry();
		luminosity = icarus_get_shared_luminosity();
		terrain = icarus_get_shared_terrain();
		
		icarus_publish_telemetry(tlm);
		icarus_publish_luminosity(luminosity);

		if (terrain != -1)
			icarus_publish_terrain(terrain);
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt_ms);
	}

	return NULL;
};

// Actuator
void* icarus_actuator_worker(void* args) {
	float dt = 1.0 / CONFIG_ICARUS_ACTUATOR_FREQUENCY;
	unsigned long dt_ms = icarus_sec_to_millis(dt);
	unsigned long current_cycle;

	command_t cmd;
	command_t prev_cmd;
	float lux;

	while(1) {
		current_cycle = icarus_millis();

		// ========== START
		cmd = icarus_get_shared_command();
		lux = icarus_get_shared_luminosity();

		if (lux < CONFIG_ICARUS_ACTUATOR_LIGHT_OVERRIDE_VALUE)
			cmd.aux |= 0x80; // LEDs flag override

		if (!icarus_equals_commands(cmd, prev_cmd))
			icarus_apply_command(prev_cmd, cmd);

		prev_cmd = cmd;
		// ========== END
		
		// Speed limiter to stick with sample rate
		icarus_delay(dt_ms - (icarus_millis() - current_cycle));
	}
	
	return NULL;
}