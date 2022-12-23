#include "../include/utils.h"
#include <math.h>

#include <stdio.h>
#include <esp_sntp.h>
#include <sys/time.h>
#include <esp_timer.h>
#include <esp_log.h>


// Trigonometry
float rad2deg(float angle) {
	return (angle * 57.2958);//(angle * (180.0 / PI));
};
float deg2rad(float angle) {
	return (angle * 0.0174533);//(angle * (PI / 180.0));
};

// Vector3D
vector3d_t icarus_add(vector3d_t a, vector3d_t b) {
	vector3d_t result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;

	return result;
};

vector3d_t icarus_subtract(vector3d_t a, vector3d_t b) {
	vector3d_t result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;

	return result;
};

vector3d_t icarus_multiply(vector3d_t a, float b) {
	vector3d_t result;
	
	result.x = a.x * b;
	result.y = a.y * b;
	result.z = a.z * b;

	return result;
};

vector3d_t icarus_divide(vector3d_t a, float b) {
	vector3d_t result;

	result.x = a.x / b;
	result.y = a.y / b;
	result.z = a.z / b;

	return result;
};



float icarus_length(vector3d_t src) {
	return sqrt((src.x * src.x) + (src.y * src.y) + (src.z * src.z));
};

vector3d_t icarus_rotate(vector3d_t source, float alpha, float beta, float gamma) {
	vector3d_t vec = source;
	vector3d_t tmp;

	// Z-Rotation
	tmp.x = vec.x * cos(gamma) + (-1) * vec.y * sin(gamma);
	tmp.y = vec.x * sin(gamma) + vec.y * cos(gamma);
	tmp.z = vec.z;
	vec = tmp;

	// Y-Rotation
	tmp.x = vec.x * cos(beta) + vec.z * sin(beta);
	tmp.y = vec.y;
	tmp.z = (-1) * vec.x * sin(beta) + vec.z * cos(beta);
	vec = tmp;

	// X-Rotation
	tmp.x = vec.x;
	tmp.y = vec.y * cos(alpha) + (-1) * vec.z * sin(alpha);
	tmp.z = vec.y * sin(alpha) + vec.z * cos(alpha);
	vec = tmp;

	return vec;
};



// Compare
bool icarus_equals_commands(command_t a, command_t b) {
	return ((a.pitch == b.pitch) && (a.roll == b.roll) && (a.yaw == b.yaw) && (a.throttle == b.throttle) && (a.aux == b.aux));
};

bool icarus_equals_vectors(vector3d_t a, vector3d_t b) {
	return ((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
};



// Timestamp
unsigned long icarus_millis() {
	return (unsigned long) esp_log_timestamp();
};

unsigned long icarus_micros() {
	return (unsigned long) esp_timer_get_time();
};

// Delays
void icarus_delay(unsigned long ms) {
	vTaskDelay(ms / portTICK_PERIOD_MS);
};

void icarus_delay_micros(unsigned long us) {
	unsigned long start = esp_timer_get_time();
    
	while((icarus_micros() - start) < us);
};

// Math
long icarus_map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
};



// System Time
void icarus_system_time_init() {
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	//sntp_setservername(0, "pool.ntp.org");
	sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
	sntp_setservername(0, "it.pool.ntp.org");

	sntp_init();

	ESP_LOGW(TAG_UTILS, "Sync Time");

	// Sync
	time_t now;
	struct tm timeinfo;
	//while (timeinfo.tm_year < (2016 - 1900)) {
	while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) {
		icarus_delay(500);
		time(&now);
		localtime_r(&now, &timeinfo);
	}


	struct timeval tv;

	// TEST
	//sntp_sync_time();

	gettimeofday(&tv, NULL);

	time(&now);
    localtime_r(&now, &timeinfo);

    printf("Data corrente: %d-%02d-%02d %02d:%02d:%02d.%06ld\n",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour + 1, timeinfo.tm_min, timeinfo.tm_sec, tv.tv_usec);
};

void icarus_system_time_print_now() {
	time_t now;
	struct tm timeinfo;
	struct timeval tv;

	time(&now);
    localtime_r(&now, &timeinfo);
	gettimeofday(&tv, NULL);

    printf("Now: %d-%02d-%02d %02d:%02d:%02d.%06ld\n",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, tv.tv_usec);
};