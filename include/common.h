#ifndef COMMON_H
#define COMMON_H


/*********************************FILE PATH*********************************/
#define XENOMAI_PATH " "

#define NETWORK_CARD_NAME "enp4s0"


/*********************************FILE PATH END*****************************/

/*********************************MOTOR************************************/

// Reduction ratio
#define REDUCTION_RATIO 160

// Encoder resolution
#define RELATIVE_RESOLUTION  9728   /*  */
// #define ABSOLUTE_RESOLUTION 524288  /* 2**19 */
#define ABSOLUTE_RESOLUTION 4096  /* 2**19 */

// torque sensor function: transfer voltage to torque 
// sensor range : +-200 Nm
// analog range : +- 10000
// 0.02 = 200 / 10000
#define TORQUE_ALALOG_TRANSITION(VOLTAGE)   (int16)(VOLTAGE) * 0.02


#define PI 3.141592

// covert radian to degree
#define PI_RAD_DEG 57.295779513

// covert degree to radian
#define PI_DEG_RAD 0.017453293

//motor rate torque (unit: mN.m)
#define RATE_TORQUE 600

// motor max torque (unit: RATE_TORQUE/1000)
#define MAX_TORQUE 1000

/*convert motor data(pos,vel())to motor(count) */
#define DATA_TO_COUNT(DATA)             (uint32)(DATA  / 360 * RELATIVE_RESOLUTION)

/*convert motor data(cout) to motor(pos,vel) */
#define COUNT_TO_DATA(COUNT)            (int32)(COUNT) / RELATIVE_RESOLUTION * 360.0 

/*convert user torque(mN.m) to motor torque(per thousand of rate torque) */
#define TORQUE_USER_TO_MOTOR(TORQUE)    (uint32)(TORQUE / RATE_TORQUE * 1000 )

/*convert motor torque(per thousand of rate torque) to user torque(mN.m) */
#define TORQUE_MOTOR_TO_USER(TORQUE)    (int32)(TORQUE) / 1000.0 * RATE_TORQUE 

// /*convert user torque(mN.m) to motor torque */
// #define TORQUE_USER_TO_JOINT(TORQUE)    (float)(TORQUE  * 1000 / RATE_TORQUE)

/*************************************MOTOR END***************************************/

/****************************************ETHERCAT*************************************/

typedef enum {  NOT_READY, 
                SWITCH_DISABLED,
                READY_SWITCH, 
                SWITCHED_ON,
                OPERATION_ENABLED,
                QUICK_STOP,
                FAULT_REACTION,
                FAULT,
                UNKNOWN} PDS_STATUS;  // Statusword(6041h) 

typedef enum {  NO_MODE_CHANGE_1,
                PROFILE_POSITION_MODE,
                VELOCITY_MODE,
                PROFILE_VELOCITY_MODE,
                TORQUE_PROFILE_MODE,
                NO_MODE_CHANGE_2,
                HOMING_MODE,
                INTERPOLATED_POSITION_MODE,
                CYCLIC_SYNCHRONOUS_POSITION_MODE,
                CYCLIC_SYNCHRONOUS_VELOCITY_MODE,
                CYCLIC_SYNCHRONOUS_TORQUE_MODE} PDS_OPERATION; // Mode of operation(6061h) 

//  typedef enum { HALT,
//                 FAULT_RESET, 
//                 ENABLE_OPERATION, 
//                 QUICK_STOP, 
//                 ENABLE_VOLTAGE, 
//                 SWITCH_ON} PDS_CONTROL; // Controlworld(6040h)

/*************************************ETHERCAT END*************************************/


/****************************************ELMO*************************************/
#define MAX_INTERPOLATION_BUFF_SIZE 16
#define ACTUAL_INTERPOLATION_BUFF_SIZE_LIMIT 10
#define DEFAULT_INTERPOLATION_TIME_PERIOD 1 /* 1ms */
/*************************************ELMO END*************************************/

#endif