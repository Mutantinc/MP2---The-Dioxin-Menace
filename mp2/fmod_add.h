//-------------------------------------------------------------------
//FMOD_ADD.H - POUR ME SIMPLIFIER LA VIE AVEC FMOD
//-------------------------------------------------------------------
#ifndef _FMOD_ADD_H_
#define _FMOD_ADD_H_


//#define USE_MEMLOAD
//#define USE_MEMLOADRESOURCE

#ifdef USE_MINIFMOD
	#include "minifmod.h"
#elif defined(USE_FMOD)
	#include "fmod.h"
#endif


// this is if you want to replace the samples with your own (in case you have compressed them)
void sampleloadcallback(void *buff, int lenbytes, int numbits, int instno, int sampno)
{
	
}

#endif


///////////////////////////////////////////////////////////////////////////////////////////


#ifndef USE_MEMLOAD		//ça veut dire qu'on utilise le support fichier tout le temps

unsigned int fileopen(char *name)
{
	return (unsigned int)fopen(name, "rb");
}

void fileclose(unsigned int handle)
{
	fclose((FILE *)handle);
}

int fileread(void *buffer, int size, unsigned int handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

void fileseek(unsigned int handle, int pos, signed char mode)
{
	fseek((FILE *)handle, pos, mode);
}

int filetell(unsigned int handle)
{
	return ftell((FILE *)handle);
}

#else		//sinon ça veut dire qu'on va charger les fichiers/ressources, puis n'utiliser que la mémoire

typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;


unsigned int memopen(char *name)
{
	MEMFILE *memfile;

	memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);




#ifndef USE_MEMLOADRESOURCE
	{	// load an external file and read it
		FILE *fp;
		fp = fopen(name, "rb");
		fseek(fp, 0, SEEK_END);
		memfile->length = ftell(fp);
		memfile->data = calloc(memfile->length,1);
		memfile->pos = 0;

		fseek(fp, 0, SEEK_SET);
		fread(memfile->data, 1, memfile->length, fp);
		fclose(fp);
	}
#else
	{	// hey look some load from resource code!
		HRSRC		rec;
		HGLOBAL		handle;

		rec = FindResource(NULL, name, RT_RCDATA);
		handle = LoadResource(NULL, rec);
		
		memfile->data = LockResource(handle);
		memfile->length = SizeofResource(NULL, rec);
		memfile->pos = 0;
	}
#endif

	return (unsigned int)memfile;
}

void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

#ifndef USE_MEMLOADRESOURCE
	free(memfile->data);			// dont free it if it was initialized with LockResource
#endif

	free(memfile);
}

int memread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;

	memcpy(buffer, (char *)memfile->data+memfile->pos, size);
	memfile->pos += size;
	
	return size;
}

void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}
#endif
