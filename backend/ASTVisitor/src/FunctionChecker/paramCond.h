/*
 * struct ParameterCond
 * 作者：刘浩文
 * 简单的记录某个参数是否会被释放或者置空的结构体
 */
struct ParameterCond
{
    bool isNulled;
    bool isFreed;
};