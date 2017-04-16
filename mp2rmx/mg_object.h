//-------------------------------------------------------------------
//MG_OBJECT.H
//-------------------------------------------------------------------
#ifndef _MG_OBJECT_H_
#define _MG_OBJECT_H_


class mgObject
{
public:
	mgVector FirstPosition;			//initial position
	mgQuaternion FirstRotation;		//initial rotation

	//*****mgMatrix Matix;
	//bool Loop

	uint PosTrkType;
	mgList<mgKeyFrame> PosTrk;

	uint RotTrkType;
	mgList<mgKeyFrame> RotTrk;

	uint ScaleTrkType;
	mgList<mgKeyFrame> ScaleTrk;

//-------------------------------------------------------------------

	inline mgObject()
	{
		FirstPosition = mgVector();
		FirstRotation = mgQuaternion();
		PosTrkType = NULL;
		RotTrkType = NULL;
		ScaleTrkType = NULL;
	}

//-------------------------------------------------------------------

	inline void ImportTracks(uint fp)
	{
		uint id = mgRead16(fp);
		
		while( id != EOF )
		{
			mgKeyFrame k;

			switch( id )
			{
				
			case MG_IMPEXP_TCBPOSTRK:
				{
					PosTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportTCBPosKey(fp);
						PosTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_BEZPOSTRK:
				{
					PosTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportBezPosKey(fp);
						PosTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_LINPOSTRK:
				{
					PosTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportLinPosKey(fp);
						PosTrk.add(k);
					}
					break;
				}
			
			//----------------------

			case MG_IMPEXP_TCBROTTRK:
				{
					RotTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportTCBRotKey(fp);
						RotTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_BEZROTTRK:
				{
					RotTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportBezRotKey(fp);
						RotTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_LINROTTRK:
				{
					RotTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportLinRotKey(fp);
						RotTrk.add(k);
					}
					break;
				}
				
			//----------------------

			case MG_IMPEXP_TCBSCATRK:
				{
					ScaleTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportTCBScaleKey(fp);
						ScaleTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_BEZSCATRK:
				{
					ScaleTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportBezScaleKey(fp);
						ScaleTrk.add(k);
					}
					break;
				}

			case MG_IMPEXP_LINSCATRK:
				{
					ScaleTrkType = id;
					uint nb = mgRead16(fp);
					for( uint i=0; i<nb; i++)
					{
						k.Frame = mgRead32(fp);
						k.ImportLinScaleKey(fp);
						ScaleTrk.add(k);
					}
					break;
				}
				
			//----------------------

			default:
				{
					mgSeek(fp, -2, SEEK_CUR);
					return;
					//break;
				}
			}

			id = mgRead16(fp);
		}
	}


//-------------------------------------------------------------------
//-------------------------------------------------------------------

	//finds the 2 keys between which there must be some interpolation and the local time [0,1] at which it occurs depending on the frame number
	inline bool GetEnclosingKeys( mgList<mgKeyFrame> *keys, uint frame, mgKeyFrame **k1, mgKeyFrame **k2, float* time )
	{
		if( keys->back()->Frame <= frame )
			return false;

		uint i=0;
		while( (signed)keys->at(i)->Frame < 0 || keys->at(i)->Frame <= frame )	// the first test is because sometimes 3ds exports some keyframes with a << time.  to be sure to have at least i==1, thus a key set before frame
		{ i++; }

		*k1 = keys->at(i-1);
		*k2 = keys->at(i);
		*time = (float)( frame - (*k1)->Frame) / (float)((*k2)->Frame - (*k1)->Frame );
		return true;
	}




//-------------------------------------------------------------------
//-------------------------------------------------------------------



	
	//interpolation for floats
	inline float Interpolate( mgList<mgKeyFrame> *keys, uint frame, float defval )
	{
		if( keys->size() )
		{
			
			float t;
			mgKeyFrame *k1, *k2;
			
			if( GetEnclosingKeys( keys, frame, &k1, &k2, &t ) )
				return k1->Float + t*( k2->Float - k1->Float );
			else
				return keys->back()->Float;
		}
		else
			return defval;
	}

	
	
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------


	
	
	//the main vector interpolation manager
	inline void Interpolate( uint frame, mgVector* outv )
	{
		if( PosTrk.size() )
		{
			if( PosTrkType == MG_IMPEXP_TCBPOSTRK )
				HermiteSpline( frame, outv );
			else if( PosTrkType == MG_IMPEXP_BEZPOSTRK )
				//*****
				//HermiteSpline( frame, outv );
				LinearInterpolation( frame, outv );
			else
				LinearInterpolation( frame, outv );
		}
		else
			*outv = FirstPosition;
	}
	

//-------------------------------------------------------------------

	//interpolation for positions
	inline void LinearInterpolation( uint frame, mgVector *outv )
	{
		float t;
		mgKeyFrame *k1, *k2;

		if( GetEnclosingKeys( &PosTrk, frame, &k1, &k2, &t ) )
			*outv = k1->Vector + t*( k2->Vector - k1->Vector );
		else
			*outv = PosTrk.back()->Vector;
	}

//-------------------------------------------------------------------

	//spline for positions
	inline void HermiteSpline( uint frame, mgVector *outv )
	{
		float t, t2, t3;
		mgKeyFrame *k1, *k2;


		if( GetEnclosingKeys( &PosTrk, frame, &k1, &k2, &t ) )
		{
			t = Ease( k1, k2, t );
			t2 = t * t;
			t3 = t2 * t;
			*outv = k1->Vector*(2.f*t3 - 3.f*t2 + 1) + k2->Vector*(-2.f*t3 + 3.f*t2) + k1->TanRight*(t3 - 2.f*t2 + t) + k2->TanLeft*(t3-t2);
		}
		else
			*outv = PosTrk.back()->Vector;
	} 


//-------------------------------------------------------------------

	//for positions
	inline void ComputeHermiteTangents(void)
	{
		uint s = PosTrk.size();
		if( s == 0 )
		{}
		else if( s == 1 )
		{
			PosTrk.erase(0);
		}
		else if( s == 2 )	//special case : only 2 keys
		{
			PosTrk.at(0)->TanRight = PosTrk.at(1)->TanLeft = (PosTrk.at(1)->Vector-PosTrk.at(0)->Vector) * (1.f-PosTrk.at(0)->TCB.T);
		}
		else
		{
			//general tangents
			for( uint i=1; i<s-1; i++ )
			{
				float d1, d2, adjust;
				d1 = PosTrk.at(i)->Frame - PosTrk.at(i-1)->Frame;
				d2 = PosTrk.at(i+1)->Frame - PosTrk.at(i)->Frame;

				adjust = 0.5f + (1.f - fabs(PosTrk.at(i)->TCB.C)) * (d1/(d1+d2) - 0.5f);
				PosTrk.at(i)->TanLeft = ( (PosTrk.at(i)->Vector-PosTrk.at(i-1)->Vector)*(1.f+PosTrk.at(i)->TCB.B)*(1.f-PosTrk.at(i)->TCB.C) + (PosTrk.at(i+1)->Vector-PosTrk.at(i)->Vector)*(1.f-PosTrk.at(i)->TCB.B)*(1.f+PosTrk.at(i)->TCB.C) ) * (1.f-PosTrk.at(i)->TCB.T) * adjust;
				
				adjust = 0.5f + (1.f - fabs(PosTrk.at(i)->TCB.C)) * (d1/(d1+d2) - 0.5f);
				PosTrk.at(i)->TanRight = ( (PosTrk.at(i)->Vector-PosTrk.at(i-1)->Vector)*(1.f+PosTrk.at(i)->TCB.B)*(1.f+PosTrk.at(i)->TCB.C) + (PosTrk.at(i+1)->Vector-PosTrk.at(i)->Vector)*(1.f-PosTrk.at(i)->TCB.B)*(1.f-PosTrk.at(i)->TCB.C) ) * (1.f-PosTrk.at(i)->TCB.T) * adjust;
			}
			
			//start & end tangents
			PosTrk.at(0)->TanRight = ((PosTrk.at(1)->Vector-PosTrk.at(0)->Vector) * 1.5f - PosTrk.at(1)->TanLeft * 0.5f ) * (1.f - PosTrk.at(0)->TCB.T);
			PosTrk.at(s-1)->TanLeft = ((PosTrk.at(s-1)->Vector-PosTrk.at(s-2)->Vector) * 1.5f - PosTrk.at(s-2)->TanRight * 0.5f ) * (1.f - PosTrk.at(s-1)->TCB.T);
		}
	}
	



//-------------------------------------------------------------------
//-------------------------------------------------------------------

	//the main quaternion interpolation manager
	inline void Interpolate( uint frame, mgQuaternion* outq )
	{
		if( RotTrk.size() )
		{
			
			if( RotTrkType != MG_IMPEXP_TCBROTTRK )
				SphericalLinearInterpolation( frame, outq );
			else
				BezierSpline( frame, outq );
		}
		else
			*outq = FirstRotation;
	}

//-------------------------------------------------------------------

	//interpolation for quaternions
	inline void SphericalLinearInterpolation( uint frame, mgQuaternion *outq )
	{
		float t;
		mgKeyFrame *k1, *k2;

		if( GetEnclosingKeys( &RotTrk, frame, &k1, &k2, &t ) )
			outq->Slerp( k1->Quaternion, k2->Quaternion, t );
		else
			*outq = RotTrk.back()->Quaternion;
	}

//-------------------------------------------------------------------

	//spline for quaternions
	inline void BezierSpline( uint frame, mgQuaternion *outq )
	{
		float t;
		mgKeyFrame *k1, *k2;
		
		if( GetEnclosingKeys( &RotTrk, frame, &k1, &k2, &t ) )
		{
			t = Ease( k1, k2, t );
			
			mgQuaternion q0, q1, q2;
			q0.Slerp( k1->Quaternion, k1->CtlRight, t );
			q1.Slerp( k1->CtlRight, k2->CtlLeft, t );
			q2.Slerp( k2->CtlLeft, k2->Quaternion, t );
			
			q0.Slerp( q0, q1, t );
			q1.Slerp( q1, q2, t);
			
			outq->Slerp( q0, q1, t );
		}
		else
			*outq = RotTrk.back()->Quaternion;
	}


//-------------------------------------------------------------------

	//for quaternions
	inline void ComputeBezierControls( void )
	{
		uint s = RotTrk.size();
		
		if( s == 0 )
		{}
		else if( s == 1)
		{
			RotTrk.erase(0);
		}
		else
		{
			//general control quads
			for( uint i=1; i<s-1; i++ )
			{
				float d1, d2, adjust;
				mgQuaternion g1, g2, g3;
				g1.Slerp( RotTrk.at(i)->Quaternion, RotTrk.at(i-1)->Quaternion, -(1.f+RotTrk.at(i)->TCB.B)/3.f );
				g2.Slerp( RotTrk.at(i)->Quaternion, RotTrk.at(i+1)->Quaternion, (1.f-RotTrk.at(i)->TCB.B)/3.f );
				
				d1 = RotTrk.at(i)->Frame - RotTrk.at(i-1)->Frame;
				d2 = RotTrk.at(i+1)->Frame - RotTrk.at(i)->Frame;
				
				adjust = 0.5f + (1.f - fabs(RotTrk.at(i)->TCB.C)) * (d1/(d1+d2) - 0.5f);
				g3.Slerp( g1, g2, 0.5f + 0.5f * RotTrk.at(i)->TCB.C );
				RotTrk.at(i)->CtlLeft.Slerp( RotTrk.at(i)->Quaternion, g3, (RotTrk.at(i)->TCB.T - 1.f)*adjust*2.f );
				
				adjust = 0.5f + (1.f - fabs(RotTrk.at(i)->TCB.C)) * (d1/(d1+d2) - 0.5f);
				g3.Slerp( g1, g2, 0.5f - 0.5f * RotTrk.at(i)->TCB.C );
				RotTrk.at(i)->CtlRight.Slerp( RotTrk.at(i)->Quaternion, g3, -(RotTrk.at(i)->TCB.T - 1.f)*adjust*2.f );
			}
			
			//start & end contrl quads
			RotTrk.at(0)->CtlRight.Slerp( RotTrk.at(0)->Quaternion, RotTrk.at(1)->Quaternion, (1.f - RotTrk.at(0)->TCB.T) * (1.f + RotTrk.at(0)->TCB.C * RotTrk.at(0)->TCB.B) / 3.f );
			RotTrk.at(s-1)->CtlLeft.Slerp(  RotTrk.at(s-1)->Quaternion, RotTrk.at(s-2)->Quaternion, (1.f - RotTrk.at(s-1)->TCB.T) * (1.f + RotTrk.at(s-1)->TCB.C * RotTrk.at(s-1)->TCB.B) / 3.f );
		}
	}




//-------------------------------------------------------------------
//-------------------------------------------------------------------




	inline float Ease( mgKeyFrame *k1, mgKeyFrame *k2, float t )
	{
		float k;
		float a = k1->TCB.EaseTo;
		float b = k2->TCB.EaseFrom;
		float s = a+b;

		if( s == 0.f )
			return t;

		if( s > 1.f )
		{
			a = a/s;
			b = b/s;
		}

		k = 1.f/( 2.f-a-b );

		if( t<a )
			return (k/a)*t*t;

		if( t>1.f-b )
			return (2.f - (k/b)*t)*t;

		return k * ( 2.f*t - a );
	}


};

//END
#endif

