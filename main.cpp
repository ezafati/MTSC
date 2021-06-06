#include <iostream>
#include <omp.h>
#include <mpi.h>
#include "mesh.h"
#include "tri_vf.h"
#include <memory>
#include <vector>




int main(int argc, char **argv)
{


#ifdef USE_MPI
    MPI_Init(&argc, &argv);
    //test(); 
    MeshLocal meshtest;
    std::string filet = "/home/ezafati/mesh";
    meshtest.ReadGmshInfos(filet);
    TriVF meshVF(meshtest);
    meshVF.AddCenterOfCells();
    //std::cout << &meshtest << "    " << &meshVF.GetMesh() << std::endl;
    MPI_Finalize();
#else 
    std::cout << "not defined " << std::endl;
#endif

	return 0;
}
