#include "Clang.hpp"

#include <iostream>

#ifdef _WIN32

#else
#include <dlfcn.h>
#endif

namespace {

extern "C" {

typedef CXIndex (*ptr_clang_createIndex)(int excludeDeclarationsFromPCH,
                                         int displayDiagnostics);
typedef void (*ptr_clang_disposeIndex)(CXIndex index);

typedef CXTranslationUnit (*ptr_clang_parseTranslationUnit)(
                                        CXIndex CIdx,
                                        const char *source_filename,
                                        const char * const *command_line_args,
                                        int num_command_line_args,
                                        struct CXUnsavedFile *unsaved_files,
                                        unsigned num_unsaved_files,
                                        unsigned options);

typedef void (*ptr_clang_disposeTranslationUnit)(CXTranslationUnit);

typedef unsigned (*ptr_clang_getNumDiagnostics)(CXTranslationUnit Unit);

typedef CXDiagnostic (*ptr_clang_getDiagnostic)(CXTranslationUnit Unit,
                                                unsigned Index);

typedef CXString (*ptr_clang_formatDiagnostic)(CXDiagnostic Diagnostic,
                                               unsigned Options);

typedef unsigned (*ptr_clang_defaultDiagnosticDisplayOptions)(void);

typedef const char * (*ptr_clang_getCString)(CXString string);

typedef void (*ptr_clang_disposeString)(CXString string);


} // extern "C"

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


struct Clang::Impl
{
   Impl()
      : pLibClang(NULL),
        pClang_createIndex(NULL),
        pClang_disposeIndex(NULL),
        pClang_parseTranslationUnit(NULL),
        pClang_disposeTranslationUnit(NULL),
        pClang_getNumDiagnostics(NULL),
        pClang_getDiagnostic(NULL),
        pClang_formatDiagnostic(NULL),
        pClang_defaultDiagnosticDisplayOptions(NULL),
        pClang_getCString(NULL),
        pClang_disposeString(NULL)
   {
   }

   void* pLibClang;
   ptr_clang_createIndex pClang_createIndex;
   ptr_clang_disposeIndex pClang_disposeIndex;
   ptr_clang_parseTranslationUnit pClang_parseTranslationUnit;
   ptr_clang_disposeTranslationUnit pClang_disposeTranslationUnit;
   ptr_clang_getNumDiagnostics pClang_getNumDiagnostics;
   ptr_clang_getDiagnostic pClang_getDiagnostic;
   ptr_clang_formatDiagnostic pClang_formatDiagnostic;
   ptr_clang_defaultDiagnosticDisplayOptions pClang_defaultDiagnosticDisplayOptions;
   ptr_clang_getCString pClang_getCString;
   ptr_clang_disposeString pClang_disposeString;
};

Clang::Clang()
   : pImpl_(new Impl())
{
}

Clang::~Clang()
{
   try
   {
      if (pImpl_->pLibClang != NULL)
      {
         std::string errorMessage;
         if (!closeLibrary(pImpl_->pLibClang, &errorMessage))
            std::cerr << errorMessage << std::endl;
      }

      delete pImpl_;
   }
   catch(...)
   {
   }
}


bool Clang::initialize(std::string* pError)
{
   if (!loadLibrary("libclang.so", (void**)&pImpl_->pLibClang, pError))
       return false;

   if (!loadSymbol(pImpl_->pLibClang,
                   "clang_createIndex",
                   (void**)&pImpl_->pClang_createIndex,
                   pError))
       return false;

   if (!loadSymbol(pImpl_->pLibClang,
                   "clang_disposeIndex",
                   (void**)&pImpl_->pClang_disposeIndex,
                   pError))
      return false;

   if (!loadSymbol(pImpl_->pLibClang,
                   "clang_parseTranslationUnit",
                   (void**)&pImpl_->pClang_parseTranslationUnit,
                   pError))
      return false;

   if (!loadSymbol(pImpl_->pLibClang,
                   "clang_disposeTranslationUnit",
                   (void**)&pImpl_->pClang_disposeTranslationUnit,
                   pError))
      return false;


   return true;
}

CXIndex Clang::createIndex(int excludeDeclarationsFromPCH,
                           int displayDiagnostics)
{
   return pImpl_->pClang_createIndex(excludeDeclarationsFromPCH,
                                     displayDiagnostics);
}

void Clang::disposeIndex(CXIndex index)
{
   pImpl_->pClang_disposeIndex(index);
}

CXTranslationUnit Clang::parseTranslationUnit(
                                   CXIndex CIdx,
                                   const char *source_filename,
                                   const char * const *command_line_args,
                                   int num_command_line_args,
                                   struct CXUnsavedFile *unsaved_files,
                                   unsigned num_unsaved_files,
                                   unsigned options)
{
   return pImpl_->pClang_parseTranslationUnit(CIdx,
                                              source_filename,
                                              command_line_args,
                                              num_command_line_args,
                                              unsaved_files,
                                              num_unsaved_files,
                                              options);
}

void Clang::disposeTranslationUnit(CXTranslationUnit unit)
{
   pImpl_->pClang_disposeTranslationUnit(unit);
}



