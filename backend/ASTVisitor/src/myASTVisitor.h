  
#include <string>
#include <unordered_map>
#include "clang/AST/Decl.h"

#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "SpaceChecker/spaceChecker.h"
#include "SwitchChecker/switchChecker.h"

using namespace std;
using namespace clang;

extern SourceManager *SM;
extern ASTContext *CTX;
extern CompilerInstance TheCompInst;d
extern unordered_map<string, EnumDecl*> EDs;

extern vector<SwitchLocation> switch_loc_list;
extern vector<Field> field_use_list;
