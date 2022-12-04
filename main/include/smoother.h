#ifndef SMOOTHER_H
#define SMOOTHER_H

#include "shared.h"


extern void icarus_init_smoother();
extern vector3d_t approx(vector3d_t src, uint8_t digits);
extern vector3d_t smooth_acc(vector3d_t src);
extern vector3d_t smooth_gyro(vector3d_t src);

#endif //! SMOOTHER_H