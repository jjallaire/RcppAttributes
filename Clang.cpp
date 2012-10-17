#include "Clang.hpp"

#include <iostream>

#ifdef _WIN32

#else
#include <dlfcn.h>
#endif

namespace {


#ifdef _WIN32


#else

bool loadLibrary(const std::string& libPath,
                 void** ppLib,
                 std::string* pError)
{
   *ppLib = NULL;
   *ppLib = ::dlopen(libPath.c_str(), RTLD_NOW);
   if (*ppLib == NULL)
   {
      *pError = std::string(::dlerror());
      return false;
   }
   else
    {
      return true;
   }
}

bool loadSymbol(void* pLib,
                const std::string& name,
                void** ppSymbol,
                std::string* pError)
{
   *ppSymbol = NULL;
   *ppSymbol = ::dlsym(pLib, name.c_str());
   if (*ppSymbol == NULL)
   {
      *pError = std::string(::dlerror());
      return false;
   }
   else
   {
      return true;
   }
}

bool closeLibrary(void* pLib, std::string* pError)
{
   if (::dlclose(pLib) != 0)
   {
      *pError = std::string(::dlerror());
      return false;
   }
   else
   {
      return true;
   }
}

#endif


} // anonymous namespace

#define LOAD_CLANG_SYMBOL(name) \
   if (!loadSymbol(pLib_, "clang_ ## name", (void**)&name, &initError_)) \
       return;


LibClang::LibClang(const std::string& libraryPath)
   : pLib_(NULL)
{
   if (!loadLibrary(libraryPath, (void**)&pLib_, &initError_))
       return;

   if (!initSymbol("clang_createIndex", (void**)&createIndex))
       return;
   if (!initSymbol("clang_disposeIndex", (void**)&disposeIndex))
       return;
   if (!initSymbol("clang_parseTranslationUnit", (void**)&parseTranslationUnit))
       return;
   if (!initSymbol("clang_disposeTranslationUnit", (void**)&disposeTranslationUnit))
       return;
   if (!initSymbol("clang_getNumDiagnostics", (void**)&getNumDiagnostics))
       return;
   if (!initSymbol("clang_getDiagnostic", (void**)&getDiagnostic))
       return;
   if (!initSymbol("clang_disposeDiagnostic", (void**)&disposeDiagnostic))
       return;
   if (!initSymbol("clang_formatDiagnostic", (void**)&formatDiagnostic))
       return;
   if (!initSymbol("clang_defaultDiagnosticDisplayOptions", (void**)&defaultDiagnosticDisplayOptions))
       return;
   if (!initSymbol("clang_getCString", (void**)&getCString))
       return;
   if (!initSymbol("clang_disposeString", (void**)&disposeString))
       return;
}

bool LibClang::isLoaded(std::string* pError)
{
   if (pLib_ != NULL)
   {
      return true;
   }
   else
   {
      *pError = initError_;
      return false;
   }
}

LibClang::~LibClang()
{
   try
   {
      if (pLib_ != NULL)
      {
         std::string errorMessage;
         if (!closeLibrary(pLib_, &errorMessage))
            std::cerr << errorMessage << std::endl;
      }
   }
   catch(...)
   {
   }
}

 bool LibClang::initSymbol(const std::string& name, void** ppSymbol)
 {
    return loadSymbol(pLib_, name, ppSymbol, &initError_);
 }
