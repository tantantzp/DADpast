// Vector3f.h: interface for the CVector3f class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VECTOR_3F
#define VECTOR_3F


#include <math.h>


class CVector3f  
{
public: 
	//�޲ι��캯�� 
	CVector3f() 
	{ 
		x = y = z = 0; 
	}; 
	//�вι��캯�� 
	CVector3f(const double inx,const double iny,const double inz) 
	{ 
		//�ֱ𽫲�����ֵ��������Ա���� 
		x = inx; 
		y = iny; 
		z = inz;  
	}; 
	//�������� 
	~CVector3f(){}; 
  
	//����=������,ʵ�������������ĸ�ֵ 
	CVector3f& operator=(const CVector3f& inVet) 
	{ 
		x = inVet.x; 
		y = inVet.y; 
		z = inVet.z; 
		return *this; 
	}; 

	//����+������,ʵ����������������� 
	CVector3f operator+(CVector3f vVector) 
	{ 
		//������ӵĽ�� 
		return CVector3f(vVector.x + x, vVector.y + y, vVector.z + z); 
	} 
	CVector3f operator+=(CVector3f vVector)
	{
		return CVector3f(vVector.x + x, vVector.y + y, vVector.z + z); 
	}

	//����-������,ʵ����������������� 
	CVector3f operator-(CVector3f vVector) 
	{ 
		//��������Ľ�� 
		return CVector3f(x - vVector.x, y - vVector.y, z - vVector.z); 
	} 

	//����*������,ʵ��һ������������һ���������ĳ˷� 
	CVector3f operator*(double num) 
	{ 
		//���������˵����� 
		return CVector3f(x * num, y * num, z * num); 
	} 

	//����/������,ʵ��һ������������һ���������ĳ��� 
	CVector3f operator/(double num) 
	{ 
		//���������˵����� 
		return CVector3f(x / num, y / num, z / num); 
	} 

	//����==������
	bool operator==(CVector3f vVector)
	{
		if (x==vVector.x && y==vVector.y && z==vVector.z)
			return true;
		else 
			return false;
	}
  
	//������x����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	void RotateX(double theta) 
	{
		double sintheta = sin(theta);
		double costheta = cos(theta);
		double sin_beta, cos_bata; 
		sin_beta = z * costheta + y * sintheta; 
		cos_bata = y * costheta - z * sintheta; 
		z = sin_beta;
		y = cos_bata; 
	}; 

	//������y����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	void RotateY(double theta) 
	{ 
		double sintheta = sin(theta);
		double costheta = cos(theta);
		double sin_beta, cos_bata; 
		sin_beta = z * costheta + x * sintheta; 
		cos_bata = x * costheta - z * sintheta;
		z = sin_beta;
		x = cos_bata; 
	}; 

	//������z����ת,����sinthetaΪ��ת�Ƕȵ�����ֵ,����costhetaΪ��ת�Ƕȵ�����ֵ 
	void RotateZ(double theta) 
	{ 
		double sintheta = sin(theta);
		double costheta = cos(theta);
		double sin_beta, cos_bata; 
		sin_beta = y * costheta + x * sintheta; 
		cos_bata = x * costheta - y * sintheta; 
		y = sin_beta;
		x = cos_bata; 
	}; 
  
	//����һ������,����scaleΪ���ŵı��� 
	void Zoom(double scale) 
	{ 
		x *= scale; 
		y *= scale; 
		z *= scale; 
	}; 
  
	//ƽ��һ������ 
	void Move(CVector3f inVect) 
	{ 
		x += inVect.x; 
		y += inVect.y; 
		z += inVect.z; 
	}; 

public:  
	double x;//��Ա����x,������x���ϵķ��� 
	double y;//��Ա����y,������y���ϵķ��� 
	double z;//��Ա����z,������z���ϵķ��� 

};

//�õ��������Ĳ�� 
CVector3f Cross(CVector3f vVector1, CVector3f vVector2); 

//�õ�һ�������ľ��Գ��� 
double Magnitude(CVector3f vNormal); 

//��һ��������λ�� 
CVector3f Normalize(CVector3f vNormal); 

//�õ�һ�����������ƽ��Ĵ�ֱ����(������λ��) 
CVector3f Normal(CVector3f vPolygon[]); 

//�õ��ռ�������ľ��� 
double Distance(CVector3f vPoint1, CVector3f vPoint2); 

//�õ��������ĵ�� 
double Dot(CVector3f vVector1, CVector3f vVector2); 

//�õ��������ļн� 
double AngleBetweenVectors(CVector3f Vector1, CVector3f Vector2); 

#endif // !defined(AFX_VECTOR3F_H__464A32D5_CD8B_410F_B4D4_DECAA91AAC91__INCLUDED_)
