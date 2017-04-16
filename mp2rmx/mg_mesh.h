//-------------------------------------------------------------------
//MG_MESH.H
//-------------------------------------------------------------------
#ifndef _MG_MESH_H_
#define _MG_MESH_H_


#define MG_DRAW_FILLED		0
#define	MG_DRAW_WIRED		1
#define	MG_DRAW_WIREDFILLED	2
#define	MG_DRAW_CARTOON		3


class mgScene;



//-------------------------------------------------------------------

class mgMesh : public mgObject
{
public:
	mgFace *Faces;							//faces' list
	uint NumberFaces;						//number of faces
	
	mgVertex *Vertices;						//vertex array
	uint NumberVertex;						//size of vertices array

#ifdef MG_USE_MULTITEXTURING
	uint NumberMeshMaps;					//number of multitextures
	mgMapping* Mappings[MG_MAX_MESH_MAPS];	//mapping arrays
	uint NumberMappings[MG_MAX_MESH_MAPS];	//size of mapping arrays
	mgMaterial* Material[MG_MAX_MESH_MAPS];	//NULL if no material

#ifdef MG_USE_BUMP_MAPPING
	mgMapping* BumpMap;
#endif	//MG_USE_BUMP_MAPPING

#ifdef MG_USE_ENVIRONMENT_MAPPING
	mgMapping* EnvMap;
#endif	//MG_USE_ENVIRONMENT_MAPPING

#else	//MG_USE_MULTITEXTURING
	mgMapping *Mappings;					//mapping array
	uint NumberMappings;					//size of mapping array
	mgMaterial* Material;					//NULL if no material, then use Color
#endif	//MG_USE_MULTITEXTURING

	mgColor Color;

	bool Smooth;
	bool HasBones;

	mgList<mgKeyFrame> MorphTrk;

//-------------------------------------------------------------------
	
	inline mgMesh()
	{
		Material = NULL;
		NumberFaces = 0;
		NumberVertex = 0;
		NumberMappings = 0;
		Color = mgColor( WHITE );
		Smooth = true;
		HasBones = false;
	}

	inline ~mgMesh()
	{
/*
//*****
		RELEASE(Faces);
		RELEASE(Vertices);
#ifdef MG_USE_MULTITEXTURING
		for( uint i=0; i<MG_MAX_MESH_MAPS; i++)
			RELEASE(Mappings[i]);
#else
		RELEASE(Mappings);
#endif
*/
	}

//-------------------------------------------------------------------
	
	inline void Transform( uint frame )
	{
		//interpolation of position & rotation
		mgMatrix modeltrans;
		mgVector tmppos;
		mgQuaternion tmprot;
		Interpolate( frame, &tmppos );
		Interpolate( frame, &tmprot );
		modeltrans.fromQuaternion( &tmprot );
		modeltrans.Translation( &tmppos );
		glMultMatrixf( (float*)&modeltrans );

		//****** SCALE TO DO
	}

//-------------------------------------------------------------------
//-------------------------------------------------------------------


	inline void Draw(uint mode)	//*****may be more efficient to save the pointer to the drawing routine for each scene
	{
		switch( mode )
		{
		case MG_DRAW_FILLED :
			DrawFilled();
			break;
		case MG_DRAW_WIRED :
			DrawWired();
			break;
		case MG_DRAW_WIREDFILLED :
			DrawWired();
			DrawFilled();
			break;
		case MG_DRAW_CARTOON :
			DrawCartoon();
			break;
		}
	}


	
	inline void DrawFilled()
	{
		if( Material )	//TEXTURED OR WITH A MATERIAL
		{
			Material->Set();
			
			if( Smooth )
			{
				for( uint i=0; i<NumberFaces; i++ )
				{
					glBegin( GL_TRIANGLES );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[0]].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[0]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[1]].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[1]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[2]].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[2]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );
					
					glEnd();
				}
			}
			else
			{
				for( uint i=0; i<NumberFaces; i++ )
				{
					glBegin( GL_TRIANGLES );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[0]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[1]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[2]] );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );
					
					glEnd();
				}
			}
		}
		else	//PLOYGONS WITH NO MATERIAL
		{
			glDisable( GL_TEXTURE_2D );
			
			if( Smooth )
			{
				for( uint i=0; i<NumberFaces; i++ )
				{
					glBegin( GL_TRIANGLES );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[0]].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[1]].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
					glNormal3fv((float*)& Vertices[Faces[i].Vertices[2]].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );
					
					glEnd();
				}
			}
			else
			{
				for( uint i=0; i<NumberFaces; i++ )
				{
					glBegin( GL_TRIANGLES );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
					
					glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
					glNormal3fv((float*)& Faces[i].Normal );
					glVertex3fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );
					
					glEnd();
				}
			}
		}
	}


//-------------------------------------------------------------------
//-------------------------------------------------------------------
	
	inline void DrawWired()
	{
		bool ls = (glIsEnabled(GL_LIGHTING)==GL_TRUE);
		glDisable(GL_LIGHTING);
		glColor4fv((float*)& Color );
		for( uint i=0; i<NumberFaces; i++ )
		{
			glBegin( GL_LINE_LOOP );

			glVertex3fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
			glVertex3fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
			glVertex3fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );

			glEnd();
		}
		
		if(ls)
			glEnable(GL_LIGHTING);
	}

//-------------------------------------------------------------------
//-------------------------------------------------------------------
	
	inline void DrawCartoon()
	{/********/}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline Draw2D( uint frame )						//BEWARE, this function is meant to be used with an Ortho mode...
	{
		glMatrixMode( GL_MODELVIEW );	//so there is no camera needed...
		glLoadIdentity();
		Transform( frame );					//so the transformations are included here

		if( Material )	//TEXTURED OR WITH A MATERIAL
		{
			Material->Set();
			
			for( uint i=0; i<NumberFaces; i++ )
			{
				glBegin( GL_TRIANGLES );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
				glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[0]] );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
				glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[1]] );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
				glTexCoord2fv((float*)& Mappings[Faces[i].Mappings[2]] );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );

				glEnd();
			}
		}
		else	//PLOYGONS WITH NO MATERIAL
		{
			glDisable( GL_TEXTURE_2D );
			for( uint i=0; i<NumberFaces; i++ )
			{
				glBegin( GL_TRIANGLES );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[0]].Color );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[0]].Vector );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[1]].Color );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[1]].Vector );
				
				glColor4fv((float*)& Vertices[Faces[i].Vertices[2]].Color );
				glVertex2fv((float*)& Vertices[Faces[i].Vertices[2]].Vector );

				glEnd();
			}
		}
		glLoadIdentity();
	}
	
//-------------------------------------------------------------------
//-------------------------------------------------------------------
	
	inline void SetDefaultVertexColors( bool vc )
	{
		//false to set the default vertex colors (there is only one global color/object)
		if(!vc)
		{
			for( uint i=0; i<NumberVertex; i++)
			{
				Vertices[i].Color = Color;
			}
		}
	}

	
	inline void MakeNormals()
	{
		uint i;
		for ( i=0; i<NumberVertex; i++)
			Vertices[i].Normal.Null();
		
		for( i=0; i<NumberFaces; i++)
		{
			Faces[i].Normal = ( Vertices[Faces[i].Vertices[1]].Vector - Vertices[Faces[i].Vertices[0]].Vector ) % ( Vertices[Faces[i].Vertices[2]].Vector - Vertices[Faces[i].Vertices[0]].Vector );
		}
			
		if( Smooth )
		{
			for( i=0; i<NumberFaces; i++)
			{
				uint j;
				for( j=0; j<3; j++)
					Vertices[Faces[i].Vertices[j]].Normal += Faces[i].Normal;
			}
			
			for( i=0; i<NumberVertex; i++)
				Vertices[i].Normal.Normalise();
		}
		else
		{
			for( i=0; i<NumberFaces; i++)
			{
				Faces[i].Normal.Normalise();
			}
		}
	}			

//-------------------------------------------------------------------

	inline void ImportMorph(uint fp)
	{
	//******todo
	}

	inline void Import(mgScene* scene, uint fp);
	

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------

	inline void Allocate( uint nbvertex, uint nbfaces )
	{
		Faces = (mgFace*)malloc( nbfaces * sizeof(mgFace) );
		NumberFaces = nbfaces;
		Vertices = (mgVertex*)malloc( nbvertex * sizeof(mgVertex) );
		NumberVertex = nbvertex;
	}

	inline void SetGenScale( float scaleX, float scaleY, float scaleZ );
	inline void SetGenColor( uint8 r, uint8 g, uint8 b );
	inline void SetGenColor( uint c );
	inline void SetGenWireColor( uint8 r, uint8 g, uint8 b );
	inline void SetGenWireColor( uint c );
	inline void SetGenMaterial( mgMaterial *mat, bool smooth );
	inline void SetGenPosition( float posX, float posY, float posZ );
	inline void SetGenRotation( float angle, float axisX, float axisY, float axisZ );
	inline void ApplyGenParameters();
	
	inline void GenCube( float base );
	inline void GenCube2( float base, float cfd, float cfh );
	//inline void GenParrallelepiped( float dimX, float dimY, float dimZ );
	inline void GenPyramid( float base, float top, float height );
	inline void GenOctahedron( float base, float top, float diag );
	inline void GenCylinder( float radius, float height );
	inline void GenCone( float base, float top, float height );
	inline void GenCone( float base, float top, float height, uint slices );
	inline void GenSphere( float radius, uint level );
	inline void GenIcosahedron( float radius );
	inline void GenSphereDgnr( float radius, uint level, uint dgnr );
	inline void GenGlenz( float radius );
	inline void GenTorus( float inner, float section, uint vdivs, uint hdivs );
	inline void GenFish();
	inline void GenBird();
	inline void GenStar( float inner, float outer, float start );
	inline void GenDisk( float radius );
	inline void GenDisk( float radius, uint divs );
	inline void GenDisk( float inner, float outer, uint slices );
	inline void GenDisk( float inner, float outer, uint slices, float start, float sweep );
	inline void GenPlane( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4 );
	
};


//END
#endif
