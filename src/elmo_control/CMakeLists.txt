add_library(elmoclient 
    elmo_client.cpp
)

target_link_libraries(elmoclient 
    ethercatmanager
)