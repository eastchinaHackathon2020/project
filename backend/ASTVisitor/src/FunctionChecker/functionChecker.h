#include "func.h"
using FCMap = std::map<std::string, SPAFunc *>;
class FuncChecker
{
    /* 
     * class FuncChecker
     * 作者：刘浩文
     * 配合spafunc进行过程间函数分析的类
     * 内部维护一个指向正在定义的函数的指针，它的名字以及一个名字-函数指针的KVmap
     */
public:
    /*
     * 构造函数
     */
    FuncChecker();
    /*
     * 拷贝构造函数不符合逻辑也没有实际的用途，通知编译器禁用
     */
    FuncChecker(const FuncChecker &fc) = delete;
    /*
     * AST遍历进入一个函数，更换当前的函数名，并且KVmap新增内容
     * @param funcName 新函数的名称
     * @param pm 这个函数的指针参数列表
     */
    void stepInFunc(std::string funcName, std::vector<std::string> pm);
    /*
     * 给当前正在处理的函数的某个指针参数设置为null或已释放
     * @param pname 待处理的指针参数的名字
     */
    void setProcFuncPtrFreeByName(std::string pname);

    void setProcFuncPtrNullByName(std::string pname);
    /*
     * 确认在其他函数调用时，确定某个参数位置是不是会在这个函数内部被置为null或被释放
     * @param fname 调用函数的名字
     * @param index 第几个参数
     */
    bool isFuncParamNullByFuncNameAndIndex(std::string fname, int index);

    bool isFuncParamFreeByFuncNameAndIndex(std::string fname, int index);
    /*
     * 查看正在处理的函数是不是有符合这个名字的参数
     * @param name 查询的参数名字
     */
    bool isProcFuncHasParam(std::string name);

private:
    SPAFunc *processingFunc;
    std::string curFuncName;
    FCMap fcMap;
};