#ifndef _GAMEEVENT_H_
#define _GAMEEVENT_H_

enum GameUserEvent
{
	EvtPlayerMove = 0,
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

#endif