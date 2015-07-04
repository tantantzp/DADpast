#ifndef myVector_H
#define myVector_H

#include <iostream>

using namespace std;

#define DEFAULT_CAPACITY 10

template<typename T> class myVector
{
public:
    int size;                                 //实际规模
    int capacity;                             //容量
    T* elem;                                  //数据区
protected:   
    void expand();                            //指数扩容
    void shrink();                            //压缩
	  
public:
    void copy(T* const A,int lo,int hi);      //数组区间复制
    myVector(int cap=DEFAULT_CAPACITY){elem=new T[capacity=cap];size=0;}//默认构造函数

    myVector(T* const A,int lo,int hi){copy(A,lo,hi);}//数组区间复制

    myVector(T* const A,int n){copy(A,0,n);}    //数组整体复制

    myVector(myVector<T> const& V,int lo,int hi){copy(V.elem,lo,hi);} //向量区间复制

    myVector(myVector<T>& V){copy(V.elem,0,V.getSize());}

    ~myVector(){delete[] elem;}

    int getSize(){return size;}                  //规模
    bool empty(){return size<=0;}             //判空

    int insert(int r,T const& e);             //在秩为r处插入元素
    int insert(T const& e) {return insert(size,e);} //在数组末端插入
    int remove(int lo,int hi);                //删除[lo,hi)之间的元素
    T remove(int r);                          //删除秩为r的元素
    void output(int lo,int hi);
    void output();

    int find(T const& e) const;               //无序整体查找

    T& operator[](int r)const;                //重载下标操作符
    myVector<T>& operator=(myVector<T>);   //重载复制操作符

    template<typename VST> void traverse(VST&);

};


#include "myVector_implenentation.h"

#endif 
