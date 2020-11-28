#ifndef __SPACECHECKER_H__
#define __SPACECHECKER_H__

#include "clang/AST/Decl.h"

#include "printer.h"

#define WARNING_TRIGGER_VARIABLE_SIZE (1*1024*1024*8)

using namespace clang;
using namespace std;

class SpaceChecker: public Printer
{

public:

    SpaceChecker();

    SpaceChecker(const SpaceChecker& bvc) = delete;

    void bigVariableCheck(VarDecl* vd);

};

double bitToMb(double bits);

#endif