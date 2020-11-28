#include <iostream>
#include <sstream>
#include <limits.h>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/AST/RecursiveASTVisitor.h"

#include "myASTVisitor.h"
#include "switchChecker.h"

using namespace clang;
using namespace std;

const vector<string> VALID_COND_TYPE_LIST
{
  "enum",
  "int",
  "unsigned int",
  "short",
  "unsigned short",
  "long",
  "unsigned long",
  "long long",
  "unsigned long long",
  "char",
  "unsigned char",
  "bool"
};

const unordered_map<string, vector<string>> VALID_CAST_TABLE
{
  {"enum",                {"enum", "int", "short", "unsigned short", "char", "unsigned char", "bool"}},
  {"int",                 {"int", "enum", "short", "unsigned short", "char", "unsigned char", "bool"}},
  {"unsigned int",        {"unsigned int", "short", "unsigned short", "char", "unsigned char", "bool"}},
  {"short",               {"short", "char", "unsigned char", "bool"}},
  {"unsigned short",      {"unsigned short", "char", "unsigned char", "bool"}},

#if LONG_MAX >= LLONG_MAX
  {"long",                {"long", "int", "enum", "unsigned int", "short", "unsigned short", "long long", "char", "unsigned char", "bool"}},
  {"unsigned long",       {"unsigned long", "int", "enum", "unsigned int", "short", "unsigned short", "unsigned long long", "char", "unsigned char", "bool"}},
  {"long long",           {"long long", "int", "enum", "unsigned int", "short", "unsigned short", "long", "char", "unsigned char", "bool"}},
  {"unsigned long long",  {"unsigned long long", "int", "enum", "unsigned int", "short", "unsigned short", "unsigned long", "char", "unsigned char", "bool"}},
#else
  {"long",                {"long", "int", "enum", "unsigned int", "short", "unsigned short", "char", "unsigned char", "bool"}},
  {"unsigned long",       {"unsigned long", "int", "enum", "unsigned int", "short", "unsigned short", "char", "unsigned char", "bool"}},
  {"long long",           {"long long", "int", "enum", "unsigned int", "short", "unsigned short", "long", "char", "unsigned char", "bool"}},
  {"unsigned long long",  {"unsigned long long", "int", "enum", "unsigned int", "short", "unsigned short", "unsigned long", "char", "unsigned char", "bool"}},
#endif

  {"char",                {"char", "bool"}},
  {"unsigned char",       {"unsigned char", "bool"}},
  {"bool",                {"bool"}}
};

SwitchChecker::SwitchChecker()
{
  if (!of.is_open()) of.open("warn.dat", std::ios::out | std::ios::trunc);
}

void SwitchChecker::typeMismatchCheck(SwitchStmt* ss)
{
  QualType condType = ss->getCond()->IgnoreImpCasts()->getType();
  vector<SwitchCase*> sces = getCaseByOrder(ss);
  for (const auto& sc : sces)
  {
    CaseStmt* cs = cast<CaseStmt>(sc);
    QualType caseType = cs->getLHS()->IgnoreImpCasts()->getType();
    if (!isCastAccepted(caseType, condType))
    {
      SourceLocation beginLoc = cs->getBeginLoc();
      string locString = beginLoc.printToString(*SM);
      stringstream ssr;

#ifdef OOP
      cout << locString.c_str() << ':' <<
        "warning: there is a mismatch between case type and cond type" << endl;
      ssr << locString.c_str() << ':' <<
        "warning: there is a mismatch between case type and cond type" << endl;
#else
      cout << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;
      ssr << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;  
#endif

      pprint(ssr.str());
    }
  }
}

void SwitchChecker::enumIncompleteCheck(SwitchStmt* ss)
{
  QualType condType = ss->getCond()->IgnoreImpCasts()->getType();

  assert(isa<EnumType>(condType));
  
  //string condStr = getFilteredTypeAsString(condType);

  if (ss->getSwitchCaseList() &&
      isa<DefaultStmt>(ss->getSwitchCaseList())
    )
  {
    return;
  }

  EnumDecl* ed = EDs[getEnumNameAsString(condType)];
  unordered_map<string, bool> enum_elems;
  unordered_map<string, string> enum_names;
  auto elems = ed->enumerators();
  for (auto const& elem : elems)
  {
    string _int = elem->getInitVal().toString(10, false);
    enum_elems[_int] = false;
    enum_names[_int] = elem->getNameAsString();
  }

  vector<SwitchCase*> sces = getCaseByOrder(ss);
  CaseStmt* cs = NULL;
  Expr::EvalResult result;
  string caseVal;
  for (const auto& sc : sces)
  {
    cs = cast<CaseStmt>(sc);
    cs->getLHS()->EvaluateAsInt(result, *CTX);
    caseVal = result.Val.getInt().toString(10, false);
    if (enum_elems.find(caseVal) != enum_elems.end())
    {
      enum_elems[caseVal] = true;
    }
  }

  vector<string> warn_elems;
  for (const auto& enum_elem : enum_elems)
  {
    if (!enum_elem.second)
    {
      warn_elems.push_back(enum_names[enum_elem.first]);
    }
  }

  SourceLocation warningLoc = ss->getBeginLoc();
  string locString = warningLoc.printToString(*SM);
  stringstream ssr;
  
  if (!warn_elems.empty())
  {

#ifdef OOP

    if (warn_elems.size() < 4)
    {
      assert(warn_elems.size() >= 1);

      cout << locString.c_str() << ':' <<
        "warning: enumeration value";
      ssr << locString.c_str() << ':' <<
        "warning: enumeration value";
      if (warn_elems.size() > 1)
      {
        cout << 's';
        ssr << 's';
      }
      cout << ' ';
      ssr << ' ';

      if (warn_elems.size() == 3)
      {
        cout << '\'' << warn_elems[2] << '\'' << ',' << ' ';
        ssr << '\'' << warn_elems[2] << '\'' << ',' << ' ';
      }
      if (warn_elems.size() == 2 || warn_elems.size() == 3)
      {
        cout << '\'' << warn_elems[1] << '\'' << ' ' <<
          "and" << ' ';
        ssr << '\'' << warn_elems[1] << '\'' << ' ' <<
          "and" << ' ';
      }
      cout << '\'' << warn_elems[0] << '\'' << ' ' <<
        "not handled in switch" << endl;
      ssr << '\'' << warn_elems[0] << '\'' << ' ' <<
        "not handled in switch" << endl;
    }
    else
    {
      cout << locString.c_str() << ':' << ' ' <<
        "warning:" << ' ' << warn_elems.size() << ' ' << "enumeration values not handled in switch:" << ' ';
      ssr << locString.c_str() << ':' << ' ' <<
        "warning:" << ' ' << warn_elems.size() << ' ' << "enumeration values not handled in switch:" << ' ';
      for (int i = 0; i < 2; i++)
      {
        cout << '\'' << warn_elems[i] << '\'' << ',' << ' ';
        ssr << '\'' << warn_elems[i] << '\'' << ',' << ' ';
      }
      cout << '\'' << warn_elems[2] << '\'' << "..." << endl;;
      ssr << '\'' << warn_elems[2] << '\'' << "..." << endl;
    }

#else
    cout << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;
    ssr << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;
#endif

    pprint(ssr.str());
  }

  warningLoc = ss->getEndLoc();
  locString = warningLoc.printToString(*SM);

#ifdef OOP
  cout << locString.c_str() << ':' << ' ' <<
    "warning: there is no \'default\' statement" << endl;
  ssr << locString.c_str() << ':' << ' ' <<
    "warning: there is no \'default\' statement" << endl;
#else
  cout << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;
  ssr << locString.c_str() << ':' << ('0' + SwitchMismatch) << endl;
#endif
  pprint(ssr.str());

}

vector<SwitchCase*> SwitchChecker::getCaseByOrder(SwitchStmt* ss)
{
  vector<SwitchCase*> ordered_sc;
  for (SwitchCase* sc = ss->getSwitchCaseList(); sc; sc = sc->getNextSwitchCase())
  {
    if (isa<CaseStmt>(sc)) {
      ordered_sc.insert(ordered_sc.begin(), sc);
    }
  }
  return ordered_sc;
}

bool SwitchChecker::isCastAccepted(QualType caseType, QualType condType)
{
  string caseStr = getFilteredTypeAsString(caseType);
  string condStr = getFilteredTypeAsString(condType);

  if (VALID_CAST_TABLE.at(condStr).empty()) {
    uint64_t caseSize = CTX->getTypeSize(caseType);
    uint64_t condSize = CTX->getTypeSize(condType);
    return caseSize <= condSize;
  }

  vector<string> v = VALID_CAST_TABLE.at(condStr);
  if (find(v.begin(), v.end(), caseStr) == v.end())
  {
    return false;
  }
  return true;
}

string SwitchChecker::getFilteredTypeAsString(QualType type)
{
  string can = type.getCanonicalType().getAsString();
  if (can.compare(0, 4, "enum") == 0)
  {
    return "enum";
  }
  return can;
}

string SwitchChecker::getEnumNameAsString(QualType type)
{
  string can = type.getCanonicalType().getAsString();

  assert(!can.compare(0, 4, "enum"));

  return can.substr(5);
}