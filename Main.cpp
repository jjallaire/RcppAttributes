
#include <cstdlib>
#include <iostream>

#include "Clang.hpp"

std::string getLibClangPath()
{
#if defined(_WIN32)
   return "C:\\Users\\jjallaire\\Desktop\\libclang.dll";
#elif defined(__APPLE__)
   return "/usr/lib/libclang.dylib";
#else
   return "/usr/lib/libclang.so";
#endif
}

LibClang libClang(getLibClangPath());


enum CXChildVisitResult functionParamFinder(CXCursor cursor,
                                            CXCursor parent,
                                            CXClientData client_data)
{
   if (cursor.kind == CXCursor_ParmDecl)
   {
      CXString displayName = libClang.getCursorSpelling(cursor);
      std::cerr << "param: " << libClang.getCString(displayName) << std::endl;
      libClang.disposeString(displayName);
   }


   return CXChildVisit_Continue;
}

enum CXChildVisitResult functionDeclartionFinder(CXCursor cursor,
                                                 CXCursor parent,
                                                 CXClientData client_data)
{
   if (cursor.kind == CXCursor_FunctionDecl &&
       libClang.isCursorDefinition(cursor))
   {
      CXString displayName = libClang.getCursorDisplayName(cursor);
      std::cerr << "function: " << libClang.getCString(displayName) << std::endl;
      libClang.disposeString(displayName);

      CXType resultType = libClang.getCursorResultType(cursor);
      displayName = libClang.getCursorDisplayName(libClang.getTypeDeclaration(resultType));
      std::cerr << "result-type: " << libClang.getCString(displayName) << std::endl;
      libClang.disposeString(displayName);


      libClang.visitChildren(cursor, functionParamFinder, NULL);

      return CXChildVisit_Continue;
   }
   else
   {
      return CXChildVisit_Recurse;
   }

}


enum CXChildVisitResult astVisitor(CXCursor cursor,
                                   CXCursor parent,
                                   CXClientData client_data)
{
   CXCursorKind cursorKind = libClang.getCursorKind(cursor);
   CXString kind = libClang.getCursorKindSpelling(cursorKind);
   CXString data = libClang.getCursorDisplayName(cursor);
   std::cerr << "kind: " << libClang.getCString(kind) << std::endl;
   std::cerr << "data: " << libClang.getCString(data) << std::endl;
   std::cerr << std::endl;
   libClang.disposeString(kind);
   libClang.disposeString(data);

   return CXChildVisit_Recurse;
}

int main(int argc, char * const argv[]) 
{
   std::string initError;
   if (!libClang.isLoaded(&initError))
   {
      std::cerr << initError << std::endl;
      return EXIT_FAILURE;
   }

   CXIndex index = libClang.createIndex(0,0);

   CXTranslationUnit TU = libClang.parseTranslationUnit(index,
                                                     0,
                                                     argv+1,
                                                     argc-1,
                                                     0,
                                                     0,
                                                     CXTranslationUnit_None);

   for (unsigned I = 0, N = libClang.getNumDiagnostics(TU); I != N; ++I) {
       CXDiagnostic Diag = libClang.getDiagnostic(TU, I);
       CXString String = libClang.formatDiagnostic(Diag,
                               libClang.defaultDiagnosticDisplayOptions());
       fprintf(stderr, "%s\n", libClang.getCString(String));
       libClang.disposeString(String);
     }


   CXCursor txCursor = libClang.getTranslationUnitCursor(TU);
   libClang.visitChildren(txCursor, astVisitor, NULL);


   libClang.disposeTranslationUnit(TU);
   libClang.disposeIndex(index);

	return EXIT_SUCCESS;
}
