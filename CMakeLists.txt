CMAKE_MINIMUM_REQUIRED(VERSION 3.23.1)
project(cad)

ADD_EXECUTABLE(${PROJECT_NAME} main.cpp)

# add_subdirectory(external)
add_library(pge_lib pge_placeholder.cpp)

target_link_libraries(${PROJECT_NAME} pge_lib)

if (${APPLE})

#########################################################
# FIND CARBON
#########################################################
FIND_LIBRARY(CARBON_LIBRARY Carbon)
target_link_libraries(${PROJECT_NAME} ${CARBON_LIBRARY})

#########################################################
# FIND PNG
#########################################################
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})
link_directories(${PNG_LIBRARY_DIRS})
add_definitions(${PNG_DEFINITIONS})
if(NOT PNG_FOUND)
    message(ERROR " PNG not found!")
endif(NOT PNG_FOUND)
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES})

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)
target_link_libraries(${PROJECT_NAME} ${GLUT_LIBRARIES})

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

else()
TARGET_LINK_LIBRARIES(${PROJECT_NAME} -lasound -lX11 -lGL -lpthread -lpng )
endif()

add_definitions(-std=c++17)

# target_link_libraries(${PROJECT_NAME} pge_lib)