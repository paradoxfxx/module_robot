#ifndef COMMON_H
#define COMMON_H

namespace common
{

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

// Mode of operation(6061h)
#define PROFILE_POSITION_MODE 1
#define VELOCITY_MODE 2
#define PROFILE_VELOCITY_MODE 3 
#define TORQUE_PROFILE_MODE 4
#define HOMING_MODE 6
#define INTERPOLATED_POSITION_MODE 7
#define CYCLIC_SYNCHRONOUS_POSITION_MODE 8
#define CYCLIC_SYNCHRONOUS_VELOCITY_MODE 9
#define CYCLIC_SYNCHRONOUS_TORQUE_MODE  10

}

#endif