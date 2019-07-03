add_library(controller SHARED

impedance_controller.cpp
pid_controller.cpp


)

target_link_libraries(controller

    ${orocos_kdl_LIBRARIES}
)