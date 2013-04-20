#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: SColor
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:58:23.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_SColor_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_irr__video__SColorf (lua_State* tolua_S)
{
 irr::video::SColorf* self = (irr::video::SColorf*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__video__SColor (lua_State* tolua_S)
{
 irr::video::SColor* self = (irr::video::SColor*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"irr::video::SColorf");
 tolua_usertype(tolua_S,"irr::video::SColor");
}

/* function: irr::video::RGB16 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_RGB1600
static int tolua_SColor_irr_video_RGB1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed int r = (( signed int)  tolua_tonumber(tolua_S,1,0));
  signed int g = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int b = (( signed int)  tolua_tonumber(tolua_S,3,0));
  {
   signed short tolua_ret = ( signed short)  irr::video::RGB16(r,g,b);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RGB16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::RGBA16 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_RGBA1600
static int tolua_SColor_irr_video_RGBA1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed int r = (( signed int)  tolua_tonumber(tolua_S,1,0));
  signed int g = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int b = (( signed int)  tolua_tonumber(tolua_S,3,0));
  signed int a = (( signed int)  tolua_tonumber(tolua_S,4,0));
  {
   signed short tolua_ret = ( signed short)  irr::video::RGBA16(r,g,b,a);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'RGBA16'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::X8R8G8B8toA1R5G5B5 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_X8R8G8B8toA1R5G5B500
static int tolua_SColor_irr_video_X8R8G8B8toA1R5G5B500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed int color = (( signed int)  tolua_tonumber(tolua_S,1,0));
  {
   signed short tolua_ret = ( signed short)  irr::video::X8R8G8B8toA1R5G5B5(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'X8R8G8B8toA1R5G5B5'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::getAlpha */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_getAlpha00
static int tolua_SColor_irr_video_getAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::getAlpha(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::getRed */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_getRed00
static int tolua_SColor_irr_video_getRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::getRed(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::getGreen */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_getGreen00
static int tolua_SColor_irr_video_getGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::getGreen(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::getBlue */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_getBlue00
static int tolua_SColor_irr_video_getBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::getBlue(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::A1R5G5B5toA8R8G8B8 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_A1R5G5B5toA8R8G8B800
static int tolua_SColor_irr_video_A1R5G5B5toA8R8G8B800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::A1R5G5B5toA8R8G8B8(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'A1R5G5B5toA8R8G8B8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::R5G6B5toA8R8G8B8 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_R5G6B5toA8R8G8B800
static int tolua_SColor_irr_video_R5G6B5toA8R8G8B800(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed int tolua_ret = ( signed int)  irr::video::R5G6B5toA8R8G8B8(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'R5G6B5toA8R8G8B8'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::R5G6B5toA1R5G5B5 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_R5G6B5toA1R5G5B500
static int tolua_SColor_irr_video_R5G6B5toA1R5G5B500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed short tolua_ret = ( signed short)  irr::video::R5G6B5toA1R5G5B5(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'R5G6B5toA1R5G5B5'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: irr::video::A1R5G5B5toR5G6B5 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_A1R5G5B5toR5G6B500
static int tolua_SColor_irr_video_A1R5G5B5toR5G6B500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isnumber(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed short color = (( signed short)  tolua_tonumber(tolua_S,1,0));
  {
   signed short tolua_ret = ( signed short)  irr::video::A1R5G5B5toR5G6B5(color);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'A1R5G5B5toR5G6B5'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_new00
static int tolua_SColor_irr_video_SColor_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed int a = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int r = (( signed int)  tolua_tonumber(tolua_S,3,0));
  signed int g = (( signed int)  tolua_tonumber(tolua_S,4,0));
  signed int b = (( signed int)  tolua_tonumber(tolua_S,5,0));
  {
   irr::video::SColor* tolua_ret = (irr::video::SColor*)  new irr::video::SColor(a,r,g,b);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::video::SColor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_new00_local
static int tolua_SColor_irr_video_SColor_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  signed int a = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int r = (( signed int)  tolua_tonumber(tolua_S,3,0));
  signed int g = (( signed int)  tolua_tonumber(tolua_S,4,0));
  signed int b = (( signed int)  tolua_tonumber(tolua_S,5,0));
  {
   irr::video::SColor* tolua_ret = (irr::video::SColor*)  new irr::video::SColor(a,r,g,b);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::video::SColor");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAlpha of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_getAlpha00
static int tolua_SColor_irr_video_SColor_getAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAlpha'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->getAlpha();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRed of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_getRed00
static int tolua_SColor_irr_video_SColor_getRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRed'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->getRed();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getGreen of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_getGreen00
static int tolua_SColor_irr_video_SColor_getGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getGreen'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->getGreen();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getGreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getBlue of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_getBlue00
static int tolua_SColor_irr_video_SColor_getBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getBlue'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->getBlue();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getBlue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setAlpha of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_setAlpha00
static int tolua_SColor_irr_video_SColor_setAlpha00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  signed int a = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setAlpha'",NULL);
#endif
  {
   self->setAlpha(a);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setAlpha'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRed of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_setRed00
static int tolua_SColor_irr_video_SColor_setRed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  signed int r = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRed'",NULL);
#endif
  {
   self->setRed(r);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setGreen of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_setGreen00
static int tolua_SColor_irr_video_SColor_setGreen00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  signed int g = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setGreen'",NULL);
#endif
  {
   self->setGreen(g);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setGreen'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setBlue of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_setBlue00
static int tolua_SColor_irr_video_SColor_setBlue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  signed int b = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setBlue'",NULL);
#endif
  {
   self->setBlue(b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setBlue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toA1R5G5B5 of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_toA1R5G5B500
static int tolua_SColor_irr_video_SColor_toA1R5G5B500(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toA1R5G5B5'",NULL);
#endif
  {
   signed short tolua_ret = ( signed short)  self->toA1R5G5B5();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toA1R5G5B5'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toOpenGLColor of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_toOpenGLColor00
static int tolua_SColor_irr_video_SColor_toOpenGLColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  unsigned char dest = (( unsigned char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toOpenGLColor'",NULL);
#endif
  {
   self->toOpenGLColor(&dest);
   tolua_pushnumber(tolua_S,(lua_Number)dest);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toOpenGLColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_set00
static int tolua_SColor_irr_video_SColor_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  signed int a = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int r = (( signed int)  tolua_tonumber(tolua_S,3,0));
  signed int g = (( signed int)  tolua_tonumber(tolua_S,4,0));
  signed int b = (( signed int)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(a,r,g,b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor__eq00
static int tolua_SColor_irr_video_SColor__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  irr::video::SColor* other = ((irr::video::SColor*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInterpolated of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColor_getInterpolated00
static int tolua_SColor_irr_video_SColor_getInterpolated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::video::SColor",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  irr::video::SColor other = *((irr::video::SColor*)  tolua_tousertype(tolua_S,2,0));
   float d = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInterpolated'",NULL);
#endif
  {
   irr::video::SColor tolua_ret = (irr::video::SColor)  self->getInterpolated(other,d);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::video::SColor(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColor");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::video::SColor));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColor");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInterpolated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: color of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_get_irr__video__SColor_color
static int tolua_get_irr__video__SColor_color(lua_State* tolua_S)
{
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->color);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: color of class  irr::video::SColor */
#ifndef TOLUA_DISABLE_tolua_set_irr__video__SColor_color
static int tolua_set_irr__video__SColor_color(lua_State* tolua_S)
{
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'color'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->color = (( signed int)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_new00
static int tolua_SColor_irr_video_SColorf_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
   float r = ((  float)  tolua_tonumber(tolua_S,2,0));
   float g = ((  float)  tolua_tonumber(tolua_S,3,0));
   float b = ((  float)  tolua_tonumber(tolua_S,4,0));
   float a = ((  float)  tolua_tonumber(tolua_S,5,0));
  {
   irr::video::SColorf* tolua_ret = (irr::video::SColorf*)  new irr::video::SColorf(r,g,b,a);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::video::SColorf");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_new00_local
static int tolua_SColor_irr_video_SColorf_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
   float r = ((  float)  tolua_tonumber(tolua_S,2,0));
   float g = ((  float)  tolua_tonumber(tolua_S,3,0));
   float b = ((  float)  tolua_tonumber(tolua_S,4,0));
   float a = ((  float)  tolua_tonumber(tolua_S,5,0));
  {
   irr::video::SColorf* tolua_ret = (irr::video::SColorf*)  new irr::video::SColorf(r,g,b,a);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::video::SColorf");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: toSColor of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_toSColor00
static int tolua_SColor_irr_video_SColorf_toSColor00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toSColor'",NULL);
#endif
  {
   irr::video::SColor tolua_ret = (irr::video::SColor)  self->toSColor();
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::video::SColor(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColor");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::video::SColor));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColor");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'toSColor'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: r of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_get_irr__video__SColorf_r
static int tolua_get_irr__video__SColorf_r(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->r);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: r of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_set_irr__video__SColorf_r
static int tolua_set_irr__video__SColorf_r(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'r'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->r = ((  float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: g of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_get_irr__video__SColorf_g
static int tolua_get_irr__video__SColorf_g(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->g);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: g of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_set_irr__video__SColorf_g
static int tolua_set_irr__video__SColorf_g(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'g'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->g = ((  float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: b of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_get_irr__video__SColorf_b
static int tolua_get_irr__video__SColorf_b(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->b);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: b of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_set_irr__video__SColorf_b
static int tolua_set_irr__video__SColorf_b(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'b'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b = ((  float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: a of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_get_irr__video__SColorf_a
static int tolua_get_irr__video__SColorf_a(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->a);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: a of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_set_irr__video__SColorf_a
static int tolua_set_irr__video__SColorf_a(lua_State* tolua_S)
{
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'a'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->a = ((  float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_set00
static int tolua_SColor_irr_video_SColorf_set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
   float aa = ((  float)  tolua_tonumber(tolua_S,2,0));
   float rr = ((  float)  tolua_tonumber(tolua_S,3,0));
   float gg = ((  float)  tolua_tonumber(tolua_S,4,0));
   float bb = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(aa,rr,gg,bb);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'set'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInterpolated of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_getInterpolated00
static int tolua_SColor_irr_video_SColorf_getInterpolated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
  irr::video::SColorf other = *((irr::video::SColorf*)  tolua_tousertype(tolua_S,2,0));
   float d = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInterpolated'",NULL);
#endif
  {
   irr::video::SColorf tolua_ret = (irr::video::SColorf)  self->getInterpolated(other,d);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::video::SColorf(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColorf");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::video::SColorf));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::video::SColorf");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInterpolated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setColorComponentValue of class  irr::video::SColorf */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_SColorf_setColorComponentValue00
static int tolua_SColor_irr_video_SColorf_setColorComponentValue00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::video::SColorf",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
  signed int index = (( signed int)  tolua_tonumber(tolua_S,2,0));
   float value = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setColorComponentValue'",NULL);
#endif
  {
   self->setColorComponentValue(index,value);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setColorComponentValue'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_SColor_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"irr",0);
  tolua_beginmodule(tolua_S,"irr");
   tolua_module(tolua_S,"video",0);
   tolua_beginmodule(tolua_S,"video");
    tolua_function(tolua_S,"RGB16",tolua_SColor_irr_video_RGB1600);
    tolua_function(tolua_S,"RGBA16",tolua_SColor_irr_video_RGBA1600);
    tolua_function(tolua_S,"X8R8G8B8toA1R5G5B5",tolua_SColor_irr_video_X8R8G8B8toA1R5G5B500);
    tolua_function(tolua_S,"getAlpha",tolua_SColor_irr_video_getAlpha00);
    tolua_function(tolua_S,"getRed",tolua_SColor_irr_video_getRed00);
    tolua_function(tolua_S,"getGreen",tolua_SColor_irr_video_getGreen00);
    tolua_function(tolua_S,"getBlue",tolua_SColor_irr_video_getBlue00);
    tolua_function(tolua_S,"A1R5G5B5toA8R8G8B8",tolua_SColor_irr_video_A1R5G5B5toA8R8G8B800);
    tolua_function(tolua_S,"R5G6B5toA8R8G8B8",tolua_SColor_irr_video_R5G6B5toA8R8G8B800);
    tolua_function(tolua_S,"R5G6B5toA1R5G5B5",tolua_SColor_irr_video_R5G6B5toA1R5G5B500);
    tolua_function(tolua_S,"A1R5G5B5toR5G6B5",tolua_SColor_irr_video_A1R5G5B5toR5G6B500);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"SColor","irr::video::SColor","",tolua_collect_irr__video__SColor);
    #else
    tolua_cclass(tolua_S,"SColor","irr::video::SColor","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"SColor");
     tolua_function(tolua_S,"new",tolua_SColor_irr_video_SColor_new00);
     tolua_function(tolua_S,"new_local",tolua_SColor_irr_video_SColor_new00_local);
     tolua_function(tolua_S,".call",tolua_SColor_irr_video_SColor_new00_local);
     tolua_function(tolua_S,"getAlpha",tolua_SColor_irr_video_SColor_getAlpha00);
     tolua_function(tolua_S,"getRed",tolua_SColor_irr_video_SColor_getRed00);
     tolua_function(tolua_S,"getGreen",tolua_SColor_irr_video_SColor_getGreen00);
     tolua_function(tolua_S,"getBlue",tolua_SColor_irr_video_SColor_getBlue00);
     tolua_function(tolua_S,"setAlpha",tolua_SColor_irr_video_SColor_setAlpha00);
     tolua_function(tolua_S,"setRed",tolua_SColor_irr_video_SColor_setRed00);
     tolua_function(tolua_S,"setGreen",tolua_SColor_irr_video_SColor_setGreen00);
     tolua_function(tolua_S,"setBlue",tolua_SColor_irr_video_SColor_setBlue00);
     tolua_function(tolua_S,"toA1R5G5B5",tolua_SColor_irr_video_SColor_toA1R5G5B500);
     tolua_function(tolua_S,"toOpenGLColor",tolua_SColor_irr_video_SColor_toOpenGLColor00);
     tolua_function(tolua_S,"set",tolua_SColor_irr_video_SColor_set00);
     tolua_function(tolua_S,".eq",tolua_SColor_irr_video_SColor__eq00);
     tolua_function(tolua_S,"getInterpolated",tolua_SColor_irr_video_SColor_getInterpolated00);
     tolua_variable(tolua_S,"color",tolua_get_irr__video__SColor_color,tolua_set_irr__video__SColor_color);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"SColorf","irr::video::SColorf","",tolua_collect_irr__video__SColorf);
    #else
    tolua_cclass(tolua_S,"SColorf","irr::video::SColorf","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"SColorf");
     tolua_function(tolua_S,"new",tolua_SColor_irr_video_SColorf_new00);
     tolua_function(tolua_S,"new_local",tolua_SColor_irr_video_SColorf_new00_local);
     tolua_function(tolua_S,".call",tolua_SColor_irr_video_SColorf_new00_local);
     tolua_function(tolua_S,"toSColor",tolua_SColor_irr_video_SColorf_toSColor00);
     tolua_variable(tolua_S,"r",tolua_get_irr__video__SColorf_r,tolua_set_irr__video__SColorf_r);
     tolua_variable(tolua_S,"g",tolua_get_irr__video__SColorf_g,tolua_set_irr__video__SColorf_g);
     tolua_variable(tolua_S,"b",tolua_get_irr__video__SColorf_b,tolua_set_irr__video__SColorf_b);
     tolua_variable(tolua_S,"a",tolua_get_irr__video__SColorf_a,tolua_set_irr__video__SColorf_a);
     tolua_function(tolua_S,"set",tolua_SColor_irr_video_SColorf_set00);
     tolua_function(tolua_S,"getInterpolated",tolua_SColor_irr_video_SColorf_getInterpolated00);
     tolua_function(tolua_S,"setColorComponentValue",tolua_SColor_irr_video_SColorf_setColorComponentValue00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46,118,105,100,101,111, 46, 95, 95, 83, 67,
    111,108,111,114, 32, 61, 32,105,114,114, 46,118,105,100,101,
    111, 46, 83, 67,111,108,111,114, 10,102,117,110, 99,116,105,
    111,110, 32,105,114,114, 46,118,105,100,101,111, 46, 83, 67,
    111,108,111,114, 40, 97, 44,114, 44,103, 44, 98, 41, 10,108,
    111, 99, 97,108, 32,114,101,116, 32, 61, 32,105,114,114, 46,
    118,105,100,101,111, 46, 95, 95, 83, 67,111,108,111,114, 58,
    110,101,119, 40, 97, 44,114, 44,103, 44, 98, 41, 10,116,111,
    108,117, 97, 46,116, 97,107,101,111,119,110,101,114,115,104,
    105,112, 40,114,101,116, 41, 10,114,101,116,117,114,110, 32,
    114,101,116, 10,101,110,100, 10,105,114,114, 46,118,105,100,
    101,111, 46, 95, 95, 83, 67,111,108,111,114,102, 32, 61, 32,
    105,114,114, 46,118,105,100,101,111, 46, 83, 67,111,108,111,
    114,102, 10,102,117,110, 99,116,105,111,110, 32,105,114,114,
     46,118,105,100,101,111, 46, 83, 67,111,108,111,114,102, 40,
    114, 44,103, 44, 98, 44, 97, 41, 10, 97, 32, 61, 32, 97, 32,
    111,114, 32, 49, 10,108,111, 99, 97,108, 32,114,101,116, 32,
     61, 32,105,114,114, 46,118,105,100,101,111, 46, 95, 95, 83,
     67,111,108,111,114,102, 58,110,101,119, 40,114, 44,103, 44,
     98, 44, 97, 41, 10,116,111,108,117, 97, 46,116, 97,107,101,
    111,119,110,101,114,115,104,105,112, 40,114,101,116, 41, 10,
    114,101,116,117,114,110, 32,114,101,116, 10,101,110,100,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_SColor (lua_State* tolua_S) {
 return tolua_SColor_open(tolua_S);
};
#endif

