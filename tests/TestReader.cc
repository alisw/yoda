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

  #ifdef WITH_ZLIB
  vector<AnalysisObject*> aos2 = YODA::read("testwriter2.yoda.gz");
  #endif

  return EXIT_SUCCESS;
}
