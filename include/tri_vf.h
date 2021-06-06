#ifndef TRIVF_HPP
#define TRIVF_HPP

#include "mstC_types.h"
#include <string>
#include <iostream>
#include <utility>
#include "mesh.h"
#include <omp.h>
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
                if(count > 1) throw std::string("Error: multiple entities have the highest dimension of the problem");
                auto iter = EntitiesElem.lower_bound(i);
                const std::vector<MTSC_INT>& tmpvct = (iter->second).second;
                nbCells = (tmpvct.size()-1)/(tmpvct[0]+1);
                VectorOfCells = const_cast<std::vector<MTSC_INT>*>(&tmpvct);
                break;
            }
        }
    }
    ~TriVF() = default;

    void AddCenterOfCells();
    const MeshGlobal& GetMesh() const{
        //if(_mesh.isEmpty()) throw std::string("Fatal error: mesh not found");
        return _mesh;
    }
    

    
private:
    const MeshGlobal& _mesh;
    MTSC_INT nbCells;
    std::vector<MTSC_INT>* VectorOfCells = nullptr;
    std::vector<MTSC_FLOAT> CellCenterCoords;

};
#endif
