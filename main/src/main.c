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
#if CONFIG_ICARUS_DEBUG_SYNC_TIME_ENABLED
	icarus_system_time_init();
#endif
	icarus_mqtt_setup();

	
	pthread_t actuator_thread;
	pthread_t mpu6050_thread;
	pthread_t bh1750_thread;
	pthread_t sr04_thread;
	pthread_t communication_thread;
	pthread_t pilot_thread;


	// Thread creation
	//pthread_create(&actuator_thread, NULL, icarus_actuator_worker, NULL);
	pthread_create(&mpu6050_thread, NULL, icarus_mpu6050_worker, NULL);
	//pthread_create(&bh1750_thread, NULL, icarus_bh1750_worker, NULL);
	//pthread_create(&sr04_thread, NULL, icarus_sr04_worker, NULL);
	pthread_create(&communication_thread, NULL, icarus_communication_worker, NULL);
	//pthread_create(&pilot_thread, NULL, <placeholder_2>, NULL);
	
	// JOIN THREADS ?
};