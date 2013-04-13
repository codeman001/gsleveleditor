#ifndef _GAMEEVENT_H_
#define _GAMEEVENT_H_

enum GameUserEvent
{
	EvtPlayerMove = 0,
	EvtPlayerCommand,    
    EvtNetworking,
    EvtNetworkDisconected,
	EvtButtonPress,
    EvtButtonRelease,
	EvtCount
};

struct SEventPlayerMove
{
	float	rotate;
	float	strength;
	bool	run;    
    
	SEventPlayerMove()
	{
		rotate		= 0.0f;
		strength	= 0.0f;
		run			= false;        
	}
};

struct SEventPlayerCommand
{
    bool    shoot;
    bool    reload;
    
    SEventPlayerCommand()
    {
        shoot   = false;
        reload  = false;
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

struct SEventButtonData
{
    std::string buttonName;
    void    *data;
};

#endif