#include "Clang.hpp"

#include <iostream>

#ifdef _WIN32

#else
#include <dlfcn.h>
#endif

namespace {

extern "C" {

// strings
typedef const char * (*ptr_clang_getCString)(CXString string);
typedef void (*ptr_clang_disposeString)(CXString string);

// indexes
typedef CXIndex (*ptr_clang_createIndex)(int excludeDeclarationsFromPCH,
                                         int displayDiagnostics);
typedef void (*ptr_clang_disposeIndex)(CXIndex index);

// diagnostics
typedef unsigned (*ptr_clang_getNumDiagnostics)(CXTranslationUnit Unit);
typedef CXDiagnostic (*ptr_clang_getDiagnostic)(CXTranslationUnit Unit,
                                                unsigned Index);
typedef void (*ptr_clang_disposeDiagnostic)(CXDiagnostic Diagnostic);
typedef CXString (*ptr_clang_formatDiagnostic)(CXDiagnostic Diagnostic,
                                               unsigned Options);
typedef unsigned (*ptr_clang_defaultDiagnosticDisplayOptions)(void);

// translation units
typedef CXTranslationUnit (*ptr_clang_parseTranslationUnit)(
                                        CXIndex CIdx,
                                        const char *source_filename,
                                        const char * const *command_line_args,
                                        int num_command_line_args,
                                        struct CXUnsavedFile *unsaved_files,
                                        unsigned num_unsaved_files,
                                        unsigned options);
typedef void (*ptr_clang_disposeTranslationUnit)(CXTranslationUnit Unit);


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


struct LibClang::Impl
{
   Impl()
      : pLibClang_(NULL),
        clang_getCString_(NULL),
        clang_disposeString_(NULL),
        clang_createIndex_(NULL),
        clang_disposeIndex_(NULL),
        clang_getNumDiagnostics_(NULL),
        clang_getDiagnostic_(NULL),
        clang_disposeDiagnostic_(NULL),
        clang_formatDiagnostic_(NULL),
        clang_defaultDiagnosticDisplayOptions_(NULL),
        clang_parseTranslationUnit_(NULL),
        clang_disposeTranslationUnit_(NULL)
   {
   }

   void* pLibClang_;
   ptr_clang_getCString clang_getCString_;
   ptr_clang_disposeString clang_disposeString_;
   ptr_clang_createIndex clang_createIndex_;
   ptr_clang_disposeIndex clang_disposeIndex_;
   ptr_clang_getNumDiagnostics clang_getNumDiagnostics_;
   ptr_clang_getDiagnostic clang_getDiagnostic_;
   ptr_clang_disposeDiagnostic clang_disposeDiagnostic_;
   ptr_clang_formatDiagnostic clang_formatDiagnostic_;
   ptr_clang_defaultDiagnosticDisplayOptions clang_defaultDiagnosticDisplayOptions_;
   ptr_clang_parseTranslationUnit clang_parseTranslationUnit_;
   ptr_clang_disposeTranslationUnit clang_disposeTranslationUnit_;
};

LibClang::LibClang()
   : pImpl_(new Impl())
{
}

LibClang::~LibClang()
{
   try
   {
      if (pImpl_->pLibClang_ != NULL)
      {
         std::string errorMessage;
         if (!closeLibrary(pImpl_->pLibClang_, &errorMessage))
            std::cerr << errorMessage << std::endl;
      }

      delete pImpl_;
   }
   catch(...)
   {
   }
}


#define LOAD_CLANG_SYMBOL(name) \
   if (!loadSymbol(pImpl_->pLibClang_, \
                   #name, \
                   (void**)&pImpl_->name ## _, pError)) \
       return false;

bool LibClang::initialize(std::string* pError)
{
   if (!loadLibrary("libclang.so", (void**)&pImpl_->pLibClang_, pError))
       return false;

   LOAD_CLANG_SYMBOL(clang_createIndex)
   LOAD_CLANG_SYMBOL(clang_disposeIndex)
   LOAD_CLANG_SYMBOL(clang_parseTranslationUnit)
   LOAD_CLANG_SYMBOL(clang_disposeTranslationUnit)
   LOAD_CLANG_SYMBOL(clang_getNumDiagnostics)
   LOAD_CLANG_SYMBOL(clang_getDiagnostic)
   LOAD_CLANG_SYMBOL(clang_disposeDiagnostic)
   LOAD_CLANG_SYMBOL(clang_formatDiagnostic)
   LOAD_CLANG_SYMBOL(clang_defaultDiagnosticDisplayOptions)
   LOAD_CLANG_SYMBOL(clang_getCString)
   LOAD_CLANG_SYMBOL(clang_disposeString)

   return true;
}

const char * LibClang::getCString(CXString string)
{
   return pImpl_->clang_getCString_(string);
}

void LibClang::disposeString(CXString string)
{
   pImpl_->clang_disposeString_(string);
}

CXIndex LibClang::createIndex(int excludeDeclarationsFromPCH,
                              int displayDiagnostics)
{
   return pImpl_->clang_createIndex_(excludeDeclarationsFromPCH,
                                     displayDiagnostics);
}

void LibClang::disposeIndex(CXIndex index)
{
   pImpl_->clang_disposeIndex_(index);
}

unsigned LibClang::getNumDiagnostics(CXTranslationUnit unit)
{
   return pImpl_->clang_getNumDiagnostics_(unit);
}

CXDiagnostic LibClang::getDiagnostic(CXTranslationUnit unit, unsigned index)
{
   return pImpl_->clang_getDiagnostic_(unit, index);
}

void LibClang::disposeDiagnostic(CXDiagnostic diag)
{
   return pImpl_->clang_disposeDiagnostic_(diag);
}

CXString LibClang::formatDiagnostic(CXDiagnostic diag, unsigned options)
{
   return pImpl_->clang_formatDiagnostic_(diag, options);
}

unsigned LibClang::defaultDiagnosticDisplayOptions(void)
{
   return pImpl_->clang_defaultDiagnosticDisplayOptions_();
}


CXTranslationUnit LibClang::parseTranslationUnit(
                                   CXIndex CIdx,
                                   const char *source_filename,
                                   const char * const *command_line_args,
                                   int num_command_line_args,
                                   struct CXUnsavedFile *unsaved_files,
                                   unsigned num_unsaved_files,
                                   unsigned options)
{
   return pImpl_->clang_parseTranslationUnit_(CIdx,
                                              source_filename,
                                              command_line_args,
                                              num_command_line_args,
                                              unsaved_files,
                                              num_unsaved_files,
                                              options);
}

void LibClang::disposeTranslationUnit(CXTranslationUnit unit)
{
   pImpl_->clang_disposeTranslationUnit_(unit);
}



