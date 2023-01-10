#include "../include/actuator.h"
#include "../include/utils.h"
#include <esp_log.h>

//static const char* TAG_ACTUATOR = "Icarus Actuator";

static servo_config_t servo_cfg = {
	.max_angle = 180,
	.min_width_us = 500,
	.max_width_us = 2500,
	.freq = 50,
	.timer_number = LEDC_TIMER_0,
	.channels = {
		.servo_pin = {
			18,//SERVO_CH0_PIN,
			19,//SERVO_CH1_PIN,
			//SERVO_CH2_PIN,
			//SERVO_CH3_PIN,
			//SERVO_CH4_PIN,
			//SERVO_CH5_PIN,
			//SERVO_CH6_PIN,
			//SERVO_CH7_PIN,
		},
		.ch = {
			LEDC_CHANNEL_0,
			LEDC_CHANNEL_1,
			//LEDC_CHANNEL_2,
			//LEDC_CHANNEL_3,
			//LEDC_CHANNEL_4,
			//LEDC_CHANNEL_5,
			//LEDC_CHANNEL_6,
			//LEDC_CHANNEL_7,
		},
	},
	.channel_number = 2,//8
};



void icarus_init_actuator() {
	ESP_LOGW(TAG_ACTUATOR, "TODO: EDIT INIT ACTUATOR");

	iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);

	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, 90); // Engine
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 1, 90); // Rudder
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 2, 90); // Aileron Sx 
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 3, 90); // Aileron Dx
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 4, 90); // Elevator 
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 5, 90); // Elevator 
};



void icarus_apply_command(command_t cmd) {
	ESP_LOGI(TAG_ACTUATOR, "Applying command: [%x %x %x %x %x]", cmd.pitch, cmd.roll, cmd.yaw, cmd.throttle, cmd.aux);

	int throttle = icarus_map(cmd.throttle, 0, 255, 0, 180);
	int yaw = icarus_map(cmd.yaw, 0, 255, 0, 180);

	int roll_sx = icarus_map(cmd.roll, 0, 255, 0, 180);
	int roll_dx = icarus_map(255 - cmd.roll, 0, 255, 0, 180);

	int pitch_sx = icarus_map(cmd.pitch, 0, 255, 0, 180);
	int pitch_dx = icarus_map(255 - cmd.pitch, 0, 255, 0, 180);
	
	

	// Engine
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, roll_sx);

	// Rudder
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 1, yaw);
	
	// Ailerons
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 2, roll_sx);
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 3, roll_dx);
	
	// Elevators
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 4, pitch_sx);
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 5, pitch_dx);

};