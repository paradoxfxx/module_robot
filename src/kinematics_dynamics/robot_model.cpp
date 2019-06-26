#include <kinematics_dynamics/robot_model.h>

namespace model 
{   

    RobotModel::RobotModel()
    {

    }

    RobotModel::RobotModel(const std::string &urdf_name)
    {

        if (!kdl_parser::treeFromFile(urdf_name, robot_tree)){

            std::cout<<"Failed to construct kdl tree"<<std::endl;
        }else{
            
            //convert tree to chain
            robot_tree.getChain(robot_tree.getRootSegment()->first,
                                robot_tree.getSegments().end()->first,
                                robot_chain);

            // get the top point name
            top_point_name = robot_tree.getSegments().end()->first;

            //Initialize gravity(to do)
            // gravity = 

            // Initialize solver
            // forward and inverse kinematic posistion solver 
            fksolver_tree_pos = std::make_shared<KDL::TreeFkSolverPos_recursive>(robot_tree);
            fksolver_chain_pos = std::make_shared<KDL::ChainFkSolverPos_recursive>(robot_chain);
            
            KDL::ChainFkSolverPos_recursive fksolver1(robot_chain);//Forward position solver
            KDL::ChainIkSolverVel_pinv iksolver1v(robot_chain);//Inverse velocity solver
            iksolver_chain_pos = std::make_shared<KDL::ChainIkSolverPos_NR>(robot_chain,fksolver1,iksolver1v);

            // forward and inverse kinematic velocity solver 
            fksolver_chain_vel = std::make_shared<KDL::ChainFkSolverVel_recursive>(robot_chain);
            iksolver_chain_vel = std::make_shared<KDL::ChainIkSolverVel_pinv>(robot_chain);

            // forward and inverse dynamics solver 
            idsolver_chain = std::make_shared<KDL::ChainIdSolver_RNE>(robot_chain,gravity);
            // fdsolver_chain = std::make_shared<KDL::ChainFdSolver_RNE>(robot_chain,gravity);

        }

    }

    bool RobotModel::forward_kinematic_pos( KDL::JntArray &joint_pos,
                                            KDL::Frame &descartes_pos){

        if(! fksolver_tree_pos->JntToCart(joint_pos,descartes_pos,top_point_name)){

            return true;
            
        }else{

            return false;
        }

    }

    bool RobotModel::inverse_kinematic_pos( KDL::JntArray &joint_pos_curr,
                                            KDL::Frame &descartes_pos,
                                            KDL::JntArray &joint_out_pos){

        if(! iksolver_chain_pos->CartToJnt(joint_pos_curr,descartes_pos,joint_out_pos)){

            return false;

        }else{
            
            return true;
        }
    }



    bool RobotModel::forward_kinematic_vel( KDL::JntArrayVel &joint_vel,
                                            KDL::FrameVel &descartes_vel){

        if(! fksolver_chain_vel->JntToCart(joint_vel, descartes_vel)){
            return false;
        }else{
            return true;
        }

    }

    bool RobotModel::inverse_kinematic_vel( KDL::JntArray& joint_pos_curr,
                                            KDL::Twist& descartes_vel, 
                                            KDL::JntArray& joint_out_vel){
        if(! iksolver_chain_vel->CartToJnt(joint_pos_curr,descartes_vel,joint_out_vel)){
            return false;
        }else{
            return true;
        }

    }

    bool RobotModel::forward_dynamic(){

    }

    bool RobotModel::inverse_dynamic(   KDL::JntArray &joint_pos_curr,
                                        KDL::JntArray &joint_vel_curr,
                                        KDL::JntArray &joint_acc_curr, 
                                        KDL::Wrenches &f_ext,
                                        KDL::JntArray &joint_torques_out){
                                            
        if(! idsolver_chain->CartToJnt(joint_pos_curr,joint_vel_curr,joint_acc_curr,f_ext,joint_acc_curr)){
            return false;
        }else{
            return true;
        }

    }

    RobotModel::~RobotModel(){

    }

}