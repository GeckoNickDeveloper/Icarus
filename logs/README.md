# LOG

I log contenuti in questa cartella racchiudono molteplici test con varie condizioni di sistema e di ambiente. Di seguito elencheremo ogni file con relative condizioni.

## /immobile
Directory contenente i test effettuati con il sensore in posizione immobile.

### ofst_s_g_wo_ofst_20hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 20Hz sampling freq
- Log Print = 1 second

### ofst_s_g_wo_ofst_50hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 50Hz sampling freq
- Log Print = 1 second

### ofst_s_g_wo_ofst_100hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 100Hz sampling freq
- Log Print = 1 second



### ga3d_ofst_s_g_wo_ofst_20hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 20Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (post rad conversion)

### ga3d_ofst_s_g_wo_ofst_50hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 50Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (post rad conversion)

### ga3d_ofst_s_g_wo_ofst_100hz_1s_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 100Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (post rad conversion)



### ga3d_ofst_s_g_wo_ofst_20hz_1s_deg_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 20Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (no rad conversion)

### ga3d_ofst_s_g_wo_ofst_50hz_1s_deg_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 50Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (no rad conversion)

### ga3d_ofst_s_g_wo_ofst_100hz_1s_deg_lp.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 100Hz sampling freq
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (no rad conversion)



### dlpf5hz_a3dga3d_osft_s_g_wo_ofst_50hz_1s_lp_rad_16G_500D.log
- Offset calucaltion
- Smoothed -> Gyro values without the offset
- 50Hz sampling freq
- Digital Low Pass Filter -> <5Hz freq passing
- Log Print = 1 second
- Every gyro lecture is approximated to 3 significant digits (before and after deg to rad conversion)