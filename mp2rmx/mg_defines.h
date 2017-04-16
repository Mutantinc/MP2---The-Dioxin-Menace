//-------------------------------------------------------------------

// MG_DEFINES.H - ALL DEFINES
//-------------------------------------------------------------------
#ifndef _MG_DEFINES_H_
#define _MG_DEFINES_H_


//-------------------------------------------------------------------
//WINDOWS
//-------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN


//-------------------------------------------------------------------
// OGL
//-------------------------------------------------------------------

#undef MG_USE_OPENGL
#define MG_USE_OPENGL
#undef MG_USE_OPENGL_EXT
//#define MG_USE_OPENGL_EXT


#define MG_MAX_TEXTURE_SIZE		2048	//this number is the max value needed. will be compared with the max tex size of the gfx card

//-------------------------------------------------------------------
//SOUND
//-------------------------------------------------------------------
#undef MG_USE_FMOD
#undef MG_USE_MINIFMOD
//#define MG_USE_FMOD		//to use Fmod (defaut if both defined)
#define MG_USE_MINIFMOD	//to use MiniFmod


//-------------------------------------------------------------------
#undef MG_USE_FMUSIC
#ifdef MG_USE_MINIFMOD
#define MG_USE_FMUSIC
#endif

#ifdef MG_USE_FMOD
#define MG_USE_FMUSIC
#endif

#ifdef MG_USE_FMOD
#ifdef MG_USE_MINIFMOD
#undef MG_USE_MINIFMOD
#endif
#endif


//-------------------------------------------------------------------
//SETUP
//-------------------------------------------------------------------
#undef MG_USE_SETUP
//#define MG_USE_SETUP

#undef MG_USE_SKINSETUP
#define MG_USE_SKINSETUP

#undef MG_USE_EXTSETUP
//#define MG_USE_EXTSETUP


//-------------------------------------------------------------------
//MISC
//-------------------------------------------------------------------
//#undef MG_USE_SMALLEST_CODE		//commented as MG_USE_SMALLEST_CODE may be defined in the preprocessor, thus mustn't be undefined
//#define MG_USE_SMALLEST_CODE		//defined in the makefile as a preprocessor definition but can be switched here too


#undef MG_USE_FULLSCREEN
#define MG_USE_FULLSCREEN


#undef MG_USE_SWITCH_FULLSCREEN
//#define MG_USE_SWITCH_FULLSCREEN


#undef MG_USE_FPS_COUNTER
#ifdef _DEBUG
#define MG_USE_FPS_COUNTER		//FPS counter...
#endif


#undef MG_USE_TEMPO
//#define MG_USE_TEMPO


#ifdef MG_USE_SMALLEST_CODE
#define	MG_DATA_DEFAULT_ASM		//file storage method
#else
//#define	MG_DATA_DEFAULT_FILE
//#define	MG_DATA_DEFAULT_ASM
#define	MG_DATA_DEFAULT_ZLIB
#define MG_DATA_ZLIB_PRELOAD	//useful only when zlib files are used. loads the whole zlib file into memory at the begining of runtime instead of loading each stream from the hd on demand and decompressing
//#define MG_DATA_ZLIB_RESOURCE	//useful only when zlib files are used. compressed data lies into the executable as a resource
#endif


#undef MG_USE_GIF				//to enable the gif decoder
//#define MG_USE_GIF
#undef MG_USE_JPG				//to enable the jpg decoder (~9200 bytes footprint)
//#define MG_USE_JPG


#undef MG_USE_MOUSE				//to be able to use the mouse
//#define MG_USE_MOUSE


#undef MG_USE_FREELOOK			//to be able to override the camera and direct it with the keyboard
//#define MG_USE_FREELOOK

//-------------------------------------------------------------------


//END
#endif