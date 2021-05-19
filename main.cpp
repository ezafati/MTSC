#include <iostream>
#include <mpi.h>
#include "mesh.h"
#include <memory>
#include <vector>



int main(int argc, char **argv)
{


#ifdef USE_MPI
    MPI_Init(&argc, &argv);
    //test();
    MeshLocal meshtest;
    std::string filet = "/home/ezafati/t21";
    meshtest.ReadGmshInfos(filet);
    meshtest.UpdateToLocalConn();
    MPI_Finalize();
#else 
    std::cout << "not defined " << std::endl;
#endif

	return 0;
}
