#ifndef COMMON_H
#define COMMON_H



// Reduction ratio
#define REDUCTION_RATIO 160

// Encoder resolution
#define RESOLUTION 622592

// covert radian to degree
#define PI_RAD_DEG 57.295779513

// covert degree to radian
#define PI_DEG_RAD 0.017453293

//motor rate torque (unit: mN.m)
#define RATE_TORQUE 1000

// motor max torque (unit: RATE_TORQUE/1000)
#define MAX_TORQUE 2220

#define NETWORK_CARD_NAME "enp4s0"

/*convert joint data(pos,vel())to motor(count) */
#define DATA_TO_COUNT(DATA)             DATA * RESOLUTION * REDUCTION_RATIO / 360

/*convert motor data(cout) to joint data(pos,vel) */
#define COUNT_TO_DATA(COUNT)            COUNT * 360 / RESOLUTION / REDUCTION_RATIO

/*convert user torque(mN.m) to motor torque(per thousand of rate torque) */
#define TORQUE_USER_TO_MOTOR(TORQUE)    TORQUE * 1000 / RATE_TORQUE

/*convert motor torque(per thousand of rate torque) to user torque(mN.m) */
#define TORQUE_MOTOR_TO_USER(TORQUE)    TORQUE * RATE_TORQUE / 1000



typedef enum {NOT_READY, 
              SWITCH_DISABLED,
              READY_SWITCH, 
              SWITCHED_ON,
              OPERATION_ENABLED,
              QUICK_STOP,
              FAULT_REACTION,
              FAULT,
              UNKNOWN} PDS_STATUS;  // Statusword(6041h) 

typedef enum {NO_MODE_CHANGE,
              PROFILE_POSITION_MODE,
              VELOCITY_MODE,
              PROFILE_VELOCITY_MODE,
              TORQUE_PROFILE_MODE,
              HOMING_MODE,
              INTERPOLATED_POSITION_MODE,
              CYCLIC_SYNCHRONOUS_POSITION_MODE,
              CYCLIC_SYNCHRONOUS_VELOCITY_MODE,
              CYCLIC_SYNCHRONOUS_TORQUE_MODE} PDS_OPERATION; // Mode of operation(6061h) 

 typedef enum {//HALT, FAULT_RESET, ENABLE_OPERATION, QUICK_STOP, ENABLE_VOLTAGE, SWITCH_ON, 
} PDS_CONTROL; // Controlworld(6040h)



#endif