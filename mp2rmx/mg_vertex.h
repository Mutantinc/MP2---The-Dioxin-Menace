//-------------------------------------------------------------------
//MG_VERTEX.H
//-------------------------------------------------------------------
#ifndef _MG_VERTEX_H_
#define _MG_VERTEX_H_


typedef struct
{
	mgVector Vector;
	mgVector Normal;
	mgColor Color;

	mgBone* Bone;

} mgVertex;



//END
#endif
