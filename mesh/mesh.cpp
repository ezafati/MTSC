#include "../include/mesh.h"
#include <fstream>
#include <algorithm>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <sstream>
#include <iterator>
#include <limits>
#include "Python.h"
#ifdef USE_MPI
#include "mpi.h"
#endif



void MeshGlobal::ReadGmshInfos(std::string filepath){
    
    bool res;
    /* Get full file path (may be different if USE_MPI) */
    auto filename  = GetFullFilePath(std::move(filepath));
    const boost::interprocess::mode_t mode = boost::interprocess::read_only; 
    boost::interprocess::file_mapping file(filename, mode);
    boost::interprocess::mapped_region region(file, mode, 0, 0);
    char* begin = static_cast<char*>(region.get_address());
    
    //Read nodes
    //get physical names
    
    res = GetPhysicalNames(begin, region.get_size(), std::string(filename));
    assert(res);
    
    //get Entities
    res = GetEntities(begin, region.get_size(), std::string(filename));
    assert(res);
    
    res = GetNodes(begin, region.get_size(), std::string(filename));
    assert(res);
    
    res = GetElements(begin, region.get_size(), std::string(filename));
    assert(res);
    
    delete[] filename;

    /* READ Nodes  total number : more efficient ?*/
    /*std::string  line;
    while (file.good()){
        std::getline(file, line);
        if (line.substr(0,6) == "$Nodes"){
            std::cout << "found" << std::endl;
        }
    }
    file.close();*/
    //return 0;
}

const char* MeshGlobal::GetFullFilePath(std::string&& filepath){
    
    #ifdef USE_MPI
        int p;
        MPI_Comm_rank(MPI_COMM_WORLD, &p);
        p++;
        filepath +=  "_" + std::to_string(p) + ".msh";
    #else
        filepath +=  "_" +  std::string(".msh");
    #endif
    char* fileNtoChar = new char[filepath.size()+1];
    strcpy(fileNtoChar, filepath.c_str());
    return fileNtoChar;
}

bool MeshGlobal::GetNodes(char* beg, int sizef, std::string&& filename){
    
    std::string line;
    std::pair<int, std::vector<MTSC_INT>> tmpp;
    std::istringstream sstre;
    MTSC_INT numno, val, minnu, maxnu;
    MTSC_FLOAT valf;
    int nument;
    std::vector<MTSC_INT> vct;
    std::vector<MTSC_FLOAT> coor;
     
    char* pos = std::find(beg, beg + sizef, '$');
    std::string key(pos, 6);
    while(key.compare("$Nodes") != 0){
        pos = std::find(pos + 1, beg + sizef, '$');
        key = std::string(pos,6);
    }
    
    //open the file again and move to the position
    std::ifstream file(filename);
    if(!file) return false;
    
    int posn = pos - beg + 7;
    file.seekg(posn, std::ios_base::beg);
    
    std::getline(file, line);
    
    sstre.str(line);
    
    sstre >> nument;
    sstre >> numno;
    sstre >> minnu;
    sstre >> maxnu;
    
    nunodes = numno;
    
    // resize
    NuNoGlobToLocal.resize(maxnu);
    // reserve the node vector
    CoorNodes.reserve(3*numno);
    auto itern = std::begin(CoorNodes);
    
    //start 
    int j = 0;
    MTSC_INT count = 0;
    MTSC_INT countno = 0;
    
    do{
        sstre.clear();
        tmpp.second.clear();
        vct.clear();
        std::getline(file, line);
        sstre.str(line);
        while(sstre >> val) vct.push_back(val);
        tmpp.second.reserve(vct[3]);
        tmpp.first = vct[1];
        for(int i=0; i<vct[3]; i++){
            std::getline(file, line);
            sstre.clear();
            sstre.str(line);
            sstre >> val;
            tmpp.second.insert(tmpp.second.begin()+i, val);
            NuNoGlobToLocal.at(val-1) = countno+1;
            countno++;
            //if(DEBUG) for(auto test: tmpp.second) std::cout << test << std::endl;
        }
        
        for(auto iter=std::cbegin(tmpp.second); iter!=std::cend(tmpp.second); iter++){
            val = *(iter);
            std::getline(file, line);
            sstre.clear();
            sstre.str(line);
            while(sstre >> valf) coor.push_back(valf);
            CoorNodes.insert(itern + count, std::cbegin(coor), std::cend(coor));
            coor.clear();
            //if(DEBUG) std::cout << CoorNodes[count] << "   " << CoorNodes[count+1] << std::endl;
            count += 3;
        }
        EntitiesNodes.insert(MapGrouptoNodesObj(vct[0], std::move(tmpp)));
        j++; 
    }while(j<nument);
    
    //if(DEBUG) for(auto test: NuNoGlobToLocal) std::cout << test << std::endl;
    return true;
    
}


bool MeshGlobal::GetPhysicalNames(char* beg, int sizef, std::string&& filename){
    
    int num, dim, tag;
    std::string name, line;
    std::pair<int, std::string> tmpp;
    std::istringstream sstre;
    
    char* pos = std::find(beg, beg + sizef, '$');
    std::string key(pos, 14);
    while(key.compare("$PhysicalNames") != 0){
        pos = std::find(pos + 1, beg + sizef, '$');
        key = std::string(pos,14);
    }
    
    //open the file again and move to the position
    std::ifstream file(filename);
    if(!file) return false;
    
    int posn = pos - beg + 15;
    file.seekg(posn, std::ios_base::beg);
    
    std::getline(file, line);
    sstre.str(line);
    
    // Get total number of named groups    
    sstre >> num;
    
    // retrive groups infos
    for(int i=0; i < num; i++){
        std::getline(file, line);
        sstre.clear();
        sstre.str(line);
        sstre >> dim;
        sstre >> tag;
        try{
            sstre >> name;
        }catch(...){};

        tmpp = std::make_pair(tag, name);
        PhysicalNames.insert(MapGroupNamesObj(dim, tmpp));
    }
    return true;
}

bool MeshGlobal::GetEntities(char* beg, int sizef, std::string&& filename){
    
    std::vector<int> vct, vcttmp;
    std::string line;
    std::istringstream sstre;
    std::pair<int, std::vector<int>> tmpp;
    MTSC_FLOAT val;
    
    char* pos = std::find(beg, beg + sizef, '$');
    std::string key(pos, 9);
    while(key.compare("$Entities") != 0){
        pos = std::find(pos + 1, beg + sizef, '$');
        key = std::string(pos,9);
    }
    
    std::ifstream file(filename);
    if(!file) return false;
    
    int posn = pos - beg + 10;
    file.seekg(posn, std::ios_base::beg);
    
    std::getline(file, line);
    sstre.str(line);
    
    while(sstre >> val) vct.push_back(int(val));
    assert(vct.size() == 4);
    
    //add points
    for(int i=0; i<vct[0]; i++){
        std::getline(file, line);
        sstre.clear();
        sstre.str(line);
        sstre >> val;
        tmpp.first = int(val);
        Entities.insert(MapGroupObj(0, tmpp));
    }
    
    // add  curves
    for(int i=0; i<vct[1]; i++){
        
        //make some clear before
        sstre.clear();
        vcttmp.clear();
        tmpp.second.clear();
        
        // get the next entity 
        std::getline(file, line);
        sstre.str(line);
        while(sstre >> val) vcttmp.push_back(abs(int(val)));
        tmpp.first =vcttmp[0];
  
        if(vcttmp[7]){
            auto [begin, end] = PhysicalNames.equal_range(1);
            for(int j=0; j < vcttmp[7]; j++){
                auto iter = std::find_if(begin, end, [&vcttmp,j](MapGroupNamesObj m){return (m.second).first==vcttmp[7+j+1];});
                if(iter!=end){
                    (iter->second).first = vcttmp[0];
                }else{
                    throw std::string("Fatal error: Physical entity not found");
                }
            }
        }
        int decal = 7 + vcttmp[7] + 2;
        tmpp.second.insert(std::cbegin(tmpp.second), std::cbegin(vcttmp) + decal, std::cend(vcttmp));
        Entities.insert(MapGroupObj(1, tmpp));
    }
    
    // add surfaces
    for(int i=0; i<vct[2]; i++){
        
        //make some clear before
        sstre.clear();
        vcttmp.clear();
        tmpp.second.clear();
        
        // get the next entity 
        std::getline(file, line);
        sstre.str(line);
        while(sstre >> val) vcttmp.push_back(abs(int(val)));
        tmpp.first =vcttmp[0];
        if(vcttmp[7]){
            auto [begin, end] = PhysicalNames.equal_range(2);
            for(int j=0; j < vcttmp[7]; j++){
                auto iter = std::find_if(begin, end, [&vcttmp,j](MapGroupNamesObj m){return (m.second).first==vcttmp[7+j+1];});
                if(iter!=end){
                    (iter->second).first = vcttmp[0];
                }else{
                    throw std::string("Fatal error: Physical entity not found");
                }
            }
        }
        int decal = 7 + vcttmp[7] + 2;
        tmpp.second.insert(std::cbegin(tmpp.second), std::cbegin(vcttmp) + decal, std::cend(vcttmp));
        Entities.insert(MapGroupObj(2, tmpp));
    }
    return true;
}


bool MeshGlobal::GetElements(char* beg, int sizef, std::string&& filename){
    
    std::vector<MTSC_INT> vct;
    std::string line;
    std::istringstream sstre;
    std::pair<int, std::vector<MTSC_INT>> tmpp;
    MTSC_INT val;
    int nument;
    
    char* pos = std::find(beg, beg + sizef, '$');
    std::string key(pos, 9);
    while(key.compare("$Elements") != 0){
        pos = std::find(pos + 1, beg + sizef, '$');
        key = std::string(pos,9);
    }
    
    std::ifstream file(filename);
    if(!file) return false;
    
    int posn = pos - beg + 10;
    file.seekg(posn, std::ios_base::beg);
    
    std::getline(file, line);
    sstre.str(line);
    
    sstre >> nument;
    
    int j=0;
    do{
        sstre.clear();
        tmpp.second.clear();
        vct.clear();
        std::getline(file, line);
        sstre.str(line);
        while(sstre >> val) vct.push_back(val);
        /* Get number of nodes */
        int nuno = GetNuno((int)vct[2]);
        /* Get element type related to the entity */
        std::string typel = GetTypeElem((int)vct[2]);
        MapGroupToElemType.insert(MapGroupElemType(vct[0], std::pair<int,std::string>(nuno, typel)));
        //std::cout << "type element " << vct[2] << "  " << typel << std::endl;
        /* allocate */
        tmpp.second.reserve(1+(nuno+1)*vct[3]);
        tmpp.first = vct[1];
        //std::cout << nuno << "   " << vct[1] << "   " << vct[3] << std::endl;
        auto it = tmpp.second.begin();
        tmpp.second.insert(it, (MTSC_INT)nuno);
        for(MTSC_INT i=0;i<vct[3];i++){
            std::getline(file, line);
            sstre.clear();
            sstre.str(line);
            while(sstre >> val){
                it++;
                tmpp.second.insert(it, val);
            }
        }
    EntitiesElements.insert(MapGrouptoElemsObj(vct[0], std::move(tmpp)));
    j++;
    }while(j<nument);
    
 
    return true;
    
}


int MeshGlobal::GetNuno(int x){
    
    Py_Initialize();
    
    PyObject* mstc_pack = PyImport_ImportModule((const char*)"mtsc_python");
    PyObject* func = PyObject_GetAttrString(mstc_pack,(const char*)"readeleminfos");

    
    
     /* Result is expected to be a dictionary*/
    PyObject* res = PyObject_CallObject(func, Py_BuildValue("(i)", x));
    size_t nuno = PyLong_AsSize_t(PyDict_GetItemString(res, "NuNodes"));

    /*make some clear */
    Py_DECREF(mstc_pack);
    Py_DECREF(func);
    Py_DECREF(res);
    /*END*/
    //Py_Finalize();
    
    return nuno;
}

std::string MeshGlobal::GetTypeElem(int x){
    
    Py_Initialize();
    
    PyObject* mstc_pack = PyImport_ImportModule((const char*)"mtsc_python");
    PyObject* func = PyObject_GetAttrString(mstc_pack,(const char*)"readeleminfos");

    
    
     /* Result is expected to be a dictionary*/
    PyObject* res = PyObject_CallObject(func, Py_BuildValue("(i)", x));
    
    const char* typel = PyUnicode_AsUTF8(PyDict_GetItemString(res, "ElemType"));
    /*make some clear */
    Py_DECREF(mstc_pack);
    Py_DECREF(func);
    Py_DECREF(res);
    /*END*/
    //Py_Finalize();
    
    return std::string(typel);
}