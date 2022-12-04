#ifndef HARDWAREMONITOR_HPP
#define HARDWAREMONITOR_HPP

#include <mpu6050.h>
#include <bh1750.h>
#include <driver/i2c.h>
#include "Vector3D.hpp"

//#define I2C_MASTER_SCL_IO           22							/*!< GPIO number used for I2C master clock */
//#define I2C_MASTER_SDA_IO           21							/*!< GPIO number used for I2C master data  */
//#define I2C_MASTER_NUM              0							/*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
//#define I2C_MASTER_FREQ_HZ          400000						/*!< I2C master clock frequency */
//#define I2C_MASTER_TX_BUF_DISABLE   0							/*!< I2C master doesn't need buffer */
//#define I2C_MASTER_RX_BUF_DISABLE   0							/*!< I2C master doesn't need buffer */
//#define I2C_MASTER_TIMEOUT_MS       1000

class HardwareMonitor {
private:
	static const uint16_t BH1750_ADDR = 0x23;
	static const uint16_t MPU6050_ADDR = 0x68;

	bh1750_handle_t bh;// Luminosity
	mpu6050_handle_t mpu;// Accelerometer, Gyroscope, Termometer

	static HardwareMonitor* instance;
	HardwareMonitor();
public:
	Vector3D getAcceleration();
	Vector3D getRotation();
	double getTemperature();
	double getLuminosity();

	static HardwareMonitor* getInstance(void);
};

#endif //! HARDWAREMONITOR_HPP