#include <memory>
#include <sstream>
#include <string>
#include <iostream>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "clang-c/Index.h"

#include "./PointerChecker/pointerChecker.h"
#include "./FunctionChecker/functionChecker.h"
#include "./SwitchChecker/switchChecker.h"
#include "./SpaceChecker/spaceChecker.h"
#include "./SlowMemoryChecker/slowMemoryChecker.h"
#include "./Common/errNo.h"

using namespace clang;
using namespace std;

//double bitToMb(double bits);

SourceManager *SM;
ASTContext *CTX;
int ForStmtEndLine = -1;
int WhileStmtEndLine = -1;
CompilerInstance TheCompInst;
int Pointer::numsOfPointer;
std::string curFuncName;
std::ofstream Printer::of;
unordered_map<string, EnumDecl *> EDs;
vector<SwitchLocation> switch_loc_list;
vector<Field> field_use_list;

// By implementing RecursiveASTVisitor, we can specify which AST nodes
// we're interested in by overriding relevant methods.
class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor>
{
public:
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}
  //functions
  bool VisitFunctionDecl(FunctionDecl *fd)
  {
    // cout << "FDECL\n";
    // cout << fd->getNameAsString() << '\n';
    // cout << "FPARAM NUM\n";
    int num = fd->getNumParams();
    // cout << num << '\n';
    // cout << "FPARAM\n";
    std::vector<std::string> xxparams;
    for (int i = 0; i < num; ++i)
    {
      ParmVarDecl *pvd = fd->getParamDecl(i);
      //cout << pvd->getNameAsString() << '\n';
      QualType pt = pvd->getType();
      //cout << pt.getAsString() << '\n';
      if (pt->isPointerType())
      {
        xxparams.push_back(pvd->getNameAsString());
      }
    }
    string funcName = fd->getNameAsString();
    pc.stepInFunc(funcName);
    if (xxparams.size() != 0 || funcName == "main")
    {
      //cout << "STEPIN " << fd->getNameAsString() << '\n';
      fc.stepInFunc(fd->getNameAsString(), xxparams);
      for (string s : xxparams)
      {
        //cout << "O " << s << '\n';
        Pointer *p = new Pointer(s, isVALID, true);
        pc.declPointer(p);
        //cout << "PC insert" << s << "  funcname" << fd->getNameAsString() << '\n';
      }
    }
    //cout << "STEPIN " << fd->getNameAsString() << '\n';
    return true;
  }

  bool VisitWhileStmt(WhileStmt *ws)
  {
    smchecker.WhileStmtEndLine = smchecker.findWhileStmtEndLine(ws);
    smchecker.inWhileStmt=true;
    Expr *cond = ws->getCond();
    smchecker.fscl.getCondLoc(cond);
    //cond->dump();
    return true;
  }
  bool VisitDoStmt(DoStmt *ds)
  {
    smchecker.DoWhileStmtEndLine = smchecker.findDoWhileStmtEndLine(ds);
    smchecker.inDoWhileStmt=true;
    Expr *cond = ds->getCond();
    smchecker.DoWhileCond = cond;
    smchecker.fscl.getCondLoc(cond);
    //cond->dump();
    return true;
  }
  bool VisitStmt(Stmt *s)
  {
    // Only care about For statements.
    smchecker.setFlags(s);
    if (isa<ForStmt>(s))
    {
      ForStmt *ForStatement = cast<ForStmt>(s);
      SourceLocation beginLoc = ForStatement->getBeginLoc();
      string beginLocString = beginLoc.printToString(*SM);
      Stmt *body = ForStatement->getBody();
      SourceLocation endLoc = ForStatement->getEndLoc();
      string endLocString = endLoc.printToString(*SM);
      char delims[] = ":";
      char *tmp = NULL;
      tmp = strtok((char *)endLocString.c_str(), delims);
      tmp = strtok(NULL, delims);
      ForStmtEndLine = atoi(tmp);

      ForStmtEndLine = smchecker.findForStmtEndLine(ForStatement);
      smchecker.inForStmt=true;
      smchecker.ForStmtEndLine=ForStmtEndLine;
      Expr *cond = ForStatement->getCond();
      smchecker.fscl.getCondLoc(cond);

      Stmt::child_range range = body->children();
      for (Stmt::child_iterator r = range.begin(); r != range.end(); r++)
      {
        VisitStmt(*r);
      }
      string locString = beginLoc.printToString(*SM);
      //printf("for loop detected::%s\n", locString.c_str());
    }
    else if (isa<DeclStmt>(s))
    {
      DeclStmt *ds = cast<DeclStmt>(s);
      SourceLocation beginLoc = ds->getBeginLoc();
      //Decl* dcl=ds->getDeclGroup().getSingleDecl();   //will hit assertion when ds->getDeclGroup() is not SingleDecl
      Decl *dcl = NULL;
      if (ds->getDeclGroup().isSingleDecl())
      {
        dcl = ds->getDeclGroup().getSingleDecl();
      }
      else
      {
        dcl = *(ds->getDeclGroup().begin());
      }

      if (isa<VarDecl>(dcl))
      {
        // VarDecl *vd = cast<VarDecl>(dcl);
        // QualType qt = vd->getType();
        // uint64_t tsize = vd->getASTContext().getTypeSize(vd->getType());
        // if (tsize >= WARNING_TRIGGER_VARIABLE_SIZE)
        // {
        //   string qtstr = qt.getAsString();
        //   string locString = beginLoc.printToString(*SM);
        //   stringstream ssr;
        //   cout << "Warning: variable is too big::" << locString.c_str() << ": " << qtstr << ": " << bitToMb(tsize) << "Mb" << endl;
        //   ssr << "Warning: variable is too big::" << locString.c_str() << ": " << qtstr << ": " << bitToMb(tsize) << "Mb" << endl;
        //   pc.pprint(ssr.str());
        // }
        VarDecl *vd = cast<VarDecl>(dcl);
        smchecker.isVarDeclInForHead(vd);
        spchecker.bigVariableCheck(vd);

        QualType qt = vd->getType();
        if (qt->isPointerType())
        {
          string pname = vd->getNameAsString();
          //std::cout << pname << '\n';
          Pointer *p;
          if (vd->hasInit())
          {
            Expr *vgi = vd->getInit()->IgnoreImpCasts();
            //vgi->dumpColor();
            if (isa<CXXNewExpr>(vgi))
            {
              p = new Pointer(pname, isVALID, true);
              pc.declPointer(p);
            }
            else if (isa<DeclRefExpr>(vgi))
            {
              DeclRefExpr *dre = cast<DeclRefExpr>(vgi);
              std::string rpname = dre->getNameInfo().getAsString();
              Pointer *rhs = pc.getPointerByName(rpname);
              p = new Pointer(pname, isVALID);
              pc.declPointer(p);
              if (rhs)
              {
                pc.assignPointer(*p, *rhs);
                //std::cout << "INIT ASSIGN" << p->getName() << " " << rhs->getName() << std::endl;
              }
            }
            else
            {
              p = new Pointer(pname, isVALID);
              pc.declPointer(p);
            }
          }
          else
          {
            p = new Pointer(pname);
            pc.declPointer(p);
          }
          pc.declPointer(p);
          //p->dump();
          //std::cout << "decl " << p->getName() << '\n';
        }
      }
    }
    return true;
  }
  bool VisitCallExpr(CallExpr *ce)
  {
    DeclRefExpr *dre = cast<DeclRefExpr>(ce->getCallee()->IgnoreImpCasts());
    string funcName = dre->getNameInfo().getAsString();
    int args = ce->getNumArgs();
    int pcount = 0;
    for (int i = 0; i < args; ++i)
    {
      Expr *arg = ce->getArg(i)->IgnoreImpCasts();
      //arg->dumpColor();
      if (arg->getType()->isPointerType())
      {
        //cout << "!" << args << endl;
        DeclRefExpr *pdre = cast<DeclRefExpr>(arg);
        string pointerParamName = pdre->getNameInfo().getAsString();
        Pointer *p = pc.getPointerByName(pointerParamName);
        //cout << funcName;
        if (fc.isFuncParamNullByFuncNameAndIndex(funcName, pcount))
        {
          p->setState(isNULL);
        }
        else if (fc.isFuncParamFreeByFuncNameAndIndex(funcName, pcount))
        {
          bool success = false;
          pc.freePointer(*p, success, pdre->getBeginLoc().printToString(*SM));
          if (!success)
          {
            SourceLocation beginLoc = ce->getBeginLoc();
            string beginLocString = beginLoc.printToString(*SM);
            std::cout << " ::" << beginLocString << '\n';
            stringstream ssr;
            ssr << " ::" << beginLocString << '\n';
            pc.pprint(ssr.str());
          }
        }
        pcount++;
      }
    }
    //fc.isFuncParamFreeByFuncNameAndIndex(funcName,);
    return true;
  }
  bool VisitCXXNullPtrLiteralExpr(CXXNullPtrLiteralExpr *cnpe)
  {
    //cout << "NULL\n";
    return true;
  }
  bool VisitBinaryOperator(BinaryOperator *stmt)
  {
    //stmt->dump();
    SourceLocation beginLoc = stmt->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    char delims[] = ":";
    char *tmp = NULL;
    tmp = strtok((char *)beginLocString.c_str(), delims);
    tmp = strtok(NULL, delims);
    int BinaryOperatorLine = atoi(tmp);

    smchecker.BinaryOperationCheck(stmt,BinaryOperatorLine,ForStmtEndLine,WhileStmtEndLine);

    if (BinaryOperatorLine > ForStmtEndLine)
    {
      //pointer assign?
      Expr *lhs = stmt->getLHS()->IgnoreImpCasts();
      Expr *rhs = stmt->getRHS()->IgnoreImpCasts();
      //rhs->dumpColor();
      if (lhs->getType()->isPointerType() &&
          (rhs->getType()->isPointerType() ||
           isa<IntegerLiteral>(rhs->IgnoreImpCasts())))
        ;
      //std::cout<<"Pointer assign"<<std::endl;
      else
        return true;
      std::string lname, rname;
      DeclRefExpr *ldre, *rdre;
      //TODO
      if (isa<DeclRefExpr>(lhs))
        ldre = cast<DeclRefExpr>(lhs);
      else
        return true;
      if (isa<DeclRefExpr>(rhs))
        rdre = cast<DeclRefExpr>(rhs);
      else
      {
        if (isa<IntegerLiteral>(rhs->IgnoreImpCasts()))
        {
          IntegerLiteral *iL = cast<IntegerLiteral>(rhs->IgnoreImpCasts());
          if (iL->getValue().toString(10, true) != "0")
            return true;
          //cout << "RHS NULLPTR\n";
          lname = ldre->getNameInfo().getAsString();
          Pointer *lp;
          lp = pc.getPointerByName(lname);
          lp->setState(isNULL);
          if (pc.getFuncName() != "main")
          {
            //cout<<"PFH NULL "<<lname<<'\n';
            if (fc.isProcFuncHasParam(lname))
            {
              //cout << "PFH NULL " << lname << '\n';
              fc.setProcFuncPtrNullByName(lname);
            }
          }
        }
        return true;
      }
      lname = ldre->getNameInfo().getAsString();
      rname = rdre->getNameInfo().getAsString();
      Pointer *lp, *rp;
      lp = pc.getPointerByName(lname);
      rp = pc.getPointerByName(rname);
      pc.assignPointer(*lp, *rp);
      //std::cout << "ASSIGN POINTER:" << lname << " " << rname << std::endl;
      return true;
    }

    return true;
  }
  bool VisitArraySubscriptExpr(ArraySubscriptExpr *ase)
  {
    SourceLocation beginLoc = ase->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    //std::cout<<"inarr\n";
    Expr *lhs = ase->getBase()->IgnoreImpCasts();
    Expr *rhs = ase->getIdx()->IgnoreImpCasts();
    int arrMaxSize = -1;
    string arrName;
    if (isa<DeclRefExpr>(lhs))
    {
      DeclRefExpr *ldre = cast<DeclRefExpr>(lhs);
      arrName = ldre->getNameInfo().getAsString();
      Pointer *p = pc.getPointerByName(arrName);
      if (p)
      {
        SourceLocation beginLoc = lhs->getBeginLoc();
        string beginLocString = beginLoc.printToString(*SM);
        pc.nullDerefCheck(*p, beginLocString);
        return true;
      }
      QualType qt = ldre->getType();
      std::string qas = qt.getAsString();
      //cout<<qt.getAsString()<<'\n';
      //TODO only 1-d
      int lbrack, rbrack;
      lbrack = qas.find_first_of('[');
      rbrack = qas.find_first_of(']');
      //arrMaxSize=
      //std::cout<<qas.substr(lbrack+1,rbrack-lbrack-1)<<endl;
      arrMaxSize = std::stoi(qas.substr(lbrack + 1, rbrack - lbrack - 1));
      //std::cout<<"AMS"<<arrMaxSize<<endl;
    }
    if (isa<IntegerLiteral>(rhs))
    {
      IntegerLiteral *il = cast<IntegerLiteral>(rhs);
      uint64_t indexSize = il->getValue().getLimitedValue();
      //std::cout<<"IDS "<<indexsize<<endl;
      if (arrMaxSize == -1)
        return true;
      if (arrMaxSize <= indexSize)
      {
        stringstream ssr;
        #ifdef OOP
        cout << "Warning: Array out-of-bound access :";
        cout << arrName;
        cout << ":Try to access index " << indexSize;
        cout << " while the max size is:" << arrMaxSize;
        cout << ":" << beginLocString << endl;

        ssr << "Warning: Array out-of-bound access :";
        ssr << arrName;
        ssr << ":Try to access index " << indexSize;
        ssr << " while the max size is:" << arrMaxSize;
        ssr << ":" << beginLocString << endl;
        pc.pprint(ssr.str());
        #else
        string mes;
        mes=beginLocString+':'+ static_cast<char>('0' +ArrayOutOfBound)+':'+arrName+'\n';
        cout<<mes;
        pc.pprint(mes);
        #endif
      }
    }
    return true;
  }
  bool VisitUnaryOperator(UnaryOperator *u)
  {
    SourceLocation beginLoc = u->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    //std::cout<<"Unary OP"<<'\n';
    if (isa<DeclRefExpr>(u->getSubExpr()->IgnoreImpCasts()))
    {
      DeclRefExpr *dre = cast<DeclRefExpr>(u->getSubExpr()->IgnoreImpCasts());
      std::string opstr = u->getOpcodeStr(u->getOpcode()).str();
      if (opstr.find('*') != opstr.npos)
      {
        std::string pname = dre->getNameInfo().getAsString();
        //if(pc.getFuncName()!="main") return true;
        //cout << "Pnullderef" << pname << '\n';
        if (pc.nullDerefCheck(*(pc.getPointerByName(pname)), dre->getBeginLoc().printToString(*SM)) < 0)
        {
#ifdef OOP
          std::cout << " ::" << beginLocString << '\n';
          stringstream ssr;
          ssr << " ::" << beginLocString << '\n';
          pc.pprint(ssr.str());
#endif
        }
      }
    }
    //std::cout<<u->getOpcodeStr(u->getOpcode()).str()<<'\n';
    return true;
  }
  bool VisitCXXDeleteExpr(CXXDeleteExpr *cde)
  {
    //std::cout<<"CXXDEL"<<'\n';
    // if (pc.getFuncName() != "main")
    //   return true;
    SourceLocation beginLoc = cde->getBeginLoc();
    string beginLocString = beginLoc.printToString(*SM);
    DeclRefExpr *dre = cast<DeclRefExpr>(cde->getArgument()->IgnoreImpCasts());
    //dre->dumpColor();
    Pointer *p2free = pc.getPointerByName(dre->getNameInfo().getAsString());
    //p2free->dump();
    bool success = false;
    pc.freePointer(*p2free, success, beginLocString);
    if (!success)
    {
#ifdef OOP
      std::cout << " ::" << beginLocString << '\n';
      stringstream ssr;
      ssr << " ::" << beginLocString << '\n';
      pc.pprint(ssr.str());
#endif
    }
    else if (pc.getFuncName() != "main")
    {
      if (fc.isProcFuncHasParam(p2free->getName()))
      {
        //cout << "PFH FREE " << p2free->getName() << '\n';
        fc.setProcFuncPtrFreeByName(p2free->getName());
      }
    }
    return true;
  }
  bool VisitSwitchStmt(SwitchStmt* s)
  {
    SourceLocation beginLoc = s->getBeginLoc();
    string stringLoc = beginLoc.printToString(*SM);
    size_t pos_of_first = stringLoc.find_first_of(':');
    size_t pos_of_second = stringLoc.find_last_of(':');

    string filename = stringLoc.substr(0, pos_of_first);
    string row = stringLoc.substr(pos_of_first + 1, pos_of_second - pos_of_first - 1);
    string col = stringLoc.substr(pos_of_second + 1);
    SwitchLocation switch_loc("", filename, atoi(row.c_str()), atoi(col.c_str()));
    switch_loc_list.push_back(switch_loc);

    if (isa<EnumType>(s->getCond()->IgnoreImpCasts()->getType()))
    {
      swchecker.enumIncompleteCheck(s);
    }
    else
    {
      swchecker.typeMismatchCheck(s); // checker
    }
    return true;
  }

  bool VisitEnumDecl(EnumDecl *ed)
  {
    string enumName = ed->getNameAsString();
    EDs[enumName] = ed;
    return true;
  }
  bool VisitCXXRecordDecl(CXXRecordDecl *crd)
  {
    if (crd->isStruct())
    {
      for (auto const &field : crd->fields())
      {
        spchecker.bigFieldCheck(field); // checker

        string field_type = field->getType().getAsString();
        string field_name = field->getNameAsString();
        SourceLocation beginLoc = field->getBeginLoc();
        string field_location = beginLoc.printToString(*SM);
        Field f(field_type, field_name, field_location, false);
        field_use_list.push_back(f);
      }
    }
    return true;
  }

  bool VisitMemberExpr(MemberExpr *me)
  {

    string member_type = me->getType().getAsString();
    string member_name = me->getMemberNameInfo().getAsString();
    for (auto &field : field_use_list)
    {
      if (
          field.type.compare(member_type) == 0 &&
          field.name.compare(member_name) == 0)
      {
        field.isUsed = true;
      }
    }
    return true;
  }

private:
  Rewriter &TheRewriter;
  PointerChecker pc;
  FuncChecker fc;
  SwitchChecker swchecker;
  SpaceChecker spchecker;
  SlowMemoryChecker smchecker;
};

class MyASTConsumer : public ASTConsumer
{
public:
  MyASTConsumer(Rewriter &R) : Visitor(R) {}

  virtual bool HandleTopLevelDecl(DeclGroupRef DR)
  {
    for (DeclGroupRef::iterator b = DR.begin(), e = DR.end(); b != e; ++b)
      // Traverse the declaration using my AST visitor.
      Visitor.TraverseDecl(*b);
    return true;
  }

private:
  MyASTVisitor Visitor;
};



int main(int argc, char *argv[])
{

  if (argc != 2)
  {
    llvm::errs() << "Usage: myASTVisitor <filename>\n";
    return 1;
  }

  TheCompInst.createDiagnostics();

  LangOptions &lo = TheCompInst.getLangOpts();
  lo.CPlusPlus = 1;

  auto TO = std::make_shared<TargetOptions>();
  TO->Triple = llvm::sys::getDefaultTargetTriple();
  TargetInfo *TI =
      TargetInfo::CreateTargetInfo(TheCompInst.getDiagnostics(), TO);
  TheCompInst.setTarget(TI);

  TheCompInst.createFileManager();
  FileManager &FileMgr = TheCompInst.getFileManager();
  TheCompInst.createSourceManager(FileMgr);
  SourceManager &SourceMgr = TheCompInst.getSourceManager();
  SM = &SourceMgr;
  TheCompInst.createPreprocessor(TU_Module);
  TheCompInst.createASTContext();

  Rewriter TheRewriter;

  TheRewriter.setSourceMgr(SourceMgr, TheCompInst.getLangOpts());

  // Set the main file
  const FileEntry *FileIn = FileMgr.getFile(argv[1]);
  SourceMgr.setMainFileID(
      SourceMgr.createFileID(FileIn, SourceLocation(), SrcMgr::C_User));
  TheCompInst.getDiagnosticClient().BeginSourceFile(
      TheCompInst.getLangOpts(), &TheCompInst.getPreprocessor());

  MyASTConsumer TheConsumer(TheRewriter);

  ASTContext &context = TheCompInst.getASTContext();
  CTX = &context;
  // Parse the file to AST, registering our consumer as the AST consumer.
  ParseAST(TheCompInst.getPreprocessor(), &TheConsumer,
           TheCompInst.getASTContext());

  // const RewriteBuffer *RewriteBuf =
  //     TheRewriter.getRewriteBufferFor(SourceMgr.getMainFileID());
  // llvm::outs() << std::string(RewriteBuf->begin(), RewriteBuf->end());

  SwitchChecker::floatAndStringCheck(argv[1]);
  SpaceChecker::unusedFieldCheck();

  return 0;
}