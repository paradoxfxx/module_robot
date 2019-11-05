/////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory>
#include <iostream>
#include "robot_controller/joint_impedance_controler.h"


int main(int argc, char const *argv[])
{
    std::shared_ptr<JointImpedanceControl> manager (new JointImpedanceControl());

    // manager->set_inertia(0.01);
    // manager->set_damping(0.001);
    // manager->set_stiffness(500);

    // manager->sent_plan(1.1, 0.5, 0.1);
    // manager->update_err(1.0, 0.49, 0.099);

    // double torque;
    // manager->get_output(torque);
    // std::cout<<"torque: "<<torque <<" mN.m." <<std::endl;

    // manager->update_err(1.05, 0.49, 0.099);
    // manager->get_output(torque);
    // std::cout<<"torque: "<<torque <<" mN.m." <<std::endl;

    return 0;
}