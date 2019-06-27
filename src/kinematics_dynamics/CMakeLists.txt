add_library(robotmodel SHARED

    robot_model.cpp
)

target_link_libraries(robotmodel

    ${orocos_kdl_LIBRARIES}
    ${kdl_parser_LIBRARIES}

)

