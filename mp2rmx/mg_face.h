//-------------------------------------------------------------------
//MG_FACE.H
//-------------------------------------------------------------------
#ifndef _MG_FACE_H_
#define _MG_FACE_H_


class mgFace
{
public:

	uint Vertices[3];		//indexes
	uint Mappings[3];		//indexes

	mgVector Normal;		//normal of the plane the face is in

//	mgMaterial* Material;
	uint Smooth;

//-------------------------------------------------------------------

/*
	inline mgFace( mgFace& orig )	//copy constructor
	{
		for( uint i=0; i<3; i++ )
		{
			Vertices[i] = orig.Vertices[i];
			Mappings[i] = orig.Mappings[i];
		}
	}
*/

	inline void SetV( uint v1, uint v2, uint v3 )
	{
		Vertices[0] = v1;
		Vertices[1] = v2;
		Vertices[2] = v3;
	}

/*
	//case for more than 3 vectors
	inline void MakeNormal ()
	{
		uint i;
		float bestlength=-1.f;
		
		for( i=0; i<NumberVertex-2; i++)
		{
			mgVector tmpvect = (Vertices[i+2].Vector - Vertices[i+1].Vector) % (Vertices[i].Vector - Vertices[i+1].Vector);
			float tmplength = tmpvect.MagnitudeSquare();
			
			if( tmplength > bestlength )
			{
				bestlength = tmplength;
				Normal = tmpvect;
			}
		}

		Normal.Normalise ();
	}
*/

};

//END
#endif

