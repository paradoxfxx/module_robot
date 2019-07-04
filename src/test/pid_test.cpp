#include <robot_controller/pid_controller.h>

#include <memory>
#include <iostream>

int main(int argc, char const *argv[])
{
    std::shared_ptr<controller::PID_controller> manager (new controller::PID_controller());

    manager->setWeights(100,0.01,0.001);
    manager->setDesiredPoint(100);
    manager->setErrorThreshold(0.001);
    manager->setOutputLowerLimit(50);
    manager->setOutputLowerLimit(-50);
    
    std::cout<<"pid_out_put: "<< manager->refresh(99.9) << "."<<std::endl;   

    return 0;
}
