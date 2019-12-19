#ifndef COMMON_H
#define COMMON_H


/*********************************FILE PATH*********************************/
#define XENOMAI_PATH " "

#define NETWORK_CARD_NAME "enp4s0"
/*********************************FILE PATH END*****************************/

/*********************************MOTOR************************************/
// Reduction ratio
#define REDUCTION_RATIO 160.0

// Encoder resolution
// #define RELATIVE_RESOLUTION  9728.0   /*  */
#define ABSOLUTE_RESOLUTION 524288.0  /* 2**19 */
#define RELATIVE_RESOLUTION 4096.0 

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
#define RATE_TORQUE 600.0

// motor max torque (unit: mN.m)
#define MAX_TORQUE 1200

// acceleration  deg/s-2
#define ACC 5000
//dcceleration 
#define DCC 5000

/*convert user torque(mN.m) to motor torque(per thousand of rate torque) */
#define TORQUE_USER_TO_MOTOR(TORQUE)    (int16_t)(int(TORQUE*1000.0/RATE_TORQUE))

/*convert motor torque(per thousand of rate torque) to user torque(mN.m) */
#define TORQUE_MOTOR_TO_USER(TORQUE)    ((int16_t)(TORQUE)/1000.0*RATE_TORQUE) 

/*convert motor data(pos,vel())to motor(count) */
#define DATA_TO_COUNT(DATA)             (int32_t)(DATA/360.0*RELATIVE_RESOLUTION)

/*convert motor data(cout) to motor(pos,vel) */
#define COUNT_TO_DATA(COUNT)            ((int32_t)(COUNT)/RELATIVE_RESOLUTION*360.0)


/*************************************MOTOR END***************************************/

/****************************************ETHERCAT*************************************/

typedef enum {  NOT_READY = 0, 
                SWITCH_DISABLED,
                READY_SWITCH, 
                SWITCHED_ON,
                OPERATION_ENABLED,
                QUICK_STOP,
                FAULT_REACTION,
                FAULT,
                UNKNOWN} PDS_STATUS;  // Statusword(6041h) 

typedef enum {  NO_MODE_CHANGE_1 = 0,
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
#define MAX_INTERPOLATION_BUFF_SIZE             16
#define ACTUAL_INTERPOLATION_BUFF_SIZE_LIMIT    10
#define DEFAULT_INTERPOLATION_TIME_PERIOD       1000000 /* 1ms */
/*************************************ELMO END*************************************/


/****************************************THREAD PRIORITY*************************************/
#define THREAD_ETHERCAT_PRIORITY        20
#define THREAD_MOTOR_FEEDBACK_PRIORITY  21
#define THREAD_TRAJECTORY_PRIORITY      22
/*************************************THREAD PRIORITY END*************************************/

#endif