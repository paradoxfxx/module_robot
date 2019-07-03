#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include <stdlib.h>
#include <math.h>

namespace controller
{

class PID_controller
{
    
public:
    /** Initializes a PID controller. */
    PID_controller();


    /** Sets the gains of the given PID. */
    void pid_set_gains(float kp, float ki, float kd);

    /** Returns the proportional gains of the controller. */
    void pid_get_gains(float &kp, float &ki, float &kd);

    /** Returns the limit of the PID integrator. */
    float pid_get_integral_limit();

    /** Returns the value of the PID integrator. */
    float pid_get_integral();

    /** Process one step if the PID algorithm. */
    float pid_process(float error);

    /** Sets a maximum value for the PID integrator. */
    void pid_set_integral_limit(float max);

    /** Resets the PID integrator to zero. */
    void pid_reset_integral();

    /** Sets the PID frequency for gain compensation. */
    void pid_set_frequency(float frequency);

    /** Gets the PID frequency for gain compensation. */
    float pid_get_frequency();

private:

    float kp_;
    float ki_;
    float kd_;
    float integrator_;
    float previous_error_;
    float integrator_limit_;
    float frequency_;

};

}
#endif