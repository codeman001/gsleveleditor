#include "Stdafx.h"
#include "State_Ingame.h"
#include "GameManager.h"

extern StateManager*		g_stateManager;
int SetHeartRatio(lua_State* state)
{
	float ratio_red     = lua_tointeger(state,1)*1.0/100;
	float ratio_pink    = lua_tointeger(state,2)*1.0/100;
	float ratio_blue    = lua_tointeger(state,3)*1.0/100;
	float ratio_black    = lua_tointeger(state,4)*1.0/100;
	
	float ratio_special_1	= lua_tointeger(state,5)*1.0/100;
	float ratio_special_2	= lua_tointeger(state,6)*1.0/100;
	float ratio_special_3	= lua_tointeger(state,6)*1.0/100;
	
	DEBUG_OUT("\n==========================================================\n");
	
	StateInGame * in_game_state = (StateInGame *)g_stateManager->m_StateInGame;
	in_game_state->m_balloons_generator.reset();
	in_game_state->m_balloons_generator.addRatio(0,ratio_red);
	in_game_state->m_balloons_generator.addRatio(1,ratio_pink);
	in_game_state->m_balloons_generator.addRatio(2,ratio_blue);
	in_game_state->m_balloons_generator.addRatio(3,ratio_black);
	in_game_state->m_balloons_generator.addRatio(4,ratio_special_1);
	in_game_state->m_balloons_generator.addRatio(5,ratio_special_2);
	in_game_state->m_balloons_generator.addRatio(6,ratio_special_3);


	DEBUG_OUT("SetHeartRatio:\n Red:%.2f\n Pink:%.2f\n Blue:%.2f\n Black:%.2f\n Special_1:%.2f\n Special_2:%.2f\n Special_3:%.2f",ratio_red,ratio_pink,ratio_blue,ratio_black,ratio_special_1,ratio_special_2,ratio_special_3);
	DEBUG_OUT("\n==========================================================\n");
	return 0;
}

extern int		MAXIMUM_HEART;
extern int		SPWAN_DELAY;
extern int		NUM_RANDOM_HEART;
int SetMaximumHeart(lua_State* state)
{
	int max = lua_tointeger(state,1);
	DEBUG_OUT("\n==========================SetMaximumHeart %d================================\n",max);
	MAXIMUM_HEART= max;
	return 0;
}

extern Heart	Heart_Array[];
int SetSpeed(lua_State* state)
{
	int type		= lua_tointeger(state,1);
	int min_speed	= lua_tointeger(state,2);
	int max_speed	= lua_tointeger(state,3);


	Heart_Array[type].min_speed = min_speed;
	Heart_Array[type].max_speed = max_speed;

	return 0;
}

int SetHeartRate (lua_State* state)
{
	int delay		= lua_tointeger(state,1);
	int num_heart	= lua_tointeger(state,2);
	
	NUM_RANDOM_HEART	= num_heart;
	SPWAN_DELAY			= delay;
	DEBUG_OUT("\n==========================SetHeartRate %d %d================================\n",delay,num_heart);
	return 0;
}

int SetPowerUpRatio (lua_State* state)
{
	int type_0		= lua_tointeger(state,1);
	int type_1		= lua_tointeger(state,2);
	int type_2		= lua_tointeger(state,2);
	int type_3		= lua_tointeger(state,2);
	int type_4		= lua_tointeger(state,2);

	float ratio_0	= type_0*1.0/100;
	float ratio_1	= type_1*1.0/100;
	float ratio_2	= type_2*1.0/100;
	float ratio_3	= type_2*1.0/100;
	float ratio_4	= type_2*1.0/100;
	DEBUG_OUT("\n==========================================================\n");
	
	StateInGame * in_game_state = (StateInGame *)g_stateManager->m_StateInGame;
	in_game_state->m_powerup_generator.reset();
	in_game_state->m_powerup_generator.addRatio(0,ratio_0);
	in_game_state->m_powerup_generator.addRatio(1,ratio_1);
	in_game_state->m_powerup_generator.addRatio(2,ratio_2);
	in_game_state->m_powerup_generator.addRatio(3,ratio_3);
	in_game_state->m_powerup_generator.addRatio(4,ratio_4);

	DEBUG_OUT("Set Powerup Ratio:\nThunder:%.2f \nCombo:%.2f \nGhost:%.2f\nSteel: %.2f \nIce: %.2f ",ratio_0,ratio_1,ratio_2,ratio_3,ratio_4);
	DEBUG_OUT("\n==========================================================\n");
	
	return 0;
}

int AddHeart (lua_State* state)
{
	int type		= lua_tointeger(state,1);
	int speed		= lua_tointeger(state,2);
	int x			= lua_tointeger(state,3);
	int y			= lua_tointeger(state,4);

	GameManager::Instanse()->AddHeart(type,speed,x,y);
	DEBUG_OUT("\n==========================AddHeart %d %d %d %d================================\n",type,speed,x,y);
	return 0;
}

void LuaRegisterCFunction()
{
	REGISTER_C_FUNCTION(SetHeartRatio);
	REGISTER_C_FUNCTION(SetMaximumHeart);
	REGISTER_C_FUNCTION(SetSpeed);
	REGISTER_C_FUNCTION(SetHeartRate);
	REGISTER_C_FUNCTION(SetPowerUpRatio);
	REGISTER_C_FUNCTION(AddHeart);
	
}