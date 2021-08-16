#include <boost/python.hpp>
#include <iostream>
#include "mesh.h"
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include "convertpycpp.h"


/*void sayHello(std::map<std::string, MTSC_FLOAT> xmap) {
       std::cout <<  xmap.size() << std::endl;
       for(auto& x: xmap) std::cout <<"    " << x.first << "   " << x.second << std::endl;
}*/

BOOST_PYTHON_MODULE(mtsc)
{
    using namespace boost::python;

    
    boost::python::to_python_converter<std::vector<MTSC_FLOAT>, vector_to_list<MTSC_FLOAT> >();
    boost::python::to_python_converter<std::vector<std::string>, vector_to_list<std::string> >();
    boost::python::to_python_converter<std::vector<MTSC_INT>, vector_to_list<MTSC_INT> >();

    /* Convert python dictionary to map container c++*/
    class_<std::map<std::string, MTSC_FLOAT> >("StrToFloatMTSC")
        .def(map_indexing_suite<std::map<std::string, MTSC_FLOAT> >() );
    class_<std::map<std::string, MTSC_INT> >("StrToIntMTSC")
        .def(map_indexing_suite<std::map<std::string, MTSC_INT> >() );
    
    //def("say_hello", &sayHello); // Python name != C++ name
    /*class_<std::vector<MTSC_FLOAT> >("MTSCFVec")
        .def(vector_indexing_suite<std::vector<MTSC_FLOAT>>());*/

    
    class_<MeshGlobal>("MeshGlobal", init<>())
        .def("GetNumberOfNodes", &MeshGlobal::GetNumberOfNodes)
        .def("ReadGmshInfos", &MeshGlobal::ReadGmshInfos)
        .def("GetNodesCoord", &MeshGlobal::GetNodesCoord);
    
    class_<MeshLocal>("MeshLocal", init<>())
        .def("ReadGmshInfos", &MeshGlobal::ReadGmshInfos);



}