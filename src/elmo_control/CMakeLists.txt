add_library(ElmoClient 
    elmo_client.cpp
)

target_link_libraries(ElmoClient 
    EthercatManager
)