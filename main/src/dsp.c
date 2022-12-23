#include "../include/dsp.h"
#include "../include/utils.h"

vector3d_t icarus_lp_filter(vector3d_t* prev, const vector3d_t next, const float cutoff) {
	// Alpha calc
	float RC = 1.0 / (cutoff * 2 * PI);
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY;
	float alpha = dt / (RC + dt);

	// Filter
	vector3d_t filtered;
	filtered =	icarus_add(*prev,
					icarus_multiply(
						icarus_subtract(next, *prev),
						alpha));


	*prev = filtered;
	return filtered;
};

vector3d_t icarus_hp_filter(vector3d_t* prev_f, vector3d_t* prev_r, const vector3d_t next, const float cutoff) {
	// Alpha calc
	float RC = 1.0 / (cutoff * 2 * PI);
	float dt = 1.0 / CONFIG_ICARUS_SENSOR_SAMPLING_FREQUENCY;
	float alpha = RC / (RC + dt);

	// Filter
	vector3d_t filtered;
	filtered =	icarus_multiply(
					icarus_add(*prev_f,
						icarus_subtract(next, *prev_r)),
					alpha);


	*prev_f = filtered;
	*prev_r = next;
	return filtered;
};
