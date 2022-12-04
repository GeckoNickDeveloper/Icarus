#ifndef SHARED_H
#define SHARED_H

#include <pthread.h>

typedef struct vector3d {
	float x;
	float y;
	float z;
} vector3d_t;

typedef struct command {
	uint8_t pitch;
	uint8_t roll;
	uint8_t yaw;
	uint8_t throttle;
	uint8_t aux;
} command_t;

typedef struct telemetry {
	vector3d_t position;
	vector3d_t velocity;
	vector3d_t orientation;
} telemetry_t;


// To decide
enum network_state {
	NETWORK_DISCONNECTED,
	NETWORK_CONNECTED
};

// To decide
enum mqtt_state {
	MQTT_DISCONNECTED,
	MQTT_CONNECTED
};


extern void icarus_init_shared();

extern telemetry_t icarus_get_shared_telemetry();
extern void icarus_set_shared_telemetry(telemetry_t new);
extern command_t icarus_get_shared_command();
extern void icarus_set_shared_command(command_t new);

#endif //! SHARED_H