#include <boost/python.hpp>
#include <iostream>
#include "mesh.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

/*static void sayHello() {
    std::cout << "Hello\n";
}*/

BOOST_PYTHON_MODULE(mtsc)
{
    using namespace boost::python;

    //def("say_hello", sayHello); // Python name != C++ name
    
     class_<std::vector<MTSC_FLOAT> >("MTSCFVec")
        .def(vector_indexing_suite<std::vector<MTSC_FLOAT>>());

    
    class_<MeshGlobal>("MeshGlobal", init<>())
        .def("GetNumberOfNodes", &MeshGlobal::GetNumberOfNodes)
        .def("ReadGmshInfos", &MeshGlobal::ReadGmshInfos)
        .def("GetNodesCoord", &MeshGlobal::GetNodesCoord);
    
    class_<MeshLocal>("MeshLocal", init<>())
        .def("ReadGmshInfos", &MeshGlobal::ReadGmshInfos);



}