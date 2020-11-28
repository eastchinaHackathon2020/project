#include "slowMemoryChecker.h"

void SlowMemoryChecker::setFlags(Stmt *bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    //printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);
    if(BinaryOperatorLine > ForStmtEndLine)
    {
      this->inForStmt=false;
      this->ForStmtEndLine=-1;
      this->numOfForLoop=0;
      //printf("numofForloop now is 0!\n");

    }

    //printf("BinaryOperatorLine: %d\nWhileStmtEndLine: %d\n\n",BinaryOperatorLine,WhileStmtEndLine);
    if(BinaryOperatorLine > WhileStmtEndLine)
    {
      this->inWhileStmt=false;
      this->WhileStmtEndLine=-1;
      this->numOfWhileLoop=0;
      //printf("numofWhileloop now is 0!\n");
    }

    //printf("BinaryOperatorLine: %d\nDoWhileStmtEndLine: %d\n\n",BinaryOperatorLine,DoWhileStmtEndLine);
    if(BinaryOperatorLine > DoWhileStmtEndLine)
    {
      this->inDoWhileStmt=false;
      this->DoWhileStmtEndLine=-1;
      this->numOfDoWhileLoop=0;
      //printf("numofDoWhileloop now is 0!\n");
    }
}

void SlowMemoryChecker::BinaryOperationCheck(BinaryOperator* bo,int BinaryOperatorLine,int ForStmtEndLine,int WhileStmtEndLine)
{
    int endLine=0;

    // printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);
    // if(BinaryOperatorLine > ForStmtEndLine)
    // {
    //   this->inForStmt=false;
    //   this->ForStmtEndLine=-1;
    //   this->numOfLoop=0;
    //   printf("numofloop now is 0!\n");
    //   return ;
    // }

    if(this->inForStmt==true)
    {
      endLine = this->ForStmtEndLine;
      BinaryOperationCheckInForStmt(bo);
    }
    else if(this->inWhileStmt == true)
    {
      endLine = this->WhileStmtEndLine;
      BinaryOperationCheckInWhileStmt(bo);
    }
    else if(this->inDoWhileStmt == true)
    {
      endLine = this->DoWhileStmtEndLine;
      BinaryOperationCheckInDoWhileStmt(bo);
    }

}

void SlowMemoryChecker::BinaryOperationCheckInDoWhileStmt(BinaryOperator* bo)
{
  SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int initVal=0;
    

    //bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);//todo : dowhile's cond is in the end of Dostmt

    //printf("Incond : %d , DowhileLoop :%d\n",BinaryOperatorInCond,this->numOfDoWhileLoop);


    if(this->numOfDoWhileLoop == 0)
    {
      this->numOfDoWhileLoop = this->getNumOfDoWhileLoop(BinaryOperatorLine,BinaryOperatorCol,initVal);
      //printf("DowhileLoop :%d \n",this->numOfDoWhileLoop);
    }

    // if(BinaryOperatorLine > WhileStmtEndLine)
    // {
    //   this->numOfWhileLoop=0;
    // }

    //printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);

    //outside of While or For
    // if(BinaryOperatorLine > endLine)
    // {
    //   // printf("endLine : %d\n",endLine);
    //   // printf("return !!! BinaryOperatorLine: %d\nForStmtEndLine: %d\nWhileStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine,WhileStmtEndLine);
    //   return ;
    // }
    
    //printf("numOfWhileLoop: %d\n",this->numOfWhileLoop);

    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    //printf("DoWhile :: initVal : %d numOfDoWhileLoop: %d lsize:%d \n\n",initVal,this->numOfDoWhileLoop,lsize);
    if(lsize<=1 && (this->numOfDoWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, ltype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, ltype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = lhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      this->DoWhileCond=NULL;

    }
    else if (rsize<=1 && (this->numOfDoWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      this->DoWhileCond=NULL;
    }
}

void SlowMemoryChecker::BinaryOperationCheckInWhileStmt(BinaryOperator* bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int initVal=0;
    

    bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);

    
    if(BinaryOperatorInCond == true && this->numOfWhileLoop == 0)
    {
      //printf("\n\nincond!\n\n");
      this->numOfWhileLoop = getNumOfLoop(bo,BinaryOperatorLine,BinaryOperatorCol,initVal);
    }

    // if(BinaryOperatorLine > WhileStmtEndLine)
    // {
    //   this->numOfWhileLoop=0;
    // }

    //printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);

    
    //printf("numOfWhileLoop: %d\n",this->numOfWhileLoop);

    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();

    //printf("While :: initVal : %d numOfWhileLoop: %d lsize:%d \n\n",initVal,this->numOfWhileLoop,lsize);
    if(lsize<=1 && (this->numOfWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;

    }
    else if (rsize<=1 && (this->numOfWhileLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;

    }
}

void SlowMemoryChecker::BinaryOperationCheckInForStmt(BinaryOperator* bo)
{

    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int initVal=0;
    if(this->VarDeclInForHead==true)
    {
      initVal = getInitValFromVarDecl(this->ForInitVarDecl);
    }
    else
    {
      if(bo->isAssignmentOp())
      {
        bool AssignmentOpInForHead = this->isAssignmentOpInForHead(bo);
        if(AssignmentOpInForHead)
        {
          initVal = getInitVal(bo);
        }

      }
    }

    bool BinaryOperatorInCond = this->isBinaryOpratorInCond(bo);

    
    if(BinaryOperatorInCond == true && this->numOfForLoop == 0)
    {
      //printf("\n\nincond!\n\n");
      this->numOfForLoop = getNumOfLoop(bo,BinaryOperatorLine,BinaryOperatorCol,initVal);
    }

    // if(BinaryOperatorLine > ForStmtEndLine)
    // {
    //   this->numOfForLoop=0;
    // }

    //printf("BinaryOperatorLine: %d\nForStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine);

    //outside of While or For
    // if(BinaryOperatorLine > endLine)
    // {
    //   // printf("endLine : %d\n",endLine);
    //   // printf("return !!! BinaryOperatorLine: %d\nForStmtEndLine: %d\nWhileStmtEndLine: %d\n\n",BinaryOperatorLine,ForStmtEndLine,WhileStmtEndLine);
    //   return ;
    // }
    
    //printf("numOfForLoop: %d\n",this->numOfForLoop);

    PresumedLoc PLoc = (*SM).getPresumedLoc(beginLoc);
    const char * fname = PLoc.getFilename();
    int line = PLoc.getLine();
    int col = PLoc.getColumn();

    Expr *lhs = bo->getLHS()->IgnoreImpCasts();
    Expr *rhs = bo->getRHS()->IgnoreImpCasts();

    QualType ltype = lhs->getType();
    QualType rtype = rhs->getType();
    bool isSameType = sameType(ltype,rtype);

    CharUnits lcu = CTX->getTypeSizeInChars(ltype);
    int lsize = lcu.getQuantity();
    CharUnits rcu = CTX->getTypeSizeInChars(rtype);
    int rsize = rcu.getQuantity();


    //printf("For :: initVal : %d numOfForLoop: %d lsize:%d \n\n",initVal,this->numOfForLoop,lsize);
    if(lsize<=1 && (this->numOfForLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), lsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      //this->numOfLoop=0;
    }
    else if (rsize<=1 && (this->numOfForLoop > this->minNumOfLoop))
    {
#ifdef OOP
      printf("SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      char tmpwarn[100];
      sprintf(tmpwarn, "SlowMemoryOperation::%s:%d:%d Type:%s SizeOfType:%d\n", fname, line, col, rtype.getAsString().c_str(), rsize);
      std::string tmpwarns(tmpwarn);
      pprint(tmpwarns);
#else
      string warns = rhs->getBeginLoc().printToString(*SM) + ':' + static_cast<char>('0' + SlowMemoryOper) + '\n';
      pprint(warns);
      std::cout << warns;
#endif

      this->VarDeclInForHead=false;
      this->ForInitVarDecl=NULL;
      //this->numOfLoop=0;
    }
}

bool SlowMemoryChecker::isVarDeclInForHead(VarDecl *vd)
{
    SourceLocation beginLoc = vd->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int VarDeclLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int VarDeclCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;

    if(VarDeclLine == condBeginLine)
    {
      LineMatch=true;
      if(VarDeclCol < condBeginCol)
      {
        ColMatch=true;
      }
    }

    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      this->ForInitVarDecl=vd;
      this->VarDeclInForHead=true;                                                                                                                                                              
      return true;
    }
    else
    {
      this->ForInitVarDecl=NULL;
      this->VarDeclInForHead=false;
      return false;
    }
}

int SlowMemoryChecker::getInitValFromVarDecl(VarDecl *vd)
{
  clang::Expr::EvalResult result;
  clang::APValue *apvalue;
  apvalue = vd->evaluateValue();

  //TODO
  //get the name of VarDecl?

  int val=0;
  if(apvalue->isInt())
  {
    llvm::APSInt intval = apvalue->getInt();
    //printf("its Int,in line %d col %d\n",line,col);
    

    QualType ty = vd->getType();
    string valString = apvalue->getAsString(TheCompInst.getASTContext(),ty);
    val = atoi(valString.c_str());
    //printf("val : %d\n",val);
    //printf("aspint: %s\n",intval.toString(10).c_str());
  }
  return val;
}

int SlowMemoryChecker::getInitVal(BinaryOperator *bo)
{
  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();

  
  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(TheCompInst.getASTContext()))
  {
    RHS->EvaluateAsInt(result,TheCompInst.getASTContext());
  }

  int val=0;
  clang::APValue apvalue = result.Val;
  if(apvalue.isInt())
  {
    llvm::APSInt intval = apvalue.getInt();
    //printf("its Int,in line %d col %d\n",line,col);
    

    QualType ty = RHS->getType();
    string valString = apvalue.getAsString(TheCompInst.getASTContext(),ty);
    val = atoi(valString.c_str());
    //printf("val : %d\n",val);
    //printf("aspint: %s\n",intval.toString(10).c_str());
  }
  return val;
}

int SlowMemoryChecker::getNumOfDoWhileLoop(int line,int col,int initVal)
{
  BinaryOperator *bo =  cast<BinaryOperator>(this->DoWhileCond);
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  static llvm::StringRef opcodeStrRef = bo->getOpcodeStr(opcode);
  const char *opcodeStr = opcodeStrRef.str().c_str();
  //printf("opcode : %s\n",opcodeStr);
  int val=0;
  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();

  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(TheCompInst.getASTContext()))
  {
    RHS->EvaluateAsInt(result,TheCompInst.getASTContext());
    
    clang::APValue apvalue = result.Val;
    if(apvalue.isInt())
    {
      llvm::APSInt intval = apvalue.getInt();
      //printf("its Int,in line %d col %d\n",line,col);


      QualType ty = RHS->getType();
      string valString = apvalue.getAsString(TheCompInst.getASTContext(),ty);
      val = atoi(valString.c_str());
      //printf("val : %d\n",val);
      //printf("aspint: %s\n",intval.toString(10).c_str());
    }
  }
  else
  {
    //if it's Unevaluable, then we should give the warning
    val=this->minNumOfLoop+1;
  }

  

  //printf("initval: %d , val: %d\n",initVal,val);
  int numOfLoop=0;
  //what kind of ComparisonOperator
  if(strcmp(opcodeStr,"<") == 0)
  {
    numOfLoop=val-initVal;
  }
  else if(strcmp(opcodeStr,">") == 0)
  {
    numOfLoop=initVal-val;
  }
  else if(strcmp(opcodeStr,"!=") == 0)
  {
    if(initVal<=val)
    {
      numOfLoop=val-initVal;
    }
    else
    {
      numOfLoop=initVal-val;
    }
  }
  
  return numOfLoop;
}

int SlowMemoryChecker::getNumOfLoop(BinaryOperator *bo,int line,int col,int initVal)
{
  if(!isComparisonOperator(bo))
  {
      return 0;
  }
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  static llvm::StringRef opcodeStrRef = bo->getOpcodeStr(opcode);
  const char *opcodeStr = opcodeStrRef.str().c_str();
  //printf("opcode : %s\n",opcodeStr);

  Expr *LHS = bo->getLHS();
  Expr *RHS = bo->getRHS();
  
  int val=0;
  clang::Expr::EvalResult result;
  if(RHS->isEvaluatable(TheCompInst.getASTContext()))
  {
    RHS->EvaluateAsInt(result,TheCompInst.getASTContext());
    //printf("getloop of While\n");

    clang::APValue apvalue = result.Val;
    if(apvalue.isInt())
    {
      llvm::APSInt intval = apvalue.getInt();
      //printf("its Int,in line %d col %d\n",line,col);
      
      QualType ty = RHS->getType();
      string valString = apvalue.getAsString(TheCompInst.getASTContext(),ty);
      val = atoi(valString.c_str());
      //printf("val : %d\n",val);
      //printf("aspint: %s\n",intval.toString(10).c_str());
    }
  }
  else
  {
    //if it's Unevaluable, then we should give the warning
    val=this->minNumOfLoop+1;
  }

  //printf("initval: %d , val: %d\n",initVal,val);
  int numOfLoop=0;
  //what kind of ComparisonOperator
  if(strcmp(opcodeStr,"<") == 0)
  {
    numOfLoop=val-initVal;
  }
  else if(strcmp(opcodeStr,">") == 0)
  {
    numOfLoop=initVal-val;
  }
  else if(strcmp(opcodeStr,"!=") == 0)
  {
    if(initVal<=val)
    {
      numOfLoop=val-initVal;
    }
    else
    {
      numOfLoop=initVal-val;
    }
  }
  
  return numOfLoop;
}

bool SlowMemoryChecker::isComparisonOperator(BinaryOperator *bo)
{
  clang::BinaryOperator::Opcode opcode = bo->getOpcode();
  bo->getID(TheCompInst.getASTContext());
  Stmt *stmt = cast<Stmt>(bo);
  
  if(bo->isComparisonOp() || bo->isEqualityOp())
  {
    return true;
  }
  else
  {
    return false;
  }


  return true;
}

bool SlowMemoryChecker::sameType(QualType ltype,QualType rtype)
{
    if (ltype->isPointerType())
    {
      ltype = ltype->getPointeeType();
    }
    if (rtype->isPointerType())
    {
      rtype = rtype->getPointeeType();
    }
    return ltype==rtype;
}

SlowMemoryChecker::SlowMemoryChecker()
{
    this->numOfWhileLoop=0;
    this->numOfForLoop=0;
    this->numOfDoWhileLoop=0;
    char minloop[16];
    const char *profile = "./smChecker.conf";
    const char *appname = "smchecker";
    const char *varname = "minloop";
    if(this->rc.GetProfileString(profile,appname,varname,minloop)==0)
    { 
      //printf("minloop: %s \n",minloop);
      this->minNumOfLoop=atoi(minloop);
      //printf("minNumOfLoop: %d\n",this->minNumOfLoop);
    }
    else
    {
      this->minNumOfLoop=1024;
    }
    this->ForStmtEndLine=-1;
    this->DoWhileStmtEndLine=-1;
    this->WhileStmtEndLine=-1;
    this->inForStmt=false;
    this->inWhileStmt=false;
    this->inDoWhileStmt=false;
}

bool SlowMemoryChecker::isAssignmentOpInForHead(BinaryOperator *bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;

    if(BinaryOperatorLine == condBeginLine)
    {
      LineMatch=true;
      if(BinaryOperatorCol < condBeginCol)
      {
        ColMatch=true;
      }
    }

    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      return true;
    }
    else
    {
      return false;
    }
}


bool SlowMemoryChecker::isBinaryOpratorInCond(BinaryOperator *bo)
{
    SourceLocation beginLoc = bo->getBeginLoc();

    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);
    tmp = strtok(NULL, delims);
    int BinaryOperatorCol = atoi(tmp);

    int condBeginLine = this->fscl.getBeginLine();
    int condBeginCol = this->fscl.getBeginCol();
    int condEndLine = this->fscl.getEndLine();
    int condEndCol = this->fscl.getEndCol();

    bool LineMatch=false;
    bool ColMatch=false;
    if(BinaryOperatorLine >= condBeginLine && BinaryOperatorLine <= condEndLine)
    {
      LineMatch=true;
      if(BinaryOperatorCol >= condBeginCol && BinaryOperatorCol <= condEndCol)
      {
        ColMatch=true;
      }
    }
    
    if(LineMatch == true && ColMatch == true)
    {
      //printf("BinaryOperatorLine: %d Col: %d \n",BinaryOperatorLine,BinaryOperatorCol);
      return true;
    }
    else
    {
      return false;
    }
}


<<<<<<< HEAD

=======
>>>>>>> a90567dab3ed2fdb95c8a1b3a738de5e06bb9169
void SlowMemoryChecker::printCallExprName(clang::CallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CallExpr: %s\n",funcname);
}

void SlowMemoryChecker::printCXXCallExprName(CXXOperatorCallExpr *c)
{
    FunctionDecl *fd = c->getDirectCallee();
    const char *funcname = fd->getNameAsString().c_str();
    printf("CXXCallExpr: %s\n",funcname);
}

int SlowMemoryChecker::findWhileStmtEndLine(WhileStmt *ws)
{
    SourceLocation beginLoc = ws->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = ws->getBody();
    SourceLocation endLoc = ws->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int WhileStmtEndLine = atoi(tmp);
    return WhileStmtEndLine;
}

int SlowMemoryChecker::findForStmtEndLine(ForStmt *fs)
{
    SourceLocation beginLoc = fs->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = fs->getBody();
    SourceLocation endLoc = fs->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int ForStmtEndLine = atoi(tmp);
    return ForStmtEndLine;
}

int SlowMemoryChecker::findDoWhileStmtEndLine(DoStmt *ds)
{
    SourceLocation beginLoc = ds->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    Stmt *body = ds->getBody();
    SourceLocation endLoc = ds->getEndLoc();
    string endLocString = endLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)endLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int DoWhileStmtEndLine = atoi(tmp);
    return DoWhileStmtEndLine;
}

bool SlowMemoryChecker::FindExprNameByToken(Expr *e)
{
    bool printNameByToken = false;
    SourceLocation loc = e->getBeginLoc();
    std::pair<FileID, unsigned> locInfo = SM->getDecomposedLoc(loc);
    bool invalidTemp = false;
    llvm::StringRef file = SM->getBufferData(locInfo.first, &invalidTemp);
    if (invalidTemp)
      return false;
    // tokenBegin即为loc对应源码内容的起始点
    const char *tokenBegin = file.data() + locInfo.second;

    clang::LangOptions opts;
    Lexer *lexer = new Lexer(SM->getLocForStartOfFile(locInfo.first), opts, file.begin(), tokenBegin, file.end());
    Token tok;
    lexer->LexFromRawLexer(tok); // 取得第一个tok，反复调用可以获取一段token流

    clang::IdentifierInfo *II;
    const char * tokenName = tok.getName();
    tok.setIdentifierInfo(II);

    if(tok.isAnyIdentifier() && printNameByToken)
    {
      llvm::StringRef sr = tok.getRawIdentifier();
      llvm::StringRef *sr1 = new StringRef("memcmp");
      //
      if(sr.contains(*sr1))
      {
        string tmp = sr.str();
        if(strcmp(tmp.c_str(),"my_memcmp"))
        {
          printf("tokenName : %s Iden:%s loc: %s \n"
          ,tokenName
          ,tmp.c_str()
          ,tok.getLocation().printToString(*SM).c_str());
        }
      }
    }
    return true;
}

int ForStmtCondLoc::findCondBeginLine(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string beginLocString = beginLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)beginLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condBeginLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condBeginCol = atoi(tmp);
      //printf("%d %d\n",condBeginLine,condBeginCol);
      this->condBeginLine=condBeginLine;
      return condBeginLine;
}

int ForStmtCondLoc::findCondBeginCol(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string beginLocString = beginLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)beginLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condBeginLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condBeginCol = atoi(tmp);
      //printf("%d %d\n",condBeginLine,condBeginCol);
      this->condBeginCol=condBeginCol;
      return condBeginCol;
      
}

int ForStmtCondLoc::findCondEndLine(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string EndLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)EndLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condEndLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condEndCol = atoi(tmp);
      //printf("%d %d\n",condEndLine,condEndCol);
      this->condEndLine=condEndLine;
      return condEndLine;
}

int ForStmtCondLoc::findCondEndCol(Expr *cond)
{
      // cond->getBeginLoc().dump(TheCompInst.getSourceManager());
      // cond->getEndLoc().dump(TheCompInst.getSourceManager());
      SourceLocation beginLoc = cond->getBeginLoc();
      SourceLocation endLoc = cond->getEndLoc();
      string EndLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)EndLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      int condEndLine = atoi(tmp);
      tmp = strtok(NULL, delims);
      int condEndCol = atoi(tmp);
      //printf("%d %d\n",condEndLine,condEndCol);
      this->condEndCol=condEndCol;
      return condEndCol;
}

void ForStmtCondLoc::getCondLoc(Expr *cond)
{
  
  this->findCondBeginLine(cond);
  this->findCondBeginCol(cond);
  this->findCondEndLine(cond);
  this->findCondEndCol(cond);
}

int ForStmtCondLoc::getBeginLine()
{
  return this->condBeginLine;
}

int ForStmtCondLoc::getBeginCol()
{
  return this->condBeginCol;
}

int ForStmtCondLoc::getEndLine()
{
  return this->condEndLine;
}

int ForStmtCondLoc::getEndCol()
{
  return this->condEndCol;
}

ForStmtCondLoc::ForStmtCondLoc()
{
    ;
}
