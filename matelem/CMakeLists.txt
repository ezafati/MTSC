set(util_source_files matelem.cpp)
add_library(matelem ${util_source_files})


target_include_directories(matelem
     PRIVATE  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} ${MPI_CXX_INCLUDE_PATH})

target_link_libraries(matelem ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${MPI_CXX_LIBRARIES})