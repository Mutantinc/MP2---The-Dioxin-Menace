//-------------------------------------------------------------------
//MG_IMPEXP.H
//-------------------------------------------------------------------
#ifndef _MG_IMPEXP_H_
#define _MG_IMPEXP_H_

//-------------------------------------------------------------------

//size of ids					16
//size of "numbers of"			16
//size of string lengths		16

//-------------------------------------------------------------------

#define MG_IMPEXP_NULL			0x0000

#define MG_IMPEXP_OBJECT		0x0110

#define MG_IMPEXP_OBJPCD_BOX	0x0120	//x1,y1,z1,x2,y2,z2
#define MG_IMPEXP_OBJPCD_SPHERE	0x0121	//x,y,z,r
#define MG_IMPEXP_OBJPCD_CYL	0x0122	//x,y,z,r,h
#define MG_IMPEXP_OBJPCD_TORUS	0x0123	//x,y,z,R,r
#define MG_IMPEXP_OBJPCD_CONE	0x0124	//x,y,z,R,r
#define MG_IMPEXP_OBJPCD_TUBE	0x0125	//x,y,z,r
#define MG_IMPEXP_OBJPCD_PYRAM	0x0126	//x1,y1,z1,x2,y2,z2,h
#define MG_IMPEXP_OBJPCD_PLANE	0x0127	//x1,y1,z1,x2,y2,z2
#define MG_IMPEXP_OBJECT_INTER	0x0128	//{x,y,z}, h, to interpolate
#define MG_IMPEXP_OBJECT_LATHE	0x0129	//{x,y,z},xa,ya,za (axis for lathe)


#define MG_IMPEXP_BONE			0x0200


#define MG_IMPEXP_MATERIAL		0x0310

#define MG_IMPEXP_TEXFILE		0x0320 //general id to reckon a texture file
#define MG_IMPEXP_TEXFILE_BMP	0x0321 //bitmap texture
#define MG_IMPEXP_TEXFILE_PCX	0x0322 //pcx texture
#define MG_IMPEXP_TEXFILE_TGA	0x0323 //targa texture
#define MG_IMPEXP_TEXFILE_JPG	0x0324 //jpeg texture

#define MG_IMPEXP_TEXDATA		0x0330 //general id to reckon texture data
#define MG_IMPEXP_TEXDATA_BMP	0x0331 //bitmap texture
#define MG_IMPEXP_TEXDATA_PCX	0x0332 //pcx texture
#define MG_IMPEXP_TEXDATA_TGA	0x0333 //targa texture
#define MG_IMPEXP_TEXDATA_JPG	0x0334 //jpeg texture

#define MG_IMPEXP_TEXPCD		0x0340	//procedural texture id base (0x0345 asks for the 5th procedure)


#define MG_IMPEXP_LIGHT			0x0400


#define MG_IMPEXP_CAMERA		0x0510
#define MG_IMPEXP_TARGET		0x0520


#define MG_IMPEXP_STRING		0x0700


#define MG_IMPEXP_TRK			0x0800
#define MG_IMPEXP_LINPOSTRK		0x0810
#define MG_IMPEXP_LINROTTRK		0x0820
#define MG_IMPEXP_LINSCATRK		0x0830

#define MG_IMPEXP_BEZPOSTRK		0x0840
#define MG_IMPEXP_BEZROTTRK		0x0850
#define MG_IMPEXP_BEZSCATRK		0x0860

#define MG_IMPEXP_TCBPOSTRK		0x0870
#define MG_IMPEXP_TCBROTTRK		0x0880
#define MG_IMPEXP_TCBSCATRK		0x0890


#define MG_IMPEXP_ROLLTRK		0x0910
#define MG_IMPEXP_FOVTRK		0x0920
#define MG_IMPEXP_MESHTRK		0x0930
#define MG_IMPEXP_IKTRK			0x0940


//END
#endif

