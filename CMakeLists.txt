cmake_minimum_required(VERSION 3.9)

project(inference_engine_node)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(NODE_NODULES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/node_modules")

set(ARTIFACT_PATH "build")

find_package(InferenceEngine 2021.4.0 REQUIRED)

if(DEFINED ENV{NODE_PATH})
    set(NODE_PATH "$ENV{NODE_PATH}")
else()
    message(FATAL_ERROR "NODE_PATH environment variable is not set")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
                    "${InferenceEngine_INCLUDE_DIRS}"
                    "${NODE_NODULES_PATH}/node-addon-api"
                    "${NODE_PATH}/include/node")

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-frtti -fexceptions")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

set_target_properties(${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ARTIFACT_PATH}/${CMAKE_BUILD_TYPE}"
                                                 LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ARTIFACT_PATH}/${CMAKE_BUILD_TYPE}"
                                                 RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ARTIFACT_PATH}/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PRIVATE ${InferenceEngine_LIBRARIES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(${PROJECT_NAME} PRIVATE "${NODE_PATH}/x64/node.lib")
endif()

add_definitions(-DNAPI_VERSION=4)
