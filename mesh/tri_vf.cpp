#include "../include/tri_vf.h"
#include  <algorithm>


void TriVF::AddCenterOfCells(){
   
   CellCenterCoords.reserve(3*nbCells);
   auto iterbc = CellCenterCoords.begin();
   // get nodes coordinates 
   const std::vector<MTSC_FLOAT>& tmpvct = _mesh.GetNodesCoord();
   int axe = 0;
   MTSC_FLOAT val = 0;
   MTSC_INT numno = 0;
   MTSC_INT count =0;
   auto ComputeSumCoordxyz = [&tmpvct,&numno,&val,&axe](const MTSC_INT& a){
      val += tmpvct[3*(a-1)+axe]/numno;
   };

   for(auto ite=VectorOfCells.cbegin(); ite!=VectorOfCells.cend(); ite++){
      auto iterb = (ite->second)->begin();
      auto itere = (ite->second)->end();
      numno = *iterb;
        
      //std::cout << numno << std::endl;
      // Define a lambda function to cumpute the barycenter coordinates of a cell
      //  axe define the x, y or z abscisse
      

      for(auto iter=iterb+1; iter!=itere; iter+=numno){
         // for effciency : renumbering the vector of cells related to each entity or group of elements
         *iter = count + 1;
         // CellCenterCoords.insert(iterbc + 3*ord, ord);
         iter++;
         for(int i=0; i<3; i++){
            //std::cout << "iter  " <<  *iter <<  "    " << *(iter+1) << "   " << *(iter+2) << std::endl;
            axe = i ;
            std::for_each(iter, iter+numno, ComputeSumCoordxyz);
            CellCenterCoords.insert(iterbc + 3*count +  axe, val);
            val=0;
         }
         count+=1;
         //break;
      }
   
   }

  /*for(auto test: CellCenterCoords) std::cout << test << std::endl;
   std::cout << "size  " << CellCenterCoords.size() << std::endl;
   std::cout << "count " << count << std::endl;*/
}
