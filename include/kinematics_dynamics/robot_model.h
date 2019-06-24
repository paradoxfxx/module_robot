#ifndef _ROBOT_MODEL_H_
#define _ROBOT_MODEL_H_

#include <kdl/chain.hpp>
#include <kdl/segment.hpp>
#include <kdl/joint.hpp>
#include <kdl/tree.hpp>

#include <kdl/treeiksolverpos_online.hpp>
#include <kdl/treefksolver.hpp>
#include <kdl/treefksolverpos_recursive.hpp>


#include <kdl_parser/kdl_parser.hpp>


namespace model
{

class RobotModel
{
    public:

        RobotModel();

        RobotModel(const std::string &urdf_name);

        bool forward_kinematic(KDL::JntArray &joint_point, KDL::Frame &descartes_point, std::string &segement_name);

        bool inverse_kinematic(KDL::JntArray &descartes_point);



        ~RobotModel();

        
    private:

        KDL::Tree robot;
        std::vector<KDL::Joint> joints;
        std::vector<KDL::Frame> frames;
        std::vector<KDL::Segment> segements;

        urdf::ModelInterface robot_model;

        KDL::TreeFkSolverPos_recursive fksolver = KDL::TreeFkSolverPos_recursive(robot);
        // KDL::TreeIkSolverPos_Online iksolver = KDL::TreeIkSolverPos_Online(robot);

};



}
#endif