#ifndef _APP_INCLUDE_
#define _APP_INCLUDE_

#ifdef GSEDITOR
	#include "uiClass.h"
	using namespace uiClass;
#else
	#include "uiString.h"
	using namespace uiClass;
#endif

#include <irrlicht.h>
using namespace irr;
using namespace irr::scene;
using namespace irr::video;

#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <fstream>
#include <string>

using namespace std;

#ifdef GSEDITOR
	#define STR_APP_TITLE "gseditor"
#else
	#define STR_APP_TITLE "game"
#endif

#endif