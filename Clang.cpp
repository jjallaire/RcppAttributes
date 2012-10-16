#include "Clang.hpp"


#ifdef _WIN32

#else
#include <dlfcn.h>
#endif

namespace {

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

#ifdef _WIN32


#else

Error loadLibrary(const std::string& libPath, void** ppLib)
{
   *ppLib = NULL;
   *ppLib = ::dlopen(libPath.c_str(), RTLD_NOW);
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

#endif


} // anonymous namespace

Clang::~Clang()
{
   try
   {
      if (pLibClang_ != NULL)
         closeLibrary(pLibClang_);
   }
   catch(...)
   {
   }
}


bool Clang::initialize(std::string* pError)
{
   Error error = loadLibrary("libclang.so", (void**)&pLibClang_);
   if (error)
   {
      *pError = error.message;
      return false;
   }

   error = loadSymbol(pLibClang_,
                      "clang_createIndex",
                      (void**)&pClang_createIndex_);
   if (error)
   {
      *pError = error.message;
      return false;
   }

   error = loadSymbol(pLibClang_,
                      "clang_disposeIndex",
                      (void**)&pClang_disposeIndex_);
   {
      *pError = error.message;
      return false;
   }

   return true;
}

CXIndex Clang::createIndex(int excludeDeclarationsFromPCH,
                           int displayDiagnostics)
{
   return pClang_createIndex_(excludeDeclarationsFromPCH, displayDiagnostics);
}

void Clang::disposeIndex(CXIndex index)
{
   pClang_disposeIndex_(index);
}


