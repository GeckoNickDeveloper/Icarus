#include "../include/Icarus/HardwareMonitor.hpp"

#include "../include/Icarus/Vector3D.hpp"

#include <bh1750.h>
#include <mpu6050.h>
#include <esp_err.h>
#include <driver/i2c.h>

namespace Icarus {

//! @todo fix
HardwareMonitor::HardwareMonitor() {
	// init i2c

	// Init bh1750 (create + power_on)
	// Init mpu6050 (create + wake_up)
};

//! @todo fix
Vector3D HardwareMonitor::getAcceleration() {
	Vector3D acc;
	mpu6050_acce_value_t mpuAcceleration;

	mpu6050_get_acce(this->mpu, &mpuAcceleration);
	acc = Vector3D(mpuAcceleration.acce_x, mpuAcceleration.acce_y, mpuAcceleration.acce_z);

	return acc;
};

//! @todo fix
Vector3D HardwareMonitor::getRotation() {
	Vector3D rot;

	mpu6050_gyro_value_t mpuRotation;

	mpu6050_get_acce(this->mpu, &mpuRotation);
	rot = Vector3D(mpuRotation.gyro_x, mpuRotation.gyro_y, mpuRotation.gyro_z);

	return rot;
};

//! @todo fix
double HardwareMonitor::getTemperature() {
	double temp;
	mpu6050_temp_value_t mpuTemperature;

	mpu6050_get_temp(this->mpu, &mpuTemperature);
	temp = mpuTemperature.temp;

	return temp;
};

//! @todo fix
double HardwareMonitor::getLuminosity() {
	double lum;

	bh1750_get_data(this->bh, &lum);

	return lum;
};

//! @todo fix
HardwareMonitor* HardwareMonitor::getInstance(void) {
	if (HardwareMonitor::instance == nullptr)
		HardwareMonitor::instance = new HardwareMonitor();
	
	return HardwareMonitor::instance;
};

}