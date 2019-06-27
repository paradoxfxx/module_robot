#ifndef _ROBOT_MODEL_H_
#define _ROBOT_MODEL_H_

#include <kdl/chain.hpp>
#include <kdl/segment.hpp>
#include <kdl/joint.hpp>
#include <kdl/tree.hpp>

#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolverpos_nr.hpp>

#include <kdl/chainfksolvervel_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>

#include <kdl/chainfdsolver_recursive_newton_euler.hpp>
#include <kdl/chainidsolver_recursive_newton_euler.hpp>

#include <kdl_parser/kdl_parser.hpp>

namespace model
{

class RobotModel
{
    public:

        RobotModel();

        RobotModel(const std::string &urdf_name);

        bool    forward_kinematic_pos( KDL::JntArray &joint_pos,
                                    KDL::Frame &descartes_pos);

        bool    inverse_kinematic_pos( KDL::JntArray &joint_pos_curr,
                                    KDL::Frame &descartes_pos,
                                    KDL::JntArray &joint_out_pos);

        bool    forward_kinematic_vel( KDL::JntArrayVel &joint_vel,
                                    KDL::FrameVel &descartes_vel);

        bool    inverse_kinematic_vel( KDL::JntArray& joint_pos_curr,
                                    KDL::Twist& descartes_vel, 
                                    KDL::JntArray& joint_out_vel);

        // bool forward_kinematic_acc();

        // bool inverse_kinematic_acc();

        bool    forward_dynamic();

        bool    inverse_dynamic(   KDL::JntArray &joint_pos_curr,
                                KDL::JntArray &joint_vel_curr,
                                KDL::JntArray &joint_acc_curr, 
                                KDL::Wrenches &f_ext,
                                KDL::JntArray &joint_torques_out);
    public:


        ~RobotModel();

    private:

        std::string top_point_name;
        std::string root_base_name;

        
    private:

        KDL::Vector gravity;
        KDL::Tree robot_tree;
        KDL::Chain robot_chain;
        std::vector<KDL::Joint> joints;
        std::vector<KDL::Frame> frames;
        std::vector<KDL::Segment> segements;

        urdf::ModelInterface robot_model;

        // forward and inverse kinematic posistion solver 
        std::shared_ptr<KDL::ChainFkSolverPos_recursive> fksolver_chain_pos;
        std::shared_ptr<KDL::ChainIkSolverPos_NR> iksolver_chain_pos;

        // forward and inverse kinematic velocity solver 
        std::shared_ptr<KDL::ChainFkSolverVel_recursive> fksolver_chain_vel;
        std::shared_ptr<KDL::ChainIkSolverVel_pinv> iksolver_chain_vel;

        // forward and inverse dynamics solver 
        std::shared_ptr<KDL::ChainFdSolver_RNE> fdsolver_chain;
        std::shared_ptr<KDL::ChainIdSolver_RNE> idsolver_chain;

};


}
#endif