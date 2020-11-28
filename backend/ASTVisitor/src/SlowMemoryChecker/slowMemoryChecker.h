#pragma once
#include "../myASTVisitor.h"

#include "../Common/printer.h"
#include "../Common/errNo.h"

#include <cstdio>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>


#include "clang/Lex/Lexer.h"
#include "clang/Analysis/CFG.h"
#include "clang/Analysis/AnalysisDeclContext.h"
#include "clang/Analysis/FlowSensitive/DataflowValues.h"
#include "clang/Analysis/Analyses/LiveVariables.h"
#include "clang/AST/APValue.h"

#include "slowMemoryChecker.h"
#include "VariableChecker.h"

#include "readConfig.h"

using namespace clang;
using namespace std;

class ForStmtCondLoc: public Printer
{

public:

    ForStmtCondLoc();

    ForStmtCondLoc(const ForStmtCondLoc& fscl) = delete;

    void getCondLoc(Expr *cond);

    int getBeginLine();

    int getBeginCol();

    int getEndLine();

    int getEndCol();

private:

    int condBeginLine;

    int condBeginCol;

    int condEndLine;

    int condEndCol;

    int findCondBeginLine(Expr *cond);

    int findCondBeginCol(Expr *cond);

    int findCondEndLine(Expr *cond);

    int findCondEndCol(Expr *cond);

};



class SlowMemoryChecker: public Printer
{

public:

    SlowMemoryChecker();

    SlowMemoryChecker(const SlowMemoryChecker& smc) = delete;

    void BinaryOperationCheck(BinaryOperator* bo,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine);

    bool sameType(QualType ltype,QualType rtype);

    void printCallExprName(clang::CallExpr *c);

    void printCXXCallExprName(CXXOperatorCallExpr *c);

    int findWhileStmtEndLine(WhileStmt *ws);

    int findForStmtEndLine(ForStmt *fs);

    int findDoWhileStmtEndLine(DoStmt *ds);

    bool FindExprNameByToken(Expr *e);

    ForStmtCondLoc fscl;

    bool isBinaryOpratorInCond(BinaryOperator *bo);

    bool isAssignmentOpInForHead(BinaryOperator *bo);

    int getNumOfLoop(BinaryOperator *bo,int line,int col,int initVal);

    bool isComparisonOperator(BinaryOperator *bo);

    int getInitVal(BinaryOperator *bo);

    bool isVarDeclInForHead(VarDecl *vd);

    VarDecl *ForInitVarDecl;

    bool VarDeclInForHead;

    int getInitValFromVarDecl(VarDecl *vd);

    int numOfForLoop;

    int numOfWhileLoop;

    int numOfDoWhileLoop;

    int minNumOfLoop;

    bool inForStmt;

    int ForStmtEndLine;

    bool inWhileStmt;

    int WhileStmtEndLine;

    bool inDoWhileStmt;

    int DoWhileStmtEndLine;

    void BinaryOperationCheckInForStmt(BinaryOperator *bo);

    void BinaryOperationCheckInWhileStmt(BinaryOperator* bo);

    void BinaryOperationCheckInDoWhileStmt(BinaryOperator* bo);

    void setFlags(Stmt *stmt);

    int getNumOfDoWhileLoop(int line,int col,int initVal);

    Expr *DoWhileCond;

    ReadConfig rc;

};
