#include "../include/tri_vf.h"
#include  <algorithm>


void TriVF::AddCenterOfCells(){
    
   CellCenterCoords.reserve(3*nbCells);
   auto iterbc = CellCenterCoords.begin();
   /* get nodes coordinates */
   const std::vector<MTSC_FLOAT>& tmpvct = _mesh.GetNodesCoord();
   auto iterb = (*VectorOfCells).begin();
   auto itere = (*VectorOfCells).end();
   MTSC_INT numno = *iterb;

   /* Define a lambda function to cumpute the barycenter coordinates of a cell
     axe define the x, y or z abscisse*/
   int axe = 0;
   MTSC_FLOAT val = 0;
   auto ComputeSumCoordxyz = [&tmpvct,&numno,&val,&axe](const MTSC_INT& a){
      val += tmpvct[3*(a-1)+axe]/numno;
   };
   
   int count =0;
   for(auto iter=iterb+1; iter!=itere; iter+=numno){
      iter++;
      for(int i=0; i<3; i++){
         //std::cout << "iter  " <<  *iter <<  "    " << *(iter+1) << "   " << *(iter+2) << std::endl;
         axe = i;
         std::for_each(iter, iter+3, ComputeSumCoordxyz);
         CellCenterCoords.insert(iterbc + 3*count +axe, val);
         val=0;
      }
      count+=1;
      //break;
   }
   
   /*for(auto test: CellCenterCoords) std::cout << test << std::endl;
   std::cout << "size  " << CellCenterCoords.size() << std::endl;
   std::cout << "count " << count << std::endl;*/
}
