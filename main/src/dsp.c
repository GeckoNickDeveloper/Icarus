#include "../include/dsp.h"

void dlpf_init(dlpf_t* lp, vector3d_t start, float cutoff, float dt) {
	float rc = 1.0 / (cutoff * 2 * PI);

	
	lp->alpha = rc / (dt + rc);
	lp->signal = start;
	
	printf("alpha = %f", lp->alpha);
};

void dlpf(dlpf_t* lp, vector3d_t sample) {
	lp->signal =	icarus_add(
						icarus_multiply(lp->signal, lp->alpha),
						icarus_multiply(sample, (1.0 - lp->alpha))
					);
};
