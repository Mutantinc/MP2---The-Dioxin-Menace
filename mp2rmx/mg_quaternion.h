//-------------------------------------------------------------------
//MG_QUATERNION.H - ABOUT THE QUATERNIONS
//-------------------------------------------------------------------
#ifndef _MG_QUATERNION_H_
#define _MG_QUATERNION_H_


class mgQuaternion
{
public:

	union
	{
		struct {float x,y,z,w;};
		float array[4];
	};	

	inline mgQuaternion()
	{ Identity(); }
	
	inline mgQuaternion(float x, float y, float z, float w)
	{
		this->x = x; 
		this->y = y; 
		this->z = z;
        this->w = w;
	}

	inline void Identity()
	{
		x = y = z = 0.f;
		w = 1.0f;
	}


//-------------------------------------------------------------------

	inline void ReadQ(uint p)
	{
		x = mgReadFlt(p);
		y = mgReadFlt(p);
		z = mgReadFlt(p);
		w = mgReadFlt(p);
	}

//-------------------------------------------------------------------

	inline void fromAngleAxis( float radians, float axisx, float axisy, float axisz )
	{
		x = axisx * sin(radians/2.0f);
		y = axisy * sin(radians/2.0f);
		z = axisz * sin(radians/2.0f);
		w = cos(radians/2.0f);
	}


	inline void Slerp( mgQuaternion &a, mgQuaternion &b, float time );	//sperical linear interpolation

//-------------------------------------------------------------------


	inline bool operator==(const mgQuaternion &a)
	{		
		return( x==a.x && y==a.y && z==a.z && w==a.w);
	}

	inline mgQuaternion &operator+=(const mgQuaternion &a)
	{		
		x+=a.x;
		y+=a.y;
		z+=a.z;
		w+=a.w;
		return *this;
	}
	
	inline mgQuaternion &operator-=(const mgQuaternion &a)
	{
		x-=a.x;
		y-=a.y;
		z-=a.z;
		w-=a.w;
		return *this;
	}
	
	inline mgQuaternion &operator*=(const float f)
	{
		x*=f;
		y*=f;
		z*=f;
		w*=f;
		return *this;
	}

	inline mgQuaternion&	operator*=(const mgQuaternion& q)
	{
		w = w*q.w-x*q.x-y*q.y-z*q.z;
		x = w*q.x+x*q.w+y*q.z-z*q.y;
		y = w*q.y+y*q.w+z*q.x-x*q.z;
		z = w*q.z+z*q.w+x*q.y-y*q.x;
		return	*this;
	}

};


//-------------------------------------------------------------------
//-------------------------------------------------------------------


	inline mgQuaternion operator-(const mgQuaternion &a)
	{
		return mgQuaternion( -a.x, -a.y, -a.z, -a.w);
	}
	
	inline mgQuaternion operator+(const mgQuaternion &a, const mgQuaternion &b)
	{
		return mgQuaternion(a.x+b.x,a.y+b.y,a.z+b.z,a.w+b.w);
	}
	
	inline mgQuaternion operator-(const mgQuaternion &a, const mgQuaternion &b)
	{
		return mgQuaternion(a.x-b.x,a.y-b.y,a.z-b.z,a.z-b.z);
	}
	
	inline mgQuaternion operator*(const mgQuaternion &a, const float b)
	{
		return mgQuaternion(a.x*b, a.y*b, a.z*b, a.w*b);
	}
	
	inline mgQuaternion operator*(float a, const mgQuaternion &b)
	{
		return mgQuaternion(a*b.x, a*b.y, a*b.z, a*b.w);
	}
	
	inline mgQuaternion operator/(const mgQuaternion &a, const float b)
	{
		float ib=1.f/b;
		return mgQuaternion(a.x*ib, a.y*ib, a.z*ib, a.w*ib);
	}
	
	inline float operator|(const mgQuaternion &a, const mgQuaternion &b)	//dot product
	{
		return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w;
	}


//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline void mgQuaternion::Slerp( mgQuaternion &a, mgQuaternion &b, float time )	//sperical linear interpolation
	{
		double cosang = a | b;
		if( cosang > 0.998 )
			mgQuaternion(a + time*( b - a ));	//normal linear interpolation to avoid strange results due to /sin(angle) when angle ~ 0 
		else
		{
			double ang = acos(cosang);
			mgQuaternion( ( sin((1.f-time)*ang) * a + sin(time*ang) * b ) / sin(ang));
		}
	}


//END
#endif

