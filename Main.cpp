
#include <stdlib.h>
#include <iostream>

#include <string>

#include <dlfcn.h>

#include "LibClang.h"

struct Error
{
   Error() {}
   Error(const std::string& message) : message(message) {}

   std::string message;

   bool isError() const { return !message.empty(); }

   typedef void (*unspecified_bool_type)();
   static void unspecified_bool_true() {}
   operator unspecified_bool_type() const
   {
      return !isError() ? 0 : unspecified_bool_true;
   }
   bool operator!() const
   {
      return !isError();
   }
};

struct Success : public Error
{
   Success() : Error() {}
};


Error loadLibrary(const std::string& libPath, int options, void** ppLib)
{
   *ppLib = NULL;
   *ppLib = ::dlopen(libPath.c_str(), options);
   if (*ppLib == NULL)
      return Error(::dlerror());
   else
      return Success();
}

Error loadSymbol(void* pLib, const std::string& name, void** ppSymbol)
{
   *ppSymbol = NULL;
   *ppSymbol = ::dlsym(pLib, name.c_str());
   if (*ppSymbol == NULL)
      return Error(::dlerror());
   else
      return Success();
}

Error closeLibrary(void* pLib)
{
   if (::dlclose(pLib) != 0)
      return Error(::dlerror());
   else
      return Success();
}

extern "C" {

typedef CXIndex (*ptr_clang_createIndex)(int excludeDeclarationsFromPCH,
                                         int displayDiagnostics);

typedef void (*ptr_clang_disposeIndex)(CXIndex index);

}




int main(int argc, char * const argv[]) 
{

   void* pLib;
   Error error = loadLibrary("libclang.so", RTLD_NOW, &pLib);
   if (error)
   {
      std::cerr << error.message << std::endl;
      return EXIT_FAILURE;
   }

   void* pSymbol;
   error = loadSymbol(pLib, "clang_createIndex", &pSymbol);
   if (error)
   {
      std::cerr << error.message << std::endl;
      return EXIT_FAILURE;
   }
   ptr_clang_createIndex pClang_CreateIndex = (ptr_clang_createIndex)pSymbol;

   error = loadSymbol(pLib, "clang_disposeIndex", &pSymbol);
   if (error)
   {
      std::cerr << error.message << std::endl;
      return EXIT_FAILURE;
   }
   ptr_clang_disposeIndex pClang_DisposeIndex = (ptr_clang_disposeIndex)pSymbol;


   CXIndex Index = pClang_CreateIndex(0,0);


   pClang_DisposeIndex(Index);

	return EXIT_SUCCESS;
}
