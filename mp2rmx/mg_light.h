//-------------------------------------------------------------------
//MG_LIGHT.H
//-------------------------------------------------------------------
#ifndef _MG_LIGHT_H_
#define _MG_LIGHT_H_
//-------------------------------------------------------------------

#define MG_LIGHT_MAX_NB			8

//-------------------------------------------------------------------


class mgLight : public mgObject
{
public:
	mgColor AmbientColor;
	mgColor DiffuseColor;
	mgColor SpecularColor;

	mgVector SpotDirection;

	float SpotExponent;
	float SpotCutOff;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;


	inline mgLight()
	{
		AmbientColor = mgColor(BLACK);
		SpecularColor = mgColor(BLACK);
		SpotExponent = 0.0f;
		SpotCutOff = 180.0f;
		ConstantAttenuation = 1.0f;
		LinearAttenuation = 0.0f;
		QuadraticAttenuation = 0.0f;
	}
	

//-------------------------------------------------------------------

	inline void Transform(uint frame, uint index)
	{

		//mgMatrix modeltrans;
		mgVector tmppos;
		Interpolate( frame, &tmppos );
		tmppos += FirstPosition;
		tmppos.w = 1.0f;
/*
		modeltrans.Translation( &tmppos );
		glMultMatrixf( (float*)&modeltrans );

		mgVector zero;
		zero.w = 1.f;
		glLightfv( GL_LIGHT0+index, GL_POSITION, (float*)& zero );
*/
		
		glLightfv( GL_LIGHT0+index, GL_POSITION, (float*)& tmppos );
	}

//-------------------------------------------------------------------

	inline void Import(uint fp)
	{
		FirstPosition.ReadV(fp);
		FirstPosition.w = 1.0f;
		
		mgRead8(fp);
		DiffuseColor.ReadC(fp);

		ImportTracks(fp);
	}

//-------------------------------------------------------------------

};

//END
#endif
