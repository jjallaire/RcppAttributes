
#include <stdlib.h>
#include <iostream>


#include "Clang.hpp"

int main(int argc, char * const argv[]) 
{
   LibClang libClang;
   std::string initError;
   if (!libClang.initialize(&initError))
   {
      std::cerr << initError << std::endl;
      return EXIT_FAILURE;
   }

   CXIndex index = libClang.createIndex(0,0);

   CXTranslationUnit TU = libClang.parseTranslationUnit(index,
                                                     0,
                                                     argv,
                                                     argc,
                                                     0,
                                                     0,
                                                     CXTranslationUnit_None);

   for (unsigned I = 0, N = libClang.getNumDiagnostics(TU); I != N; ++I) {
       CXDiagnostic Diag = libClang.getDiagnostic(TU, I);
       CXString String = libClang.formatDiagnostic(Diag,
                               libClang.defaultDiagnosticDisplayOptions());
       fprintf(stderr, "%s\n", libClang.getCString(String));
       libClang.disposeString(String);
     }

   libClang.disposeTranslationUnit(TU);
   libClang.disposeIndex(index);

	return EXIT_SUCCESS;
}
