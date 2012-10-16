
#include <stdlib.h>
#include <iostream>


#include "Clang.hpp"


int main(int argc, char * const argv[]) 
{
   Clang clang;
   std::string initError;
   if (!clang.initialize(&initError))
   {
      std::cerr << initError << std::endl;
      return EXIT_FAILURE;
   }

   CXIndex index = clang.createIndex(0,0);


   clang.disposeIndex(index);

	return EXIT_SUCCESS;
}
