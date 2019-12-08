add_library(RobotJointClient 

    robot_joint_client.cpp
)
target_link_libraries(RobotJointClient 
    EthercatManager
    ElmoClient
)