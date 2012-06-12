#ifndef _MENUFXOBJ_H_
#define _MENUFXOBJ_H_

#include "CGameUI.h"

class CMenuFxObj
{
protected:
	gameswf::character	*m_character;
public:
	CMenuFxObj( gameswf::character* ch );

	virtual ~CMenuFxObj();
};

#endif