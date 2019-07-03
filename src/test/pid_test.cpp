#include <robot_controller/pid_controller.h>

#include <memory>
#include <iostream>

int main(int argc, char const *argv[])
{
    std::shared_ptr<controller::PID_controller> manager (new controller::PID_controller());

    manager->pid_set_gains(100,0.5,0.1);
    manager->pid_set_frequency(1);
    manager->pid_set_integral_limit(100);
    
    float tar_pos = 100;
    float err_pos;
    float error = 0.5;
    err_pos = manager->pid_process(error);
    std::cout<<"pid out put: "<<err_pos <<std::endl;

    return 0;
}
