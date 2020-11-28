#pragma once
#include "../myASTVisitor.h"

#include "../Common/printer.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

#include "clang/Frontend/CompilerInstance.h"

#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Analysis/FlowSensitive/DataflowValues.h"
#include "clang/Analysis/Analyses/LiveVariables.h"

#include "clang/AST/APValue.h"

using namespace clang;
using namespace std;

class VariableChecker: public Printer
{

public:

    VariableChecker();

    VariableChecker(const VariableChecker& lv) = delete;

    void PrintCFG(FunctionDecl *func);

    void PrintLiveVariables(FunctionDecl *func);

};