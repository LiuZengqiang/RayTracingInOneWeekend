cmake_minimum_required(VERSION 3.10.0)
project(RayTracingInOneWeekend VERSION 0.1.0 LANGUAGES C CXX)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(RayTracingInOneWeekend main.cpp)
