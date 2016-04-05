#ifndef STUDENT_CODE_H
#define STUDENT_CODE_H

#include <exception>

#include "halfEdgeMesh.h"

using namespace std;

namespace CMU462 {
  class EdgeEditException: public exception {
  public:
    EdgeEditException(int exptno){
      this->exptno = exptno;
    }

    void exptInfo(){
      switch (this->exptno){
        case 10:{
          std::cerr << "Cannot flip this edge." << std::endl;
          break;
        }
        case 11:{
          std::cerr << "Cannot flip a boundary." << std::endl;
          break;
        }
        case 20:{
          std::cerr << "Cannot split this edge." << std::endl;
          break;
        }
        case 21:{
          std::cerr << "Cannot split a boundary." << std::endl;
          break;
        }
        case 30:{
          std::cerr << "Cannot collapse this edge." << std::endl;
          break;
        }
        case 31:{
          std::cerr << "Cannot collapse a boundary." << std::endl;
          break;
        }
        default:{
          std::cerr << "Cannot edit this edge." << std::endl;
        }
      }
    }


  private:
    int exptno;

  };

  class MeshResampler{

      public:

         MeshResampler(){};
         ~MeshResampler(){}

         void upsample  ( HalfedgeMesh& mesh );
         void downsample( HalfedgeMesh& mesh );
         void resample  ( HalfedgeMesh& mesh );
   };
}

#endif // STUDENT_CODE_H
