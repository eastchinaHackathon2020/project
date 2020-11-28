#ifndef __SPACECHECKER_H__
#define __SPACECHECKER_H__

#include <string>
#include <vector>

#include "clang/AST/Decl.h"

#include "../Common/printer.h"

#define WARNING_TRIGGER_VARIABLE_SIZE (1 * 1024 * 1024 * 8)

using namespace clang;
using namespace std;

struct Field
{

    string type;

    string name;

    string location;

    bool isUsed;

    Field(string t, string n, string l, bool u) : type(t),
                                                  name(n),
                                                  location(l),
                                                  isUsed(u)
    {
    }
};

class SpaceChecker : public Printer
{

public:
    SpaceChecker();

    SpaceChecker(const SpaceChecker &bvc) = delete;

    void bigVariableCheck(VarDecl *vd);

    void bigFieldCheck(FieldDecl *fd);

    static void unusedFieldCheck();

private:
    double bitToMb(double bits);
};

#endif