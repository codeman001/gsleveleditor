#ifndef _GAMEEVENT_H_
#define _GAMEEVENT_H_

enum GameUserEvent
{
	EvtPlayerMove = 0,
    EvtNetworking,
    EvtButtonRelease,
	EvtCount
};

struct SEventPlayerMove
{
	float	rotate;
	float	strength;
	bool	run;
    bool    runFast;
    
	SEventPlayerMove()
	{
		rotate		= 0.0f;
		strength	= 0.0f;
		run			= false;
        runFast     = false;
	}
};

struct SEventNetworking
{
    int     eventID;
    int     deviceID;    
    void    *data;
};

struct SEventButtonRelease
{
    std::string buttonName;
    void    *data;
};

#endif