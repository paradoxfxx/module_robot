#ifndef PID_CONTROLLER_H_
#define PID_CONTROLLER_H_

#include <stdbool.h>
#include <stdint.h>

namespace controller
{

class PID_controller
{
	float last_error;
        float last_output;
        float last_last_error;
        float set_point;
        float Kp, Ki, Kd;
        float integral;
        float interval;
        float error_threshold;
        float output_upper_limit;
        float output_lower_limit;
        float error;
        
    public:
        /// Public constructor
        PID_controller();

        /// Set the proportional term. 
        void setKp(const float &Kp);

        /// Set the integral term. 
	    void setKi(const float &Ki);
        
        /// Set the derivative term. 
        void setKd(const float &Kd);

        /// Set the P, I, D terms respectively. 
        void setWeights(const float &Kp, const float &Ki, const float &Kd);

        /// Set the minimun error for computation of the PID loop. The default is 0.0
        /// It can be set to prevent integral windup or updates with minimal errors
        void setErrorThreshold(const float &error_threshold);

        /// Set the lower limit of the output. Output will be clamped to this value.
        /// If clamped, the integral will not be computed to prevent integral windup.
        /// To set the upper limit use setOutputUpperLimit
        /// YOU NEED TO SET IT!
        void setOutputLowerLimit(const float &output_lower_limit);

        /// Set the upper limit of the output. Output will be clamped to this value.
        /// If clamped, the integral will not be computed to prevent integral windup.
        /// To set the lower limit use setOutputLowerLimit
        /// YOU NEED TO SET IT!
        void setOutputUpperLimit(const float &output_upper_limit);

        /// Set the desired point of the output.
        /// This is the output the PID loop will try to reach.
        /// error will be computed subtracting the feedback input from this value
        void setDesiredPoint(const float &desired_point);

        /// The refresh funcion that make all the PID computation.
        /// Call it at specific rate, setted by setRefreshRate or setRefreshInterval (read descriptions).
        /// (use a timer interrupt if used on a embedded microcontroller)
        /// The feedback_input is the value from the feedback.
        /// The returned value is the output value of the filter.
        float refresh(const float &feedback_input);

};

}
#endif