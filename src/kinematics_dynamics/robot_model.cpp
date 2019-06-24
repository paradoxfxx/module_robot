#include <kinematics_dynamics/robot_model.h>

namespace model 
{   

    RobotModel::RobotModel()
    {

    }

    RobotModel::RobotModel(const std::string &urdf_name)
    {

        if (!kdl_parser::treeFromFile(urdf_name, robot)){

            std::cout<<"Failed to construct kdl tree"<<std::endl;
        }

    }

    bool RobotModel::forward_kinematic(KDL::JntArray &joint_point, KDL::Frame &descartes_point, std::string &segement_name){

        if(! fksolver.JntToCart(joint_point,descartes_point,segement_name)){

            return true;
            
        }else{

            return false;
        }

    }

    bool RobotModel::inverse_kinematic(KDL::JntArray &descartes_point){

    }

    RobotModel::~RobotModel(){

    }

}