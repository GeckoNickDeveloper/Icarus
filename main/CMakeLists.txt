set(srcs ${platform_srcs}
	"src/actuator.c"
	"src/communicator.c"
	"src/sensors.c"
	"src/utils.c"
	"src/shared.c"
	"src/network.c"
	"src/queue.c"
	"src/smoother.c"
	"src/workers.c"
	
	"src/dsp.c"
	
	"src/main.c"

	# Rubatissimo a causa di problemi di import
	"src/iot_servo.c"
	"src/mpu6050.c"
)

set(include_dirs "include")

idf_component_register(
	SRCS "${srcs}"
	INCLUDE_DIRS "${include_dirs}"
)