cmake_minimum_required(VERSION 3.20)
project(SocketExample C)

set(CMAKE_C_STANDARD 11)

# Target for the Server
add_executable(Server server.c)
target_link_libraries(Server ws2_32)

# Target for the Client
add_executable(Client client.c)
target_link_libraries(Client ws2_32)
