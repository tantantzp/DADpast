#pragma once

#include <cmath>
#include <cfloat>
#include <iostream>

namespace MATH
{

	// _________________________ vector of dimensional 2 _______________
	template <typename T> 
	class CVector2
	{
	public:
		CVector2 ()											// default constructor
		{ x = y =  T(0);}


		CVector2 ( const T& x, const T& y)		// normal constructor
		{
			this->x = x;
			this->y = y;
		}

		CVector2 ( const CVector2& v)
		{
			this->x = v.x;
			this->y = v.y;
		}

		~CVector2 ( )
		{}

		T operator & ( CVector2& v ) const					// dot product
		{
			return this->x*v.x + this->y*v.y;
		}

		CVector2 operator + ( CVector2& v ) const			// add
		{
			return CVector2(this->x+v.x, this->y+v.y);
		}

		CVector2 operator + ( const CVector2& v ) const			// add
		{
			return CVector2(this->x+v.x, this->y+v.y);
		}

		CVector2 operator - ( const CVector2& v ) const			// subtraction
		{
			return CVector2(this->x-v.x, this->y-v.y);
		}

		T operator * ( const CVector2& v ) const 
		{
			return this->x*v.y - this->y*v.x;
		}

		CVector2 operator * ( T k )	const						// scale 
		{
			return CVector2(this->x*k, this->y*k);
		}

		CVector2& operator = ( const CVector2& v )
		{
			this->x = v.x;
			this->y = v.y;

			return *this;
		}

		T operator [] (int index) const
		{
			return m[index];
		}

		T& operator [] (int index)
		{
			return m[index];
		}

		operator T*()
		{
			return m;
		}

		void unify()
		{
			double vnor = norm()+DBL_EPSILON;
			x = T(x/vnor);
			y = T(y/vnor);
		}

		void RangeUnify(T min, T max)
		{
			if (x>max)	x=max;
			if (y>max)	y=max;

			if (x<min)	x=min;
			if (y<min)	y=min;
		}

		double norm()
		{
			return sqrt(x*x + y*y);
		}

		template <typename U>
		friend std::ostream& operator << (std::ostream& out, const CVector2<U>& v);
		template <typename U>
		friend std::istream& operator >> (std::istream& in, CVector2<U>& v);
		template <typename U>
		friend bool linear_dependent(const CVector2<U>& v1, const CVector2<U>& v2, const CVector2<U>& v3);

	public:
		union
		{	struct{ T m[2]; };
		struct{ T x,y; };
		struct {T col, row;};
		};
	};

	template <typename U>
	std::ostream& operator << (std::ostream& out, const CVector2<U>& v)
	{
		return out << v.x << " " << v.y;
	}

	template <typename U>
	std::istream& operator >> (std::istream& in, CVector2<U>& v)
	{
		return in >> v.x >> v.y;
	}

	template <typename U>
	bool linear_dependent(const CVector2<U>& v1, const CVector2<U>& v2, const CVector2<U>& v3)
	{
		double det = v2.x*v3.y - v3.x*v2.y - v1.x*v3.y + v3.x*v1.y
			         +v1.x*v2.y - v2.x*v1.y;

		return (fabs(det) < 2*DBL_EPSILON);
	}


	typedef CVector2<float>			vector2f;
	typedef CVector2<double>		vector2d;
	typedef CVector2<int>			vector2i;
}