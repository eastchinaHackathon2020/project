#include <iostream>
#include <sstream>

#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "myASTVisitor.h"
#include "spaceChecker.h"

SpaceChecker::SpaceChecker()
{
  if (!of.is_open()) of.open("warn.dat", std::ios::out | std::ios::trunc);
}

void SpaceChecker::bigVariableCheck(VarDecl* vd)
{
  SourceLocation beginLoc = vd->getBeginLoc();
  QualType qt = vd->getType();
  uint64_t tsize = vd->getASTContext().getTypeSize(vd->getType());
  if (tsize >=  WARNING_TRIGGER_VARIABLE_SIZE) {
    string qtstr = qt.getAsString();
    string locString = beginLoc.printToString(*SM);
    stringstream ssr;

#ifdef OOP
    cout << locString.c_str() << ':' << ' ' <<
      "warning: variable" << ' ' <<
      '\'' << qtstr << '\'' << ' ' <<
      '(' << bitToMb(tsize) << "Mb" << ')' << ' ' <<
      "is too big" << endl;
    ssr << locString.c_str() << ':' << ' ' <<
      "warning: variable" << ' ' <<
      '\'' << qtstr << '\'' << ' ' <<
      '(' << bitToMb(tsize) << "Mb" << ')' << ' ' <<
      "is too big" << endl;
#else
    cout << locString.c_str() << ':' << ('0' + SpaceProblem) << endl;
    ssr << locString.c_str() << ':' << ('0' + SpaceProblem) << endl;
#endif

    pprint(ssr.str());
  }
}

double bitToMb(double bits)
{
  return bits / 8 / 1024 / 1024;
}