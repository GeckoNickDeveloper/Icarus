#include "../include/sensors.h"
#include "../include/smoother.h"
#include "../include/utils.h"
#include <math.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <driver/gpio.h>

//static const char* TAG_SENSORS = "Icarus Sensors";

static mpu6050_handle_t mpu6050;
static bh1750_handle_t bh1750;

static vector3d_t gravity;

// Offsets
static vector3d_t gyro_offset;

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
	orientation.y = acos(vec.x / (sqrt(pow(vec.x, 2) + pow(vec.z, 2))));// Y-axis angle

	return orientation;
};

vector3d_t init_orientation(vector3d_t vec) {
	vector3d_t orientation = {0.0, 0.0, 0.0};

	orientation.x = atan2(vec.y, vec.z);		// X-axis angle
	orientation.y = atan2(-vec.x, sqrt((vec.y * vec.y) + (vec.z * vec.z)));// Y-axis angle
	orientation.z = 0.0;

	return orientation;
};




static void icarus_gyro_offset_init() {
	int dt = 1000 / CONFIG_ICARUS_SENSOR_MPU6050_SAMPLING_FREQUENCY;
	unsigned long current_cycle;

	vector3d_t raw;
	vector3d_t sum = {0.0, 0.0, 0.0};
	
	int i;

	// Filling the queue
	//for (i = 0; i < CONFIG_ICARUS_QUEUE_SIZE; ++i) {
	//	current_cycle = icarus_millis();
	//	smooth_gyro(icarus_get_acceleration());
	//	icarus_delay(dt - (icarus_millis() - current_cycle));
	//}

	// Actual evaluation
	for (i = 0; i < CONFIG_ICARUS_SMOOTHING_THERM_SAMPLES; ++i) {
		current_cycle = icarus_millis();

		// Start
		
		raw = icarus_get_rotation();
		sum = icarus_add(sum, raw);
		//ESP_LOGW(TAG_SENSORS, "Vec [%f, %f, %f]", vec.x, vec.y, vec.z);
		
		// End
		icarus_delay(dt - (icarus_millis() - current_cycle));
	}

	// orientation
	telemetry_t tlm = icarus_get_shared_telemetry();
	tlm.orientation.z = 0.0;
	tlm.orientation.y = 0.0;
	tlm.orientation.z = 0.0;
	icarus_set_shared_telemetry(tlm);

	gyro_offset = icarus_divide(sum, i);

	ESP_LOGE(TAG_SENSORS, "GOffset [%f, %f, %f]", rad2deg(gyro_offset.x), rad2deg(gyro_offset.y), rad2deg(gyro_offset.z));
};

void icarus_init_sensors() {
	esp_err_t error;

	// I2C Config
	ESP_ERROR_CHECK(icarus_i2c_master_init());

	// Custom config
	mpu6050_acce_fs_t acc_scale; 
	mpu6050_gyro_fs_t gyro_scale; 
	mpu6050_bandwidth_t bw;
	bh1750_measure_mode_t lux_resolution;

	// acc config
#if CONFIG_ICARUS_SENSOR_MPU6050_ACCELEROMETER_RANGE_2G
	acc_scale = ACCE_FS_2G;
#elif CONFIG_ICARUS_SENSOR_MPU6050_ACCELEROMETER_RANGE_4G
	acc_scale = ACCE_FS_4G;
#elif CONFIG_ICARUS_SENSOR_MPU6050_ACCELEROMETER_RANGE_8G
	acc_scale = ACCE_FS_8G;
#elif CONFIG_ICARUS_SENSOR_MPU6050_ACCELEROMETER_RANGE_16G
	acc_scale = ACCE_FS_16G;
#else
	acc_scale = ACCE_FS_4G;
#endif

	// gyro config
#if CONFIG_ICARUS_SENSOR_MPU6050_GYROSCOPE_RANGE_250DEG
	gyro_scale = GYRO_FS_250DPS;
#elif CONFIG_ICARUS_SENSOR_MPU6050_GYROSCOPE_RANGE_500DEG
	gyro_scale = GYRO_FS_500DPS;
#elif CONFIG_ICARUS_SENSOR_MPU6050_GYROSCOPE_RANGE_1000DEG
	gyro_scale = GYRO_FS_1000DPS;
#elif CONFIG_ICARUS_SENSOR_MPU6050_GYROSCOPE_RANGE_2000DEG
	gyro_scale = GYRO_FS_2000DPS;
#else
	gyro_scale = GYRO_FS_500DPS;
#endif

	// Lowpass config
#if CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_5HZ
	bw = LOWPASS_BANDWIDTH_5;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_10HZ
	bw = LOWPASS_BANDWIDTH_10;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_21HZ
	bw = LOWPASS_BANDWIDTH_21;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_44HZ
	bw = LOWPASS_BANDWIDTH_44;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_94HZ
	bw = LOWPASS_BANDWIDTH_94;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_184HZ
	bw = LOWPASS_BANDWIDTH_184;
#elif CONFIG_ICARUS_SENSOR_MPU6050_LOWPASS_FILTER_260HZ
	bw = LOWPASS_BANDWIDTH_260;
#else
	bw = LOWPASS_BANDWIDTH_260;
#endif

	// BH1750 Config
#if CONFIG_ICARUS_SENSOR_BH1750_RESOLUTION_0_5LX
	lux_resolution = BH1750_CONTINUE_HALFLX_RES;
#elif CONFIG_ICARUS_SENSOR_BH1750_RESOLUTION_1LX
	lux_resolution = BH1750_CONTINUE_1LX_RES;
#elif CONFIG_ICARUS_SENSOR_BH1750_RESOLUTION_4LX
	lux_resolution = BH1750_CONTINUE_4LX_RES;
#else
	lux_resolution = BH1750_CONTINUE_1LX_RES;
#endif


	// MPU6050 Config
	mpu6050 = mpu6050_create(I2C_NUM_0, MPU6050_I2C_ADDRESS);
	error = mpu6050_wake_up(mpu6050);
	//error = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
	error = mpu6050_config(mpu6050, acc_scale, gyro_scale, bw);

	// BH1750 Config
	bh1750 = bh1750_create(I2C_NUM_0, BH1750_I2C_ADDRESS_DEFAULT);
	error = bh1750_power_on(bh1750);
	error = bh1750_set_measure_mode(bh1750, lux_resolution);

	// SP04 Config
	gpio_set_direction(CONFIG_ICARUS_SENSOR_SR04_TERRAIN_TRIGGER, GPIO_MODE_INPUT);
	gpio_set_direction(CONFIG_ICARUS_SENSOR_SR04_TERRAIN_ECHO, GPIO_MODE_OUTPUT);



	// Auxilary Init
	icarus_gyro_offset_init();
	
	// Gravity Init
	//gravity = icarus_get_acceleration();
	telemetry_t tlm = icarus_get_shared_telemetry();
	tlm.orientation = init_orientation(icarus_get_acceleration());
	icarus_set_shared_telemetry(tlm);

};



vector3d_t icarus_get_acceleration() {
	vector3d_t acc = {0,0,0};
	mpu6050_acce_value_t raw;

	mpu6050_get_acce(mpu6050, &raw);
	acc.x = raw.acce_x;
	acc.y = raw.acce_y;
	acc.z = raw.acce_z;
	acc = approx(acc, CONFIG_ICARUS_SMOOTHING_APPROXIMATION_DIGITS);

	acc = icarus_multiply(acc, G);

	return acc;
};

vector3d_t icarus_get_rotation() {
	vector3d_t rot = {0,0,0};
	mpu6050_gyro_value_t raw;

	mpu6050_get_gyro(mpu6050, &raw);
	rot.x = (raw.gyro_x);
	rot.y = (raw.gyro_y);
	rot.z = (raw.gyro_z);
	
	rot = approx(rot, CONFIG_ICARUS_SMOOTHING_APPROXIMATION_DIGITS);
	rot.x = deg2rad(rot.x);
	rot.y = deg2rad(rot.y);
	rot.z = deg2rad(rot.z);
	rot = approx(rot, CONFIG_ICARUS_SMOOTHING_APPROXIMATION_DIGITS);

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


vector3d_t icarus_get_gyro_offset() {
	return gyro_offset;
};



// MPU6050 filtered
vector3d_t icarus_get_gravity() {
	return gravity;
};

vector3d_t icarus_get_linear_acceleration() {
	vector3d_t acc;
	vector3d_t linear;
	
	acc = icarus_get_acceleration();

	linear = icarus_subtract(
		acc,
		icarus_extract_gravity(acc)
	);

	return linear;
};

vector3d_t icarus_extract_gravity(vector3d_t acc) {
	float rc = 1.0 / (0.5 * 2 * PI);
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_MPU6050_SAMPLING_FREQUENCY;
	float alpha = dt / (dt + rc);

	gravity = icarus_add(
		icarus_multiply(gravity, alpha),
		icarus_multiply(acc, (1.0 - alpha))
	);

	return gravity;
};

vector3d_t icarus_get_orientation(vector3d_t orientation) {
	vector3d_t res;
	
	res.x = acos(gravity.y / (sqrt(pow(gravity.y, 2) + pow(gravity.z, 2)))) - (PI/2);
	res.y = acos(gravity.x / (sqrt(pow(gravity.x, 2) + pow(gravity.z, 2)))) - (PI/2);
	res.z = orientation.z;

	return res;
};




float icarus_get_proximity() {
	float distance;

	unsigned long t0;
	unsigned long t1;
	float delta;

	// Trigger sensor
	gpio_set_level(CONFIG_ICARUS_SENSOR_SR04_TERRAIN_TRIGGER, 1);
	icarus_delay_micros(10);
	gpio_set_level(CONFIG_ICARUS_SENSOR_SR04_TERRAIN_TRIGGER, 0);
	t0 = icarus_micros();
	
	// Wait for echo
	while(
		gpio_get_level(CONFIG_ICARUS_SENSOR_SR04_TERRAIN_ECHO) &&
		((icarus_micros() - t0) < (CONFIG_ICARUS_SENSOR_SR04_TIMEOUT * 1000))
	);

	t1 = icarus_micros();
	delta = (float) (t1 - t0) / 1000000.0; // us -> s
	
	if (delta <= (CONFIG_ICARUS_SENSOR_SR04_TIMEOUT / 1000)) // Config is in millis, now is in seconds
		distance = delta * SOUND_SPEED * 0.5;
	else
		distance = -1;

	return distance;
};