#ifndef _APP_INCLUDE_
#define _APP_INCLUDE_

#if defined(GSEDITOR) || defined(GSPARTICLE) || defined(GSANIMATION)
	#include "uiClass.h"
	using namespace uiClass;
#else
	#include "uiClass/uiSingleton.h"
	#include "uiClass/uiString.h"
	using namespace uiClass;	
#endif

#include <irrlicht.h>
#include <os.h>
using namespace irr;
using namespace irr::scene;
using namespace irr::video;


#if defined(GSEDITOR) && defined(GSGAMEPLAY)
// SPARK lib
#include <SPK.h>
#include <SPK_IRR.h>
using namespace SPK;
using namespace SPK::IRR;
#endif 

#include <algorithm>
#include <vector>
#include <list>
#include <map>
#include <stack>
#include <queue>
#include <fstream>
#include <string>

#ifndef GSGAMEPLAY
using namespace std;
#endif

#ifdef GSGAMEPLAY
	#if defined(ANDROID) || defined (IOS) || defined (MACOSX)
		#define USE_ZIPPACKAGE
	#endif
#endif

#ifdef GSEDITOR
	#define STR_APP_TITLE "gseditor - developer: hongduc.dev@gmail.com"
#else
	#define STR_APP_TITLE L"game - developer: hongduc.dev@gmail.com"
#endif

#endif