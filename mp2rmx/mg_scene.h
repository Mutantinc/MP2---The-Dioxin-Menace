//-------------------------------------------------------------------
//MG_SCENE.H
//-------------------------------------------------------------------
#ifndef _MG_SCENE_H_
#define _MG_SCENE_H_



class mgScene
{
public:
	uint FirstFrame;
	uint LastFrame;
	uint FrameRate;
	//*****uint CurrentFrame;

	uint StartTime;
	uint ElapsedTime;

	bool Stop;

	mgColor BackGroundColor;
	mgColor AmbientLight;
	
	mgList<mgMesh> Meshes;
	bool VertexColors;
	mgList<mgMaterial> Materials;
	mgList<mgLight> Lights;
	mgList<mgCamera> Cameras;
	uint CameraSelected;

	uint Renderer;

//-------------------------------------------------------------------
	
	inline mgScene()
	{
		FirstFrame = 0;
		LastFrame = 0-1;
		FrameRate = 25;
		ElapsedTime = 0;
		Stop = true;
		BackGroundColor = mgColor( BLACK );
		AmbientLight = mgColor( WHITE );
		VertexColors = false;
		CameraSelected = 0;
		Renderer = MG_DRAW_FILLED;
	}

//-------------------------------------------------------------------

	inline uint GetCurrentFrame()
	{
		return ( mgTime - StartTime ) * FrameRate / 1000;
	}

//-------------------------------------------------------------------

	inline void Draw( uint frame, bool resetmatrix )
	{
		//TIME COMPUTATIONS
		if( Stop )
			return;

		uint i;

		//CAMERA COMPUTATIONS
#ifdef MG_USE_FREELOOK
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45.f, 4.f/3.f, 1.f, 1000.f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glTranslatef( -mgFreeLookPosX, -mgFreeLookPosY, -mgFreeLookPosZ );
		glRotatef( mgFreeLookRotX, 1.f, 0.f, 0.f );
		glRotatef( mgFreeLookRotY, 0.f, 1.f, 0.f );
		glRotatef( mgFreeLookRotZ, 0.f, 0.f, 1.f );
#else
		mgCamera* cam = Cameras.at(CameraSelected);
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( RAD_TO_DEG(cam->Interpolate( &cam->FovTrk, frame, cam->Fov )), 4.f/3.f, cam->Near, cam->Far );

		glMatrixMode( GL_MODELVIEW );
		if( resetmatrix )
		{
			glLoadIdentity();
			cam->Transform( frame );
		}
#endif

		//LIGHTS COMPUTATION, ONLY THE CAMERA MUST BE INITIALIZED AT THIS POINT (if not, use push/pop)
		for( i=0; i<Lights.size(); i++)
		{
			Lights.at(i)->Transform( frame, i );
		}
		
		//MESHES COMPUTATION
		for( i=0; i<Meshes.size(); i++)
		{
			glPushMatrix();
			Meshes.at(i)->Transform( frame );
			Meshes.at(i)->Draw( Renderer );
			glPopMatrix();
		}
	}

//-------------------------------------------------------------------

	inline void AddCamera( float posx, float posy, float posz, float targetx, float targety, float targetz )
	{
		mgCamera cam;
		cam.Target.FirstPosition = mgVector( targetx, targety, targetz );
		cam.FirstPosition = mgVector( posx, posy, posz );
		Cameras.add(cam);
	}

	inline void AddLight( float posx, float posy, float posz, uint32 color )
	{
		mgLight l;
		l.FirstPosition = mgVector( posx, posy, posz );
		l.DiffuseColor = mgColor( color );
		Lights.add(l);
	}

//-------------------------------------------------------------------

	inline void Setup(void)
	{
		uint i;

		Unset();

		//GLOBAL SETTINGS
		glClearColor(BackGroundColor.r, BackGroundColor.g, BackGroundColor.b, BackGroundColor.a);
		glClearDepth(1.0f);

		//LIGHT MODEL
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, (float*)& AmbientLight );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE );
		glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

		//LIGHTNING
		for( i=0; i<Lights.size() && i<MG_LIGHT_MAX_NB; i++)
		{
			glEnable( GL_LIGHTING );
			glEnable( GL_LIGHT0+i );
			glLightfv( GL_LIGHT0+i, GL_AMBIENT, (float*)& Lights.at(i)->AmbientColor );
			glLightfv( GL_LIGHT0+i, GL_DIFFUSE, (float*)& Lights.at(i)->DiffuseColor );
			glLightfv( GL_LIGHT0+i, GL_SPECULAR,(float*)&  Lights.at(i)->SpecularColor );
			
			glLightfv( GL_LIGHT0+i, GL_POSITION, (float*)& Lights.at(i)->FirstPosition );
			glLightfv( GL_LIGHT0+i, GL_SPOT_DIRECTION, (float*)& Lights.at(i)->SpotDirection );
			
			glLightf( GL_LIGHT0+i, GL_SPOT_EXPONENT, Lights.at(i)->SpotExponent);
			glLightf( GL_LIGHT0+i, GL_SPOT_CUTOFF, Lights.at(i)->SpotCutOff);
			
			glLightf( GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, Lights.at(i)->ConstantAttenuation );
			glLightf( GL_LIGHT0+i, GL_LINEAR_ATTENUATION, Lights.at(i)->LinearAttenuation );
			glLightf( GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, Lights.at(i)->QuadraticAttenuation );
		}

		//GEOMETRY SETUP & KEYFRAMING SETUP
		for( i=0; i<Meshes.size(); i++)
		{
			Meshes.at(i)->SetDefaultVertexColors(VertexColors);
			Meshes.at(i)->MakeNormals();

			Meshes.at(i)->ComputeHermiteTangents();
			Meshes.at(i)->ComputeBezierControls();
		}
	}



	inline void Unset(void)
	{
		uint i;

		//GLOBAL SETTINGS
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClearDepth(1.0f);

		//LIGHTNING
		glDisable( GL_LIGHTING );
		for( i=0; i<MG_LIGHT_MAX_NB; i++)
			glDisable( GL_LIGHT0+i );
	}



	inline void Play(void)
	{
		Stop = false;
		StartTime = mgTime - ElapsedTime;
	}



	inline void Pause(void)
	{
		Stop = true;
		ElapsedTime = mgTime - StartTime;
	}


	inline void SetupAndPlay()
	{
		Setup();
		Play();
	}

//-------------------------------------------------------------------

	inline void Import(char* name, uint mode)
	{
		mgSetDataAccessMode(mode);
		uint fp;
		
#ifdef MG_USE_SMALLEST_CODE
		fp = mgOpen(name);
#else
		if( (fp = mgOpen(name)) == NULL )
		{
			FatalError(strcat("3D file not found: ", name));
			return;
		}
#endif
		
		FirstFrame = mgRead32(fp);
		LastFrame = mgRead32(fp);
		FrameRate = mgRead16(fp);
		//*****CurrentFrame = 0;
		Stop = true;
		
		BackGroundColor.ReadC(fp);
		AmbientLight.ReadC(fp);
		VertexColors = (mgRead8(fp)==1);

		uint id = mgRead16(fp);
		
		while( id != EOF )
		{
			switch( id )
			{
			case MG_IMPEXP_OBJECT:
				{
				mgMesh mesh;
				mesh.Import(this, fp);
				Meshes.add(mesh);
				break;
				}
				
			case MG_IMPEXP_OBJPCD_BOX:
			case MG_IMPEXP_OBJPCD_SPHERE:
			case MG_IMPEXP_OBJPCD_CYL:
			case MG_IMPEXP_OBJPCD_TORUS:
			case MG_IMPEXP_OBJPCD_CONE:
			case MG_IMPEXP_OBJPCD_TUBE:
			case MG_IMPEXP_OBJPCD_PYRAM:
			case MG_IMPEXP_OBJPCD_PLANE:
			case MG_IMPEXP_OBJECT_INTER:
			case MG_IMPEXP_OBJECT_LATHE:
				break;
				
			case MG_IMPEXP_BONE:
				{
					mgBone bone;
					bone.Import(fp);
					break;
				}
				
			case MG_IMPEXP_MATERIAL:
				{
					mgMaterial mat;
					uint nbmat = (uint)mgRead16(fp);
					for( uint i=0; i<nbmat; i++ )
					{
						mat.Import(fp);
						Materials.add(mat);
					}
					break;
				}
				
			case MG_IMPEXP_LIGHT:
				{
				mgLight light;
				light.Import(fp);
				Lights.add(light);
				break;
				}
				
			case MG_IMPEXP_CAMERA:
				{
				mgCamera cam;
				cam.Import(fp);
				Cameras.add(cam);
				break;
				}
			}
			
			id = mgRead16(fp);
		}
	}
};


//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------



inline void mgMesh::Import(mgScene* scene, uint fp)
{
	uint i,j;
	FirstPosition.ReadV(fp);
	FirstRotation.ReadQ(fp);
	
	NumberVertex = mgRead32(fp);
	Vertices = (mgVertex*)malloc(NumberVertex * sizeof(mgVertex));	//normals stay undetermined, thus must be recalculated somewhere after
	
	for( i=0; i<NumberVertex; i++)
		Vertices[i].Vector.ReadV(fp);

	
	NumberFaces = mgRead32(fp);
	Faces = (mgFace*)malloc(NumberFaces * sizeof(mgFace));

	for( i=0; i<NumberFaces; i++)
	{
		for( j=0; j<3; j++)
		{
			Faces[i].Vertices[j] = mgRead32(fp);
		}
		
		Faces[i].Smooth = (uint)mgRead8(fp);
	}
	
	
	uint nbmeshmaps = mgRead8(fp);

#ifdef MG_USE_MULTITEXTURING
	NumberMeshMaps = nbmeshmaps;

	for( uint mp=0; mp<nbmeshmaps; mp++ )
	{
		//***** TODO
	}
#else
	NumberMappings = mgRead32(fp);
	Mappings = (mgMapping*)malloc(NumberMappings * sizeof(mgMapping));
	
	for( i=0; i<NumberMappings; i++)
	{
		Mappings[i].u = mgReadFlt(fp);
		Mappings[i].v = mgReadFlt(fp);
	}
	
	for( i=0; i<NumberFaces; i++)
	{
		for( j=0; j<3; j++)
		{
			Faces[i].Mappings[j] = mgRead32(fp);
		}
	}
#endif
	
	
	if(scene->VertexColors)
	{
		for( i=0; i<NumberVertex; i++)
			Vertices[i].Color.ReadC(fp);
	}


	ImportTracks(fp);
	
	uint8 flag = mgRead8(fp);
	if( flag )
	{
		Color.ReadC(fp);	//wire color or global object color
	}
	else
	{
		Color = mgColor(WHITE);
		Material = scene->Materials.at( mgRead16(fp) );
	}


	uint id = mgRead16(fp);
	if( id == MG_IMPEXP_MESHTRK )
		ImportMorph(fp);
	else if( id != EOF )
		mgSeek(fp, -2, SEEK_CUR);
	else
		return;
}



//END
#endif