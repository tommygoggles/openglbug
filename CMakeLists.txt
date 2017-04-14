#Be sure to add CMake to the system PATH so that you can use it from the command line. Add it for the current or all users as appropriate. 

#CMake can, of course, be installed as part of Cygwin.!!!!!!

#errors with the cmake-gui may be down to it not finding the compiler... tried visual studio 64 bit 2014? ok?

#need to have selecting between platfrom, sdl, sdl2, glut, win, fmod switch, debug/release/etc.




cmake_minimum_required(VERSION 2.8)


#The project command names your project. Optionally you can specify what language the project supports, any of CXX, C, JAVA, or FORTRAN. 
project(openglbug)



#make it use the build type!!!
#if(NOT CMAKE_BUILD_TYPE) 
#    set(CMAKE_BUILD_TYPE Debug)
#endif(NOT CMAKE_BUILD_TYPE)









# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
#set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})



# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}")


 
#Bring the headers, such as Student.h into the project
#include_directories(include)
 
 
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB COMMON_SOURCES "*.cpp")

file(GLOB TGSDL2_SOURCES "tgsdl2/*.cpp")

file(GLOB TGGLUT_SOURCES "tgglut/*.cpp")



#This command tells CMake you want to make an executable and adds it as a target.
#The first argument is the name of the executable and the rest are the source files.
#You may notice that header files aren’t listed. CMake handles dependencies automatically so headers don’t need to be listed. 
#add_executable(hello helloworld.cpp)




#how about defines? compile flags? icon/etc.? "-lmingw32 -lSDL2main -lSDL2" OpenGL32? , mac stuff?? fmod? glut? windows? steam? GLEW?editedbyme?



#find_file(SDL2_INCLUDE_DIR NAME SDL.h HINTS SDL2)
#find_library(SDL2_LIBRARY NAME SDL2)

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)




find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} )
link_libraries(${OPENGL_gl_LIBRARY})


find_package(SDL2)
IF( SDL2_FOUND )
add_executable(glbug ${COMMON_SOURCES} ${TGSDL2_SOURCES})
include_directories( ${SDL2_INCLUDE_DIR} )
target_link_libraries(glbug ${SDL2_LIBRARY})
ENDIF()
    




find_package(GLUT)
IF( GLUT_FOUND )
add_executable(glbugglut ${COMMON_SOURCES} ${TGGLUT_SOURCES})
include_directories(${GLUT_INCLUDE_DIRS})
target_link_libraries(glbugglut ${GLUT_LIBRARY})
ENDIF()

#add_compile_options(<option> ...)
#-fno-omit-frame-pointer ?


















