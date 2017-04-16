//---------------------------------------------------------------------------------------------
//MG_TIMELINE.H - EFFECTS' CLASS
//---------------------------------------------------------------------------------------------
#ifndef _MG_TIMELINE_H_
#define _MG_TIMELINE_H_



typedef struct
{
	sint Time;				//Time the effect lasts	( < 0 is a neverending effect )
	sint Order;				//Order of the module when the effect starts
	sint Row;				//Row of the Module when the effect starts
	void (*Gfx)();			//Specific graphic function (load/free scene)
	bool (*Sfx)();			//Specific sound function (load/free module), returns true if TimeBase has to be refreshed
	void (*Efx)();			//The effect itself, returns true if the Effect MUST end.
	bool Initialized;		//true if initialization is complete

} mgEffect;




class mgTimeLine
{
public:
	uint FrameTotal;		//total number of frames displayed since the begining of the demo
	uint Frame;				//number of frames displayed in this effect
	
	uint Time;				//time since TimeBase
	uint TimeAbs;			//current absolute time;
	uint TimeBase;			//time of reference (ms since 1970...) when the time system was restarted (the begining one effect)
	uint TimeGap;			//delay between the previous frame and the current one

	uint CurrentEffect;		//effect number
	mgEffect* Effects;		//the list of effects



	mgTimeLine()
	{
		FrameTotal = 0;
		Frame = 0;
		CurrentEffect = 0;
	}
//------------------------------------------------------------------------------------------

	inline void Start(void)
	{
		InitEffect(0);
		ResetTime();
	}

	inline void ResetTime(void)
	{

		Time = 0;
		TimeBase = mgGetTime();
		TimeGap = 0;
	}

//------------------------------------------------------------------------------------------

	inline void InitEffect(uint nb)
	{
		if( !Effects[nb].Initialized )
		{
			Effects[nb].Initialized = true;
			
			if( Effects[nb].Gfx )	//sfx then gfx in case of a ResetTime()
				(*(Effects[nb].Gfx))();
			
			if( Effects[nb].Sfx )
				if( (*(Effects[nb].Sfx))() )
					ResetTime();
		}
	}
	
//------------------------------------------------------------------------------------------
	
	bool Run(void)
	{
		if( Effects[CurrentEffect].Efx )
		{
			TimeAbs = mgGetTime();
			TimeGap = TimeAbs - TimeBase - Time;
			Time = TimeAbs - TimeBase; //Time += TimeGap;

			sint maxtime = Effects[CurrentEffect].Time;

			sint order = 0;
			sint row = 0;
			
#ifdef MG_USE_FMUSIC
			
			order = mgSoundGetOrder(CurMod);
			row = mgSoundGetRow(CurMod);
			bool same;
			
			if( Effects[CurrentEffect].Order < 0 || Effects[CurrentEffect].Row < 0 )	//then time must be used. do not put values when using a mp3 or midi...
				same = ( (signed)Time < maxtime );		//do not replace by <= (the effect can change on thistm=0)
			else
				same = ( order != Effects[CurrentEffect].Order || row != Effects[CurrentEffect].Row );
			
			if( same )	//haven't we reached the end of the effect ?
#else
			if( (signed)Time < maxtime )	//process current effect
#endif
			{
				(*(Effects[CurrentEffect].Efx))();
				
				//framecounter + various infos
#ifdef MG_USE_FPS_COUNTER
				glColor4ub( 255,255,255,180 );
				
				if( TimeGap )	//test to avoid a divby0
				{
					uint fps = 1000/TimeGap;
					Text2D( 0.0f, 0.0f, 0.4f, mgFontList, "fps:%-4d", fps );
				}
				else
				{
					Text2D( 0.0f, 0.0f, 0.4f, mgFontList, "fps:" );
				}

				Text2D( 0.15f, 0.0f, 0.4f, mgFontList, "frames:%-4d", FrameTotal );
				Text2D( 0.40f, 0.0f, 0.4f, mgFontList, "order:%-4d", order );
				Text2D( 0.55f, 0.0f, 0.4f, mgFontList, "row:%-4d", row );
#endif

				mgFlip();
				FrameTotal++;
				Frame++;
				return true;
			}
			else	//go to next effect
			{
				CurrentEffect++;
				InitEffect(CurrentEffect);
				return true;
			}
			
		}
		else
			return false;
	}
	
		
	

//------------------------------------------------------------------------------------------
//Function to start at some effect with synchonized music (needs Fmod Api)
//------------------------------------------------------------------------------------------
#ifdef MG_USE_FMOD
	void GotoEffect( uint nb )
	{
		uint i=0;

		Start();
		while( Effects[i].Efx && i < nb )	//we must get to the post-init of the asked effect
		{
			Time += Effects[i].Time;
			i++;
			InitEffect(i);
		}
		CurrentEffect = i;


		if( CurMod && Effects[i].Order >= 0 && Effects[i].Row >= 0 )
		{
			mgSoundSetOrder(CurMod,Effects[i].Order);
			mgSoundSetRow(CurMod,Effects[i].Row);
		}
		else if( CurStream )
		{
			mgSoundSetTime(CurStream, TimeBase);
		}

		//else, 
		//either there is no sound,
		//or there is a time-synch, it's not proper and the music just starts here...
	}
#endif



};



//-------------------------------------------------------------------
//INIT AND UNINIT DUMMY EFFECTS
//-------------------------------------------------------------------

bool mgEffectTrue(void) { return true; }
bool mgEffectFalse(void) { return false; }
void mgEffectNull(void) {}



mgTimeLine CurTimeLine;
#define mgTime		CurTimeLine.Time
#define mgTimeAbs	CurTimeLine.TimeAbs
#define mgTimeBase	CurTimeLine.TimeBase
#define mgTimeGap	CurTimeLine.TimeGap
#define mgFrame		CurTimeLine.Frame
#define mgFrameAbs	CurTimeLine.FrameAbs



//END
#endif
