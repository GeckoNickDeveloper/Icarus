#include "../include/shared.h"

typedef struct shared {
	command_t cmd;
	telemetry_t tlm;
	float trn;			// Terrain distance, if greater of 4m returns 1
	float lux;			// Luminosity percepted by the UAV
} shared_t;


static pthread_mutex_t mtx;
static shared_t shared_data = {
	.cmd = {
		.roll = 0x00,
		.pitch = 0x00,
		.yaw = 0x00,
		.throttle = 0x00,
		.aux = 0x00,
	},
	.tlm = {
		.position = {0.0, 0.0, 0.0},
		.velocity = {0.0, 0.0, 0.0},
		.orientation = {0.0, 0.0, 0.0},
	},
	.trn = 0.0,
	.lux = 0.0,
};


void icarus_init_shared() {
	pthread_mutex_init(&mtx, NULL);
};



telemetry_t icarus_get_shared_telemetry() {
	telemetry_t result;
	
	pthread_mutex_lock(&mtx);
	result = shared_data.tlm;
	pthread_mutex_unlock(&mtx);

	return result;
};

void icarus_set_shared_telemetry(telemetry_t new) {
	pthread_mutex_lock(&mtx);
	shared_data.tlm = new;
	pthread_mutex_unlock(&mtx);
};



command_t icarus_get_shared_command() {
	command_t result;
	
	pthread_mutex_lock(&mtx);
	result = shared_data.cmd;
	pthread_mutex_unlock(&mtx);

	return result;
};

void icarus_set_shared_command(command_t new) {
	pthread_mutex_lock(&mtx);
	shared_data.cmd = new;
	pthread_mutex_unlock(&mtx);
};



float icarus_get_shared_terrain() {
	float result;
	
	pthread_mutex_lock(&mtx);
	result = shared_data.trn;
	pthread_mutex_unlock(&mtx);

	return result;
};

void icarus_set_shared_terrain(float new) {
	pthread_mutex_lock(&mtx);
	shared_data.trn = new;
	pthread_mutex_unlock(&mtx);
};



float icarus_get_shared_luminosity() {
	float result;
	
	pthread_mutex_lock(&mtx);
	result = shared_data.lux;
	pthread_mutex_unlock(&mtx);

	return result;
};

void icarus_set_shared_luminosity(float new) {
	pthread_mutex_lock(&mtx);
	shared_data.lux = new;
	pthread_mutex_unlock(&mtx);
};