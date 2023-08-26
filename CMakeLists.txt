cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
project(JumpGame)


# sfml is build from source and is present in externallibs folder
add_subdirectory(externallibs/SFML)


link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/externallibs/SFML/lib)

add_executable(${PROJECT_NAME} src/main.cpp src/Camera.cpp src/Platform.cpp src/PlatformPool.cpp)

if(WIN32)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;$(ProjectDir)/externallibs/SFML/lib/Debug;"
    )
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/externallibs/SFML/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-window-d)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-audio-d)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics-d)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-network-d)
target_link_libraries(${PROJECT_NAME} PUBLIC sfml-system-d)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC sfml-main-d)
endif()
