//-------------------------------------------------------------------
//PRIM2D.H - FONCTIONS 2D HARDWARE / SOFTWARE
//-------------------------------------------------------------------
#ifndef _prim2D_H_
#define _prim2D_H_

#include <math.h>
#include "demo_add.h"

//extern LPDIRECT3DDEVICE7 lpD3DDevice;
extern HFONT Font;
extern LPDIRECTDRAW7 lpDD;





//---------------------------------------------------------------------------------------------
// mon clipper 2D
//---------------------------------------------------------------------------------------------
void clipper(LPRECT drect, LPRECT srect)
{

	if((*drect).left<0)
	{
		if((*drect).right<=0)
			{(*drect).left=0; (*drect).top=0; (*drect).right=0; (*drect).bottom=0; return;}

		(*srect).left=-((*drect).left*((*srect).right-(*srect).left))/((*drect).right-(*drect).left);
		(*drect).left=0;
	}

	if((*drect).right>(signed)width)
	{
		if ((*drect).left>=(signed)width)
			{(*drect).left=0; (*drect).top=0; (*drect).right=0; (*drect).bottom=0; return;}

		(*srect).right=(*srect).right-(((*drect).right-width)*((*srect).right-(*srect).left))/((*drect).right-(*drect).left);
		(*drect).right=width;
	}

	if((*drect).top<0)
	{
		if((*drect).bottom<=0)
			{(*drect).left=0; (*drect).top=0; (*drect).right=0; (*drect).bottom=0; return;}

		(*srect).top=-((*drect).top*((*srect).bottom-(*srect).top))/((*drect).bottom-(*drect).top);
		(*drect).top=0;
	}

	if((*drect).bottom>(signed)height)
	{
		if((*drect).top>=(signed)height)
			{(*drect).left=0; (*drect).top=0; (*drect).right=0; (*drect).bottom=0; return;}

		(*srect).bottom=(*srect).bottom-(((*drect).bottom-height)*((*srect).bottom-(*srect).top))/((*drect).bottom-(*drect).top);
		(*drect).bottom=height;
	}
}



//---------------------------------------------------------------------------------------------
// blits crop ou non, transparence ou non, ce ne sont que des blits simples à cause du clipper
//---------------------------------------------------------------------------------------------
void dI(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 ssurf, int xs, int ys, int ws, int hs)
{
	HRESULT ddrval;

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	if(wd==NULL || hd==NULL)
		{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
		{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);
	
    while(1)
    {
        ddrval = dsurf->Blt(&drect,ssurf,&srect,NULL,NULL);
        if( ddrval == DD_OK ) return;
        if( ddrval != DDERR_WASSTILLDRAWING ) return;
    }
}



void dIt(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 ssurf, int xs, int ys, int ws, int hs, int ck)

{
	HRESULT ddrval;

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	if(wd==NULL || hd==NULL)
	{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
	{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);

	DDCOLORKEY ck2={ck,ck};
	ssurf->SetColorKey(DDCKEY_SRCBLT,&ck2);

    while(1)
    {
        ddrval = dsurf->Blt(&drect,ssurf,&srect,DDBLT_KEYSRC,NULL);
        if( ddrval == DD_OK ) return;
        if( ddrval != DDERR_WASSTILLDRAWING ) return;
    }
}


////////////////////////////////////////////////////////////////////

void dIo1(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, LPDIRECTDRAWSURFACE7 ssurf, int opa)
{
	//ici on fait tout à la main avec Lock()/Unlock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	RECT drect={xd,yd,xd+ddsd.dwWidth,yd+ddsd.dwHeight};
	RECT srect={0,0,ddsd.dwWidth,ddsd.dwHeight};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int i,j,da,sa,dai,sai;

	da=4*(width*drect.top+drect.left);
	sa=ddsd.lPitch*srect.top+4*srect.left;

    for(j=drect.top;j<drect.bottom;j++)
	{
		dai=da;
		sai=sa;

		for(i=drect.left;i<drect.right;i++)
		{
			backad[dai]=(opa*surfad[sai]+(255-opa)*backad[dai])>>8;
			backad[dai+1]=(opa*surfad[sai+1]+(255-opa)*backad[dai+1])>>8;
			backad[dai+2]=(opa*surfad[sai+2]+(255-opa)*backad[dai+2])>>8;
			dai+=4;
			sai+=4;
		}
		da+=4*pitch;
		sa+=ddsd.lPitch;
	}
}



void dIo1t(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, LPDIRECTDRAWSURFACE7 ssurf, int opa, int colk)
{
	//ici on fait tout à la main avec Lock()/Unlock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	RECT drect={xd,yd,xd+ddsd.dwWidth,yd+ddsd.dwHeight};
	RECT srect={0,0,ddsd.dwWidth,ddsd.dwHeight};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int i,j,da,sa,dai,sai;

	da=4*(pitch*drect.top+drect.left);
	sa=ddsd.lPitch*srect.top+4*srect.left;

    for(j=drect.top;j<drect.bottom;j++)
	{
		dai=da;
		sai=sa;

		for(i=drect.left;i<drect.right;i++)
		{
			if( ((int*)(surfad))[sai/4] != colk )
			{
				backad[dai]=(opa*surfad[sai]+(255-opa)*backad[dai])>>8;
				backad[dai+1]=(opa*surfad[sai+1]+(255-opa)*backad[dai+1])>>8;
				backad[dai+2]=(opa*surfad[sai+2]+(255-opa)*backad[dai+2])>>8;
			}
			dai+=4;
			sai+=4;
		}
		da+=4*pitch;
		sa+=ddsd.lPitch;
	}
}



void dIo2(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 ssurf, int xs, int ys, int ws, int hs, int opa)
{
	//ici on fait tout à la main avec Lock()/Unlock()....optimisé à donf
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	if(wd==NULL || hd==NULL)
	{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
	{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int i,j,acx,acy;
	const int mul=8;
	int dai, da=4*(pitch*drect.top+drect.left);
	int sai, sad;
	const int sa=(ddsd.lPitch*srect.top+4*srect.left);
	const int grx=((srect.right-srect.left-1)<<mul)/(drect.right-drect.left-1);
	const int gry=((srect.bottom-srect.top-1)<<mul)/(drect.bottom-drect.top-1);

	acy=0;
    for(j=drect.top;j<drect.bottom;j++)
	{
		acx=0;
		sad=sa+ddsd.lPitch*(acy>>mul);
		dai=da;
		for(i=drect.left;i<drect.right;i++)
		{
			sai=sad+4*(acx>>mul);
			backad[dai]=(opa*surfad[sai]+(255-opa)*backad[dai])>>8;
			backad[dai+1]=(opa*surfad[sai+1]+(255-opa)*backad[dai+1])>>8;
			backad[dai+2]=(opa*surfad[sai+2]+(255-opa)*backad[dai+2])>>8;
			dai+=4;
			acx+=grx;
		}
		acy+=gry;
		da+=4*pitch;
	}
}



void dIo2t(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 ssurf, int xs, int ys, int ws, int hs, int opa, int colk)
{
	//ici on fait tout à la main avec Lock()/Unlock()....optimisé à donf
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	if(wd==NULL || hd==NULL)
	{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
	{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int i,j,acx,acy;
	const int mul=8;
	int dai, da=4*(width*drect.top+drect.left);
	int sai, sad;
	const int sa=(ddsd.lPitch*srect.top+4*srect.left);
	const int grx=((srect.right-srect.left-1)<<mul)/(drect.right-drect.left-1);
	const int gry=((srect.bottom-srect.top-1)<<mul)/(drect.bottom-drect.top-1);

	acy=0;
    for(j=drect.top;j<drect.bottom;j++)
	{
		acx=0;
		sad=sa+ddsd.lPitch*(acy>>mul);
		dai=da;
		for(i=drect.left;i<drect.right;i++)
		{
			sai=sad+4*(acx>>mul);

			if( ((int*)(surfad))[sai/4] != colk )
			{
				backad[dai]=(opa*surfad[sai]+(255-opa)*backad[dai])>>8;
				backad[dai+1]=(opa*surfad[sai+1]+(255-opa)*backad[dai+1])>>8;
				backad[dai+2]=(opa*surfad[sai+2]+(255-opa)*backad[dai+2])>>8;
			}
			dai+=4;
			acx+=grx;
		}
		acy+=gry;
		da+=4*pitch;
	}

}




////////////////////////////////////////////////////////////////////

void dIa(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, LPDIRECTDRAWSURFACE7 ssurf)
{
	//ici on fait tout à la main avec Lock()/Unlock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	RECT drect={xd,yd,xd+ddsd.dwWidth,yd+ddsd.dwHeight};
	RECT srect={0,0,ddsd.dwWidth,ddsd.dwHeight};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int opa;
	int i,j,da,sa,dai,sai;

	da=4*(width*drect.top+drect.left);
	sa=ddsd.lPitch*srect.top+4*srect.left;

    for(j=drect.top;j<drect.bottom;j++)
	{
		dai=da;
		sai=sa;

		for(i=drect.left;i<drect.right;i++)
		{
			opa=surfad[sai+3];
			if (opa != 0)
			{
				backad[dai]=(opa*surfad[sai]+(255-opa)*backad[dai])>>8;
				backad[dai+1]=(opa*surfad[sai+1]+(255-opa)*backad[dai+1])>>8;
				backad[dai+2]=(opa*surfad[sai+2]+(255-opa)*backad[dai+2])>>8;
			}
			dai+=4;
			sai+=4;
		}
		da+=4*pitch;
		sa+=ddsd.lPitch;
	}
}



////////////////////////////////////////////////////////////////////


void dIr(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 ssurf, int xs, int ys, int ws, int hs, int angle)
{
	HRESULT ddrval;

	DDSURFACEDESC2 ddsd;
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	DDCAPS hel;
	lpDD->GetCaps(NULL, &hel);

	if(wd==NULL || hd==NULL)
		{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
		{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);
	
	DDBLTFX ddbltfx;
	ddbltfx.dwSize=sizeof(ddbltfx);
	//ddbltfx.dwROP=hel.dwRops[7];
	ddbltfx.dwRotationAngle=angle; 
	
	while(1)
    {
        ddrval = dsurf->Blt( &drect, ssurf, &srect, DDBLT_ROP | DDBLT_ROTATIONANGLE, &ddbltfx );
        if( ddrval == DD_OK ) return;
        if( ddrval != DDERR_WASSTILLDRAWING ) return;
    }
}



//------------------------------------------------------------
// fonction de fade in/out
//------------------------------------------------------------
void dIf(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, LPDIRECTDRAWSURFACE7 ssurf, int opa)
{
	//ici on fait tout à la main avec Lock()/Unlock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	RECT drect={xd,yd,xd+ddsd.dwWidth,yd+ddsd.dwHeight};
	RECT srect={0,0,ddsd.dwWidth,ddsd.dwHeight};
	clipper(&drect,&srect);
	
	BYTE* surfad=Lock(ssurf);
	BYTE* backad=Lock(dsurf);
	Unlock(ssurf);
	Unlock(dsurf);

	int i,j,da,sa,dai,sai;

	da=4*(pitch*drect.top+drect.left);
	sa=ddsd.lPitch*srect.top+4*srect.left;

    for(j=drect.top;j<drect.bottom;j++)
	{
		dai=da;
		sai=sa;

		for(i=drect.left;i<drect.right;i++)
		{
			backad[dai]=(opa*surfad[sai])/256;
			backad[dai+1]=(opa*surfad[sai+1])/256;
			backad[dai+2]=(opa*surfad[sai+2])/256;
			dai+=4;
			sai+=4;
		}
		da+=4*pitch;
		sa+=ddsd.lPitch;
	}
}











/*
void dItex(int xd, int yd, int wd, int hd, LPDIRECTDRAWSURFACE7 surf, int xs, int ys, int ws, int hs, int opa)
{
	//ici on fait tout à la main avec Lock()/Unlock()
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	surf->GetSurfaceDesc(&ddsd);

	if(wd==NULL || hd==NULL)
	{wd=ddsd.dwWidth; hd=ddsd.dwHeight;}

	if(ws==NULL || hs==NULL)
	{xs=0; ys=0; ws=ddsd.dwWidth; hs=ddsd.dwHeight;}

	RECT drect={xd,yd,xd+wd,yd+hd};
	RECT srect={xs,ys,xs+ws,ys+hs};
	clipper(&drect,&srect);
	
	D3DVERTEX vrect[4];
	vrect[0] = D3DVERTEX(D3DVECTOR(0,1,0),D3DVECTOR(0,0,-1),0,1);
	vrect[1] = D3DVERTEX(D3DVECTOR(1,1,0),D3DVECTOR(0,0,-1),1,1);
	vrect[2] = D3DVERTEX(D3DVECTOR(0,0,0),D3DVECTOR(0,0,-1),0,0);
	vrect[3] = D3DVERTEX(D3DVECTOR(1,0,0),D3DVECTOR(0,0,-1),1,0);

    lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, vert, 0L, 0L);

    if(FAILED(lpD3DDevice->BeginScene()))
        Error("could not begin a rendering");
	lpD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,D3DFVF_VERTEX,vrect,4,D3DDP_WAIT);
    lpD3DDevice->EndScene();
 
}
*/







//---------------------------------------------------------------------------------------------
// fonctions de vidange de l'écran
//---------------------------------------------------------------------------------------------
void cls(LPDIRECTDRAWSURFACE7 dsurf, int col)
{
	DDBLTFX ddbltfx;
	HRESULT	ddrval;

	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = col;
	while(1)
	{
		ddrval = dsurf->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
        if(ddrval == DD_OK) return;
        if(ddrval != DDERR_WASSTILLDRAWING) return;
	}
}



void clsrect(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, int col)
{
	/*
	//un ptit clipper au cas où
	if(xd<0)
		xd=0;
	if(yd<0)
		yd=0;
	if(xd+wd>width)
		wd=width-xd;
	if(yd+hd>height || )
		hd=height-yd;
	if(xd+wd<=0 || xd>=width || yd+hd<=0 || yd>=height)
		return;
	*/

	DDBLTFX ddbltfx;
	HRESULT	ddrval;
	RECT rect={xd,yd,xd+wd,yd+hd};

	ddbltfx.dwSize = sizeof( ddbltfx );
	ddbltfx.dwFillColor = col;
	while(1)
	{
		ddrval = dsurf->Blt(&rect, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
        if(ddrval == DD_OK) return;
        if(ddrval != DDERR_WASSTILLDRAWING) return;
	}
}



void clsorect(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd, int opa, int col)
{
	BYTE* backad=Lock(dsurf);
	Unlock(dsurf);

	int r,g,b;
	r=(col>>16)%256;
	g=(col>>8)%256;
	b=col%256;
	int i,j,da, dai;

	da=4*(pitch*yd+xd);

    for(j=yd;j<yd+hd;j++)
	{
		dai=da;

		for(i=xd;i<xd+wd;i++)
		{	//ordre: B/G/R
			backad[dai]=((255-opa)*backad[dai]+opa*b)>>8;
			backad[dai+1]=((255-opa)*backad[dai+1]+opa*b)>>8;
			backad[dai+2]=((255-opa)*backad[dai+2]+opa*b)>>8;
			dai+=4;
		}
		da+=4*pitch;
	}
}





//---------------------------------------------------------------------------------------------
// fonction d'affichage de texte
//---------------------------------------------------------------------------------------------
void Print(LPDIRECTDRAWSURFACE7 dsurf, int x,int y, char *str, int col)
{
	HDC hdc;

	if(dsurf->GetDC(&hdc)==DD_OK)
	{
		SelectObject(hdc,Font);
		SetTextColor(hdc, RGB((col/65536)%256,(col/256)%256,col%256));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, x, y, str, strlen(str));
		dsurf->ReleaseDC(hdc);
	}
}

#endif




//---------------------------------------------------------------------------------------------
// fonction qui fait un affichage type TV (une ligne sur deux)
// pas de vérifications
//---------------------------------------------------------------------------------------------
void dItv(LPDIRECTDRAWSURFACE7 dsurf, int xd, int yd, int wd, int hd)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	dsurf->GetSurfaceDesc(&ddsd);

	BYTE* surfad=Lock(dsurf);
	Unlock(dsurf);

	int off1=(yd+1)*ddsd.lPitch+4*xd, off2;
	
	for (int tvy=0; tvy<hd; tvy+=4)
	{
		off2=off1;

		for (int tvx=0; tvx<wd; tvx++)
		{
			surfad[off2]=surfad[off2]/2;
			surfad[off2+1]=surfad[off2+1]/2;
			surfad[off2+2]=surfad[off2+2]/2;
			off2+=4;
		}

		off1+=ddsd.lPitch;
		off2=off1;

		for (int tvx2=0; tvx2<wd; tvx2++)
		{
			surfad[off2]=surfad[off2]/2;
			surfad[off2+1]=surfad[off2+1]/2;
			surfad[off2+2]=surfad[off2+2]/2;
			off2+=4;
		}

		off1+=3*ddsd.lPitch;
	}
}




//---------------------------------------------------------------------------------------------
// fonction de neige
//---------------------------------------------------------------------------------------------
void neige(LPDIRECTDRAWSURFACE7 dsurf, int x, int y, int w, int h)
{
	int i,j;
	int da,dai;
	char s;
	BYTE* surfad=Lock(dsurf);
	Unlock(dsurf);

	dai=4*(pitch*y+x);
	for (j=y;j<y+h;j++)
	{
		da=dai;

		for (i=x;i<x+w;i++)
		{
			s=rand()%256;
			surfad[da]=s;
			surfad[da+1]=s;
			surfad[da+2]=s;
			//surfad[da++]=0;
			da+=4;
		}
		dai+=4*pitch;
	}
}


//---------------------------------------------------------------------------------------------
// fonction qui prend un sprite et l'affiche sans fin partout sur l'ecran
//---------------------------------------------------------------------------------------------
void mosaic(LPDIRECTDRAWSURFACE7 dsurf, int x, int y, LPDIRECTDRAWSURFACE7 ssurf)
{

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	ssurf->GetSurfaceDesc(&ddsd);

	int nw=width/ddsd.dwWidth;
	if(x%ddsd.dwWidth != 0)
		nw++;
	if((width-x)%ddsd.dwWidth != 0)
		nw++;

	int nh=height/ddsd.dwHeight;
	if(y%ddsd.dwHeight != 0)
		nh++;
	if((height-y)%ddsd.dwHeight != 0)
		nh++;

	int xo=(x+ddsd.dwWidth-1)%ddsd.dwWidth-ddsd.dwWidth+1;
	int yo=(y+ddsd.dwHeight-1)%ddsd.dwHeight-ddsd.dwHeight+1;


	int i,j;
	for(j=0;j<nh;j++)
		for(i=0;i<nw;i++)
			dI(dsurf,xo+i*ddsd.dwWidth,yo+j*ddsd.dwHeight,ddsd.dwWidth,ddsd.dwHeight,ssurf,0,0,ddsd.dwWidth,ddsd.dwHeight);
}







