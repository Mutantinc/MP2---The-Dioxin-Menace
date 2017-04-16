//-------------------------------------------------------------------
//MG_CAMERA.H
//-------------------------------------------------------------------
#ifndef _MG_CAMERA_H_
#define _MG_CAMERA_H_


class mgTarget : public mgObject
{
public:
	inline void Import(uint fp)
	{
		FirstPosition.ReadV(fp);
		ImportTracks(fp);
	}
};

//-------------------------------------------------------------------

class mgCamera : public mgObject
{
public:
	float Roll;				//up-down in radians

	float Near;				//near plane
	float Far;				//far plane

	float Fov;				//fov in radians
	//float AspectRatio;		//aspect ratio

	bool Orthographic;		//ortho projection ?

	mgTarget Target;		//target

	mgList<mgKeyFrame> RollTrk;
	mgList<mgKeyFrame> FovTrk;

//-------------------------------------------------------------------

	inline mgCamera()
	{
		Roll = 0.f;
		Near = 0.1f;
		Far = 1000.f;
		Fov = mgPi*45.f/180.f;
		Orthographic = false;
	}

//-------------------------------------------------------------------

	inline void Transform(uint frame)
	{
		float roll = Interpolate( &RollTrk, frame, Roll );

		mgVector campos, tgtpos;
		Interpolate( frame, &campos );
		Target.Interpolate( frame, &tgtpos );

		
		mgVector worldup(sin(roll), 0.f, cos(roll));    
		mgVector view = tgtpos - campos;
		view.Normalise();

		if( view.x == 0.0f && view.y == 0.0f )
			worldup = mgVector( -view.z, 0.0f, 0.0f );

		float dot=worldup*view;
		mgVector up=worldup-dot*view;
		up.Normalise();

		gluLookAt( campos.x, campos.y, campos.z, tgtpos.x, tgtpos.y, tgtpos.z, up.x, up.y, up.z );
	}

/*
	inline void Camera::Camera2Matrix()
	{
		
		Vector cVec, pivot;
		float    focus;
		float    ax, ay, az;
		float    sinx, siny, sinz,cosx, cosy, cosz;
		
		m.Identity();
		
		pivot=eyepoint.Negated();
		cVec=target-eyepoint;
		focus = cVec.Magnitude();
		
		ax = (float)-atan2 (cVec.x, cVec.z);
		ay = (float)asin (cVec.y / focus);
		az = -roll;
		
		sinx = (float)sin (ax); cosx = (float)cos (ax); siny = (float)sin (ay);
		cosy = (float)cos (ay); sinz = (float)sin (az); cosz = (float)cos (az);
		
		m.xx =  (sinx * siny * sinz + cosx * cosz) * perspX;
		m.yx =  (cosy * sinz) * perspX;
		m.zx =  (sinx * cosz - cosx * siny * sinz) * perspX;
		
		m.xy =  (sinx * siny * cosz - cosx * sinz) * perspY;
		m.yy =  (cosy * cosz) * perspY;
		m.zy =  (-cosx * siny * cosz - sinx * sinz) * perspY;
		
		m.xz = (-sinx * cosy);
		m.yz = ( siny);
		m.zz = ( cosx * cosy);
		
		m.PreTranslation(pivot);
	}
*/	
//-------------------------------------------------------------------
	
	inline void Import(uint fp)
	{
		FirstPosition.ReadV(fp);
		FirstRotation.ReadQ(fp);
		
		Near = mgReadFlt(fp);
		if( Near == 0.f )
			Near = 0.1f;
		Far = mgReadFlt(fp);
		Fov = mgReadFlt(fp);
		
		mgKeyFrame k;
		uint id = mgRead16(fp);
		
		while( id != EOF )
		{
			if( id == MG_IMPEXP_FOVTRK )
			{
				//*****if i want other calculations than linerp, i should include a type
				uint nb = mgRead16(fp);
				for( uint i=0; i<nb; i++)
				{
					k.Frame = mgRead32(fp);
					k.ImportFloatKey(fp);
					FovTrk.add(k);
				}
				break;
			}
			
			else if( id == MG_IMPEXP_ROLLTRK )
			{
				//*****if i want other calculations than linerp, i should include a type
				uint nb = mgRead16(fp);
				for( uint i=0; i<nb; i++)
				{
					k.Frame = mgRead32(fp);
					k.ImportFloatKey(fp);
					RollTrk.add(k);
				}
				break;
			}
			
			else if( (id & MG_IMPEXP_TRK) == MG_IMPEXP_TRK )
			{
				mgSeek(fp, -2, SEEK_CUR);
				ImportTracks(fp);
			}
			
			else if( id == MG_IMPEXP_TARGET )
			{
				Target.Import(fp);
				break;
			}
			
			else
			{
				mgSeek(fp, -2, SEEK_CUR);
				return;
				//break;
			}
			
			id = mgRead16(fp);
		}
	}
};


//END
#endif

