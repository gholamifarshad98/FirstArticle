# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(deprivedStereo)
set(CMAKE_INSTALL_PREFIX "CMAKE_CURRENT_SOURCE_DIR/../../install" CACHE PATH "..." FORCE)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Declare the executable target built from your sources
add_executable(${PROJECT_NAME} main.cxx mathOp.cxx mathOp.hxx)


# Link your application with OpenCV libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})


install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION )#${PROJECT_NAME})

IF (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
ENDIF()
