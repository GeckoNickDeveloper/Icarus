#ifndef DPS_H
#define DPS_H

//extern float icarus_lp_filter(const float prev, const float next, const float cutoff);
//extern float icarus_hp_filter(const float prev, const float next, const float cutoff);

extern vector3d_t icarus_lp_filter(vector3d_t* prev, const vector3d_t next, const float cutoff);
extern vector3d_t icarus_hp_filter(vector3d_t* prev_f, vector3d_t* prev_r, const vector3d_t next, const float cutoff);

#endif //! DPS_H