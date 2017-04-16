//-------------------------------------------------------------------
//MG_MATERIAL.H
//-------------------------------------------------------------------
#ifndef _MG_MATERIAL_H_
#define _MG_MATERIAL_H_



class mgMaterial
{
public:

	mgColor AmbientColor;
	mgColor DiffuseColor;
	mgColor SpecularColor;
	//mgColor EmissionColor;

	float Shininess;
	float Transparency;

#ifdef MG_USE_MULTITEXTURING
	mgTexture Textures[];
#else
	mgTexture Texture;
#endif


#ifdef MG_USE_BUMP_MAPPING
	mgTexture Bump;	//***** 3 textures normalement ?
#endif
 
#ifdef MG_USE_ENVIRONMENT_MAPPING
	mgTexture Environment;
#endif

//-------------------------------------------------------------------

	inline mgMaterial()
	{}

//-------------------------------------------------------------------

	inline void Set(void)
	{
		glMaterialfv( GL_FRONT, GL_AMBIENT, (float*)& AmbientColor );
		glMaterialfv( GL_FRONT, GL_DIFFUSE, (float*)& DiffuseColor );
		glMaterialfv( GL_FRONT, GL_SPECULAR, (float*)& SpecularColor );
		glMaterialf( GL_FRONT, GL_SHININESS, Shininess );
		if( Texture.ID )
		{
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, Texture.ID );
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
	}

//-------------------------------------------------------------------
	
	inline void Import(uint fp)
	{
		AmbientColor.ReadC(fp);
		DiffuseColor.ReadC(fp);
		SpecularColor.ReadC(fp);
		
		Shininess = mgReadFlt(fp);
		Transparency = mgReadFlt(fp);
		
		uint nbtex = (uint)mgRead8(fp);
		if( nbtex )
			AmbientColor = DiffuseColor = SpecularColor = mgColor( WHITE );

		for( uint i=0; i<nbtex; i++ )	//*****a modifier pour que ça gère le bump + env
#ifdef MG_USE_MULTITEXTURING
			Textures[i].Import(fp);
#else
			Texture.Import(fp);
#endif
	}
};



//END
#endif
