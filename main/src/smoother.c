#include "../include/smoother.h"
#include "../include/queue.h"
#include "../include/utils.h"
#include <math.h>
#include <esp_log.h>

static const char* TAG = "Icarus Smoother";

static queue_t acc_queue = {NULL, NULL};
static vector3d_t acc_sum = {0.0, 0.0, 0.0};
static queue_t gyro_queue = {NULL, NULL};
static vector3d_t gyro_sum = {0.0, 0.0, 0.0};


void icarus_init_smoother() {
	vector3d_t zero = {0,0,0};
	int i;
	for (i = 0; i < CONFIG_ICARUS_QUEUE_SIZE; ++i) {
		enqueue(&acc_queue, zero);
		enqueue(&gyro_queue, zero);
	}
};



vector3d_t approx(vector3d_t src, uint8_t digits) {
	vector3d_t result;
	float factor = pow(10.0, digits);

	result.x = (((int) (src.x * factor)) / factor);
	result.y = (((int) (src.y * factor)) / factor);
	result.z = (((int) (src.z * factor)) / factor);

	return result;
};

vector3d_t smooth_acc(vector3d_t src) {
	vector3d_t result;
	node_t* node;

	enqueue(&acc_queue, src);
	acc_sum = icarus_add(acc_sum, src);
	node = dequeue(&acc_queue);
	acc_sum = icarus_subtract(acc_sum, node->value);

	result = icarus_divide(acc_sum, CONFIG_ICARUS_QUEUE_SIZE);

	free(node);

	return result;
};

vector3d_t smooth_gyro(vector3d_t src) {
	vector3d_t result;
	node_t* node;

	enqueue(&gyro_queue, src);
	gyro_sum = icarus_add(gyro_sum, src);
	node = dequeue(&gyro_queue);
	gyro_sum = icarus_subtract(gyro_sum, node->value);

	result = icarus_divide(gyro_sum, CONFIG_ICARUS_QUEUE_SIZE);

	free(node);

	return result;
};