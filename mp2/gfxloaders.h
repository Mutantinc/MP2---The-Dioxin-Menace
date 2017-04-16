//-----------------------------------------------------------------------------------------
// GFXLOADERS.H - LOADERS D'IMAGES SUPER BASIQUES
// Attention: les PITCHes (le nb d'octets par scan > RGB(A)*Width) des surfaces DX
// sont alignées zarbi et il faut les retrouver avec GetSurfaceDesc puis ddsd.lPitch
//-----------------------------------------------------------------------------------------
#ifndef _GFXLOADERS_H_
#define _GFXLOADERS_H_

#include <ddraw.h>
#include <stdio.h>

extern BYTE *Lock(LPDIRECTDRAWSURFACE7 lpDDStoLock);
extern VOID Unlock(LPDIRECTDRAWSURFACE7 lpDDStoUnlock);
extern void Error(LPSTR str);




//-----------------------------------------------------------------------------------------
//LOADERS POUR LES FICHIERS .PCX & .TGA
//-----------------------------------------------------------------------------------------
//loader de PCX (265 cols -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdPCX(LPDIRECTDRAW7 lpDD, LPSTR nom)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	FILE* fp;
	BYTE* surfad;
	char pal[768];
	int octet1,octet2;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
    
	
	if ((fp=fopen(nom,"rb"))==NULL)
		Error(strcat(nom," not found"));
	
	fseek(fp,2,0);
	char comp=fgetc(fp);
	
	fseek(fp,10,0);
	int off=fgetc(fp);
	off=off+(fgetc(fp)<<8);
	fseek(fp,6,0);
	off=off-fgetc(fp);
	ddsd.dwHeight=off+1-(fgetc(fp)<<8);
	
	off=fgetc(fp);
	off=off+(fgetc(fp)<<8);
	fseek(fp,4,0);
	off=off-fgetc(fp);
	ddsd.dwWidth=off+1-(fgetc(fp)<<8);
	
	fseek(fp,66,0);
	off=fgetc(fp);
	int scan=off+(fgetc(fp)<<8);
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;
		
	surfad=Lock(lpDDSTemp);

	fseek(fp,-768,2);
	fread(pal,768,1,fp);
	
	fseek(fp,0x80L,0);	//taille du header:128
	
	if(comp)
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)fgetc(fp);
				if ((octet1 & 0xC0)==0xC0)
				{
					octet1-=0xC0;
					octet2=((int)fgetc(fp))*3;
					for (j=0;j<octet1;j++)
					{
						surfad[i++]=pal[octet2+2];
						surfad[i++]=pal[octet2+1];
						surfad[i++]=pal[octet2];
						surfad[i++]=0;
					}
				}
				else
				{
					octet2=octet1*3;
					surfad[i++]=pal[octet2+2];
					surfad[i++]=pal[octet2+1];
					surfad[i++]=pal[octet2];
					surfad[i++]=0;
				}
			}
		}
	}
	else	
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)fgetc(fp);
				octet2=octet1*3;
				surfad[i++]=pal[octet2+2];
				surfad[i++]=pal[octet2+1];
				surfad[i++]=pal[octet2];
				surfad[i++]=0;
			}
		}
	}

	fclose(fp);
	Unlock(lpDDSTemp);
	return lpDDSTemp;
}


//-----------------------------------------------------------------------------------------
//loader de TGA (24 bits -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdTGA(LPDIRECTDRAW7 lpDD, LPSTR nom)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	FILE* fp;
	BYTE* surfad;
	unsigned int R,G,B/*,A*/;
	unsigned int val;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	
	
	if ((fp=fopen(nom,"rb"))==NULL)
		Error(strcat(nom," not found"));

	int head=fgetc(fp);
	fseek(fp,1,1);
	char comp=fgetc(fp);
	fseek(fp,9L,1);
	int off=fgetc(fp);
	ddsd.dwWidth=(fgetc(fp)<<8)+off;
	off=fgetc(fp);
	ddsd.dwHeight=(fgetc(fp)<<8)+off;
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;

	surfad=Lock(lpDDSTemp);
	
	if(head==0)
		fseek(fp,18L,0);
	else
		fseek(fp,head,0);
	
	if(comp==10)
	{	
		k=ddsd.dwHeight-1;
		i=4*pitch*k;
		
		while(k>=0)
		{
			val=fgetc(fp);
			if((val&0x80)==0x80)
			{
				val-=0x80;
				R=fgetc(fp),G=fgetc(fp),B=fgetc(fp);
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=R;
					surfad[i++]=G;
					surfad[i++]=B;
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
			}
			else
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=fgetc(fp);
					surfad[i++]=fgetc(fp);
					surfad[i++]=fgetc(fp);
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
		}
	}
	else
	{
		for(i=ddsd.dwHeight-1;i>=0;i--)
		{
			for(j=4*pitch*i;j<(signed)(4*pitch*i+4*ddsd.dwWidth);)
			{
				surfad[j++]=fgetc(fp);
				surfad[j++]=fgetc(fp);
				surfad[j++]=fgetc(fp);
				surfad[j++]=0;
				
			}
		}
	}
	
	fclose(fp);
	Unlock(lpDDSTemp);
	return lpDDSTemp;
}















//-----------------------------------------------------------------------------------------
//LOADERS POUR LES RESOURCES A BASE DE .PCX & .TGA
//-----------------------------------------------------------------------------------------
//loader de PCX (265 cols -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdRcPCX(LPDIRECTDRAW7 lpDD, int nom)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	unsigned int fp;
	BYTE* surfad;
	char pal[768];
	int octet1,octet2;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
    
	
	if ((fp=rcopen(nom))==NULL)
	{
		char buf[10];
		Error(strcat("resource not found: ", int2str(nom, buf)));
	}
	
	blobseek(fp,2,0);
	char comp=blobgetc(fp);
	
	blobseek(fp,10,0);
	int off=blobgetc(fp);
	off=off+(blobgetc(fp)<<8);
	blobseek(fp,6,0);
	off=off-blobgetc(fp);
	ddsd.dwHeight=off+1-(blobgetc(fp)<<8);
	
	off=blobgetc(fp);
	off=off+(blobgetc(fp)<<8);
	blobseek(fp,4,0);
	off=off-blobgetc(fp);
	ddsd.dwWidth=off+1-(blobgetc(fp)<<8);
	
	blobseek(fp,66,0);
	off=blobgetc(fp);
	int scan=off+(blobgetc(fp)<<8);
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;
		
	surfad=Lock(lpDDSTemp);

	blobseek(fp,-768,2);
	blobread(pal,768,fp);
	
	blobseek(fp,0x80L,0);	//taille du header:128
	
	if(comp)
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)blobgetc(fp);
				if ((octet1 & 0xC0)==0xC0)
				{
					octet1-=0xC0;
					octet2=((int)blobgetc(fp))*3;
					for (j=0;j<octet1;j++)
					{
						surfad[i++]=pal[octet2+2];
						surfad[i++]=pal[octet2+1];
						surfad[i++]=pal[octet2];
						surfad[i++]=0;
					}
				}
				else
				{
					octet2=octet1*3;
					surfad[i++]=pal[octet2+2];
					surfad[i++]=pal[octet2+1];
					surfad[i++]=pal[octet2];
					surfad[i++]=0;
				}
			}
		}
	}
	else	
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)blobgetc(fp);
				octet2=octet1*3;
				surfad[i++]=pal[octet2+2];
				surfad[i++]=pal[octet2+1];
				surfad[i++]=pal[octet2];
				surfad[i++]=0;
			}
		}
	}

	Unlock(lpDDSTemp);
	return lpDDSTemp;
}


//-----------------------------------------------------------------------------------------
//loader de TGA (24 bits -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdRcTGA(LPDIRECTDRAW7 lpDD, int nom)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	unsigned int fp;
	BYTE* surfad;
	unsigned int R,G,B/*,A*/;
	unsigned int val;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	
	
	if ((fp=rcopen(nom))==NULL)
	{
		char buf[10];
		Error(strcat("resource not found: ", int2str(nom, buf)));
	}


	int head=blobgetc(fp);
	blobseek(fp,1,1);
	char comp=blobgetc(fp);
	blobseek(fp,9L,1);
	int off=blobgetc(fp);
	ddsd.dwWidth=(blobgetc(fp)<<8)+off;
	off=blobgetc(fp);
	ddsd.dwHeight=(blobgetc(fp)<<8)+off;
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;

	surfad=Lock(lpDDSTemp);
	
	if(head==0)
		blobseek(fp,18L,0);
	else
		blobseek(fp,head,0);
	
	if(comp==10)
	{	
		k=ddsd.dwHeight-1;
		i=4*pitch*k;
		
		while(k>=0)
		{
			val=blobgetc(fp);
			if((val&0x80)==0x80)
			{
				val-=0x80;
				R=blobgetc(fp),G=blobgetc(fp),B=blobgetc(fp);
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=R;
					surfad[i++]=G;
					surfad[i++]=B;
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
			}
			else
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
		}
	}
	else
	{
		for(i=ddsd.dwHeight-1;i>=0;i--)
		{
			for(j=4*pitch*i;j<(signed)(4*pitch*i+4*ddsd.dwWidth);)
			{
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=0;
				
			}
		}
	}
	
	Unlock(lpDDSTemp);
	return lpDDSTemp;
}





















//-----------------------------------------------------------------------------------------
//LOADERS POUR LES BLOBS A BASE DE .PCX & .TGA
//-----------------------------------------------------------------------------------------
//loader de PCX (265 cols -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdMemPCX(LPDIRECTDRAW7 lpDD, unsigned int blobfile, int numb)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	unsigned int fp;
	BYTE* surfad;
	char pal[768];
	int octet1,octet2;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
    
	
	fp=blobcrop(blobfile,numb);

	
	blobseek(fp,2,0);
	char comp=blobgetc(fp);
	
	blobseek(fp,10,0);
	int off=blobgetc(fp);
	off=off+(blobgetc(fp)<<8);
	blobseek(fp,6,0);
	off=off-blobgetc(fp);
	ddsd.dwHeight=off+1-(blobgetc(fp)<<8);

	off=blobgetc(fp);
	off=off+(blobgetc(fp)<<8);
	blobseek(fp,4,0);
	off=off-blobgetc(fp);
	ddsd.dwWidth=off+1-(blobgetc(fp)<<8);

	blobseek(fp,66,0);
	off=blobgetc(fp);
	int scan=off+(blobgetc(fp)<<8);
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;
		
	surfad=Lock(lpDDSTemp);

	blobseek(fp,-768,2);
	blobread(pal,768,fp);
	
	blobseek(fp,0x80L,0);	//taille du header:128
	
	if(comp)
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)blobgetc(fp);
				if ((octet1 & 0xC0)==0xC0)
				{
					octet1-=0xC0;
					octet2=((int)blobgetc(fp))*3;
					for (j=0;j<octet1;j++)
					{
						surfad[i++]=pal[octet2+2];
						surfad[i++]=pal[octet2+1];
						surfad[i++]=pal[octet2];
						surfad[i++]=0;
					}
				}
				else
				{
					octet2=octet1*3;
					surfad[i++]=pal[octet2+2];
					surfad[i++]=pal[octet2+1];
					surfad[i++]=pal[octet2];
					surfad[i++]=0;
				}
			}
		}
	}
	else	
	{
		for (k=0;k<(signed)ddsd.dwHeight;k++)
		{
			for(i=4*pitch*k;i<4*(pitch*k+scan);)
			{
				octet1=(int)blobgetc(fp);
				octet2=octet1*3;
				surfad[i++]=pal[octet2+2];
				surfad[i++]=pal[octet2+1];
				surfad[i++]=pal[octet2];
				surfad[i++]=0;
			}
		}
	}

	Unlock(lpDDSTemp);
	return lpDDSTemp;
}


//-----------------------------------------------------------------------------------------
//loader de TGA (24 bits -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdMemTGA(LPDIRECTDRAW7 lpDD, unsigned int blobfile, int numb)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	unsigned int fp;
	BYTE* surfad;
	unsigned int R,G,B/*,A*/;
	unsigned int val;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	

	fp=blobcrop(blobfile,numb);


	int head=blobgetc(fp);
	blobseek(fp,1,1);
	char comp=blobgetc(fp);
	blobseek(fp,9L,1);
	int off=blobgetc(fp);
	ddsd.dwWidth=(blobgetc(fp)<<8)+off;
	off=blobgetc(fp);
	ddsd.dwHeight=(blobgetc(fp)<<8)+off;
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;

	surfad=Lock(lpDDSTemp);
	
	if(head==0)
		blobseek(fp,18L,0);
	else
		blobseek(fp,head,0);
	
	if(comp==10)
	{	
		k=ddsd.dwHeight-1;
		i=4*pitch*k;
		
		while(k>=0)
		{
			val=blobgetc(fp);
			if((val&0x80)==0x80)
			{
				val-=0x80;
				R=blobgetc(fp),G=blobgetc(fp),B=blobgetc(fp);
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=R;
					surfad[i++]=G;
					surfad[i++]=B;
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
			}
			else
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=0;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
		}
	}
	else
	{
		for(i=ddsd.dwHeight-1;i>=0;i--)
		{
			for(j=4*pitch*i;j<(signed)(4*pitch*i+4*ddsd.dwWidth);)
			{
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=0;
				
			}
		}
	}
	
	Unlock(lpDDSTemp);
	return lpDDSTemp;
}




//-----------------------------------------------------------------------------------------
//loader de TGA (avec Alpha channel: 32 bits -> 32 bits)
//-----------------------------------------------------------------------------------------
LPDIRECTDRAWSURFACE7 LdMemTGA32(LPDIRECTDRAW7 lpDD, unsigned int blobfile, int numb)
{
	LPDIRECTDRAWSURFACE7	lpDDSTemp;
	DDSURFACEDESC2			ddsd;
	unsigned int fp;
	BYTE* surfad;
	unsigned int R,G,B,A;
	unsigned int val;
	int i,j,k;
	
    ZeroMemory(&ddsd, sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
	

	fp=blobcrop(blobfile,numb);


	int head=blobgetc(fp);
	blobseek(fp,1,1);
	char comp=blobgetc(fp);
	blobseek(fp,9L,1);
	int off=blobgetc(fp);
	ddsd.dwWidth=(blobgetc(fp)<<8)+off;
	off=blobgetc(fp);
	ddsd.dwHeight=(blobgetc(fp)<<8)+off;
	
	if(lpDD->CreateSurface(&ddsd, &lpDDSTemp, NULL) != DD_OK)
		Error("Memory sucked out by evil pouletz !");

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize=sizeof(ddsd);
	lpDDSTemp->GetSurfaceDesc(&ddsd);
	int pitch=ddsd.lPitch/4;

	surfad=Lock(lpDDSTemp);
	
	if(head==0)
		blobseek(fp,18L,0);
	else
		blobseek(fp,head,0);
	
	if(comp==10)
	{	
		k=ddsd.dwHeight-1;
		i=4*pitch*k;
		
		while(k>=0)
		{
			val=blobgetc(fp);
			if((val&0x80)==0x80)
			{
				val-=0x80;
				R=blobgetc(fp), G=blobgetc(fp), B=blobgetc(fp), A=blobgetc(fp);
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=R;
					surfad[i++]=G;
					surfad[i++]=B;
					surfad[i++]=A;
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
			}
			else
				for(j=0;j<=(signed)val;j++)
				{
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					surfad[i++]=blobgetc(fp);
					if(i>=(signed)(4*pitch*k+4*ddsd.dwWidth))
					{
						k-=1;
						i=4*pitch*k;
					}
				}
		}
	}
	else
	{
		for(i=ddsd.dwHeight-1;i>=0;i--)
		{
			for(j=4*pitch*i;j<(signed)(4*pitch*i+4*ddsd.dwWidth);)
			{
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
				surfad[j++]=blobgetc(fp);
			}
		}
	}
	
	Unlock(lpDDSTemp);
	return lpDDSTemp;
}



#endif