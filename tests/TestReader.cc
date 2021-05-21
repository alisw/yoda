#include "YODA/Histo1D.h"
#include "YODA/ReaderYODA.h"
#include "YODA/IO.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <memory>
#include <type_traits>

using namespace std;
using namespace YODA;

int main() {

  vector<AnalysisObject*> aos1 = YODA::read("testwriter2.yoda");

  Index idx = YODA::ReaderYODA::create().mkIndex(std::string("testwriter2.yoda"));
  std::cout << "idx 1:" << idx.toString() << std::endl;
  Index idx2= YODA::mkIndex(std::string("testwriter2.yoda"));
  std::cout << "idx 2:" << idx2.toString() << std::endl;
  
  #ifdef WITH_ZLIB
  vector<AnalysisObject*> aos2 = YODA::read("testwriter2.yoda.gz");
  #endif

  return EXIT_SUCCESS;
}
