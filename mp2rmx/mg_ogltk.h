//--------------------------------------------------------------------
// MG_OGLTK.H - LA BOITE A OUTILS OPENGL
//--------------------------------------------------------------------
#ifndef _MG_OGLTK_H_
#define _MG_OGLTK_H_




//---------------------------------------------------------------------------------------------
//FUNCTIONS ENABLE/DISABLE
//---------------------------------------------------------------------------------------------

//swaps back et front buffer
void mgFlip()
{
	glFinish();
	SwapBuffers( hDC );
}



//copies back buffer to front buffer
void mgCopyBackToFrontBuffer()
{ glCopyPixels( 0, 0, cursize.right-1, cursize.bottom-1, GL_COLOR ); }



#define mgFogEnable()		glEnable(GL_FOG)
#define mgFogDisable()		glDisable(GL_FOG)


//clears main buffers
void mgCls()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//clears zbuffer only
#define mgClz()				glClear( GL_DEPTH_BUFFER_BIT )


void mgClearColor( uint32 col )
{
	mgColor c = mgColor(col);
	glClearColor( c.r, c.g, c.b, c.a );
}


void mgGetClearColor( mgColor* c )
{
	glGetFloatv( GL_COLOR_CLEAR_VALUE, (float*)c );
}


//---------------------------------------------------------------------------------------------
// FUNCTIONS TO USE SYSTEM FONTS
//---------------------------------------------------------------------------------------------
GLuint	mgFontTex[96];
GLuint mgFontList;

void mgFontInit(HDC hdc, char* fontname, int height, int weight, GLuint* list, GLuint* texture)
{
	int i;	//index des boucles

	HFONT font = CreateFont(
							-height,
							0,
							0,
							0,
							FW_NORMAL,
							FALSE,
							FALSE,
							FALSE,
							ANSI_CHARSET,
							OUT_TT_PRECIS,
							CLIP_DEFAULT_PRECIS,
							ANTIALIASED_QUALITY,
							FF_DONTCARE|DEFAULT_PITCH,
							fontname);				//GDI object created

	SelectObject(hdc, font);						//selected like a pen
	
	(*list) = glGenLists(96);						//generating 96 rendering macros
	glGenTextures(96, texture);						//for 96 textures

	GLYPHMETRICS glyphmetrics;						//units given by GetGlyphOutline
	DWORD glyphsize;								//size of a gfx given by GetGlyphOutline
	const GLint glyphb2w = 32;						//width of a texture
	const GLint glyphb2h = 32;						//height of a texture
	int maxheight;									//max height of a char (< height)
	GLfloat boxX = 0.5f;							//for the aspect ratio
	GLfloat boxY = 1.0f;							//initializing for the space [ ]
	GLfloat cellX = 0.5f;							//width of the char + spacing with next char
	GLfloat origX = 0.0f;							//width to slide to have centered letters
	GLfloat origY = 0.0f;							//height to slide to have chars at the good height
	void *glyphbuf1 = malloc(height * height);		//we don't even know width, but as width<height...let's overestimate!
	void *glyphbuf2 = malloc(glyphb2w * glyphb2h);	//for the squared texture to bind
	const MAT2 glyphmat2 = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};	//glyphs transformation matrix (identity here)

	glPixelTransferf(GL_INDEX_SHIFT, 0.0f);
	glPixelTransferf(GL_INDEX_OFFSET, 0.0f);
	glPixelStoref(GL_UNPACK_LSB_FIRST, 0.0f);

/*
	GLfloat map[256];								//les glyphs arrivent avec une couleur entre 0 et 65 et nous on a besoin entre 0 et 255 -> conversion
	for(i=0; i<=64; i++)							//on n'a besoin que des 65 premiers, le reste on le laisse indéterminé
		map[i] = (float)i/64.0f;

	glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 256, map);	//mapping des indexes sur l'alpha (on fait une fonte transparente)
*/
	//on trouve la hauteur maxi d'un char (on a choisi ! pour ça)
	GetGlyphOutline(hdc, 33 /* ! */, GGO_GRAY8_BITMAP, &glyphmetrics, 0, NULL, &glyphmat2);
	maxheight = glyphmetrics.gmBlackBoxY;

	//d'abord l'espace
	ZeroMemory(glyphbuf2, glyphb2w*glyphb2h);
	//ratiox = 0.5f;	//deja mis ces valeurs à l'init
	//ratioy = 1.0f;
	//...et les autres aussi

	for(i=0; i<96; i++)
	{
		//paramètres de la texture
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//fabrication de la texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyphb2w, glyphb2h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyphbuf2);
		
		//compilation de l'affichage de la texture
		glNewList(*list+i, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glTranslatef(origX, origY, 0.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, boxY, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( boxX, boxY, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( boxX, 0.0f, 0.0f);
		glEnd();
		glTranslatef(-origX+cellX, -origY, 0.0f);
		glEndList();

		//génération du caractère suivant (+33 au lieu de +32). on met la charue avant les boeufs mais c'est pour optimiser
		glyphsize = GetGlyphOutline(hdc, (unsigned char)(i+33), GGO_GRAY8_BITMAP, &glyphmetrics, 0, NULL, &glyphmat2);
		GetGlyphOutline(hdc, (unsigned char)(i+33), GGO_GRAY8_BITMAP, &glyphmetrics, glyphsize, glyphbuf1, &glyphmat2);

		for( uint u=0; u<(glyphmetrics.gmBlackBoxX*glyphmetrics.gmBlackBoxY); u++ )
			((uint8*)glyphbuf1)[u] = (255 * (uint)(((uint8*)glyphbuf1)[u]))/64;

		gluScaleImage(GL_ALPHA, glyphmetrics.gmBlackBoxX, glyphmetrics.gmBlackBoxY, GL_UNSIGNED_BYTE, glyphbuf1, glyphb2w, glyphb2h, GL_UNSIGNED_BYTE, glyphbuf2);

		//calcul des décalages et ratios suivants (il le fera pour i=96 aussi mais c'est pas grave)
		boxX = (GLfloat)glyphmetrics.gmBlackBoxX/maxheight;
		boxY = (GLfloat)glyphmetrics.gmBlackBoxY/maxheight;
		cellX = (GLfloat)glyphmetrics.gmCellIncX/maxheight;
		origX = (GLfloat)glyphmetrics.gmptGlyphOrigin.x/maxheight;
		origY = 1.0f - (GLfloat)glyphmetrics.gmptGlyphOrigin.y/maxheight;
	}

	RELEASE(glyphbuf1);
	RELEASE(glyphbuf2);
	DeleteObject(font);
}



void mgFontKill(GLuint list)
{
	glDeleteLists(list, 96);
	glDeleteTextures(96, mgFontTex);
}


//---------------------------------------------------------------------------------------------
//displays a 2D text in raster mode, top-left corner is (0,0)
//it's possible to print ~20 chars / line with scale 1 in 640*480
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_TEXTURE_2D);
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//---------------------------------------------------------------------------------------------
void Text2R(GLint x, GLint y, GLfloat scale, GLuint list, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(0, cursize.right, cursize.bottom, 0);	//if the viewport changes as the window is resized, then this line is useless

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    //glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    //glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);
    glScalef(32.0f*scale, 32.0f*scale, 32.0f*scale);

	glListBase(list - 32);
	glListBase(mgFontList - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	//glPopAttrib();

	glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}




//---------------------------------------------------------------------------------------------
//displays a 2D text with coords between 0 et 1, top-left corner is (0,0)
//it's possible to print ~20 chars / line with scale 1
//---------------------------------------------------------------------------------------------
void Text2D(GLfloat x, GLfloat y, GLfloat scale, GLuint list, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT );
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTranslatef(x, y, 0.0f);
    glScalef(scale/20.0f, scale/20.0f, scale/20.0f);

	glListBase(list - 32);
	glListBase(mgFontList - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	glPopAttrib();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



//---------------------------------------------------------------------------------------------
//displays a 3D text where we are positionned...like a quad
//---------------------------------------------------------------------------------------------
void Text3D(GLfloat scale, GLuint list, char* format, ...)
{
	va_list args;
	char buffer[255];
	
	va_start(args, format);
	wvsprintf(buffer, format, args);
	va_end(args);

    glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glScalef(scale, -scale, scale);

	glListBase(list - 32);
	glCallLists(lstrlen(buffer), GL_UNSIGNED_BYTE, buffer);

	glPopAttrib();
}


//---------------------------------------------------------------------------------------------
//raster line independant from the window size
//-------------------------------------------------------------------
//need to be in raster mode, and especially...
//glEnable(GL_BLEND);
//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//---------------------------------------------------------------------------------------------
void Line2R(GLint x1, GLint y1, GLint x2, GLint y2)
{
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//gluOrtho2D(0, cursize.right, cursize.bottom, 0);

	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();

	glPopAttrib();

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}




//---------------------------------------------------------------------------------------------
//proportional line with coords between 0.0 and 1.0
//---------------------------------------------------------------------------------------------
void Line2D(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

    glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();

	glPopAttrib();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


//END
#endif