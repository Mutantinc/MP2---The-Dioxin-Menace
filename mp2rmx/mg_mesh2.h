//-------------------------------------------------------------------
//MG_SCENE2.H
//-------------------------------------------------------------------
#ifndef _MG_MESH2_H_
#define _MG_MESH2_H_

//state variables
mgVector mgMeshGenScale;
mgColor mgMeshGenColor;
mgColor mgMeshGenWireColor;
mgMaterial* mgMeshGenMaterial;
bool mgMeshGenSmooth;
mgVector mgMeshGenPosition;
mgQuaternion mgMeshGenRotation;

//-------------------------------------------------------------------

inline void mgMesh::SetGenScale( float scalex, float scaley, float scalez )
{
	mgMeshGenScale.x = scalex;
	mgMeshGenScale.y = scaley;
	mgMeshGenScale.z = scalez;
}

inline void mgMesh::SetGenColor( uint8 r, uint8 g, uint8 b )
{
	mgMeshGenColor = mgColor( r, g, b );
}

inline void mgMesh::SetGenColor( uint c )
{
	mgMeshGenColor = mgColor(c);
}

inline void mgMesh::SetGenWireColor( uint8 r, uint8 g, uint8 b )
{
	mgMeshGenWireColor = mgColor( r, g, b );
}

inline void mgMesh::SetGenWireColor( uint c )
{
	mgMeshGenWireColor = mgColor( c );
}

inline void mgMesh::SetGenMaterial( mgMaterial *mat, bool smooth )
{
	mgMeshGenMaterial = mat;
	mgMeshGenSmooth = smooth;
}


inline void mgMesh::SetGenPosition( float posx, float posy, float posz )
{
	mgMeshGenPosition.x = posx;
	mgMeshGenPosition.y = posy;
	mgMeshGenPosition.z = posz;
}

inline void mgMesh::SetGenRotation( float angle, float axisx, float axisy, float axisz )
{
	mgMeshGenRotation.fromAngleAxis( angle, axisx, axisy, axisz );
}


inline void mgMesh::ApplyGenParameters()	//always call at the end of the generation of the mesh
{
	Color = mgMeshGenWireColor;
	Material = mgMeshGenMaterial;
	FirstPosition = mgMeshGenPosition;
	FirstRotation = mgMeshGenRotation;
	Smooth = mgMeshGenSmooth;

	for(uint i=0; i<NumberVertex; i++)
		Vertices[i].Color = mgMeshGenColor;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

inline void mgMesh::GenCube( float base )
{
	Allocate( 8, 12 );

	float dimx = base/2*mgMeshGenScale.x;
	float dimy = base/2*mgMeshGenScale.y;
	float dimz = base/2*mgMeshGenScale.z;

	Vertices[ 0].Vector = mgVector( dimx, dimy, dimz);
	Vertices[ 1].Vector = mgVector( dimx,-dimy, dimz);
	Vertices[ 2].Vector = mgVector(-dimx,-dimy, dimz);
	Vertices[ 3].Vector = mgVector(-dimx, dimy, dimz);
	Vertices[ 4].Vector = mgVector( dimx, dimy,-dimz);
	Vertices[ 5].Vector = mgVector( dimx,-dimy,-dimz);
	Vertices[ 6].Vector = mgVector(-dimx,-dimy,-dimz);
	Vertices[ 7].Vector = mgVector(-dimx, dimy,-dimz);

	Faces[ 0].SetV( 2, 1, 0 );
	Faces[ 1].SetV( 2, 0, 3 );
	Faces[ 2].SetV( 3, 0, 4 );
	Faces[ 3].SetV( 3, 4, 7 );
	Faces[ 4].SetV( 4, 7, 5 );
	Faces[ 5].SetV( 5, 7, 6 );
	Faces[ 6].SetV( 5, 6, 1 );
	Faces[ 7].SetV( 1, 6, 2 );
	Faces[ 8].SetV( 2, 6, 3 );
	Faces[ 9].SetV( 3, 6, 7 );
	Faces[10].SetV( 4, 5, 0 );
	Faces[11].SetV( 0, 5, 1 );

	ApplyGenParameters();
}

inline void mgMesh::GenCube2( float base, float cfd, float cfh )	//a chamferred cube, cfd is the reduced-base, not the shift
{
	Allocate( 32, 60 );

	float dimx = base/2*mgMeshGenScale.x;
	float dimy = base/2*mgMeshGenScale.y;
	float dimz = base/2*mgMeshGenScale.z;
	float cfw = (base-cfd)/2.f;

	Vertices[ 0].Vector = mgVector( dimx,  dimy,  dimz);
	Vertices[ 1].Vector = mgVector( dimx, -dimy,  dimz);
	Vertices[ 2].Vector = mgVector(-dimx, -dimy,  dimz);
	Vertices[ 3].Vector = mgVector(-dimx,  dimy,  dimz);
	Vertices[ 4].Vector = mgVector( dimx,  dimy, -dimz);
	Vertices[ 5].Vector = mgVector( dimx, -dimy, -dimz);
	Vertices[ 6].Vector = mgVector(-dimx, -dimy, -dimz);
	Vertices[ 7].Vector = mgVector(-dimx,  dimy, -dimz);

	Vertices[ 8].Vector = mgVector( dimx-cfw, dimy-cfw, dimz+cfh);
	Vertices[ 9].Vector = mgVector(-dimx+cfw, dimy-cfw, dimz+cfh);
	Vertices[10].Vector = mgVector(-dimx+cfw,-dimy+cfw, dimz+cfh);
	Vertices[11].Vector = mgVector( dimx-cfw,-dimy+cfw, dimz+cfh);

	Vertices[12].Vector = mgVector( dimx-cfw,-dimy-cfh, dimz-cfw);
	Vertices[13].Vector = mgVector(-dimx+cfw,-dimy-cfh, dimz-cfw);
	Vertices[14].Vector = mgVector(-dimx+cfw,-dimy-cfh,-dimz+cfw);
	Vertices[15].Vector = mgVector( dimx-cfw,-dimy-cfh,-dimz+cfw);

	Vertices[16].Vector = mgVector( dimx-cfw,-dimy+cfw,-dimz-cfh);
	Vertices[17].Vector = mgVector(-dimx+cfw,-dimy+cfw,-dimz-cfh);
	Vertices[18].Vector = mgVector(-dimx+cfw, dimy-cfw,-dimz-cfh);
	Vertices[19].Vector = mgVector( dimx-cfw, dimy-cfw,-dimz-cfh);

	Vertices[20].Vector = mgVector( dimx-cfw, dimy+cfh,-dimz+cfw);
	Vertices[21].Vector = mgVector(-dimx+cfw, dimy+cfh,-dimz+cfw);
	Vertices[22].Vector = mgVector(-dimx+cfw, dimy+cfh, dimz-cfw);
	Vertices[23].Vector = mgVector( dimx-cfw, dimy+cfh, dimz-cfw);

	Vertices[24].Vector = mgVector( dimx+cfh, dimy-cfw,-dimz+cfw);
	Vertices[25].Vector = mgVector( dimx+cfh, dimy-cfw, dimz-cfw);
	Vertices[26].Vector = mgVector( dimx+cfh,-dimy+cfw, dimz-cfw);
	Vertices[27].Vector = mgVector( dimx+cfh,-dimy+cfw,-dimz+cfw);

	Vertices[28].Vector = mgVector(-dimx-cfh, dimy-cfw, dimz-cfw);
	Vertices[29].Vector = mgVector(-dimx-cfh, dimy-cfw,-dimz+cfw);
	Vertices[30].Vector = mgVector(-dimx-cfh,-dimy+cfw,-dimz+cfw);
	Vertices[31].Vector = mgVector(-dimx-cfh,-dimy+cfw, dimz-cfw);


	Faces[ 0].SetV( 8, 0, 3);
	Faces[ 1].SetV( 8, 3, 9);
	Faces[ 2].SetV( 9, 3, 2);
	Faces[ 3].SetV( 9, 2,10);
	Faces[ 4].SetV(10, 2, 1);
	Faces[ 5].SetV(10, 1,11);
	Faces[ 6].SetV(11, 1, 0);
	Faces[ 7].SetV(11, 0, 8);
	Faces[ 8].SetV( 8, 9,11);
	Faces[ 9].SetV( 11,9,10);

	Faces[10].SetV(12, 1, 2);
	Faces[11].SetV(12, 2,13);
	Faces[12].SetV(13, 2, 6);
	Faces[13].SetV(13, 6,14);
	Faces[14].SetV(14, 6, 5);
	Faces[15].SetV(14, 5,15);
	Faces[16].SetV(15, 5, 1);
	Faces[17].SetV(15, 1,12);
	Faces[18].SetV(12,13,15);
	Faces[19].SetV(15,13,14);

	Faces[20].SetV(16, 5, 6);
	Faces[21].SetV(16, 6,17);
	Faces[22].SetV(17, 6, 7);
	Faces[23].SetV(17, 7,18);
	Faces[24].SetV(18, 7, 4);
	Faces[25].SetV(18, 4,19);
	Faces[26].SetV(19, 4, 5);
	Faces[27].SetV(19, 5,16);
	Faces[28].SetV(16,17,19);
	Faces[29].SetV(19,17,18);

	Faces[30].SetV(20, 4, 7);
	Faces[31].SetV(20, 7,21);
	Faces[32].SetV(21, 7, 3);
	Faces[33].SetV(21, 3,22);
	Faces[34].SetV(22, 3, 0);
	Faces[35].SetV(22, 0,23);
	Faces[36].SetV(23, 0, 4);
	Faces[37].SetV(23, 4,20);
	Faces[38].SetV(20,21,23);
	Faces[39].SetV(23,21,22);

	Faces[40].SetV(24, 4, 0);
	Faces[41].SetV(24, 0,25);
	Faces[42].SetV(25, 0, 1);
	Faces[43].SetV(25, 1,26);
	Faces[44].SetV(26, 1, 5);
	Faces[45].SetV(26, 5,27);
	Faces[46].SetV(27, 5, 4);
	Faces[47].SetV(27, 4,24);
	Faces[48].SetV(24,25,27);
	Faces[49].SetV(27,25,26);

	Faces[50].SetV(28, 3, 7);
	Faces[51].SetV(28, 7,29);
	Faces[52].SetV(29, 7, 6);
	Faces[53].SetV(29, 6,30);
	Faces[54].SetV(30, 6, 2);
	Faces[55].SetV(30, 2,31);
	Faces[56].SetV(31, 2, 3);
	Faces[57].SetV(31, 3,28);
	Faces[58].SetV(28,29,31);
	Faces[59].SetV(31,29,30);

	ApplyGenParameters();
}


/*
inline void mgMesh::GenParrallelepiped( float dimX, float dimY, float dimZ )
{
	SetGenScale( dimX, dimY, dimZ );
	GenCube( 1.0f );
}
*/

//-------------------------------------------------------------------

inline void mgMesh::GenPyramid( float base, float top, float height )
{
	Allocate( 8, 12 );

	float bdimx = base/2*mgMeshGenScale.x;
	float bdimz = base/2*mgMeshGenScale.z;

	float tdimx = top/2*mgMeshGenScale.x;
	float tdimz = top/2*mgMeshGenScale.z;

	Vertices[ 0].Vector = mgVector( tdimx, height, tdimz);
	Vertices[ 1].Vector = mgVector( bdimx,   0.0f, bdimz);
	Vertices[ 2].Vector = mgVector(-bdimx,   0.0f, bdimz);
	Vertices[ 3].Vector = mgVector(-tdimx, height, tdimz);
	Vertices[ 4].Vector = mgVector( tdimx, height,-tdimz);
	Vertices[ 5].Vector = mgVector( bdimx,   0.0f,-bdimz);
	Vertices[ 6].Vector = mgVector(-bdimx,   0.0f,-bdimz);
	Vertices[ 7].Vector = mgVector(-tdimx, height,-tdimz);

	Faces[ 0].SetV( 2, 1, 0 );
	Faces[ 1].SetV( 2, 0, 3 );
	Faces[ 2].SetV( 3, 0, 4 );
	Faces[ 3].SetV( 3, 4, 7 );
	Faces[ 4].SetV( 4, 7, 5 );
	Faces[ 5].SetV( 5, 7, 6 );
	Faces[ 6].SetV( 5, 6, 1 );
	Faces[ 7].SetV( 1, 6, 2 );
	Faces[ 8].SetV( 2, 3, 6 );
	Faces[ 9].SetV( 3, 7, 6 );
	Faces[10].SetV( 5, 4, 0 );
	Faces[11].SetV( 1, 5, 0 );

	ApplyGenParameters();
}

//-------------------------------------------------------------------

inline void mgMesh::GenCylinder( float radius, float height )
{
	GenCone( radius, radius, height, 24 );
}

inline void mgMesh::GenCone( float base, float top, float height )
{
	GenCone( base, top, height, 24 );
}

inline void mgMesh::GenCone( float base, float top, float height, uint slices )
{
	float angle; uint i;

	if( top == 0.f || base == 0.f )
	{
		float h2, b2;
		Allocate( slices+1, slices );

		if( top == 0.f )
		{ h2 = height; b2 = base; }
		else
		{ h2 = 0.f; b2 = top; }

		for( i=0; i<slices; i++ )
		{
			angle = i*mg2Pi/slices;
			Vertices[i].Vector = mgVector( b2*cos(angle), height-h2, b2*sin(angle));
			Faces[i].SetV( slices, i, (i+1)%slices );
		}

		Vertices[slices].Vector = mgVector( 0.f, h2, 0.f );
	}
	else
	{
		Allocate( 2*slices, 2*slices );
		
		for( i=0; i<2*slices; i=i+2 )
		{
			angle = i*mgPi/slices;
			Vertices[i].Vector = mgVector( base*cos(angle), 0.f, base*sin(angle) );
			Vertices[i+1].Vector = mgVector( top*cos(angle), height, top*sin(angle) );

			Faces[i].SetV( i, i+1, (i+2)%(2*slices) );
			Faces[i+1].SetV( (i+3)%(2*slices), (i+2)%(2*slices), i+1 );
		}
	}

	ApplyGenParameters();
}


//-------------------------------------------------------------------

inline void mgMesh::GenIcosahedron( float radius )
{
	Allocate( 12, 20 );

#define tau 0.8506508084f	/* t=(1+sqrt(5))/2, tau=t/sqrt(1+t^2)  */
#define one 0.5257311121f	/* one=1/sqrt(1+t^2) , unit sphere     */

	float tauR = tau*radius;
	float oneR = one*radius;

	Vertices[ 0].Vector = mgVector( tauR, oneR,   0 );
	Vertices[ 1].Vector = mgVector(-tauR, oneR,   0 );
	Vertices[ 2].Vector = mgVector(-tauR,-oneR,   0 );
	Vertices[ 3].Vector = mgVector( tauR,-oneR,   0 );
	Vertices[ 4].Vector = mgVector( oneR,  0 , tauR );
	Vertices[ 5].Vector = mgVector( oneR,  0 ,-tauR );
	Vertices[ 6].Vector = mgVector(-oneR,  0 ,-tauR );
	Vertices[ 7].Vector = mgVector(-oneR,  0 , tauR );
	Vertices[ 8].Vector = mgVector(  0 , tauR, oneR );
	Vertices[ 9].Vector = mgVector(  0 ,-tauR, oneR );
	Vertices[10].Vector = mgVector(  0 ,-tauR,-oneR );
	Vertices[11].Vector = mgVector(  0 , tauR,-oneR );

    Faces[ 0].SetV( 4, 8, 7 );
    Faces[ 1].SetV( 4, 7, 9 );
    Faces[ 2].SetV( 5, 6, 11 );
    Faces[ 3].SetV( 5, 10, 6 );
    Faces[ 4].SetV( 0, 4, 3 );
    Faces[ 5].SetV( 0, 3, 5 );
    Faces[ 6].SetV( 2, 7, 1 );
    Faces[ 7].SetV( 2, 1, 6 );
    Faces[ 8].SetV( 8, 0, 11 );
    Faces[ 9].SetV( 8, 11, 1 );
    Faces[10].SetV( 9, 10, 3 );
    Faces[11].SetV( 9, 2, 10 );
    Faces[12].SetV( 8, 4, 0 );
    Faces[13].SetV( 11, 0, 5 );
    Faces[14].SetV( 4, 9, 3 );
    Faces[15].SetV( 5, 3, 10 );
    Faces[16].SetV( 7, 8, 1 );
    Faces[17].SetV( 6, 1, 11 );
    Faces[18].SetV( 7, 2, 9 );
    Faces[19].SetV( 6, 10, 2 );

	ApplyGenParameters();
}


inline void mgMesh::GenSphere( float radius, uint divs )	//divs is the number of iterations of the division per 4 of each triangle
{
	GenIcosahedron(radius);
	uint i, j, k;
	
	for( i=0; i<divs; i++ )
	{
		mgVertex* newvs = (mgVertex*)malloc( sizeof(mgVertex)*(NumberVertex+3*NumberFaces));
		mgFace* newfs = (mgFace*)malloc( sizeof(mgFace)*4*NumberFaces);
		
		for( j=0; j<NumberVertex; j++ )
			newvs[j].Vector = Vertices[j].Vector;

		for( j=0; j<NumberFaces; j++)
		{
			for( k=0; k<3; k++)
			{
				newvs[NumberVertex+3*j+k].Vector = Vertices[Faces[j].Vertices[k]].Vector + Vertices[Faces[j].Vertices[(k+1)%3]].Vector;
				newvs[NumberVertex+3*j+k].Vector.Normalise();
				newvs[NumberVertex+3*j+k].Vector*=radius;

				newfs[4*j+k].Vertices[0] = Faces[j].Vertices[k];
				newfs[4*j+k].Vertices[1] = NumberVertex+3*j+k;
				newfs[4*j+k].Vertices[2] = NumberVertex+3*j+(k+2)%3;
			}

			newfs[4*j+3].Vertices[0] = NumberVertex+3*j;
			newfs[4*j+3].Vertices[1] = NumberVertex+3*j+1;
			newfs[4*j+3].Vertices[2] = NumberVertex+3*j+2;
		}

		RELEASE(Vertices); Vertices = newvs; NumberVertex+=3*NumberFaces;
		RELEASE(Faces); Faces = newfs; NumberFaces*=4;
	}

	ApplyGenParameters();
}



inline void mgMesh::GenSphereDgnr( float radius, uint level, uint dgnr )	//level: nb it dgrn iterations, dgrn: 0->chewed sphere, 1->spike, 2->dirty isoca (with smoothing), 3->some normals inversed, 4->exploded
{
	GenIcosahedron(radius);
	if( dgnr > 4 )
		return;

	uint i, j, k;
	for( i=0; i<level; i++ )	//then we subdivide it
	{
		if( dgnr > 2 )
		{
			mgVertex* newvs = (mgVertex*)malloc( sizeof(mgVertex)*(NumberVertex+3*NumberFaces));
			mgFace* newfs = (mgFace*)malloc( sizeof(mgFace)*4*NumberFaces);
			
			for( j=0; j<NumberVertex; j++ )
				newvs[j].Vector = Vertices[j].Vector;
			

			if( dgnr  == 3 )
			{
				for( j=0; j<NumberFaces; j++)
				{
					for( k=0; k<3; k++)
					{
						newvs[NumberVertex+3*j+k].Vector = Vertices[Faces[j].Vertices[k]].Vector + Vertices[Faces[j].Vertices[(k+1)%3]].Vector;
						newvs[NumberVertex+3*j+k].Vector.Normalise();
						newvs[NumberVertex+3*j+k].Vector*=radius;
						
						newfs[4*j+k].Vertices[0] = Faces[j].Vertices[k];
						newfs[4*j+k].Vertices[1] = NumberVertex+3*j+(k+2)%3;
						newfs[4*j+k].Vertices[2] = NumberVertex+3*j+k;
					}
					
					newfs[4*j+3].Vertices[0] = NumberVertex+3*j;
					newfs[4*j+3].Vertices[1] = NumberVertex+3*j+1;
					newfs[4*j+3].Vertices[2] = NumberVertex+3*j+2;
				}
			}
			else if( dgnr == 4 )
			{
				for( j=0; j<NumberFaces; j++)
				{
					for( k=0; k<3; k++)
					{
						newvs[NumberVertex+3*j+k].Vector = Vertices[Faces[j].Vertices[k]].Vector + Vertices[Faces[j].Vertices[(k+1)%3]].Vector;
						newvs[NumberVertex+3*j+k].Vector.Normalise();
						newvs[NumberVertex+3*j+k].Vector*=radius;
						
						newfs[4*j+k].Vertices[0] = Faces[j].Vertices[k];
						newfs[4*j+k].Vertices[1] = Faces[j].Vertices[(k+1)%3];
						newfs[4*j+k].Vertices[2] = NumberVertex+3*j+k;
					}
					
					newfs[4*j+3].Vertices[0] = NumberVertex+3*j;
					newfs[4*j+3].Vertices[1] = NumberVertex+3*j+1;
					newfs[4*j+3].Vertices[2] = NumberVertex+3*j+2;
				}
			}

			RELEASE(Vertices); Vertices = newvs; NumberVertex+=3*NumberFaces;
			RELEASE(Faces); Faces = newfs; NumberFaces*=4;
		}
		else
		{
			mgVertex* newvs = (mgVertex*)malloc( sizeof(mgVertex)*(NumberVertex+NumberFaces));
			mgFace* newfs = (mgFace*)malloc( sizeof(mgFace)*3*NumberFaces);
			
			for( uint j=0; j<NumberVertex; j++ )
				newvs[j].Vector = Vertices[j].Vector;
			
			if( dgnr == 0 )
			{
				for( j=0; j<NumberFaces; j++ )
				{
					newvs[NumberVertex+j].Vector = (Vertices[Faces[j].Vertices[0]].Vector+Vertices[Faces[j].Vertices[1]].Vector+Vertices[Faces[j].Vertices[2]].Vector);
					newvs[NumberVertex+j].Vector.Normalise();
					newvs[NumberVertex+j].Vector*=radius;
					newfs[3*j  ].SetV( Faces[j].Vertices[0], Faces[j].Vertices[1], NumberVertex+j );
					newfs[3*j+1].SetV( Faces[j].Vertices[1], Faces[j].Vertices[2], NumberVertex+j );
					newfs[3*j+2].SetV( Faces[j].Vertices[2], Faces[j].Vertices[0], NumberVertex+j );
				}
			}

			else if( dgnr == 1 )
			{
				for( j=0; j<NumberFaces; j++ )
				{
					newvs[NumberVertex+j].Vector = (Vertices[Faces[j].Vertices[0]].Vector+Vertices[Faces[j].Vertices[1]].Vector+Vertices[Faces[j].Vertices[2]].Vector);
					newvs[NumberVertex+j].Vector*=(1.f/radius);
					newfs[3*j  ].SetV( Faces[j].Vertices[0], Faces[j].Vertices[1], NumberVertex+j );
					newfs[3*j+1].SetV( Faces[j].Vertices[1], Faces[j].Vertices[2], NumberVertex+j );
					newfs[3*j+2].SetV( Faces[j].Vertices[2], Faces[j].Vertices[0], NumberVertex+j );
				}
			}

			else if( dgnr == 2 )
			{
				for( j=0; j<NumberFaces; j++ )
				{
					newvs[NumberVertex+j].Vector = (Vertices[Faces[j].Vertices[0]].Vector+Vertices[Faces[j].Vertices[1]].Vector+Vertices[Faces[j].Vertices[2]].Vector)/3.f;
					newfs[3*j  ].SetV( Faces[j].Vertices[0], Faces[j].Vertices[1], NumberVertex+j );
					newfs[3*j+1].SetV( Faces[j].Vertices[1], Faces[j].Vertices[2], NumberVertex+j );
					newfs[3*j+2].SetV( Faces[j].Vertices[2], Faces[j].Vertices[0], NumberVertex+j );
				}
			}
				
			RELEASE(Vertices); Vertices = newvs; NumberVertex+=NumberFaces;
			RELEASE(Faces); Faces = newfs; NumberFaces*=3;
		}
	}
	ApplyGenParameters();
}



inline void mgMesh::GenGlenz( float radius )
{/******todo*/}


//-------------------------------------------------------------------

inline void mgMesh::GenTorus( float inner, float section, uint vdivs, uint hdivs )
{
	Allocate( hdivs*vdivs, 2*hdivs*vdivs );

	float delta1 = mg2Pi/hdivs, delta2 = mg2Pi/vdivs;
	float alpha, beta, cosa, sina, x, x1, y1, z1;
	uint i, j;

	for( i=0; i<hdivs; i++ )
	{
		alpha = i*delta1; cosa = cos(alpha); sina = sin(alpha);

		for( j=0; j<vdivs; j++ )
		{
			beta = j*delta2;
			x = (inner+section)+section*cos(beta);		//y=0
			x1=cosa*x; y1=sina*x; z1=section*sin(beta);	//z1=z
			Vertices[i*vdivs+j].Vector = mgVector( x1, y1, z1 );
			Faces[2*(i*vdivs+j)].SetV( (i+1)%hdivs*vdivs+j, (i+1)%hdivs*vdivs+(j+1)%vdivs, i*vdivs+(j+1)%vdivs );
			Faces[2*(i*vdivs+j)+1].SetV( (i+1)%hdivs*vdivs+j, i*vdivs+(j+1)%vdivs, i*vdivs+j );
		}
	}
	ApplyGenParameters();
}


//-------------------------------------------------------------------

inline void mgMesh::GenFish()
{/******todo*/}

inline void mgMesh::GenBird()
{/******todo*/}

//-------------------------------------------------------------------

inline void mgMesh::GenStar( float inner, float outer, float start )	//start in degrees. pentacle:x=X*sin(18)/cos(36)
{																		//becomes a pentagon if inner > outer
	Allocate( 10, 8 );
	uint i;

	start = DEG_TO_RAD(start);
	for( i=0; i<10; )
	{
		Vertices[i++].Vector = mgVector( outer*cos(start), outer*sin(start), 0.f );
		start += mgPi/5;
		Vertices[i++].Vector = mgVector( inner*cos(start), inner*sin(start), 0.f );
		start += mgPi/5;
	}
	
	for( i=0; i<5; i++ )
		Faces[i].SetV( 2*i+1, (2*i+2)%10, (2*i+3)%10 );

	Faces[5].SetV( 1, 3, 5 );
	Faces[6].SetV( 5, 7, 9 );
	Faces[7].SetV( 1, 5, 9 );

	ApplyGenParameters();
}

//-------------------------------------------------------------------

inline void mgMesh::GenDisk( float radius )
{
	GenDisk(radius, 20);
}

inline void mgMesh::GenDisk( float radius, uint divs )
{
	Allocate( divs+1, divs );

	uint i;
	for( i=0; i<divs; i++ )
		Vertices[i].Vector = mgVector( radius*cos(i*mg2Pi/divs), radius*sin(i*mg2Pi/divs), 0.f );

	Vertices[divs].Vector = mgVector( 0.f, 0.f, 0.f );

	for( i=0; i<divs; i++)
		Faces[i].SetV( divs, i, (i+1)%divs );

	ApplyGenParameters();
}

inline void mgMesh::GenDisk( float inner, float outer, uint slices )
{
	if( inner != 0.f )
		GenDisk( inner, outer, slices, 0, 360);
	else
		GenDisk(outer, slices);
}

inline void mgMesh::GenDisk( float inner, float outer, uint slices, float start, float sweep )
{
	float slide = mg2Pi/slices;
	uint tax1=0, tax2=0;
	if( start/slide != (float)((sint)start/(sint)slide) )
		tax1++;
	if( (start+sweep)/slide != (float)((sint)(start+sweep)/(sint)slide) )
		tax2++;

	uint nbvf = 2*((uint)(start+sweep)/(uint)slide-(uint)start/(uint)slide+tax1+tax2);
	Allocate( nbvf, nbvf );
	ApplyGenParameters();

	start = DEG_TO_RAD(start);
	sweep = DEG_TO_RAD(sweep);

	uint i;
	if( tax1 )
	{
		Vertices[0].Vector = mgVector( inner*cos(start), inner*sin(start), 0.f );
		Vertices[1].Vector = mgVector( outer*cos(start), outer*sin(start), 0.f );
	}

	//*****marche pas
	for( i=(uint)(start/slide)+1; i<(uint)((start+sweep)/slide); i++ )
	{
		Vertices[2*i].Vector = mgVector( inner*cos(i*slide), inner*sin(i*slide), 0.f );
		Vertices[2*i+1].Vector = mgVector( outer*cos(i*slide), outer*sin(i*slide), 0.f );
	}

	if( tax2 )
	{
		Vertices[2*i+2].Vector = mgVector( inner*cos(start+sweep), inner*sin(start+sweep), 0.f );
		Vertices[2*i+3].Vector = mgVector( outer*cos(start+sweep), outer*sin(start+sweep), 0.f );
	}

	for( i=0; i<nbvf; i=i+2 )
	{
		Faces[i].SetV( i, i+1, (i+3)%nbvf );
		Faces[i+1].SetV( (i+3)%nbvf, (i+2)%nbvf, i );
	}
}

//-------------------------------------------------------------------

inline void mgMesh::GenPlane( float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4 )
{
	Allocate( 4, 2 );
	
	Vertices[0].Vector = mgVector( x1, y1, z1 );
	Vertices[1].Vector = mgVector( x2, y2, z2 );
	Vertices[2].Vector = mgVector( x3, y3, z3 );
	Vertices[3].Vector = mgVector( x4, y4, z4 );
	
	Faces[0].SetV( 2, 1, 0 );
	Faces[1].SetV( 0, 2, 3 );

	ApplyGenParameters();
}


//-------------------------------------------------------------------


//END
#endif