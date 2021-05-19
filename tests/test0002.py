import  mpi4py.MPI 
from mtsc import MeshGlobal, MeshLocal
p = MeshLocal()
p.ReadGmshInfos("/home/ezafati/mesh")
