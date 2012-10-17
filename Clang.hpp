#ifndef CLANG_HPP
#define CLANG_HPP

#include <string>

#include "Clang-C.h"

class LibClang
{
public:
   LibClang();
   ~LibClang();

private:
   // disallow copying and asignment
   LibClang(const LibClang&);
   LibClang& operator=(const LibClang&);

public:

   bool initialize(std::string* pError);

   CXIndex createIndex(int excludeDeclarationsFromPCH,
                       int displayDiagnostics);
   void disposeIndex(CXIndex index);

   CXTranslationUnit parseTranslationUnit(
                                      CXIndex CIdx,
                                      const char *source_filename,
                                      const char * const *command_line_args,
                                      int num_command_line_args,
                                      struct CXUnsavedFile *unsaved_files,
                                      unsigned num_unsaved_files,
                                      unsigned options);

   void disposeTranslationUnit(CXTranslationUnit unit);

   unsigned getNumDiagnostics(CXTranslationUnit unit);

   CXDiagnostic getDiagnostic(CXTranslationUnit unit, unsigned index);

   CXString formatDiagnostic(CXDiagnostic diag, unsigned options);

   unsigned defaultDiagnosticDisplayOptions(void);

   const char * getCString(CXString string);

   void disposeString(CXString string);


private:
   struct Impl;
   Impl* pImpl_;
};

#endif // CLANG_HPP
