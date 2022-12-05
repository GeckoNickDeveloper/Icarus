#include "../include/utils.h"
#include <math.h>

#include <stdio.h>
#include <esp_sntp.h>
#include <sys/time.h>
#include <esp_timer.h>


// Trigonometry
float rad2deg(float angle) {
	return (angle * 57.2958);//(angle * (PI / 180.0));
};
float deg2rad(float angle) {
	return (angle * 0.0174533);//(angle * (180.0 / PI));
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
void print_timestamp() {
	struct timeval tv;
	time_t nowtime;
	struct tm *nowtm;
	char tmbuf[64], buf[64];

	//sntp_sync_time(NULL);
	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
	printf("[%s.%06ld]", tmbuf, tv.tv_usec);
	//snprintf(buf, sizeof buf, "[%s%ld]", tmbuf, tv.tv_usec % 1000000);
};




unsigned long icarus_micros() {
	return (unsigned long) esp_timer_get_time();
};

// Delays
void icarus_delay(unsigned long ms) {
	vTaskDelay(ms / portTICK_PERIOD_MS);
};

void icarus_delay_micros(unsigned long us) {
	unsigned long m = esp_timer_get_time();
    if(us){
		unsigned long e = (m + us);
		if(m > e){ //overflow
			while(esp_timer_get_time() > e);
		}
		while(esp_timer_get_time() < e);
    }
};