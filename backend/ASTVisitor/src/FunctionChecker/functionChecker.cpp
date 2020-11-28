#include "functionChecker.h"
FuncChecker::FuncChecker()
{
    fcMap.clear();
    curFuncName = "";
    processingFunc = nullptr;
}
void FuncChecker::stepInFunc(std::string funcName, std::vector<std::string> pm)
{
    if (processingFunc == nullptr) //first function
    {
        processingFunc = new SPAFunc(funcName, pm);
    }
    else
    {
        fcMap[curFuncName] = processingFunc;
        processingFunc = new SPAFunc(funcName, pm);
    }
    curFuncName = funcName;
}
void FuncChecker::setProcFuncPtrFreeByName(std::string pname)
{
    processingFunc->freePtrByName(pname);
}
void FuncChecker::setProcFuncPtrNullByName(std::string pname)
{
    processingFunc->nullPtrByName(pname);
}
bool FuncChecker::isFuncParamFreeByFuncNameAndIndex(std::string fname, int index)
{
    if (fcMap.count(fname) < 1)
    {
        std::cout << "ERR calling non exist function\n";
        exit(-1);
    }
    SPAFunc *f = fcMap[fname];
    return f->isNthParamFreed(index);
}
bool FuncChecker::isFuncParamNullByFuncNameAndIndex(std::string fname, int index)
{
    if (fcMap.count(fname) < 1)
    {
        std::cout << "ERR calling non exist function\n";
        exit(-1);
    }
    SPAFunc *f = fcMap[fname];
    return f->isNthParamNulled(index);
}
bool FuncChecker::isProcFuncHasParam(std::string name)
{
    return processingFunc->funcHasParam(name);
}