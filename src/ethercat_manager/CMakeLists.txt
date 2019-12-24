add_library( EthercatManager 

    ethercat_manager.cpp
)

target_link_libraries( EthercatManager
    ${LIB_XENOMAI}
)