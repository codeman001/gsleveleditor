#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: SColor
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:01:28.
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

static int tolua_collect_s16 (lua_State* tolua_S)
{
 s16* self = (s16*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__video__SColorf (lua_State* tolua_S)
{
 irr::video::SColorf* self = (irr::video::SColorf*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_s32 (lua_State* tolua_S)
{
 s32* self = (s32*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"s16");
 tolua_usertype(tolua_S,"u8");
 tolua_usertype(tolua_S,"s32");
 tolua_usertype(tolua_S,"irr::video::SColor");
 tolua_usertype(tolua_S,"irr::video::SColorf");
 tolua_usertype(tolua_S,"f32");
}

/* function: irr::video::RGB16 */
#ifndef TOLUA_DISABLE_tolua_SColor_irr_video_RGB1600
static int tolua_SColor_irr_video_RGB1600(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s32 r = *((s32*)  tolua_tousertype(tolua_S,1,0));
  s32 g = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 b = *((s32*)  tolua_tousertype(tolua_S,3,0));
  {
   s16 tolua_ret = (s16)  irr::video::RGB16(r,g,b);
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s32 r = *((s32*)  tolua_tousertype(tolua_S,1,0));
  s32 g = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 b = *((s32*)  tolua_tousertype(tolua_S,3,0));
  s32 a = *((s32*)  tolua_tousertype(tolua_S,4,0));
  {
   s16 tolua_ret = (s16)  irr::video::RGBA16(r,g,b,a);
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s32 color = *((s32*)  tolua_tousertype(tolua_S,1,0));
  {
   s16 tolua_ret = (s16)  irr::video::X8R8G8B8toA1R5G5B5(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::getAlpha(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::getRed(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::getGreen(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::getBlue(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::A1R5G5B5toA8R8G8B8(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s32 tolua_ret = (s32)  irr::video::R5G6B5toA8R8G8B8(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s16 tolua_ret = (s16)  irr::video::R5G6B5toA1R5G5B5(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,1,"s16",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s16 color = *((s16*)  tolua_tousertype(tolua_S,1,0));
  {
   s16 tolua_ret = (s16)  irr::video::A1R5G5B5toR5G6B5(color);
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s32 a = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 r = *((s32*)  tolua_tousertype(tolua_S,3,0));
  s32 g = *((s32*)  tolua_tousertype(tolua_S,4,0));
  s32 b = *((s32*)  tolua_tousertype(tolua_S,5,0));
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  s32 a = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 r = *((s32*)  tolua_tousertype(tolua_S,3,0));
  s32 g = *((s32*)  tolua_tousertype(tolua_S,4,0));
  s32 b = *((s32*)  tolua_tousertype(tolua_S,5,0));
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
   s32 tolua_ret = (s32)  self->getAlpha();
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
   s32 tolua_ret = (s32)  self->getRed();
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
   s32 tolua_ret = (s32)  self->getGreen();
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
   s32 tolua_ret = (s32)  self->getBlue();
   {
#ifdef __cplusplus
    void* tolua_obj = new s32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s32");
#endif
   }
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  s32 a = *((s32*)  tolua_tousertype(tolua_S,2,0));
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  s32 r = *((s32*)  tolua_tousertype(tolua_S,2,0));
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  s32 g = *((s32*)  tolua_tousertype(tolua_S,2,0));
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  s32 b = *((s32*)  tolua_tousertype(tolua_S,2,0));
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
   s16 tolua_ret = (s16)  self->toA1R5G5B5();
   {
#ifdef __cplusplus
    void* tolua_obj = new s16(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(s16));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"s16");
#endif
   }
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
     !tolua_isusertype(tolua_S,2,"u8",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  u8* dest = ((u8*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'toOpenGLColor'",NULL);
#endif
  {
   self->toOpenGLColor(dest);
  }
 }
 return 0;
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  s32 a = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 r = *((s32*)  tolua_tousertype(tolua_S,3,0));
  s32 g = *((s32*)  tolua_tousertype(tolua_S,4,0));
  s32 b = *((s32*)  tolua_tousertype(tolua_S,5,0));
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
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColor* self = (irr::video::SColor*)  tolua_tousertype(tolua_S,1,0);
  irr::video::SColor other = *((irr::video::SColor*)  tolua_tousertype(tolua_S,2,0));
  f32 d = *((f32*)  tolua_tousertype(tolua_S,3,0));
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
  tolua_pushusertype(tolua_S,(void*)&self->color,"s32");
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
  if (!tolua_isusertype(tolua_S,2,"s32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->color = *((s32*)  tolua_tousertype(tolua_S,2,0))
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
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  f32 r = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 g = *((f32*)  tolua_tousertype(tolua_S,3,0));
  f32 b = *((f32*)  tolua_tousertype(tolua_S,4,0));
  f32 a = *((f32*)  tolua_tousertype(tolua_S,5,0));
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
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  f32 r = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 g = *((f32*)  tolua_tousertype(tolua_S,3,0));
  f32 b = *((f32*)  tolua_tousertype(tolua_S,4,0));
  f32 a = *((f32*)  tolua_tousertype(tolua_S,5,0));
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
  tolua_pushusertype(tolua_S,(void*)&self->r,"f32");
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
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->r = *((f32*)  tolua_tousertype(tolua_S,2,0))
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
  tolua_pushusertype(tolua_S,(void*)&self->g,"f32");
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
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->g = *((f32*)  tolua_tousertype(tolua_S,2,0))
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
  tolua_pushusertype(tolua_S,(void*)&self->b,"f32");
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
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->b = *((f32*)  tolua_tousertype(tolua_S,2,0))
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
  tolua_pushusertype(tolua_S,(void*)&self->a,"f32");
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
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->a = *((f32*)  tolua_tousertype(tolua_S,2,0))
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
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
  f32 aa = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 rr = *((f32*)  tolua_tousertype(tolua_S,3,0));
  f32 gg = *((f32*)  tolua_tousertype(tolua_S,4,0));
  f32 bb = *((f32*)  tolua_tousertype(tolua_S,5,0));
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
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
  irr::video::SColorf other = *((irr::video::SColorf*)  tolua_tousertype(tolua_S,2,0));
  f32 d = *((f32*)  tolua_tousertype(tolua_S,3,0));
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
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::video::SColorf* self = (irr::video::SColorf*)  tolua_tousertype(tolua_S,1,0);
  s32 index = *((s32*)  tolua_tousertype(tolua_S,2,0));
  f32 value = *((f32*)  tolua_tousertype(tolua_S,3,0));
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

