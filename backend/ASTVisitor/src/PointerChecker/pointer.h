#pragma once
#include <string>
#include <iostream>
#include <utility>

/*
 * 常量值表示某个指针的状态：
 * 1-空指针
 * 2-可以合法取地址的指针
 * 3-未定义的指针（如未初始化）
 * 4-已经被释放的指针
 */
const int isNULL = 1; 
const int isVALID = 2; 
const int isUNPREDICTABLE = 3;
const int isFREED = 4;
class Pointer
{
/*
 * class Pointer
 * 作者：刘浩文
 * 表示指针的类，name表示指针名
 * isnewed表示这个指针是否是通过new关键字产生的
 * state表示指针的当前状态
 * id表示这个指针的标识数字（一般是被创造的次序）
 */
public:
    Pointer() = default;
    Pointer(const Pointer& p) = default;
/*
 * 静态内容：维护指针总量
 */
    static int numsOfPointer;
    static void numsAdd()
    {
        ++numsOfPointer;
    }
/*
 * 显式构造函数，一般指针都是通过这个函数创造的
 * @param name 指针名（必需的参数）
 * @param state 指定指针的状态
 * @param nw 指定指针是否是通过new产生的
 */
    explicit Pointer(std::string name):
    _name(std::move(name)),
    _state(isUNPREDICTABLE),
    _id(numsOfPointer),
    _isnewed(false)
    {
        numsAdd();
    }
    Pointer(std::string name,const int &ps):
    _name(std::move(name)),
    _state(ps),
    _id(numsOfPointer),
    _isnewed(false)
    {
        numsAdd();
    }
    Pointer(std::string name,const int &ps,const bool &nw):
    _name(std::move(name)),
    _state(ps),
    _id(numsOfPointer),
    _isnewed(nw)
    {
        numsAdd();
    }
    int getState() const
    {
        return _state;
    }
    int getid() const
    {
        return _id;
    }
    bool getNew() const
    {
        return _isnewed;
    }
    std::string getName() const
    {
        return _name;
    }
    int setState(int newState)
    {
        if(newState >=1 && newState <=4) _state=newState;
        else return -1;
        return 1;
    }
    void setisNew(bool b)
    {
        _isnewed=b;
    }
/*
 * 操作符重载：根据id判断是不是相同的指针
 */
    bool operator==(Pointer& rhs) const {return this->_id==rhs.getid();}
    bool operator!=(Pointer& rhs) const {return !(*this==rhs);}
/*
 * dump函数
 * 输出指针的详细信息，调试用
 */
    void dump()
    {
        std::cout<<"___POINTER DUMP____"<<std::endl;
        std::string strstate;
        switch(this->_state)
        {
            case 1:strstate="isNULL";break;
            case 2:strstate="isVALID";break;
            case 3:strstate="isUNP";break;
            case 4:strstate="isFREED";break;
            default:break;
        }
        std::cout<<"P name:"<<_name<<" \\P state:"<<strstate<<" \\P id:"<<_id;
        if(_isnewed) std::cout<<"  NEWED";
        std::cout<<'\n';
    }
private:
    std::string _name;
    bool _isnewed;
    int _state;
    int _id;
};