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
			CONFIG_ICARUS_ACTUATOR_ENGINE_PIN,			// Engine		=> Throttle
			CONFIG_ICARUS_ACTUATOR_RUDDER_PIN,			// Rudder		=> Yaw
			CONFIG_ICARUS_ACTUATOR_AILERON_SX_PIN,		// Aileron Sx 
			CONFIG_ICARUS_ACTUATOR_AILERON_DX_PIN,		// Aileron Dx	=> Roll
			CONFIG_ICARUS_ACTUATOR_ELEVATOR_SX_PIN,		// Elevator Sx
			CONFIG_ICARUS_ACTUATOR_ELEVATOR_DX_PIN,		// Elevator Dx	=> Pitch
			//SERVO_CH6_PIN,
			//SERVO_CH7_PIN,
		},
		.ch = {
			LEDC_CHANNEL_0,
			LEDC_CHANNEL_1,
			LEDC_CHANNEL_2,
			LEDC_CHANNEL_3,
			LEDC_CHANNEL_4,
			LEDC_CHANNEL_5,
			//LEDC_CHANNEL_6,
			//LEDC_CHANNEL_7,
		},
	},
	.channel_number = 6,//8
};



void icarus_init_actuator() {
	ESP_LOGW(TAG_ACTUATOR, "TODO: EDIT INIT ACTUATOR");

	iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);

	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, 0); // Engine
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 1, 90); // Rudder
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 2, 90); // Aileron Sx 
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 3, 90); // Aileron Dx
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 4, 90); // Elevator Sx
	iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 5, 90); // Elevator Dx
};



void icarus_apply_command(const command_t prev, command_t cmd) {
	ESP_LOGI(TAG_ACTUATOR, "Applying command: [%x %x %x %x %x]", cmd.pitch, cmd.roll, cmd.yaw, cmd.throttle, cmd.aux);

	int throttle = icarus_map(cmd.throttle, 0, 255, 0, 180);
	int yaw = icarus_map(cmd.yaw, 0, 255, CONFIG_ICARUS_ACTUATOR_YAW_MIN_ANGLE, CONFIG_ICARUS_ACTUATOR_YAW_MAX_ANGLE);

	int roll_sx = icarus_map(cmd.roll, 0, 255, CONFIG_ICARUS_ACTUATOR_ROLL_MIN_ANGLE, CONFIG_ICARUS_ACTUATOR_ROLL_MAX_ANGLE);
	int roll_dx = icarus_map(255 - cmd.roll, 0, 255, CONFIG_ICARUS_ACTUATOR_ROLL_MIN_ANGLE, CONFIG_ICARUS_ACTUATOR_ROLL_MAX_ANGLE);

	int pitch_sx = icarus_map(cmd.pitch, 0, 255, CONFIG_ICARUS_ACTUATOR_PITCH_MIN_ANGLE, CONFIG_ICARUS_ACTUATOR_PITCH_MAX_ANGLE);
	int pitch_dx = icarus_map(255 - cmd.pitch, 0, 255, CONFIG_ICARUS_ACTUATOR_PITCH_MIN_ANGLE, CONFIG_ICARUS_ACTUATOR_PITCH_MAX_ANGLE);
	
	// Aux
	int led_flag = (cmd.aux >> 7) & 0x01;

	// Engine
	if (cmd.throttle != prev.throttle)
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, roll_sx);

	// Rudder
	if (cmd.yaw != prev.yaw)
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 1, yaw);
	
	// Ailerons
	if (cmd.roll != prev.roll) {
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 2, roll_sx);
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 3, roll_dx);
	}
	
	// Elevators
	if (cmd.pitch != prev.pitch) {
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 4, pitch_sx);
		iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 5, pitch_dx);
	}

	// Aux
	if (cmd.pitch != prev.pitch) {
		gpio_set_level(CONFIG_ICARUS_ACTUATOR_LEDS_PIN, led_flag);
	}
};