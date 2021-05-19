set(util_source_files mesh.cpp)
add_library(mesh ${util_source_files})


target_include_directories(mesh
     PRIVATE  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_PATH})

target_link_libraries(mesh ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MPI_CXX_LIBRARIES})

