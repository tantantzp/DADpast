// Vector3f.cpp: implementation of the CVector3f class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector3f.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//�õ��������Ĳ�� 

CVector3f Cross(CVector3f vVector1, CVector3f vVector2) 
{ 
	//����һ�����ɲ�˽�������� 
	CVector3f vNormal; 

	//�õ���������X���ϵ�ͶӰֵ 
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y)); 

	//�õ���������Y���ϵ�ͶӰֵ 
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z)); 

	//�õ���������Z���ϵ�ͶӰֵ 
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x)); 

	//���ش����� 
	return vNormal; 
} 

//�õ�һ�������ľ��Գ��� 
double Magnitude(CVector3f vNormal) 
{ 
	return sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) ); 
} 

//��һ��������λ�� 
CVector3f Normalize(CVector3f vNormal) 
{ 
	//�õ��������ľ��Գ��� 
	double magnitude = Magnitude(vNormal); 

	//��ÿ�������ֱ���Դ˳��� 
	vNormal.x /= magnitude;  
	vNormal.y /= magnitude;  
	vNormal.z /= magnitude;  

	//���ش����� 
	return vNormal;  
} 

//�õ�һ�����������ƽ��Ĵ�ֱ����(������λ��) 
CVector3f Normal(CVector3f vPolygon[]) 
{ 
	//�õ������ߵ����� 
	CVector3f vVector1 = vPolygon[2] - vPolygon[0]; 
	CVector3f vVector2 = vPolygon[1] - vPolygon[0]; 

	//�õ����������Ĳ�� 
	CVector3f vNormal = Cross(vVector1, vVector2); 

	//��λ�� 
	vNormal.x = Normalize(vNormal).x;   
	vNormal.y = Normalize(vNormal).y; 
	vNormal.z = Normalize(vNormal).z; 
  
	//���ش˱��� 
	return vNormal;  
} 

//�õ������ľ��� 
double Distance(CVector3f vPoint1, CVector3f vPoint2) 
{ 
	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) + 
						(vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) + 
						(vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) ); 

	return (double)distance; 
} 

//�õ��������ĵ�� 

double Dot(CVector3f vVector1, CVector3f vVector2) 
{ 
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) ); 
} 

//�õ��������ļн� 
double AngleBetweenVectors(CVector3f Vector1, CVector3f Vector2) 
{ 
	//�õ��������ĵ�� 
	double dotProduct = Dot(Vector1, Vector2); 

	//�õ����������ȵĳ˻� 
	double vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ; 

	//�õ��������н� 
	double angle = acos( dotProduct / vectorsMagnitude ); 

	//���ؽǶ�ֵ 
	return( angle ); 
}

