# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(Cerenkov)

# for MPI <
#find_package(MPI REQUIRED)

# modify these variables if needed
#set(CMAKE_C_COMPILER mpicxx)
#set(CMAKE_CXX_COMPILER mpiicpc)

#set(CMAKE_CXX_INCLUDE_PATH )

# for MPI >

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# for MPI <
#find_package(G4mpi REQUIRED)
# for MPI >

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#for MPI <
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
#			      ${Geant4_INCLUDE_DIR}
#			      ${G4mpi_INCLUDE_DIR})
#for MPI >

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
			      ${Geant4_INCLUDE_DIR})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(Cerenkov Cerenkov.cc ${sources} ${headers})
#target_link_libraries(Cerenkov ${G4mpi_LIBRARIES} ${Geant4_LIBRARIES})
target_link_libraries(Cerenkov ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build Cerenkov. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(CERENKOV_SCRIPTS
  Cerenkov.out
  Cerenkov.in
  icons.mac
  gui.mac
  run.png
  run1.mac
  init_vis.mac
  vis.mac
  muangle.mac
  muenergy.mac
  muangle_collect.mac
  muangle10.mac
  muenergy10.mac
  muangle_collect10.mac
  zenith.mac
  ana.cc
  ana.hh
  inform.sh
  delta.cc
  Makedelta
  )

foreach(_script ${CERENKOV_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS Cerenkov DESTINATION bin)

