cmake_minimum_required(VERSION 3.10)
project(MTSC)

# Find Boost
find_package(Boost COMPONENTS python3)

# Find Python
find_package(PythonInterp 3 REQUIRED)
find_package(PythonLibs 3 REQUIRED)

#FIND mpi
find_package(MPI REQUIRED)

#c++ flags
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -O3 -fPIC" )

#definition 
add_definitions(-DUSE_MPI)

add_subdirectory(mesh)
add_subdirectory(matelem)


#libraries list 
set (LIB1_SOURCES main.cpp  mtsc_py.cpp)


# Create library
add_library(mtsc SHARED ${LIB1_SOURCES})

target_include_directories(mtsc
    PRIVATE include  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_PATH})

target_link_libraries(mtsc mesh matelem  ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MPI_CXX_LIBRARIES})
SET_TARGET_PROPERTIES(mtsc PROPERTIES PREFIX "")


# add exec file 
add_executable(mtsc_exec ${LIB1_SOURCES}) 

target_include_directories(mtsc_exec
     PRIVATE include ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_PATH})

target_link_libraries(mtsc_exec mesh matelem ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MPI_CXX_LIBRARIES})
