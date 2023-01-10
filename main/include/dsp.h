#ifndef DSP_H
#define DSP_H

#include "shared.h"
#include "utils.h"

typedef struct {
	vector3d_t signal;
	float alpha;
} dlpf_t;

extern void dlpf_init(dlpf_t* lp, vector3d_t start, float cutoff, float dt);
extern void dlpf(dlpf_t* lp, vector3d_t sample);

#endif //! DSP_H