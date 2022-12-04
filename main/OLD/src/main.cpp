#include <stdio.h>

//#include <Icarus/Vector3D.hpp>
#include "../include/Icarus/Actuator.hpp"
#include "../include/Icarus/Network.hpp"
#include "../include/Icarus/Communicator.hpp"

#include <pthread.h>

/**
 * @note 'Actuator', 'Network', 'HardwareMonitor' sono TUTTI signleton, dato che lavorano con l'hardawre puro
 * @note 'Shared' è singleton per via della sua unicità
 */

extern "C" void app_main(void) {
	Network* network = new Network();
	Actuator* actuator = new Actuator();
	Communicator* communicator = new Communicator("mqtt://mqtt.eclipseprojects.io");

	pthread_t communicationThread;

	pthread_create(&communicationThread, NULL, Communicator::taskHelper, communicator);
	
	printf("Ciao\n\n");
}
