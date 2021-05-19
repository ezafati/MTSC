#ifndef MESH_HPP
#define MESH_HPP

#include "mstC_types.h"
#include <string>
#include <utility>
#include  <map>
#include <vector>
#include <iostream>






class MeshGlobal
{
    friend class MeshLocal;
public:
    MeshGlobal(){}
    MeshGlobal(std::string& filep):FileAbsPath(filep){}
    ~MeshGlobal() =default;
    
    /* Some types definitiosn */
    typedef std::pair<int, std::pair<int, std::string>> MapGroupNamesObj;
    typedef std::pair<int, std::pair<int, std::vector<int>>>  MapGroupObj;
    typedef std::pair<int, std::pair<int, std::vector<MTSC_INT>>>  MapGrouptoNodesObj;
    typedef std::pair<int, std::pair<int, std::vector<MTSC_INT>>>  MapGrouptoElemsObj;
    

    
    
    const char* GetFullFilePath(std::string&& filepath);
    void ReadGmshInfos(std::string filepath);
    
    bool GetPhysicalNames(char* pos, int sizef, std::string&& filepath);
    bool GetEntities(char* pos, int sizef, std::string&& filepath);
    bool GetNodes(char* pos, int sizef, std::string&& filepath);
    bool GetElements(char* pos, int sizef, std::string&& filepath);
    
    /* Get the number of the node in the element giving
     * the gmsh number  (infos stored in base.json file)*/
    int  GetNuno(int gmshnu);
    
    /* Get total number of nodes*/
    MTSC_INT GetNumberOfNodes() const{
        if(!nunodes && FileAbsPath.empty()) {
        try{
            throw std::string("Fatal error: First read mesh file");
        }catch(...){}
        }
        return nunodes;
    }
    
   std::vector<MTSC_FLOAT> GetNodesCoord() const{
        return CoorNodes;
    }

private:
    
    std::string FileAbsPath; // mesh file path
    MTSC_INT nunodes; // total number of nodes

    /*Container to store:
     * key [int]: element dimension
       value [pair<int, std::string>]: (Entity_tag, tag_name) */
    std::multimap<int, std::pair<int, std::string>> PhysicalNames;
    
    /*Container to store:
     * key [int]: element dimension
       value [pair<int, std::vector<int>]: (Entity_tag, vector of
              elements bounding Entity_tag(see below)) 
        * std::vector<int>  =  empty vector if key = 0
        * std::vector<int>  =  vector of points  if key = 1
        * std::vector<int>  =  vector of curves if key = 2
        * std::vector<int>  =  vector of surfaces if key = 3*/
    std::multimap<int, std::pair<int, std::vector<int>>> Entities;
    
    std::multimap<int, std::pair<int, std::vector<MTSC_INT>>> EntitiesNodes;
    
    /*Container to store:
     * key [int]: element dimension
       value [pair<int, std::vector<int>]: (Entity_tag, vector V contains connectivities
                    for each entity (see details below)) 
        * V[0] = n  : number of elements for each element in Entity with Entity_tag 
        * V[k], .., V[k+n-1]: GMSH Nodes Tags for the element k*/
    std::multimap<int, std::pair<int, std::vector<MTSC_INT>>> EntitiesElements;
    
    /* Vector  contains all nodes coordinates*/
    std::vector<MTSC_FLOAT> CoorNodes;
    
    /* Vector to map global node number to local node number
     * Exemple: 
     * gn:  GMSH Node Tag
     * NuNoGlobToLocal[gn] Local node tag in the cuurent process*/
    std::vector<MTSC_INT> NuNoGlobToLocal;
    
        
};

class MeshLocal : public MeshGlobal
{
public:
    MeshLocal(){};
    /**/
    void GlobalToLocalConn(){
        if(NuNoGlobToLocal.empty()) throw std::string("Fatal Error: NuNoGlobToLocal vector is empty");
        
    };
    
    /* Used to update the global numbering to local numbering for connectivity vectors*/
    void UpdateToLocalConn(){
        for(auto i: {0,1,2,3}){
            auto retour = EntitiesElements.equal_range(i);
            if(retour.first != retour.second){
                for(auto iter=retour.first;iter!=retour.second;iter++){
                    std::vector<MTSC_INT>& tmpvct = (iter->second).second;
                    int nuno = tmpvct[0];
                    //auto iterc = tmpvct.begin() + 1;
                    MTSC_INT tmpvctsize = tmpvct.size();
                    MTSC_INT count = 1;
                    while(true){
                        for(int k=0;k<nuno;k++) tmpvct.at(count+k+1)=NuNoGlobToLocal.at(tmpvct.at(count+k+1)-1);
                        count += 1 + nuno;
                        if(count==tmpvctsize) break;
                    }
                }
            }
        }
    };
private:
    std::multimap<int, std::pair<int, std::vector<MTSC_INT>>> LocalConn;
};
#endif
