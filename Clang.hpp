#ifndef CLANG_HPP
#define CLANG_HPP

#include <string>

#include "LibClang.h"

class Clang
{
public:
   Clang();
   ~Clang();

private:
   // disallow copying and asignment
   Clang(const Clang&);
   Clang& operator=(const Clang&);

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

private:
   struct Impl;
   Impl* pImpl_;
};

#endif // CLANG_HPP
