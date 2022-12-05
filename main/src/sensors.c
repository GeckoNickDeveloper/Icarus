#include "../include/sensors.h"
#include "../include/smoother.h"
#include "../include/utils.h"
#include <math.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <driver/gpio.h>

static const char* TAG = "Icarus Sensors";

static mpu6050_handle_t mpu6050;
static bh1750_handle_t bh1750;

static vector3d_t gravity;
static vector3d_t avg_acc_err;
static vector3d_t avg_gyro_err;

static esp_err_t icarus_i2c_master_init() {
	int i2c_master_port = CONFIG_ICARUS_I2C_PORT;

	i2c_config_t conf = {
		.mode = I2C_MODE_MASTER,	// Always master
		.sda_io_num = CONFIG_ICARUS_I2C_SDA,
		.scl_io_num = CONFIG_ICARUS_I2C_SCL,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = CONFIG_ICARUS_I2C_FREQ,
    };

    i2c_param_config(i2c_master_port, &conf);
	return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
};



vector3d_t extract_orientation(vector3d_t vec) {
	vector3d_t orientation = {0.0, 0.0, 0.0};

	orientation.x = acos(vec.y / (sqrt(pow(vec.y, 2) + pow(vec.z, 2))));// X-axis angle
	orientation.y = -acos(vec.x / (sqrt(pow(vec.x, 2) + pow(vec.z, 2))));// Y-axis angle

	return orientation;
};
static void icarus_gravity_init() {
	vector3d_t approximated;
	vector3d_t vec;
	
	int i;
	for (i = 0; i < CONFIG_ICARUS_QUEUE_SIZE + CONFIG_ICARUS_THERM_SAMPLES; ++i) {
		approximated = icarus_get_acceleration();
		vec = approx(smooth_acc(approximated), CONFIG_ICARUS_APPROXIMATION_DIGITS);
		//ESP_LOGW(TAG, "Vec [%f, %f, %f]", vec.x, vec.y, vec.z);
	}

	// orientation
	//telemetry_t tlm = icarus_get_shared_telemetry();
	//tlm.orientation = extract_orientation(vec);
	//icarus_set_shared_telemetry(tlm);

	// Init gravity
	gravity.x = gravity.y = 0.0;
	gravity.z = icarus_length(vec);
	gravity = approx(gravity, CONFIG_ICARUS_APPROXIMATION_DIGITS);
	ESP_LOGI(TAG, "Gravity [%f, %f, %f]", gravity.x, gravity.y, gravity.z);
};

void icarus_init_sensors() {
	esp_err_t error;

	// I2C Config
	ESP_ERROR_CHECK(icarus_i2c_master_init());

	// MPU6050 Config
	mpu6050 = mpu6050_create(I2C_NUM_0, MPU6050_I2C_ADDRESS);
	error = mpu6050_wake_up(mpu6050);
	error = mpu6050_config(mpu6050, ACCE_FS_16G, GYRO_FS_500DPS);

	// BH1750 Config
	bh1750 = bh1750_create(I2C_NUM_0, BH1750_I2C_ADDRESS_DEFAULT);
	error = bh1750_power_on(bh1750);
	error = bh1750_set_measure_mode(bh1750, BH1750_CONTINUE_1LX_RES);

	// SP04 Config
	gpio_set_direction(CONFIG_ICARUS_TERRAIN_TRIGGER, GPIO_MODE_INPUT);
	gpio_set_direction(CONFIG_ICARUS_TERRAIN_ECHO, GPIO_MODE_OUTPUT);

	// Auxilary Init
	// Avg Err Estimation
	icarus_gravity_init();

	//vector3d_t rot = icarus_get_rotation();
	//ESP_LOGE(TAG, "Rot [%f, %f, %f]", rot.x, rot.y, rot.z);
};



vector3d_t icarus_get_acceleration() {
	vector3d_t acc = {0,0,0};
	mpu6050_acce_value_t raw;

	mpu6050_get_acce(mpu6050, &raw);
	acc.x = raw.acce_x;
	acc.y = raw.acce_y;
	acc.z = raw.acce_z;
	//acc = approx(acc, 3);
	//ESP_LOGE(TAG, "Acce [%f, %f, %f]", acc.x, acc.y, acc.z);

	acc = icarus_multiply(acc, G);

	return acc;
};

vector3d_t icarus_get_rotation() {
	vector3d_t rot = {0,0,0};
	mpu6050_gyro_value_t raw;

	mpu6050_get_gyro(mpu6050, &raw);
	rot.x = deg2rad(raw.gyro_x);
	rot.y = deg2rad(raw.gyro_y);
	rot.z = deg2rad(raw.gyro_z);

	return rot;
};

float icarus_get_temperature() {
	float temp = 0;
	mpu6050_temp_value_t raw;

	mpu6050_get_temp(mpu6050, &raw);
	temp = raw.temp;

	return temp;
};

float icarus_get_luminosity() {
	float lux = 0;

	bh1750_get_data(bh1750, &lux);

	return lux;
};


// Thread worker
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
		ESP_LOGW(TAG, "SMOOTH [%f, %f, %f]", acc.x, acc.y, acc.z);
		acc = icarus_rotate(acc, tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
		acc = icarus_subtract(acc, gravity);
		ESP_LOGI(TAG, "LINEAR [%f, %f, %f]", acc.x, acc.y, acc.z);

		// Moto unif. acc.
		tlm.velocity =	icarus_add(tlm.velocity, icarus_multiply(acc, delta));
		tlm.position =	icarus_add(tlm.position,		// x(t) = x +
							icarus_add(tlm.velocity,	// V * t +
								icarus_multiply(acc, 0.5 * delta * delta))); // 0.5 * a * t^2

		icarus_set_shared_telemetry(tlm);
		prev = now;

		ESP_LOGE(TAG, "POSITION [%f, %f, %f]", tlm.position.x, tlm.position.y, tlm.position.z);
		ESP_LOGE(TAG, "VELOCITY [%f, %f, %f]", tlm.velocity.x, tlm.velocity.y, tlm.velocity.z);
		ESP_LOGE(TAG, "ORIENTATION [%f, %f, %f]\r\n", tlm.orientation.x, tlm.orientation.y, tlm.orientation.z);
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

		ESP_LOGI(TAG, "Distance [%f]", distance);
	}
	
	return NULL;
};