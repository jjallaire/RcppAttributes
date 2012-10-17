#ifndef CLANG_HPP
#define CLANG_HPP

#include <string>

#include "Clang-C.h"

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


class LibClang
{
public:
   LibClang(const std::string& libraryPath);
   bool isLoaded(std::string* pError);
   ~LibClang();

private:
   // disallow copying and assignment
   LibClang(const LibClang&);
   LibClang& operator=(const LibClang&);

public:

   ptr_clang_getCString getCString;
   ptr_clang_disposeString disposeString;
   ptr_clang_createIndex createIndex;
   ptr_clang_disposeIndex disposeIndex;
   ptr_clang_getNumDiagnostics getNumDiagnostics;
   ptr_clang_getDiagnostic getDiagnostic;
   ptr_clang_disposeDiagnostic disposeDiagnostic;
   ptr_clang_formatDiagnostic formatDiagnostic;
   ptr_clang_defaultDiagnosticDisplayOptions defaultDiagnosticDisplayOptions;
   ptr_clang_parseTranslationUnit parseTranslationUnit;
   ptr_clang_disposeTranslationUnit disposeTranslationUnit;

private:
   bool initSymbol(const std::string& name, void** ppSymbol);

private:
   void* pLib_;
   std::string initError_;
};

#endif // CLANG_HPP
