#ifndef _ECAT_DC_H_
#define _ECAT_DC_H_

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

long long dc_pi_sync(long long  reftime, long long  cycletime, int32_t shift_time);

#ifdef __cplusplus
}
#endif


#endif //_ECAT_DC_H_

