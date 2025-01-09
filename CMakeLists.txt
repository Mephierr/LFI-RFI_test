cmake_minimum_required(VERSION 3.5) # Проверка версии CMake.

project(LFI-RFI_TEST)

find_package(CURL REQUIRED)

add_executable(LFI-RFI_TEST main.cpp)

target_link_libraries(LFI-RFI_TEST PRIVATE CURL::libcurl)