#ifndef CLANG_HPP
#define CLANG_HPP

#include <string>

#include "Clang-C.h"


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

   // strings
   const char * (*getCString)(CXString string);
   void (*disposeString)(CXString string);

   // indexes
   CXIndex (*createIndex)(int excludeDeclarationsFromPCH,
                          int displayDiagnostics);
   void (*disposeIndex)(CXIndex index);

   // diagnostics
   unsigned (*getNumDiagnostics)(CXTranslationUnit Unit);
   CXDiagnostic (*getDiagnostic)(CXTranslationUnit Unit,
                                                   unsigned Index);
   void (*disposeDiagnostic)(CXDiagnostic Diagnostic);
   CXString (*formatDiagnostic)(CXDiagnostic Diagnostic, unsigned Options);
   unsigned (*defaultDiagnosticDisplayOptions)(void);

   // translation units
   CXTranslationUnit (*parseTranslationUnit)(
                                       CXIndex CIdx,
                                       const char *source_filename,
                                       const char * const *command_line_args,
                                       int num_command_line_args,
                                       struct CXUnsavedFile *unsaved_files,
                                       unsigned num_unsaved_files,
                                       unsigned options);
   void (*disposeTranslationUnit)(CXTranslationUnit Unit);

private:
   void* pLib_;
   std::string initError_;
};

#endif // CLANG_HPP
