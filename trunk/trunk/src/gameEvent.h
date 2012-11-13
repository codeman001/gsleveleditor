#ifndef _GAMEEVENT_H_
#define _GAMEEVENT_H_

enum GameUserEvent
{
	EvtPlayerMove = 0,
	EvtPlayerCommand,    
    EvtNetworking,
    EvtNetworkDisconected,
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

struct SEventPlayerCommand
{
    bool    shoot;
    bool    reload;
    bool    aim;
    
    SEventPlayerCommand()
    {
        shoot   = false;
        reload  = false;
        aim     = false;
    }
};

struct SEventNetworking
{
    int     eventID;
    int     deviceID;    
    void    *data;
};

struct SEventNetworkingDisconected
{
    int     deviceID;
    short   hostKeyID;
    void    *dev;
};

struct SEventButtonRelease
{
    std::string buttonName;
    void    *data;
};

#endif