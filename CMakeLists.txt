cmake_minimum_required(VERSION 3.17)
project(project_oop2)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

add_executable(project_oop2 WIN32 "")

add_subdirectory (include)
add_subdirectory (src)
add_subdirectory(resources)
add_subdirectory (resources/images)
add_subdirectory (resources/sounds)
add_subdirectory (resources/fonts)
add_subdirectory (external/box2d/src)


set (SFML_STATIC_LIBRARIES TRUE)
set (SFML_DIR "C:/SFML/SFML-2.5.1/lib/cmake/SFML")
configure_file(C:/SFML/SFML-2.5.1/bin/openal32.dll ${CMAKE_BINARY_DIR} COPYONLY)

#include_directories(/usr/local/include)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

set_target_properties (sfml-system sfml-main sfml-window sfml-graphics sfml-audio sfml-network PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(project_oop2 sfml-main sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

target_link_libraries (project_oop2 box2d)