cmake_minimum_required(VERSION 3.16)
project(CTwitter2)
set(CMAKE_C_STANDARD 99)

add_executable(CTwitter2 main.c slre.c cJSON.c)

target_link_libraries(CTwitter2 ws2_32)
