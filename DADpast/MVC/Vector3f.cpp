// Vector3f.cpp: implementation of the CVector3f class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Vector3f.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//得到两向量的叉乘 

CVector3f Cross(CVector3f vVector1, CVector3f vVector2) 
{ 
	//定义一个容纳叉乘结果的向量 
	CVector3f vNormal; 

	//得到此向量在X轴上的投影值 
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y)); 

	//得到此向量在Y轴上的投影值 
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z)); 

	//得到此向量在Z轴上的投影值 
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x)); 

	//返回此向量 
	return vNormal; 
} 

//得到一个向量的绝对长度 
double Magnitude(CVector3f vNormal) 
{ 
	return sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) + (vNormal.z * vNormal.z) ); 
} 

//将一个向量单位化 
CVector3f Normalize(CVector3f vNormal) 
{ 
	//得到此向量的绝对长度 
	double magnitude = Magnitude(vNormal); 

	//让每个分量分别除以此长度 
	vNormal.x /= magnitude;  
	vNormal.y /= magnitude;  
	vNormal.z /= magnitude;  

	//返回此向量 
	return vNormal;  
} 

//得到一个三点决定的平面的垂直向量(经过单位化) 
CVector3f Normal(CVector3f vPolygon[]) 
{ 
	//得到两条边的向量 
	CVector3f vVector1 = vPolygon[2] - vPolygon[0]; 
	CVector3f vVector2 = vPolygon[1] - vPolygon[0]; 

	//得到这两向量的叉乘 
	CVector3f vNormal = Cross(vVector1, vVector2); 

	//单位化 
	vNormal.x = Normalize(vNormal).x;   
	vNormal.y = Normalize(vNormal).y; 
	vNormal.z = Normalize(vNormal).z; 
  
	//返回此变量 
	return vNormal;  
} 

//得到两点间的距离 
double Distance(CVector3f vPoint1, CVector3f vPoint2) 
{ 
	double distance = sqrt( (vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) + 
						(vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) + 
						(vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z) ); 

	return (double)distance; 
} 

//得到两向量的点积 

double Dot(CVector3f vVector1, CVector3f vVector2) 
{ 
	return ( (vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z) ); 
} 

//得到两向量的夹角 
double AngleBetweenVectors(CVector3f Vector1, CVector3f Vector2) 
{ 
	//得到两向量的点积 
	double dotProduct = Dot(Vector1, Vector2); 

	//得到两向量长度的乘积 
	double vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2) ; 

	//得到两向量夹角 
	double angle = acos( dotProduct / vectorsMagnitude ); 

	//返回角度值 
	return( angle ); 
}

