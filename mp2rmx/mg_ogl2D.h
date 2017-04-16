//-------------------------------------------------------------------
//MG.OGL2D.H - 2D SPECIFIC RASTER FUNCTIONS
//-------------------------------------------------------------------
#ifndef _MG_OGL2D_H_
#define _MG_OGL2D_H_


#define MG_RGB	GL_RGB
#define MG_RGBA	GL_RGBA



//-------------------------------------------------------------------
//SWITCHES TO 2D MODE - (0,0) ON TOP-LEFT OR 3D MODE
//-------------------------------------------------------------------

void mgRasterMode(void)
{
	mgViewPortMode = mgRasterMode;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ubv((unsigned char*)&WHITE);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glPolygonMode(GL_FRONT, GL_FILL);	

	glDisable(GL_DEPTH_TEST);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//1 seems to be the size of 1 int (?), so aligned

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (float)mgWidth, (float)mgHeight, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void mgRasterTransform( sint xd, sint yd, sint wd, sint hd, float deg )	//all parameters are absolute
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef( (float)mgWidth/(float)wd, (float)mgHeight/(float)hd, 1.f );
	glRotatef( deg, 0.f, 0.f, -1.f );
	glTranslatef( -(float)xd, -(float)yd, 0.f );
}


void mg3DMode(void)
{
	mgViewPortMode = mg3DMode;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4ubv((unsigned char*)&WHITE);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPolygonMode(GL_FRONT, GL_FILL);	

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	//Perspective reset only, not set

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-------------------------------------------------------------------
//RASTERIZATION FUNCTIONS
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_TEXTURE_2D);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//-------------------------------------------------------------------

void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex, sint xs, sint ys, sint ws, sint hs, uint8 opa, bool hflip, bool vflip )
{
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, tex.ID);

	if( !(opa && tex.ID) )		//nothing to draw
		return;
	
	if( !wd || !hd )
		return;
	//{ wd=tex.Width; hd=tex.Height; }
	
	if( !ws || !hs )
	{ xs=0; ys=0; ws=tex.Width; hs=tex.Height; }

	glColor4ub(255,255,255,opa);
	float x1 = (float)xs/(float)tex.Width;
	float x2 = (float)(xs+ws)/(float)tex.Width;
	float y1 = (float)ys/(float)tex.Height;
	float y2 = (float)(ys+hs)/(float)tex.Height;
	float txy;

	if(hflip)
	{
		txy = x1;
		x1 = x2;
		x2 = txy;
	}

	if(vflip)
	{
		txy = y1;
		y1 = y2;
		y2 = txy;
	}

	glBegin(GL_QUADS);

	glTexCoord2f(x1, y1);
	glVertex2i( xd, yd );
	
	glTexCoord2f(x2, y1);
	glVertex2i( xd+wd, yd );
	
	glTexCoord2f(x2, y2);
	glVertex2i( xd+wd, yd+hd );
	
	glTexCoord2f(x1, y2);
	glVertex2i( xd, yd+hd );
	
	glEnd();
}


//coords opa
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex, sint xs, sint ys, sint ws, sint hs, uint8 opa )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, opa, false, false );
}

//coords (1/2) opa
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex, uint8 opa )
{
	mgDrawTex(xd, yd, wd, hd, tex, 0, 0, 0, 0, opa );
}

//coords
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex )
{
	mgDrawTex(xd, yd, wd, hd, tex, 0, 0, 0, 0, 0xFF );
}

//coords flip
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex, sint xs, sint ys, sint ws, sint hs, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, 0xFF, hflip, vflip );
}

//coords
void mgDrawTex(sint xd, sint yd, sint wd, sint hd, mgTexture& tex, sint xs, sint ys, sint ws, sint hs )
{
	mgDrawTex(xd, yd, wd, hd, tex, xs, ys, ws, hs, 0xFF, false, false );
}

//opa flip
void mgDrawTex(sint xd, sint yd, mgTexture& tex, uint8 opa, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, tex.Width, tex.Height, tex, 0, 0, 0, 0, opa, hflip, vflip );
}

//opa
void mgDrawTex(sint xd, sint yd, mgTexture& tex, uint8 opa )
{
	mgDrawTex(xd, yd, tex.Width, tex.Height, tex, 0, 0, 0, 0, opa, false, false );
}

//flip
void mgDrawTex(sint xd, sint yd, mgTexture& tex, bool hflip, bool vflip )
{
	mgDrawTex(xd, yd, tex.Width, tex.Height, tex, 0, 0, 0, 0, 0xFF, hflip, vflip );
}

//no transformation
void mgDrawTex(sint xd, sint yd, mgTexture& tex )
{
	mgDrawTex(xd, yd, tex.Width, tex.Height, tex, 0, 0, 0, 0, 0xFF, false, false );
}



//---------------------------------------------------------------------------------------------
//CLEAR THE SCREEN
//---------------------------------------------------------------------------------------------

void mgCls(sint xd, sint yd, sint wd, sint hd, uint opa, uint32 col)
{
	mgTexture tex;
	tex.Width = tex.Height = 1;
	tex.Data = (uint8*)&col;
	tex.Format = MG_RGBA;
	tex.BindNoRelease();	//the bind is here
	mgDrawTex( xd, yd, wd, hd, tex, 0, 0, 1, 1, opa );
	tex.Unbind();
}

void mgCls(sint xd, sint yd, sint wd, sint hd, uint32 col)
{	mgCls(xd, yd, wd, hd, 0xFF, col);	}

void mgCls(sint xd, sint yd, sint wd, sint hd )
{
	mgColor c;
	mgGetClearColor( &c );
	mgCls(xd, yd, wd, hd, 0xFF, c.AsRGB32() );
}

void mgCls( uint opa, uint32 col )
{
	mgColor c;
	mgGetClearColor( &c );
	mgCls( 0, 0, mgWidth, mgHeight, opa, col );
}

void mgCls( uint32 col )
{	mgCls( 0, 0, mgWidth, mgHeight, 0xFF, col );	}





void mgFade(sint xd, sint yd, sint wd, sint hd, uint8 opa, uint32 col)
{	mgCls(xd, yd, wd, hd, 255-opa, col);	}



void mgFade(uint8 opa, uint32 col)
{	mgCls(0, 0, mgWidth, mgHeight, 255-opa, col);	}



void mgFade(uint8 opa)
{	mgCls(0, 0, mgWidth, mgHeight, 255-opa, BLACK);	}



//------------------------------------------------------------
//CINEMASCOPE
//------------------------------------------------------------
void mgCinemaScope( float ratio )
{
	void(*displaysaved)() = mgViewPortMode;
	mgRasterMode();
	uint h = (mgHeight - mgWidth/ratio )/2;
	mgCls( 0, 0, mgWidth, h, BLACK);
	mgCls( 0, mgHeight-h, mgWidth, h, BLACK);
	displaysaved();
}

void mgCinemaScope()
{	mgCinemaScope( 16.f/9.f );	}



//------------------------------------------------------------
//HORIZON, RASTER-LIKE
//------------------------------------------------------------
void mgHorizon( uint sky1, uint sky2, uint gnd1, uint gnd2, float skyheight )
{
	void(*displaymode)() = mgViewPortMode;
	mgRasterMode();
	glDisable(GL_TEXTURE_2D);
	bool ls = (glIsEnabled(GL_LIGHTING)==GL_TRUE);
	glDisable(GL_LIGHTING);

	mgColor csky1 = mgColor(sky1);
	mgColor csky2 = mgColor(sky2);
	mgColor cgnd1 = mgColor(gnd1);
	mgColor cgnd2 = mgColor(gnd2);
	uint h2 = (uint)((float)mgHeight*skyheight);

	glBegin(GL_QUADS);

	glColor4fv((float*)& csky1 );
	glVertex2i( 0, 0 );
	glVertex2i( mgWidth, 0 );
	
	glColor4fv((float*)& csky2 );
	glVertex2i( mgWidth, h2 );
	glVertex2i( 0, h2 );

	
	glColor4fv((float*)& cgnd1 );
	glVertex2i( 0, h2 );
	glVertex2i( mgWidth, h2 );
	
	glColor4fv((float*)& cgnd2 );
	glVertex2i( mgWidth, mgHeight );
	glVertex2i( 0, mgHeight );
	
	glEnd();

	if(ls)
		glEnable(GL_LIGHTING);
	displaymode();
}



//---------------------------------------------------------------------------------------------
//TV DISPLAY (2 normal, 2 dimed...)
//---------------------------------------------------------------------------------------------
void mgTVInit()
{
	mgTexTV = new mgTexture;
	unsigned char pix[2] = { 0xFF, 0xA0 };
	mgTexTV->Width = 1;
	mgTexTV->Height = 2;
	glGenTextures(1, &mgTexTV->ID);
	glBindTexture(GL_TEXTURE_2D, mgTexTV->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1, 2, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pix);
}


void mgTV(sint xd, sint yd, sint wd, sint hd)
{
	void(*displaymode)() = mgViewPortMode;
	mgRasterMode();
	bool ls = (glIsEnabled(GL_LIGHTING)==GL_TRUE);
	glDisable(GL_LIGHTING);
	
	float h2 = hd/2;

	glBlendFunc( GL_ZERO, GL_SRC_ALPHA );
	glBindTexture(GL_TEXTURE_2D, mgTexTV->ID);
	mgDrawTex( xd, yd, wd, hd, *mgTexTV, 0, 0, 1, h2 );

	if(ls)
		glEnable(GL_LIGHTING);
	displaymode();
}


void mgTV()
{ mgTV(0, 0, mgWidth, mgHeight ); }



//---------------------------------------------------------------------------------------------
//SNOW
//---------------------------------------------------------------------------------------------

void mgSnowInit()
{
#define mgTexSnowDim	64
	mgTexSnow = new mgTexture;
	uint8* pix = (uint8*)malloc( mgTexSnowDim * mgTexSnowDim );
	for( uint i=0; i<(mgTexSnowDim*mgTexSnowDim); i++ )
		pix[i] = 128+rand()%128;

	mgTexSnow->Width = mgTexSnow->Height = mgTexSnowDim;
	glGenTextures(1, &mgTexSnow->ID);
	glBindTexture(GL_TEXTURE_2D, mgTexSnow->ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, mgTexSnowDim, mgTexSnowDim, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pix);
}


void mgSnow( uint dgnr )
{

	void(*displaymode)() = mgViewPortMode;
	mgRasterMode();
	bool ls = (glIsEnabled(GL_LIGHTING)==GL_TRUE);
	glDisable(GL_LIGHTING);
	
	sint x = rand()%64;
	sint y = rand()%64;

	glBlendFunc( GL_ZERO, GL_SRC_ALPHA );
	glBindTexture(GL_TEXTURE_2D, mgTexSnow->ID);

	if( dgnr == 0)
	{
		mgDrawTex( 0, 0, mgWidth, mgHeight, *mgTexSnow, x, y, x+mgWidth, y+mgHeight );
	}
	else if (dgnr == 1 )
	{
		sint hdivs = mgWidth/mgTexSnowDim;
		sint vdivs = mgHeight/mgTexSnowDim;
		mgDrawTex( 0, 0, mgWidth, mgHeight, *mgTexSnow, x, y, x+hdivs, y+vdivs );
	}

	if(ls)
		glEnable(GL_LIGHTING);
	displaymode();
}



//---------------------------------------------------------------------------------------------
//MOSAIC : TO DISPLAY THE SAME PATTERN EVERYWHERE ON THE SCREEN
//---------------------------------------------------------------------------------------------
void mgMosaic( sint xd, sint yd, mgTexture& tex )
{
	sint x0, y0;

	sint nw = mgWidth/tex.Width;
	if( (x0 = xd%tex.Width)>0 )
		x0-=tex.Width;
	if( x0 < 0 )
		nw++;
	if( (mgWidth-xd)%tex.Width != 0 )
		nw++;

	sint nh = mgHeight/tex.Height;
	if( (y0 = yd%tex.Height)>0 )
		y0-=tex.Height;
	if( y0 < 0 )
		nh++;
	if( (mgHeight-yd)%tex.Height != 0 )
		nh++;

	sint i,j;
	for( j=0; j<nh; j++ )
		for( i=0; i<nw; i++ )
			mgDrawTex( x0+i*tex.Width, y0+j*tex.Height, tex );
}



//END
#endif
