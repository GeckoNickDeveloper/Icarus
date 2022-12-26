#include "../include/shared.h"
#include "../include/actuator.h"
#include "../include/communicator.h"
#include "../include/network.h"
#include "../include/sensors.h"
#include "../include/utils.h"
#include "../include/queue.h"
#include "../include/smoother.h"
#include "../include/workers.h"

#include <pthread.h>

void app_main() {
	icarus_init_shared();
	icarus_init_smoother();
	icarus_init_sensors();
	icarus_init_actuator();
	icarus_network_setup();
#if CONFIG_ICARUS_TESTING_ENABLE_SYSTEM_TIME_SYNC
	icarus_system_time_init();
#endif
	icarus_mqtt_setup();

	
	pthread_t actuator_thread;
	pthread_t sensors_thread;
	pthread_t proximity_thread;
	pthread_t communication_thread;
	pthread_t pilot_thread;


	// Thread creation
	//pthread_create(&actuator_thread, NULL, icarus_actuator_worker, NULL);
	//pthread_create(&sensors_thread, NULL, icarus_sensor_worker, NULL);
	//pthread_create(&proximity_thread, NULL, icarus_proximity_worker, NULL);
	//pthread_create(&communication_thread, NULL, icarus_communication_worker, NULL);
	//pthread_create(&pilot_thread, NULL, <placeholder_2>, NULL);
	
	// JOIN THREADS ?
};