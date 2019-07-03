#include <robot_controller/pid_controller.h>

#include <iostream>

namespace controller
{

	PID_controller::PID_controller() :
        last_error(0.f),
        last_last_error(0.f),
        last_output(0.f),
        integral(0.f),
        error_threshold(0.f)
        {}

	void PID_controller::setKp(const float &Kp) {
		this->Kp = Kp;
	}

	void PID_controller::setKi(const float &Ki) {
		this->Ki = Ki*interval;
	}

	void PID_controller::setKd(const float &Kd) {
		this->Kd = Kd/interval;
	}

	void PID_controller::setWeights(const float &Kp, const float &Ki, const float &Kd) {
		setKp(Kp);
		setKi(Ki);
		setKd(Kd);
    }

	void PID_controller::setRefreshInterval(const float &refresh_interval) {
		interval = refresh_interval;
	}

	void PID_controller::setRefreshRate(const float &refresh_rate) {
		interval = 1.f/refresh_rate;
	}

	void PID_controller::setErrorThreshold(const float &error_threshold) {
		this->error_threshold = error_threshold;
	}

	void PID_controller::setOutputLowerLimit(const float &output_lower_limit) {
		this->output_lower_limit = output_lower_limit;
	}

	void PID_controller::setOutputUpperLimit(const float &output_upper_limit) {
		this->output_upper_limit = output_upper_limit;
	}

	void PID_controller::setDesiredPoint(const float &desired_point) {
		set_point = desired_point;
	}

	float PID_controller::refresh(const float &feedback_input) {

		error = set_point-feedback_input;
		if (error >= error_threshold or error <= -error_threshold) {

			// last_output = Kp*error + Ki*integral + Kd*(error-last_error);
            last_output = Kp * error - Ki * last_error + Kd * last_last_error; 

            last_last_error = last_error;
            last_error = error;
            
			if (last_output > output_upper_limit) {

				last_output = output_upper_limit;
				// if (integral/error < 0.f) {
				// 	integral += (error+last_error)/2.f;
				// 	last_error = error;
				// }
				return output_upper_limit;
			}
			if (last_output < output_lower_limit) {

				// if (integral/error < 0.f) {
				// 	integral += (error+last_error)/2.f;
				// 	last_error = error;
				// }
				last_output = output_lower_limit;
				return output_lower_limit;
			}
			// integral += (error+last_error)/2.f;
			// last_error = error;
		}
		return last_output;
	}

};