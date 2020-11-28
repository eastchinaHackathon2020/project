#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "paramCond.h"
using PStateMap = std::map<std::string, int>;
class SPAFunc
{
/*  class SPAFunc:
 *  作者：刘浩文
 *  这个类表示了指针处理和函数调用结合的时候
 *  记录一些函数是否会对作为参数的指针进行操作的类
 */
public:
/*
 * 逻辑上不允许产生一个没有名字等的实例。 
 * 也没有必要利用拷贝构造函数，他们做禁用处理
 */
    SPAFunc() = delete;
    SPAFunc(const SPAFunc &sf) = delete;
    
    /*
     * 显示声明的构造函数
     * 需要给出函数名，以及指针参数的名字和个数
     * @param gFuncName 函数名
     * @param paramNames 存放指针变量的名字的vector
     */
    explicit SPAFunc(const std::string &gfuncName,
                     std::vector<std::string> paramNames) : funcName(gfuncName)
    {
        ParameterCond initPC;
        initPC.isFreed = initPC.isNulled = false;
        size_t ptrParamNum = paramNames.size();
        for (int i = 0; i != ptrParamNum; ++i)
        {
            pointParamConds.push_back(initPC);
            pointParamMap[paramNames.at(i)] = i;
        }
    }
    /*
     * 返回第n个指针是否在函数内部被释放
     * @param nTH 第n个指针参数
     * @return bool值
     */
    bool isNthParamFreed(int nTH)
    {
        return pointParamConds.at(nTH).isFreed;
    }
    /*
     * 返回第n个指针是否在函数内部被置为null
     * @param nTH 第n个指针参数
     * @return bool值
     */
    bool isNthParamNulled(int nTH)
    {
        return pointParamConds.at(nTH).isNulled;
    }
    /* 
     * 通过参数名释放、置空第n个指针
     * 为了方便起见使用名字而不用指针顺序，内部维护一个map表示这种对应关系
     * @param name 需要释放、置空的指针名
     */
    void freePtrByName(std::string name)
    {
        if (pointParamMap.count(name) < 1)
        {
            std::cout << "ERR Func has no param name " << name;
            exit(-1);
        }
        pointParamConds.at(pointParamMap[name]).isFreed = true;
    }
    void nullPtrByName(std::string name)
    {
        if (pointParamMap.count(name) < 1)
        {
            std::cout << "ERR Func has no param name " << name;
            exit(-1);
        }
        pointParamConds.at(pointParamMap[name]).isNulled = true;
    }
    /* 
     * 查找某个变量名是否是这个函数的参数
     * 为了方便起见使用名字而不用指针顺序，内部维护一个map表示这种对应关系
     * @param name 需要查找的变量名
     */
    bool funcHasParam(std::string name)
    {
        bool ret=(pointParamMap.count(name) >= 1);
        return ret;
    }
private:
    std::string funcName;
    PStateMap pointParamMap;
    std::vector<ParameterCond> pointParamConds;
};