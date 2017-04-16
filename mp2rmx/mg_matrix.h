//-------------------------------------------------------------------
//MG_MATRIX.H - ...MY NAME IS TRINITY...
//-------------------------------------------------------------------
#ifndef _MG_MATRIX_H_
#define _MG_MATRIX_H_

inline mgMatrix operator*(mgMatrix a, mgMatrix b);


class mgMatrix
{
public:
	float xx,xy,xz,xw;
	float yx,yy,yz,yw;
	float zx,zy,zz,zw;
	float wx,wy,wz,ww;

	inline mgMatrix()
	{ Identity(); }

	inline mgMatrix(mgVector *v)
	{ Translation(v); }

	inline void Identity()
	{
		   xy=xz=xw=0.0f;
		yx=   yz=yw=0.0f;
		zx=zy=   zw=0.0f;
		wx=wy=wz=   0.0f;

		xx=yy=zz=ww=1.0f;
	};

    inline void fromQuaternion(mgQuaternion *q)
    {
      /*
      qt_mgMatrix: convert a unit quaternion to rotation mgMatrix.

          ( 1-yy-zz , xy+wz   , xz-wy   )
      T = ( xy-wz   , 1-xx-zz , yz+wx   )
          ( xz+wy   , yz-wx   , 1-xx-yy )
      */
      float fx2, fy2, fz2, fwx, fwy, fwz,
        fxx, fxy, fxz, fyy, fyz, fzz;

      fx2 = q->x + q->x; fy2 = q->y + q->y; fz2 = q->z + q->z;
      fwx = q->w * fx2;   fwy = q->w * fy2;   fwz = q->w * fz2;
      fxx = q->x * fx2;   fxy = q->x * fy2;   fxz = q->x * fz2;
      fyy = q->y * fy2;   fyz = q->y * fz2;   fzz = q->z * fz2;

      xx = 1.0f - (fyy + fzz);
      yx = fxy - fwz;
      zx = fxz + fwy;
      wx = 0.0f;
      xy = fxy + fwz;
      yy = 1.0f - (fxx + fzz);
      zy = fyz - fwx;
      wy = 0.0f;
      xz = fxz - fwy;
      yz = fyz + fwx;
      zz = 1.0f - (fxx + fyy);
      wz = 0.0f;
    }


	inline void Inverse()
	{
		mgMatrix r;
		float d=1.0f/( 
			xx * ( yy * zz - yz * zy ) -
			xy * ( yx * zz - yz * zx ) +
			xz * ( yx * zy - yy * zx ) );
		
		r.xx =  d * ( yy * zz - yz * zy );
		r.xy = -d * ( xy * zz - xz * zy );
		r.xz =  d * ( xy * yz - xz * yy );
		
		r.yx = -d * ( yx * zz - yz * zx );
		r.yy =  d * ( xx * zz - xz * zx );
		r.yz = -d * ( xx * yz - xz * yx );
		
		r.zx =  d * ( yx * zy - yy * zx );
		r.zy = -d * ( xx * zy - xy * zx );
		r.zz =  d * ( xx * yy - xy * yx );
		
		r.wx = -( wx * r.xx + wy * r.yx + wz * r.zx );
		r.wy = -( wx * r.xy + wy * r.yy + wz * r.zy );
		r.wz = -( wx * r.xz + wy * r.yz + wz * r.zz );	
		*this=r;
	}
		
	inline void Translation(mgVector *v)
	{
		wx+=v->x;
		wy+=v->y;
		wz+=v->z;
	}

	
    inline void PreTranslation(mgVector *v)
    {
		
		wx = xx*v->x + yx*v->y + zx*v->z + wx;
		wy = xy*v->x + yy*v->y + zy*v->z + wy;
		wz = xz*v->x + yz*v->y + zz*v->z + wz;
	/*
		wx = xx*v->x + xy*v->y + xz*v->z + wx;
		wy = yx*v->x + yy*v->y + yz*v->z + wy;
		wz = zx*v->x + zy*v->y + zz*v->z + wz;
	*/
    }


	inline mgVector GetTranslation()
	{
		return mgVector(wx, wy, wz);
	}

	inline mgMatrix Scale(float s)
	{
		Identity();
		xx=s;
		yy=s;
		zz=s;
		return *this;
	}

	inline mgMatrix Scale(mgVector v)
	{
		Identity();
		xx=v.x;
		yy=v.y;
		zz=v.z;
		return *this;
	}

	inline mgMatrix NoScale(mgVector v)
	{
		xx=1.f;
		yy=1.f;
		zz=1.f;
		return *this;
	}

	inline mgMatrix Rotation(mgVector v)
	{
		float sx=sin(v.x);
		float sy=sin(v.y);
		float sz=sin(v.z);
		float cx=cos(v.x);
		float cy=cos(v.y);
		float cz=cos(v.z);

		xx=cy*cz; 
		xy=cy*sz; 
		xz=-sy;
		yx=sx*sy*cz-cx*sz; 
		yy=sx*sy*sz+cx*cz; 
		yz=sx*cy;
		zx=cx*sy*cz+sx*sz; 
		zy=cx*sy*sz-sx*cz; 
		zz=cx*cy;

		return *this;
	}


	inline void Space(mgVector space)
	{
		float dot = mgVector(0.f, 1.f, 0.f)*space;
		mgVector up = mgVector(0.f, 1.f, 0.f)-dot*space;
		up.Normalise();
		mgVector right=up%space;    
		
		Identity();
		xx=right.x;
		xy=up.x;
		xz=space.x;
		yx=right.y;
		yy=up.y;
		yz=space.y;
		zx=right.z;
		zy=up.z;
		zz=space.z;
		wx=0;
		wy=0;
		wz=0;
	}

	inline mgVector TransInverseBase (mgVector v)
	{
		mgVector r;
		/*
		r.x=v.x*zz+ v.y*zy+ v.z*zx;
		r.y=v.x*yz+ v.y*yy+ v.z*yx;
		r.z=v.x*xz+ v.y*xy+ v.z*yy;
		*/

		r.x=v.x*xx+ v.y*xy+ v.z*xz;
		r.y=v.x*yx+ v.y*yy+ v.z*yz;
		r.z=v.x*zx+ v.y*zy+ v.z*zz;

		return r;
	}
	
/*
	inline void Camera(float roll, mgVector position, mgVector target)
	{
		mgVector worldup(sin(-roll), cos(-roll), 0.0f);    
		mgVector view=-(target-position);
		view.Normalise();
		float dot=worldup*view;
		mgVector up=worldup-dot*view;
		up.Normalise();
		mgVector right=up%view;    
		
		Identity();
		xx=right.x;
		xy=up.x;
		xz=view.x;
		yx=right.y;
		yy=up.y;
		yz=view.y;
		zx=right.z;
		zy=up.z;
		zz=view.z;
		wx=-(position*right);
		wy=-(position*up);
		wz=-(position*view);
		
		//Inverse();
	};
	
	void RaytraceCamera(float roll, mgVector position, mgVector target)
	{
		mgVector worldup(sin(-roll), cos(-roll), 0.0f);    
		mgVector view=-(target-position);
		view.Normalise();
		float dot=worldup*view;
		mgVector up=worldup-dot*view;
		up.Normalise();
		mgVector right=up%view;    
		
		Identity();
		xx=right.x;
		xy=up.x;
		xz=view.x;
		yx=right.y;
		yy=up.y;
		yz=view.y;
		zx=right.z;
		zy=up.z;
		zz=view.z;
		wx=0;
		wy=0;
		wz=0;
		Inverse();
	};
*/

	mgMatrix operator *=(mgMatrix m)
	{
		*this=*this*m;
		return *this;
	}
};


//-------------------------------------------------------------------


inline mgVector operator*(mgVector &v, mgMatrix &m)
{
	mgVector r;
	r.x=v.x*m.xx+ v.y*m.yx+ v.z*m.zx+ m.wx;
	r.y=v.x*m.xy+ v.y*m.yy+ v.z*m.zy+ m.wy;
	r.z=v.x*m.xz+ v.y*m.yz+ v.z*m.zz+ m.wz;
	return r;
}


//We know / makes no sense, what we mean is a multiplication without translation !
inline mgVector operator/(mgVector v, mgMatrix m)
{
	mgVector r;
	r.x=v.x*m.xx+ v.y*m.yx+ v.z*m.zx;
	r.y=v.x*m.xy+ v.y*m.yy+ v.z*m.zy;
	r.z=v.x*m.xz+ v.y*m.yz+ v.z*m.zz;
	return r;
}


inline mgVector &mgVector::operator*=(const mgMatrix m)
{
	mgVector r;
	r.x=x*m.xx+ y*m.yx+ z*m.zx+ m.wx;
	r.y=x*m.xy+ y*m.yy+ z*m.zy+ m.wy;
	r.z=x*m.xz+ y*m.yz+ z*m.zz+ m.wz;
	*this=r;
	return *this;
}

//We know / makes no sense, what we mean is a multiplication without translation !
inline mgVector &mgVector::operator/=(const mgMatrix m)
{
	mgVector r;
	r.x=x*m.xx+ y*m.yx+ z*m.zx;
	r.y=x*m.xy+ y*m.yy+ z*m.zy;
	r.z=x*m.xz+ y*m.yz+ z*m.zz;
	*this=r;
	return *this;
}


inline mgMatrix operator*(mgMatrix a, mgMatrix b)
{
	mgMatrix temp;
	temp.xx=a.xx*b.xx+a.yx*b.xy+a.zx*b.xz;
	temp.yx=a.xx*b.yx+a.yx*b.yy+a.zx*b.yz;
	temp.zx=a.xx*b.zx+a.yx*b.zy+a.zx*b.zz;
	temp.wx=a.xx*b.wx+a.yx*b.wy+a.zx*b.wz+a.wx;
	temp.xy=a.xy*b.xx+a.yy*b.xy+a.zy*b.xz;
	temp.yy=a.xy*b.yx+a.yy*b.yy+a.zy*b.yz;
	temp.zy=a.xy*b.zx+a.yy*b.zy+a.zy*b.zz;
	temp.wy=a.xy*b.wx+a.yy*b.wy+a.zy*b.wz+a.wy;
	temp.xz=a.xz*b.xx+a.yz*b.xy+a.zz*b.xz;
	temp.yz=a.xz*b.yx+a.yz*b.yy+a.zz*b.yz;
	temp.zz=a.xz*b.zx+a.yz*b.zy+a.zz*b.zz;
	temp.wz=a.xz*b.wx+a.yz*b.wy+a.zz*b.wz+a.wz;
	return temp;
}



//END
#endif