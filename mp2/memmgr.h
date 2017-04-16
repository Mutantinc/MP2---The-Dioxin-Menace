//-------------------------------------------------------------------
//MEMMGR.H - POUR GESTION DE LA MEMOIRE, DES RESSOURCES, DES FICHIERS
//-------------------------------------------------------------------
#ifndef _MEMMGR_H_
#define _MEMMGR_H_

typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;




//-----------------------------------------------------------------------------------------
//FONCTIONS SUR LES RESOURCES
//-----------------------------------------------------------------------------------------

unsigned int rcopen(int name)
{
	MEMFILE *memfile;

	memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);

	HRSRC		rec;
	HGLOBAL		handle;

	if((rec = rec = FindResource(NULL, MAKEINTRESOURCE(name), RT_RCDATA))==NULL)
		return NULL;

	handle = LoadResource(NULL, rec);
		
	memfile->data = LockResource(handle);
	memfile->length = SizeofResource(NULL, rec);
	memfile->pos = 0;

	return (unsigned int)memfile;
}


void rcclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;
	free(memfile);
}





//-----------------------------------------------------------------------------------------
//FONCTIONS SUR LA MEMOIRE & LES BLOBS
//-----------------------------------------------------------------------------------------

unsigned int blobopen(LPSTR nom)
{
		MEMFILE *blobfile;
		blobfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);

		FILE *fp;
		fp = fopen(nom, "rb");
		fseek(fp, 0, SEEK_END);
		blobfile->length = ftell(fp);
		blobfile->data = calloc(blobfile->length,1);
		blobfile->pos = 0;

		fseek(fp, 0, SEEK_SET);
		fread(blobfile->data, 1, blobfile->length, fp);
		fclose(fp);

		return (unsigned int)blobfile;
}



void blobclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;
	free(memfile->data);
	free(memfile);
}



int blobread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;

	memcpy(buffer, (char *)memfile->data+memfile->pos, size);
	memfile->pos += size;
	
	return size;
}



void blobseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + pos;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}



int blobtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}



unsigned char blobgetc(unsigned int handle)
{
	char buffer;
	MEMFILE *memfile = (MEMFILE *)handle;

//speed concern
//	if (memfile->pos + 1 >= memfile->length)
//		return NULL;

	buffer=((char *)memfile->data+memfile->pos)[0];
	memfile->pos += 1;
	
	return buffer;
}



unsigned int blobcrop(unsigned int handle, int nb)
{
	MEMFILE *memblob = (MEMFILE *)handle;
	MEMFILE *memfile;
	memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);

	int* data= (int*)memblob->data;
	int* first = data + data[0] + 1;	//on ne multiplie pas par 4 car on a un pointeur de "int", pas de "char"
	
	if (!nb)
	{
		memfile->data = first;
		memfile->length = data[1];
	}
	else
	{
		memfile->data = (char*)first + data[nb];
		memfile->length = data[nb+1] - data[nb];
	}

	memfile->pos = 0;

	return (unsigned int)memfile;
}


#endif