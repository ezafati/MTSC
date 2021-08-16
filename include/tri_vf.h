#ifndef TRIVF_HPP
#define TRIVF_HPP

#include "mstC_types.h"
#include <string>
#include <iostream>
#include <utility>
#include "mesh.h"
#include <omp.h>
#include <memory>
#ifdef USE_MPI
#include "mpi.h"
#endif


class TriVF{
public:
  
    TriVF(const MeshGlobal& gmesh):_mesh(gmesh){

        /* get entities elements */
        const auto& EntitiesElem = _mesh.GetEntitiesElements();

        //std::vector<MTSC_INT>& tmpvct = EntitiesElem
        for(int i=3;i>0;i--){
            int count = EntitiesElem.count(i);
            if(count){
                /* we only allow count == 1: only one entity is allowed for the highest dimension*/
                //if(count > 1) throw std::string("Error: multiple entities have the highest dimension of the problem");
                auto ret = EntitiesElem.equal_range(i);
                for(auto iter = ret.first; iter!=ret.second; iter++){
                    const std::vector<MTSC_INT>& tmpvct = (iter->second).second;
                    nbCells += (tmpvct.size()-1)/(tmpvct[0]+1); // +1 because the rank  of the cell is in tmpvct 
                    //std::cout << "key   " << (iter->second).first << std::endl;
                    VectorOfCells.insert(std::make_pair((iter->second).first,const_cast<std::vector<MTSC_INT>*>(&tmpvct)));
                }         
                break;
            }
        }
        MapFacesToTriangle.resize(nbCells);
        for(auto iter=MapFacesToTriangle.begin(); iter!=MapFacesToTriangle.end(); iter++){
            *iter = new MTSC_INT[3];
        }
    }
    ~TriVF(){
        for(auto iter=MapFacesToTriangle.begin(); iter!=MapFacesToTriangle.end(); iter++){
            delete[] *iter;
            *iter = nullptr;
        }
    };

    void AddCenterOfCells();
    const MeshGlobal& GetMesh() const{
        //if(_mesh.isEmpty()) throw std::string("Fatal error: mesh not found");
        return _mesh;
    }
    

    
private:
    const MeshGlobal& _mesh;
    MTSC_INT nbCells = 0;
    std::map<int,std::vector<MTSC_INT>*> VectorOfCells;
    std::vector<MTSC_FLOAT> CellCenterCoords;
    std::vector<MTSC_INT*> MapFacesToTriangle; 

};

typedef std::shared_ptr<TriVF> TriVFPtr;
#endif
