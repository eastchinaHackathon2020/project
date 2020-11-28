#include <map>
#include "pointer.h"
#include "printer.h"
#include "unionFind.h"
const int UF_CAPACITY=100;
class PointerChecker:public Printer
{
public:
    PointerChecker():UF(UF_CAPACITY)
    {
        if(!of.is_open()) of.open("warn.dat",std::ios::out|std::ios::trunc);
        pMap.clear();
    }
    PointerChecker(const PointerChecker& pc) = delete;
    int nullDerefCheck(Pointer p2deref);
    int freeCheck(Pointer p2free);
    void declPointer(Pointer* p2decl);
    Pointer* getPointerByName(const std::string& s);
    void assignPointer(Pointer lhs,Pointer rhs);
    void freePointer(Pointer p2free,bool &success);
private:
    UnionFind UF;
    std::map<int,Pointer*> pMap;
    std::map<std::string,Pointer*> pMapByName;
};