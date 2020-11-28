#pragma once
#include <fstream>
class Printer
{
/*
 * class printer
 * 作者：刘浩文
 * 打印机基类，所有Checker继承这个类
 * 类中有一个静态的ofstream
 * 保证所有类实体都是用同一个ofstream
 * 不会出现错误覆盖的情况
 */
public:
/*
 * 默认构造函数
 */
    Printer()
    {

    }
/*
 * 输出函数
 * @param:待打印的字符串
 */
    static void pprint(const std::string &s)
    {
        of << s;
    }

protected:
/*
 * 静态的ofstream文件输出流
 * 保证出现多个文件读取位置覆盖日志文件
 */
    static std::ofstream of;
};