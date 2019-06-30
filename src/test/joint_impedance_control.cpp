#include <memory>
#include <iostream>

class JointImpedanceControl
{

public:

    JointImpedanceControl();

    void	set_stiffness(double StiffnessScalar);
	void	set_damping(double DampingScalar);
	void	set_inertia(double IneratiaScalar);

    void    sent_plan(double pos, double vel, double acc);

    void    update_err(double jnt_pos, double jnt_vel, double jnt_acc);
    void    get_output(double &jnt_tor);
    void    get_err(double &pos_err, double &vel_err, double &acc_err);

private:
    
    double inertia;
    double damping;
    double stiffness;

    double tar_pos;
    double tar_vel;
    double tar_acc;

    double err_pos;
    double err_vel;
    double err_acc;

    double torque;
};


JointImpedanceControl::JointImpedanceControl(){

    inertia = 0;
    damping = 0;
    stiffness = 0;

    tar_pos = 0;
    tar_vel = 0;
    tar_acc = 0;

    err_pos = 0;
    err_vel = 0;
    err_acc = 0;

    torque = 0;
}

void JointImpedanceControl::set_stiffness(double StiffnessScalar){
    stiffness = StiffnessScalar;
}

void JointImpedanceControl::set_damping(double DampingScalar){
    damping = DampingScalar;
}

void JointImpedanceControl::set_inertia(double IneratiaScalar){
    inertia = IneratiaScalar;
}

void JointImpedanceControl::sent_plan(double pos, double vel, double acc){
    tar_pos = pos;
    tar_vel = vel;
    tar_acc = acc;
 }

void JointImpedanceControl::update_err(double jnt_pos, double jnt_vel, double jnt_acc){

    torque = 0;

    err_pos = tar_pos - jnt_pos;
    err_vel = tar_vel - jnt_vel;
    err_acc = tar_acc - jnt_acc;

    torque += inertia * err_acc;
    torque += damping * err_vel;
    torque += stiffness * err_pos;

}

void JointImpedanceControl::get_output(double &jnt_tor){

    jnt_tor = torque;
}


void JointImpedanceControl::get_err(double &pos_err, double &vel_err, double &acc_err){

    pos_err = err_pos;
    vel_err = err_vel;
    acc_err = err_acc;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    std::shared_ptr<JointImpedanceControl> manager (new JointImpedanceControl());

    manager->set_inertia(0.01);
    manager->set_damping(0.001);
    manager->set_stiffness(500);

    manager->sent_plan(1.1, 0.5, 0.1);
    manager->update_err(1.0, 0.49, 0.099);

    double torque;
    manager->get_output(torque);
    std::cout<<"torque: "<<torque <<" mN.m." <<std::endl;

    manager->update_err(1.05, 0.49, 0.099);
    manager->get_output(torque);
    std::cout<<"torque: "<<torque <<" mN.m." <<std::endl;

    return 0;
}
