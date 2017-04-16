//-------------------------------------------------------------------
//MG_TEXTURE.H
//-------------------------------------------------------------------
// Warning if DirectX is utilized:
// the PITCHes (number of bytes per scanline > RGB(A)*Width) on DX surfaces
// are weirdly aligned and they should be retreived with GetSurfaceDesc then ddsd.lPitch
//-------------------------------------------------------------------
// Warning if OpenGL is utilized:
// the PITCHes are aligned on 4 PIXELS by default
//-------------------------------------------------------------------
// ALPHA: the less, the more transparent
//-------------------------------------------------------------------

#ifndef _MG_TEXTURE_H_
#define _MG_TEXTURE_H_

#ifdef MG_USE_GIF
#include "../misc/lzw_dec.h"		//for GIFs
#endif

#ifdef MG_USE_JPG
#include "jpeg/ld_jpg.h"		//for JPEGs
#endif

#define MG_RGB	GL_RGB
#define MG_RGBA GL_RGBA

//-------------------------------------------------------------------

//typedef  AUX_RGBImageRec	mgImage;			//using OpenGL type

/*
typedef struct {
	int width, height;
    unsigned char *data;
} mgImage;
*/

//-------------------------------------------------------------------

class mgTexture
{
public:
	uint ID;			//handle when transfered into gfx card memory
	uint8* Data;		//raw datas are there
	sint Width;
	sint Height;
	uint Format;		//...MG_RGB->24b, MG_RGBA->32b
	bool UTiling;
	bool VTiling;
	bool Filtering;
	
	
	inline mgTexture()
	{
		ID = 0;
		Data = NULL;
		UTiling = false;
		VTiling = false;
		Filtering = false;
	}


	inline ~mgTexture()
	{
		//RELEASE(Image); //*****fait une erreur à la fin, va savoir pourquoi, idem avec 'delete'
	}

//-------------------------------------------------------------------
	
	inline void FilteringOff()
	{
		Filtering = false;
	}

	inline void FilteringOn()
	{
		Filtering = true;
	}

	inline void Tiling( bool h, bool v )
	{
		UTiling = h;
		VTiling = v;
	}

//-------------------------------------------------------------------

	inline void ScaleImage( sint width, sint height )
	{
		if( width == Width && height == Height )
			return;

		uint depth;
		if( Format == MG_RGBA )
			depth = 4;
		else if( Format == MG_RGB )
			depth = 3;
		else
			return;

		uint8* newimage = (uint8*)malloc(width*height*depth+1);

		int i, j, k=0;
		for( j=0; j<height; j++ )
			for( i=0; i<width; i++ )
			{
				*(uint*)&(newimage[k]) = *(uint*)&(Data[depth*(((j*Height)/height) * Width + ((i*Width)/width))]);
				k+=depth;
			}


//		too bad this glu function is so slow
//		gluScaleImage( mode, Width, Height, GL_UNSIGNED_BYTE, Data, width, height, GL_UNSIGNED_BYTE, newimage );
		
		RELEASE(Data);
		Data = newimage;
		Width = width;
		Height = height;
	}



	inline void BindNoRelease(void)
	{
		uint ox = Width, oy = Height;
		uint nx = pow2over(Width), ny = pow2over(Height);
		ScaleImage( nx, ny );
		Width = ox; Height = oy;

		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		if( Format == MG_RGBA )
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, nx, ny, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		else if( Format == MG_RGB )
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, nx, ny, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		//else if ...
		//{}

		if( Filtering)
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		}
		else
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		}
		
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, UTiling ? GL_REPEAT : GL_CLAMP );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, VTiling ? GL_REPEAT : GL_CLAMP );
	}
	

	inline void Bind(void)
	{
		if( Data )
		{
			BindNoRelease();
			RELEASE( Data );
		}
	}

	inline void Unbind(void)	//releases gfx card datas only
	{
		glDeleteTextures(1, &ID);
	}
	
	
//-------------------------------------------------------------------
//Import functions for exported 3D files
//***** maybe merge with LoadSwitch
//-------------------------------------------------------------------
	inline void Import(uint fp)
	{
		uint16 textype = mgRead16(fp);
		uint texsize;
		char* texname;

		if( (textype & MG_IMPEXP_TEXFILE) == MG_IMPEXP_TEXFILE )
		{
			texsize = mgRead16(fp);
			texname = (char*)calloc(1, texsize+1);
			mgRead(texname, texsize, fp);
		}
		else if( (textype & MG_IMPEXP_TEXDATA) == MG_IMPEXP_TEXDATA )
		{
			mgDataVirtualLength = mgRead32(fp);		//in case we have MG_DATA_INCLUDED later...
		}


		if( textype == MG_IMPEXP_TEXDATA_BMP )
			LoadBMP((char*)fp, MG_DATA_INCLUDED );
		else if( textype == MG_IMPEXP_TEXDATA_PCX )
			LoadPCX((char*)fp, MG_DATA_INCLUDED );
		else if( textype == MG_IMPEXP_TEXDATA_TGA )
			LoadTGA((char*)fp, MG_DATA_INCLUDED );
#ifdef MG_USE_JPG
		else if( textype == MG_IMPEXP_TEXDATA_JPG )
			LoadJPG((char*)fp, MG_DATA_INCLUDED );
#endif

#ifndef MG_USE_SMALLEST_CODE
		else if( textype == MG_IMPEXP_TEXFILE_BMP )
			LoadBMP( texname, MG_DATA_FILE );
		else if( textype == MG_IMPEXP_TEXFILE_PCX )
			LoadPCX( texname, MG_DATA_FILE );
		else if( textype == MG_IMPEXP_TEXFILE_TGA )
			LoadTGA( texname, MG_DATA_FILE );
#ifdef MG_USE_JPG
		else if( textype == MG_IMPEXP_TEXFILE_JPG )
			LoadJPG( texname, MG_DATA_FILE );
#endif
#endif

		else if( (textype & MG_IMPEXP_TEXPCD) == MG_IMPEXP_TEXPCD )
			{
				uint procnumber = textype - MG_IMPEXP_TEXPCD;
				//***** to be continued
			}

		//else {}	//not absolutely necessary

		RELEASE(texname);

		UTiling = (mgRead8(fp) != 0);
		VTiling = (mgRead8(fp) != 0);
		Filtering = (mgRead8(fp) != 0);
		Bind();
	}
	




//-------------------------------------------------------------------
//BMP Loader (24 -> 24)
//-------------------------------------------------------------------
	inline void LoadBMP(char* name, uint mode)	//only MG_DATA_FILE
	{
#ifdef MG_USE_SMALLEST_CODE
		//no need to load heavy bitmaps when trying to generate a smaller code
#else
		mgSetDataAccessMode( mode );
		uint handle = (uint)mgOpen( name );

		if( handle == NULL)
		{
			Error( strcat( "GFX file not found: ", name ));
			return; //NULL;
		}
		else
		{
			//***** i must do my own bmp decoder (the OpenGL decoder gives upside-down images)
			mgClose( handle );
			AUX_RGBImageRec *image = new AUX_RGBImageRec;
			image = auxDIBImageLoad(name);
			Data = (uint8*)image->data;
			Width = image->sizeX;
			Height = image->sizeY;
			Format = MG_RGB;
		}
#endif
	}
	
	
	
	
	
//-------------------------------------------------------------------
//PCX Loader (265 cols -> 32 bits)
//-------------------------------------------------------------------
	inline void LoadPCX(char* name, uint mode)
	{
		mgSetDataAccessMode(mode);
		sint val1, val2;
		sint i, j, k;
		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if( (fp=mgOpen(name)) == NULL )
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif

		mgSeek(fp,2,SEEK_SET);	uint8 comp=mgGetc(fp);
		mgSeek(fp,10,SEEK_SET);	uint off = mgRead16(fp);	mgSeek(fp,6,SEEK_SET);	Height=off-mgRead16(fp)+1;
		off = mgRead16(fp);	mgSeek(fp,4,SEEK_SET);	Width=off-mgRead16(fp)+1;
		mgSeek(fp,66,SEEK_SET);	sint scan = mgRead16(fp);	//even
		
		Data = (uint8*)malloc( (Width*Height+1)*4 );	//+4 bytes (+1*4) because the last pixel may be the pixel from the edge of the last scanline and it would overwrite the int just after the malloc
		
		uint8 pal[769];	//256*(r,g,b)+1 (+1 because i always read the byte after the color to have a 32 bits color which can lead to reading a byte outside the pal buffer for the last color)
		mgSeek(fp,-768,SEEK_END);
		mgRead(pal,768,fp);
		
		mgSeek(fp,0x80L,SEEK_SET);	//header size:128
		
		if(comp)
		{
			for( k=0; k<Height; k++ )
			{
				for( i=Width*k; i<Width*k+scan; )	//recalculating here ensures we are on the good pixel even if there are artefacts due to scan
				{
					val1 = mgGetc(fp);
					if( (val1 & 0xC0)==0xC0 )
					{
						val1 -= 0xC0;
						val2 = mgRead8(fp)*3;

						for( j=0; j<val1; j++ )
							if( i<Width*(k+1) )
								((uint32*)Data)[i++]=*((uint32*)&pal[val2]) | 0xFF000000;
							else
								i++;
					}
					else
						((uint32*)Data)[i++]=*((uint32*)&pal[val1*3]) | 0xFF000000;
				}
			}
		}
		else	
		{
			for( k=0; k<Height; k++ )
				for( i=Width*k; i<Width*k+scan; )
					((uint32*)Data)[i++]=*((uint32*)&pal[mgRead8(fp)*3]) | 0xFF000000;
		}

		mgClose(fp);
		Format = MG_RGBA;
	}



	
	
//-------------------------------------------------------------------
//TGA Loader (24/32 bits -> 24/32 bits)
//-------------------------------------------------------------------
	inline void LoadTGA(char* name, uint mode)
	{
		mgSetDataAccessMode(mode);
		uint cnt; static uint val;
		int i,j, k, k2, k3;
		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if ((fp=mgOpen(name))==NULL)
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif
		
		uint head = mgRead8(fp);
		mgSeek(fp,1,SEEK_CUR);	uint comp = mgRead8(fp);
		mgSeek(fp,9,SEEK_CUR);
		Width = mgRead16(fp);
		Height = mgRead16(fp);
		uint depth = mgRead8(fp)>>3;	// >>3 is /8
		
		Data = (uint8*)malloc(Width*Height*depth+1);	//+1 because of the last alpha value written
		
		if( mgRead8(fp) & 0x40 )
		{
			k = Height;
			k2 = 0;
			k3 = -1;
		}
		else
		{
			k = 0;
			k2 = Height;
			k3 = 1;
		}


		if(head!=0)
			mgSeek(fp,head,SEEK_SET);

		
		if( depth == 3 )	//RGB
		{
			Format = MG_RGB;
			if( comp==10 )
			{	
				i=3*Width*k;
				
				while( k != k2 )
				{
					cnt = mgGetc(fp);
					if( (cnt&0x80)==0x80 )
					{
						cnt-=0x80;
						
						((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
						for( j=cnt; j>=0 ;j-- )
						{
							*((uint32*)&Data[i]) = val;
							i+=3;
							
							if( i>=3*Width*(k+1) )
							{
								k+=k3;
								i=3*Width*k;
							}
						}
					}
					else
					{
						for( j=cnt; j>=0; j--)
						{
							((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
							*((uint32*)&Data[i]) = val;
							i+=3;
							
							if( i>=3*Width*(k+1) )
							{
								k+=k3;
								i=3*Width*k;
							}
						}
					}
				}
			}
			else
			{
				for( k=Height-1; k>=0; k-- )
					for( i=3*Width*k; i<3*Width*(k+1); i=i+3 )
					{ Data[i+2] = mgGetc(fp); Data[i+1] = mgGetc(fp); Data[i] = mgGetc(fp); }
			}
		}
		
		else if( depth == 4 )	//RGBA
		{
			Format = MG_RGBA;
			if( comp==10 )
			{	
				k=Height-1;
				i=4*Width*k;
				
				while( k != k2 )
				{
					cnt=mgGetc(fp);
					if((cnt&0x80)==0x80)
					{
						cnt-=0x80;
						
						((uint8*)&val)[3] = mgGetc(fp); ((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
						for( j=cnt; j>=0 ;j-- )
						{
							*((uint32*)&Data[i]) = val;
							i+=4;
							
							if( i>=4*(Width+1)*k )
							{
								k+=k3;
								i=4*Width*k;
							}
						}
					}
					else
					{
						for( j=cnt; j>=0 ;j-- )
						{
							((uint8*)&val)[3] = mgGetc(fp); ((uint8*)&val)[2] = mgGetc(fp); ((uint8*)&val)[1] = mgGetc(fp); ((uint8*)&val)[0] = mgGetc(fp);
							*((uint32*)&Data[i]) = val;
							i+=4;
							
							if( i>=4*(Width+1)*k )
							{
								k+=k3;
								i=4*Width*k;
							}
						}
					}
				}
			}
			else
			{
				for( k=Height-1; k>=0; k-- )
					for( i=4*Width*k; i<4*Width*(k+1); i=i+4 )
					{ Data[i+2] = mgGetc(fp); Data[i+1] = mgGetc(fp); Data[i+0] = mgGetc(fp); Data[i+3] = mgGetc(fp); }
			}
		}

		else
		{
			return; //NULL;
		}
		
		mgClose(fp);
	}




	
//-------------------------------------------------------------------
//GIF Loader ( 256 cols -> 32 bits )
//-------------------------------------------------------------------
#ifdef MG_USE_GIF
inline void LoadGIF(char* name, uint mode)
{
	mgSetDataAccessMode(mode);

	int i;
	bool finished;
	int nbcol;
	uint8 buffer[ 16 ];
	uint8 cmap[ 768 ];  // should be enough...
	uint8* image8b;

		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if ((fp=mgOpen(name))==NULL)
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif
	
	mgRead( buffer, 13, fp );

	//checks the file type
//	if ( strncmp( (char *)buffer, "GIF", 3 ) )
//		return;
//	if ( buffer[3] != '8' || ( buffer[4] != '7' && buffer[4] != '9' ) || buffer[5]<'A' || buffer[5]>'z' )
//		return;

	if ( !(buffer[10]&0x80) ) 
		return;
	
	nbcol = 1 << ( (buffer[10]&0x0F) + 1 );
	mgRead( cmap, 3*nbcol, fp );
	
	finished = false;
	while( !finished )
	{
		char c;
		c = mgGetc(fp);
		switch( c )
		{
		case '!':   // Extension block
            c = mgGetc(fp);    // ID
            while(1)
            {
				c = mgGetc(fp);
				if ( c<=0 ) break;
				for( i=c; i; --i ) c = mgGetc(fp);
            }
			break;
			
		case ',':   // Image
            mgRead( buffer, 9, fp );
            //if ( buffer[8]&0x40 ) return;	//interlaced format ?
			
            Width = buffer[4] | (buffer[5]<<8 );
            Height = buffer[6] | (buffer[7]<<8 );
            if ( Width<=0 || Width>=65536 || Height<=0 || Height>=65536 )
				return;
            if ( buffer[8]&0x80 ) //local color map ?
				return;

			image8b = (uint8*)malloc(Width*Height);

            c = 0;
            c = mgGetc(fp);
            if ( c<2 || c>9 )
				return;
			
            if ( LZW_Decoder( (unsigned char*)image8b, fp, Width*Height, (int)c ) )
				return;
            finished = true;
			break;
			
		case ';': default:
            finished = true;
			break;
		}
	}
	mgClose( fp );


	//converting to 32 bits only
	Data = (uint8*)malloc(Width*Height*4);
	for( i=0; i<Width*Height; i++ )
	{
		((uint32*)Data)[i] = *(uint32*)&(cmap[3*image8b[i]]) | 0xFF000000;
	}



	//converting to 32 bits + flipping vertically
//	Data = (uint8*)malloc(Width*Height*4);
//	k=0;
//	for( j=Height-1; j>=0; j-- )
//	{
//		l = j*Width;
//		for( i=0; i<Width; i++)
//			((uint32*)Data)[l++] = *(uint32*)&(cmap[3*image8b[k++]]) | 0xFF000000;
//	}

	RELEASE( image8b );
	Format = MG_RGBA;
}
#endif



/*
//-------------------------------------------------------------------
//JPG Loader ()
//-------------------------------------------------------------------
#ifdef MG_USE_JPG

inline void LoadJPG(char* name, uint mode)
{
	mgSetDataAccessMode(mode);

	BIT_STRM *Bit_Strm = (BIT_STRM*)JPEG_Load_Bitstream( name );

#ifndef MG_USE_SMALLEST_CODE
	if ( Bit_Strm==NULL )
	{
		Error(strcat("GFX file error: ", name));
		return;
	}
#endif
	
	int nb_comp;
	JPEG_Decode_Bitstream( Bit_Strm, &nb_comp, 0x3888, &Width, &Height, (unsigned char**)&Data );
	//nb_comp=1 if grayscaled, 3 otherwise (RGB)

	Format = MG_RGB;
}

#endif

*/

/*
//-------------------------------------------------------------------
//PNG Loader ()
//-------------------------------------------------------------------
	inline void LoadPNG(char* name, uint mode)
	{}

*/



/*
//-------------------------------------------------------------------
//Procedural generation of textures
//-------------------------------------------------------------------

	//there can be several functions PCD_WhatYouWant
	inline void PCD(void)
	{  }

*/


//-------------------------------------------------------------------
//Adds an Alpha layer to an image
//-------------------------------------------------------------------

inline void ToRGBA()
{
	if( Format == MG_RGBA )
		return;

	uint32* newdata = (uint32*)malloc(Width*Height*4);
	
	for( sint i=0; i<Width*Height; i++)
		newdata[i] = *((uint32*)&(Data[3*i])) | 0xFF000000;
	
	RELEASE(Data);
	Data = (uint8*)newdata;
	Format = MG_RGBA;
}


//-------------------------------------------------------------------
//COLORKEYING
//-------------------------------------------------------------------

inline void SetColorKey( uint32 col )	//allows to set several colorkeys. c is written by the coder as ABGR (RGBA in memory).
{
	ToRGBA();
	for( sint i=0; i<Width*Height; i++)
	{
		if( (((uint32*)Data)[i] & 0x00FFFFFF) == (col & 0x00FFFFFF) )
			((uint32*)Data)[i] &= 0x00FFFFFF;
		
		//if( (((uint32*)Data)[i] | 0xFF000000) == (c | 0xFF000000) )
		//	((uint32*)Data)[i] |= 0xFF000000;
	}
}



//-------------------------------------------------------------------
//BUNDLE OF ALL ABOVE
//-------------------------------------------------------------------
inline void LoadSwitch( char* name, uint mode )
{
		mgSetDataAccessMode(mode);
		
#ifdef MG_USE_SMALLEST_CODE
		uint fp = mgOpen(name);
#else
		uint fp;
		if( (fp=mgOpen(name)) == NULL )
		{
			Error(strcat("GFX file not found: ", name));
			return; //NULL;
		}
#endif

		uint32 hdany, hdjpg;
		hdany = mgRead32(fp);
		mgSeek(fp,6,SEEK_SET);
		hdjpg = mgRead32(fp);
		mgSeek(fp,0,SEEK_SET);


		//BMP test:
		if( (uint16)hdany == 0x4d42 )		//BM == 42 4d ...
		{
			LoadBMP( name, mode );
			return;
		}
/*
		//PNG test
		if( hdany == 0x474E5089 )	//89+PNG == 89 50 4E 47 ...
		{
			LoadPNG( name, mode );
			return;
		}
*/
#ifdef MG_USE_GIF
		//GIF test
		if( hdany == 0x38464947 )	//GIF8..== 38 46 49 47 ...
		{
			LoadGIF( name, mode );
			return;
		}
#endif

#ifdef MG_USE_JPG
		//JPEG test
		if( hdjpg == 0x4649464a )	//JFIF == 4a 46 49 46 ...
		{
			LoadJPG( name, mode );
			return;
		}
#endif

		//PCX test
		if( (uint8)hdany == 0x0A )			//a PCX always begin with 10 ...
		{
			LoadPCX( name, mode );
			return;
		}

		//TGA usual format has no particular identifier. Let's try and pray ! 
		LoadTGA( name, mode );

		mgClose(fp);
}



inline void LoadSwitch( char* name )
{
	LoadSwitch( name, MG_DATA_DEFAULT );
}



inline void Load( char* name )
{
	LoadSwitch( name );
	//FilteringOn();
	Bind();
}

inline void LoadF( char* name, bool filtering )
{
	LoadSwitch( name );
	filtering ? FilteringOn() : FilteringOff();
	Bind();
}


inline void LoadK( char* name, uint32 col )	//load must produce a 32 bits bitmap
{
	LoadSwitch( name );
	FilteringOff();
	SetColorKey(col);
	Bind();
}

//end of the class

};



//END
#endif
