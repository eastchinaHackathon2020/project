#ifndef __SWITCHCHECKER_H__
#define __SWITCHCHECKER_H__

#include <string>
#include <vector>
#include <unordered_map>

#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"

#include "printer.h"

using namespace clang;
using namespace std;

extern const vector<string> VALID_COND_TYPE_LIST;

extern const unordered_map<string, vector<string>> VALID_CAST_TABLE;

class SwitchChecker: public Printer
{

public:

    SwitchChecker();

    SwitchChecker(const SwitchChecker& sc) = delete;

    void typeMismatchCheck(SwitchStmt* ss);

    void enumIncompleteCheck(SwitchStmt* ss);

private:

    vector<SwitchCase*> getCaseByOrder(SwitchStmt* ss);

    bool isCastAccepted(QualType caseType, QualType condType);

    string getFilteredTypeAsString(QualType type);

    string getEnumNameAsString(QualType type);
};

#endif