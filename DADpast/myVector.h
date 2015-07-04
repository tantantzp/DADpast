#ifndef myVector_H
#define myVector_H

#include <iostream>

using namespace std;

#define DEFAULT_CAPACITY 10

template<typename T> class myVector
{
public:
    int size;                                 //ʵ�ʹ�ģ
    int capacity;                             //����
    T* elem;                                  //������
protected:   
    void expand();                            //ָ������
    void shrink();                            //ѹ��
	  
public:
    void copy(T* const A,int lo,int hi);      //�������临��
    myVector(int cap=DEFAULT_CAPACITY){elem=new T[capacity=cap];size=0;}//Ĭ�Ϲ��캯��

    myVector(T* const A,int lo,int hi){copy(A,lo,hi);}//�������临��

    myVector(T* const A,int n){copy(A,0,n);}    //�������帴��

    myVector(myVector<T> const& V,int lo,int hi){copy(V.elem,lo,hi);} //�������临��

    myVector(myVector<T>& V){copy(V.elem,0,V.getSize());}

    ~myVector(){delete[] elem;}

    int getSize(){return size;}                  //��ģ
    bool empty(){return size<=0;}             //�п�

    int insert(int r,T const& e);             //����Ϊr������Ԫ��
    int insert(T const& e) {return insert(size,e);} //������ĩ�˲���
    int remove(int lo,int hi);                //ɾ��[lo,hi)֮���Ԫ��
    T remove(int r);                          //ɾ����Ϊr��Ԫ��
    void output(int lo,int hi);
    void output();

    int find(T const& e) const;               //�����������

    T& operator[](int r)const;                //�����±������
    myVector<T>& operator=(myVector<T>);   //���ظ��Ʋ�����

    template<typename VST> void traverse(VST&);

};


#include "myVector_implenentation.h"

#endif 
