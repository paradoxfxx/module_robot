class JointImpedanceControl
{

public:

    JointImpedanceControl();
    /*set the joint stifness */
    void	set_stiffness(double StiffnessScalar);

    /*set the joint damping */
	void	set_damping(double DampingScalar);

    /*set the joint inertia */
	void	set_inertia(double IneratiaScalar);

    void    set_torque_threshold(int threshold);

    void    sent_plan(double &pos, double &vel, double &acc);

    void    update_err(double &jnt_pos, double &jnt_vel, double &jnt_acc);

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

    int torque_threshold;
};