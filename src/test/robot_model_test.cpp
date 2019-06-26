#include <kinematics_dynamics/robot_model.h>

#include <memory>

int main(int argc, char const *argv[])
{
    std::shared_ptr<model::RobotModel> robot (new model::RobotModel());
    
    return 0;
}
