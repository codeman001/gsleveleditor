#ifndef _APP_INCLUDE_
#define _APP_INCLUDE_

#if defined(GSEDITOR) || defined(PARTICLE_EDITOR) || defined(GSANIMATION)
	#include "uiClass.h"
	using namespace uiClass;
#else
	#include "uiSingleton.h"
	#include "uiString.h"
	using namespace uiClass;
#endif

#include <irrlicht.h>
#include <os.h>
using namespace irr;
using namespace irr::scene;
using namespace irr::video;


// SPARK lib
#include <SPK.h>
#include <SPK_IRR.h>
using namespace SPK;
using namespace SPK::IRR;

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
	#define STR_APP_TITLE L"game"
#endif

#endif