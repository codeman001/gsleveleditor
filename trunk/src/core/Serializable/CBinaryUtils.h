#ifndef _BINARY_UTILS_H_
#define _BINARY_UTILS_H_

#include "stdafx.h"
#include "CGameColladaSceneNode.h"

class CBinaryUtils: public uiSingleton<CBinaryUtils>
{
public:
	CBinaryUtils();
	virtual ~CBinaryUtils();
	
	void saveColladaScene( CGameColladaSceneNode* node );
	void saveColladaMesh( CGameColladaMesh* mesh );	
	void saveMaterial( SMaterial* mat );

};

#endif