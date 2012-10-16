#ifndef CLANG_HPP
#define CLANG_HPP

#include <string>

#include "LibClang.h"

extern "C" {

typedef CXIndex (*ptr_clang_createIndex)(int excludeDeclarationsFromPCH,
                                         int displayDiagnostics);
typedef void (*ptr_clang_disposeIndex)(CXIndex index);

} // extern "C"

class Clang
{
public:
   Clang()
      : pLibClang_(NULL),
        pClang_createIndex_(NULL),
        pClang_disposeIndex_(NULL)
   {
   }

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

private:
   void* pLibClang_;
   ptr_clang_createIndex pClang_createIndex_;
   ptr_clang_disposeIndex pClang_disposeIndex_;
};

#endif // CLANG_HPP
