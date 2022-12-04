#include "../include/actuator.h"
#include "../include/utils.h"
#include <esp_log.h>

static const char* TAG = "Icarus Actuator";


void icarus_init_actuator() {
	ESP_LOGW(TAG, "TODO: INIT ACTUATOR");
};



void icarus_apply_command(command_t cmd) {
	// TODO
	ESP_LOGI(TAG, "Applying command: [%x%x%x%x%x]", cmd.pitch, cmd.roll, cmd.yaw, cmd.throttle, cmd.aux);
};

// Thread worker
void* icarus_actuator_worker(void* args) {
	command_t cmd;
	command_t prev_cmd;

	while(1) {
		cmd = icarus_get_shared_command();

		if (!icarus_equals_commands(cmd, prev_cmd))
			icarus_apply_command(cmd);
	
		prev_cmd = cmd;

		icarus_delay(1000);
	}
	
	return NULL;
}