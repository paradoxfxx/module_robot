add_library(robotclient 

    robot_client.cpp
)
target_link_libraries(robotclient 
    ethercatmanager
    elmoclient
)