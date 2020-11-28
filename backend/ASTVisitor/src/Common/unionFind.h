#pragma once
#include <vector>
class UnionFind
{
/*
 * class UnionFind
 * 并查集类，在进行指针分析的时候使用
 */
private:
    std::vector<int> parent;
    int count;
    std::vector<int> rank;

public:
/*
 * 不允许默认构造和拷贝构造
 * 必须给出并查集类的大小
 */
    UnionFind() = delete;
    UnionFind(const UnionFind &) = delete;
/*
 * 给出并查集大小来初始化并查集类
 * @param: count-并查集大小
 */
    UnionFind(int count)
    {
        parent.resize(count);
        rank.resize(count);
        this->count = count;
        for (int i = 0; i < count; ++i)
        {
            parent[i] = i;
            rank[i] = 1;
        }
    }
/*
 * 析构函数
 *  
 */
    ~UnionFind()
    {
        parent.clear();
        rank.clear();
    }
/*
 *  查找+路径压缩
 *  @param 查找一个节点的父节点
 *  @return p的父节点 
 */
    int find(int p)
    {
        if (p != parent[p])
            parent[p] = find(parent[p]);
        return parent[p];
    }
/*
 *  查找p，q两个节点是不是在一个集合
 *  @param p q两个节点
 *  @return bool值，代表pq是否在一个集合
 */
    bool isConnected(int p, int q)
    {
        return find(p) == find(q);
    }
/*
 *  把p节点并入q节点所在集合
 *  @param p，q两节点
 */
    void unionElement(int p, int q)
    {
        int pRoot = find(p), qRoot = find(q);
        if (pRoot == qRoot)
            return;
        if (rank[pRoot] < rank[qRoot])
            parent[pRoot] = qRoot;
        else if (rank[qRoot] < rank[pRoot])
            parent[qRoot] = pRoot;
        else
        {
            parent[pRoot] = qRoot;
            rank[qRoot] += 1;
        }
    }
/*
 *  让p节点独立
 *  @param 需要从集合独立的节点
 */
    void reset(int p)
    {
        parent[p] = p;
    }
};