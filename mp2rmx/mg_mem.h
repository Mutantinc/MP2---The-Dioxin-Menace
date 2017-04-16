//-------------------------------------------------------------------
//MG_MEM.H - MEMORY, RESOURCES, FILES MANAGEMENT
//Please note that usually, size_t fread( void *buffer, size_t size, size_t count, FILE *stream ); returns 'count' when successful.
//whereas mgRead returns the number of bytes actually read !!!
//-------------------------------------------------------------------
#ifndef _MG_MEM_H_
#define _MG_MEM_H_


#include "../zlib/zlib.h"


//-----------------------------------------------------------------------------------------
// DEFINITIONS & STRUCT - BEWARE: WORKS FOR MODs TOO
//-----------------------------------------------------------------------------------------
#define		MG_DATA_DEFAULT			0	//default mode defined by the programmer and/or the preprocessor
#define		MG_DATA_FILE			1	//file
#define		MG_DATA_MEMORY			2	//files in memory
#define		MG_DATA_RC				3	//visual studio resources (in mem)
#define		MG_DATA_ASM				4	//asm data (in mem)
#define		MG_DATA_INCLUDED		5	//encapsulated data (file/mem)
#define		MG_DATA_ZLIB_FILE		6	//zlib files decompressed from the archive kept on the hard disk
#define		MG_DATA_ZLIB_MEMORY		7	//zlib files decompressed from the archive previously loaded into memory ( faster access than MG_DATA_ZLIB_FILE ones )

#define		RELEASE(x)	if( x!=0 ) { delete(x); x = NULL; }


typedef struct
{
	uint length;	//length of the memory file
	uint pos;		//offset from begining of memory file
	void *data;		//datas
} MEMFILE;	



uint (*mgOpen)( char* name );
void (*mgClose)( uint handle);
int (*mgRead)( void* buffer, int size, uint handle );
void (*mgSeek)( uint handle, int pos, signed char mode );
int (*mgTell)( uint handle );
uint8 (*mgGetc)( uint handle );

void mgSetDataAccessMode( uint mode );
uint	mgDataAccessMode;

uint	mgDataAccessModeSaved;	//for MG_DATA_INCLUDED only
uint	mgDataVirtualLength;	//for MG_DATA_INCLUDED only

uint	mgZlibFileHandle;		//unique handle for MG_DATA_ZLIB_FILE or MG_DATA_ZLIB_MEMORY
								//code a handle-stack if necessary
uint	mgZlibFastHandle;		//to save the file handle if the file is open several times in a row, preventing several decompressions (eg with LoadSwitch)

//-----------------------------------------------------------------------------------------
//FUNCTIONS ON FILES
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE	//we don't want to have some LIBCT.LIB linkage for files i/o when requesting small code

uint mgFileOpen( char *name)
{
	return (uint)fopen(name, "rb");
}

void mgFileClose(uint handle)
{
	fclose((FILE *)handle);
}

int mgFileRead(void *buffer, int size, uint handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

void mgFileSeek(uint handle, int offset, signed char mode)
{
	/*return*/ fseek((FILE *)handle, offset, mode);
}

int mgFileTell(uint handle)
{
	return ftell((FILE *)handle);
}


uint8 mgFileGetc( uint handle)
{
	return (uint8)fgetc((FILE*)handle);
}

#endif



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON MEMORY FILES : MEMOPEN PUTS A FILE IN MEMORY
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

uint mgMemOpen( char* name )
{
	MEMFILE *memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));

	FILE *fp;
	if( (fp = fopen(name, "rb")) == NULL)
		return NULL;
	
	fseek(fp, 0, SEEK_END);
	memfile->length = ftell(fp);
	memfile->data = malloc(memfile->length);
	memfile->pos = 0;
	
	fseek(fp, 0, SEEK_SET);
	fread(memfile->data, 1, memfile->length, fp);
	fclose(fp);
	
	return (uint)memfile;
}
#endif


void mgMemClose( uint handle )
{
	MEMFILE* memfile = (MEMFILE*)handle;
	RELEASE(memfile->data);
	RELEASE(memfile);
}


int mgMemRead( void *buffer, int size, uint handle )
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;
	
	memcpy(buffer, (char *)memfile->data + memfile->pos, size);
	memfile->pos += size;
	
	return size;
}



void mgMemSeek( uint handle, int offset, signed char mode )
{
	MEMFILE* memfile = (MEMFILE*)handle;

	if (mode == SEEK_SET) 
		memfile->pos = offset;
	else if (mode == SEEK_CUR) 
		memfile->pos += offset;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + offset;
	
	if ( memfile->pos > memfile->length )
		memfile->pos = memfile->length;
	
	//return 0;
}



int mgMemTell( uint handle )
{
	return ((MEMFILE*)handle)->pos;
}



uint8 mgMemGetc( uint handle )	//sans vérification
{
	MEMFILE* memfile = (MEMFILE*)handle;

	//	if (memfile->pos + 1 >= memfile->length)
	//		return NULL;
	
	uint8 buffer;
	buffer = *((uint8 *)memfile->data+memfile->pos);
	memfile->pos += 1;
	return buffer;
}




//-----------------------------------------------------------------------------------------
//FUNCTIONS ON ASM INCLUDED FILES
//-----------------------------------------------------------------------------------------
uint mgAsmOpen( char *name )
{
	MEMFILE		*memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));

	memfile->data = (char*)name + sizeof(uint32);	//+4 bytes (for the length of the following section)
	memfile->length = *(uint32*)name;
	memfile->pos = 0;

	return (uint)memfile;
}


void mgAsmClose( uint handle ) {}


//-----------------------------------------------------------------------------------------
//FUNCTIONS ON FILES DIRECTLY INCLUDED IN THE CODE AND BEING READ (ALREADY OPENED). CAN WORK WITH FILE* OR MEMFILE*
//-----------------------------------------------------------------------------------------
uint mgIncOpen( char *name)
{
	MEMFILE		*memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));

	memfile->data = (void*)name;
	memfile->length = mgDataVirtualLength;
	memfile->pos = 0;
	return (uint)name;
}



void mgIncClose(uint handle)
{
	mgSetDataAccessMode( mgDataAccessModeSaved );
	mgClose(handle);
	mgSetDataAccessMode( MG_DATA_INCLUDED );
}



int mgIncRead(void *buffer, int size, uint handle)
{
	mgSetDataAccessMode( mgDataAccessModeSaved );
	MEMFILE* memfile = (MEMFILE*)handle;
	uint nbread = mgRead(buffer, size, (uint)memfile->data);
	mgSetDataAccessMode( MG_DATA_INCLUDED );
	memfile->pos += nbread;
	return nbread;
}



void mgIncSeek(uint handle, int offset, signed char mode)
{
	mgSetDataAccessMode( mgDataAccessModeSaved );
	MEMFILE* memfile = (MEMFILE*)handle;
	uint absoff;

	if (mode == SEEK_SET)
	{
		memfile->pos = offset;
		absoff = mgTell((uint)memfile->data) - memfile->pos + offset;
	}
	else if (mode == SEEK_CUR) 
	{
		memfile->pos += offset;
		absoff = offset;
	}
	else if (mode == SEEK_END)
	{
		memfile->pos = memfile->length + offset;
		uint savedpos = mgTell((uint)memfile->data);
		mgSeek((uint)memfile->data, 0, SEEK_END);
		absoff = mgTell((uint)memfile->data) - memfile->length - (savedpos - memfile->pos) + offset;
		mgSeek((uint)memfile->data, savedpos, SEEK_SET);
	}

	mgSeek((uint)memfile->data, absoff, mode);
	
	if ( memfile->pos > memfile->length )
		memfile->pos = memfile->length;

	mgSetDataAccessMode( MG_DATA_INCLUDED );
	//return 0;
}



int mgIncTell( uint handle )
{
	mgSetDataAccessMode( mgDataAccessModeSaved );
	return mgTell(handle);
	mgSetDataAccessMode( MG_DATA_INCLUDED );
}



uint8 mgIncGetc( uint handle)
{
	mgSetDataAccessMode( mgDataAccessModeSaved );
	MEMFILE* memfile = (MEMFILE*)handle;
	memfile->pos++;
	uint8 c = (uint8)mgGetc((uint)memfile->data);
	mgSetDataAccessMode( MG_DATA_INCLUDED );
	return c;
}




//-----------------------------------------------------------------------------------------
//FUNCTIONS ON VISUAL STUDIO RESOURCES
//THE INCLUDED DATA MUST BE OF TYPE "RCDATA"
//"name" is the ID (the #define) of the resource
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE

uint mgRcOpen( char *name )
{
	MEMFILE		*memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));
	HRSRC res;

	if((res = FindResource(NULL, MAKEINTRESOURCE((DWORD)name), RT_RCDATA))==NULL)
		return NULL;
	
	HGLOBAL handle = LoadResource(NULL, res);
	
	memfile->data = LockResource(handle);
	memfile->length = SizeofResource(NULL, res);
	memfile->pos = 0;
	
	return (uint)memfile;
}


void mgRcClose( uint handle )
{
	MEMFILE* memfile = (MEMFILE*)handle;
	RELEASE(memfile);
}

#endif



//-----------------------------------------------------------------------------------------
//FUNCTIONS ON ZLIB FILES
//-----------------------------------------------------------------------------------------
#ifndef MG_USE_SMALLEST_CODE


uint mgZlibFileOpen( char* name )	//decompress from file
{
	if( mgZlibFastHandle )
	{
		((MEMFILE*)mgZlibFastHandle)->pos = 0;
		return mgZlibFastHandle;
	}

	uint zf = mgFileOpen( mgZlibFile );
	if( !zf ) return NULL;

	uint count;	mgFileRead( &count, sizeof(uint), zf );
	uint nb = (uint)*((uint16*)name);
	if( nb >= count ) return NULL;

	mgFileSeek( zf, (2*nb)*sizeof(uint), SEEK_CUR );
	uLongf origsize; mgFileRead( &origsize, sizeof(uLongf), zf );
	uint offset; mgFileRead( &offset, sizeof(uint), zf );

	uLong compsize;
	if( nb+1 == count )
	{
		mgFileSeek( zf, 0, SEEK_END );
		compsize = mgFileTell( zf );
	}
	else
	{
		mgFileSeek( zf, sizeof(uint), SEEK_CUR );
		mgFileRead( &compsize, sizeof(uint), zf );
	}
	compsize -= offset;

	mgFileSeek( zf, offset, SEEK_SET );
	Bytef* compdata = (Bytef*)malloc( compsize );
	mgFileRead( compdata, compsize, zf );
	mgFileClose( zf );

	MEMFILE* memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));
	memfile->data = malloc( origsize );
	memfile->length = origsize;
	memfile->pos = 0;
	if( uncompress( (Bytef*)memfile->data, &origsize, compdata, compsize ) != Z_OK )
	{
		RELEASE( compdata );
		RELEASE( memfile->data );
		RELEASE( memfile );
		return NULL;
	}

	RELEASE( compdata );
	mgZlibFastHandle = (uint)memfile;
	return (uint)memfile;
}



uint mgZlibMemOpen( char* name )	//decompress from memory
{
	if( mgZlibFastHandle )
	{
		((MEMFILE*)mgZlibFastHandle)->pos = 0;
		return mgZlibFastHandle;
	}

	if( !mgZlibFileHandle ) return NULL;

	uint count;	mgMemRead( &count, sizeof(uint), mgZlibFileHandle );
	uint nb = (uint)*((uint16*)name);
	if( nb >= count ) return NULL;

	mgMemSeek( mgZlibFileHandle, (2*nb)*sizeof(uint), SEEK_CUR );	//we are already at +4
	uLongf origsize; mgMemRead( &origsize, sizeof(uint), mgZlibFileHandle );
	uint offset; mgMemRead( &offset, sizeof(uint), mgZlibFileHandle );

	uLong compsize;
	if( nb+1 == count )
	{
		mgMemSeek( mgZlibFileHandle, 0, SEEK_END );
		compsize = mgMemTell( mgZlibFileHandle );
	}
	else
	{
		mgMemSeek( mgZlibFileHandle, sizeof(uint), SEEK_CUR );
		mgMemRead( &compsize, sizeof(uint), mgZlibFileHandle );
	}
	compsize -= offset;

	mgMemSeek( mgZlibFileHandle, 0, SEEK_SET );		//to reinitialize for the next access

	MEMFILE* memfile;
	memfile = (MEMFILE *)malloc(sizeof(MEMFILE));
	memfile->data = malloc( origsize );
	memfile->length = origsize;
	memfile->pos = 0;
	if( uncompress( (Bytef*)memfile->data, &origsize, (Bytef*)((MEMFILE*)mgZlibFileHandle)->data + offset, compsize ) != Z_OK )
	{
		RELEASE( memfile->data );
		RELEASE( memfile );
		return NULL;
	}

	mgZlibFastHandle = (uint)memfile;
	return (uint)memfile;
}



void mgZlibClose( uint handle )
{
	if( mgZlibFastHandle )
	{
		mgZlibFastHandle = NULL;
		MEMFILE* memfile = (MEMFILE*)handle;
		RELEASE(memfile->data);
		RELEASE(memfile);
	}
}


#endif



//-----------------------------------------------------------------------------------------
// CALLBACK FUNCTIONS ATTRIBUTION TO BE ABLE TO SEEK DATA ANYWHERE
//-----------------------------------------------------------------------------------------
void mgSetDataAccessMode( uint mode )
{

	if( !mode )
	{
#ifdef MG_DATA_DEFAULT_ASM
		mode = MG_DATA_ASM;
#elif defined( MG_DATA_DEFAULT_FILE )
		mode = MG_DATA_FILE;
#elif defined( MG_DATA_DEFAULT_ZLIB )
#ifdef MG_DATA_ZLIB_PRELOAD
		mode = MG_DATA_ZLIB_MEMORY;
#elif defined( MG_DATA_ZLIB_RESOURCE )
		mode = MG_DATA_ZLIB_MEMORY;
#else
		mode = MG_DATA_ZLIB_FILE;
#endif
#endif
	}

	switch(mode)
	{

	case MG_DATA_ASM:
		mgOpen = mgAsmOpen;
		mgClose = mgAsmClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

#ifndef MG_USE_SMALLEST_CODE
	case MG_DATA_INCLUDED:
		mgDataAccessModeSaved = mgDataAccessMode;
		mgOpen = mgIncOpen;
		mgClose = mgIncClose;
		mgRead = mgIncRead;
		mgSeek = mgIncSeek;
		mgTell = mgIncTell;
		mgGetc = mgIncGetc;
		break;

	case MG_DATA_FILE:
		mgOpen = mgFileOpen;
		mgClose = mgFileClose;
		mgRead = mgFileRead;
		mgSeek = mgFileSeek;
		mgTell = mgFileTell;
		mgGetc = mgFileGetc;
		break;

	case MG_DATA_MEMORY:
		mgOpen = mgMemOpen;
		mgClose = mgMemClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

	case MG_DATA_RC:
		mgOpen = mgRcOpen;
		mgClose = mgRcClose;
		mgRead = mgMemRead;;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

	case MG_DATA_ZLIB_FILE:
		mgOpen = mgZlibFileOpen;
		mgClose = mgZlibClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

	case MG_DATA_ZLIB_MEMORY:
		mgOpen = mgZlibMemOpen;
		mgClose = mgZlibClose;
		mgRead = mgMemRead;
		mgSeek = mgMemSeek;
		mgTell = mgMemTell;
		mgGetc = mgMemGetc;
		break;

#endif
	}

	mgDataAccessMode = mode;
}


//-----------------------------------------------------------------------------------------
// SIMPLE FUNCTIONS TO READ SOME DATA TYPES
//-----------------------------------------------------------------------------------------

uint mgRead8( uint p )
{
	return (uint)mgGetc(p);//may be EOF
}


uint mgRead16( uint p )
{
	uint16 s;
	if( mgRead(&s, sizeof(uint16), p))
		return (uint)s;
	else
		return EOF;
}


uint mgRead32( uint p )
{
	uint32 i;
	if( mgRead(&i, sizeof(uint), p))
		return (uint)i;
	else
		return EOF;
}


float mgReadFlt( uint p )
{
	float f;
	if( mgRead(&f, sizeof(float), p))
		return f;
	else
		return EOF;
}





//END
#endif