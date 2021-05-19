import  mpi4py.MPI 
from mtsc import MeshGlobal, MeshLocal
p = MeshGlobal()
p.ReadGmshInfos("/home/ezafati/mesh")
print(p.GetNumberOfNodes())
print(len(p.GetNodesCoord()))
