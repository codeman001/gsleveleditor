#include "stdafx.h"

using namespace irr;
using namespace irr::core;

/*
** Lua binding: aabbox3d
** Generated automatically by tolua++-1.0.92 on 04/21/13 13:56:34.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_aabbox3d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_EIntersectionRelation3D (lua_State* tolua_S)
{
 EIntersectionRelation3D* self = (EIntersectionRelation3D*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__vector3d_f32_ (lua_State* tolua_S)
{
 irr::core::vector3d<f32>* self = (irr::core::vector3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__aabbox3d_f32_ (lua_State* tolua_S)
{
 irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"irr::core::plane3d<f32>");
 tolua_usertype(tolua_S,"irr::core::aabbox3d<f32>");
 tolua_usertype(tolua_S,"irr::core::line3d<f32>");
 tolua_usertype(tolua_S,"EIntersectionRelation3D");
 tolua_usertype(tolua_S,"irr::core::vector3d<f32>");
}

/* method: new of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::aabbox3d<f32>");
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

/* method: new_local of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new00_local
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::aabbox3d<f32>");
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

/* method: new of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new01
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector3d<f32>* min = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* max = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(*min,*max);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new01_local
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector3d<f32>* min = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* max = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(*min,*max);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new02
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector3d<f32>* init = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(*init);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new02_local
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector3d<f32>* init = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(*init);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new03
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
   float minx = ((  float)  tolua_tonumber(tolua_S,2,0));
   float miny = ((  float)  tolua_tonumber(tolua_S,3,0));
   float minz = ((  float)  tolua_tonumber(tolua_S,4,0));
   float maxx = ((  float)  tolua_tonumber(tolua_S,5,0));
   float maxy = ((  float)  tolua_tonumber(tolua_S,6,0));
   float maxz = ((  float)  tolua_tonumber(tolua_S,7,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(minx,miny,minz,maxx,maxy,maxz);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__new03_local
static int tolua_aabbox3d_irr_core_aabbox3d_f32__new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
   float minx = ((  float)  tolua_tonumber(tolua_S,2,0));
   float miny = ((  float)  tolua_tonumber(tolua_S,3,0));
   float minz = ((  float)  tolua_tonumber(tolua_S,4,0));
   float maxx = ((  float)  tolua_tonumber(tolua_S,5,0));
   float maxy = ((  float)  tolua_tonumber(tolua_S,6,0));
   float maxz = ((  float)  tolua_tonumber(tolua_S,7,0));
  {
   irr::core::aabbox3d<f32>* tolua_ret = (irr::core::aabbox3d<f32>*)  new irr::core::aabbox3d<f32>(minx,miny,minz,maxx,maxy,maxz);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::aabbox3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32___eq00
static int tolua_aabbox3d_irr_core_aabbox3d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* other = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: addInternalPoint of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalPoint00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* p = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addInternalPoint'",NULL);
#endif
  {
   self->addInternalPoint(*p);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addInternalPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: addInternalBox of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalBox00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* b = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'addInternalBox'",NULL);
#endif
  {
   self->addInternalBox(*b);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'addInternalBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reset of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__reset00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__reset00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
   float x = ((  float)  tolua_tonumber(tolua_S,2,0));
   float y = ((  float)  tolua_tonumber(tolua_S,3,0));
   float z = ((  float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reset'",NULL);
#endif
  {
   self->reset(x,y,z);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reset'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: reset of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__reset01
static int tolua_aabbox3d_irr_core_aabbox3d_f32__reset01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* initValue = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reset'",NULL);
#endif
  {
   self->reset(*initValue);
  }
 }
 return 0;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__reset00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: reset of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__reset02
static int tolua_aabbox3d_irr_core_aabbox3d_f32__reset02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* initValue = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reset'",NULL);
#endif
  {
   self->reset(*initValue);
  }
 }
 return 0;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__reset01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPointInside of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__isPointInside00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__isPointInside00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* p = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointInside'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPointInside(*p);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointInside'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPointTotalInside of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__isPointTotalInside00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__isPointTotalInside00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* p = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointTotalInside'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPointTotalInside(*p);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointTotalInside'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersectsWithBox of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithBox00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* other = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersectsWithBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersectsWithBox(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'intersectsWithBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersectsWithLine of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::line3d<f32>* line = ((irr::core::line3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersectsWithLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersectsWithLine(*line);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'intersectsWithLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersectsWithLine of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine01
static int tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* linemiddle = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* linevect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
   float halflength = ((  float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersectsWithLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersectsWithLine(*linemiddle,*linevect,halflength);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: classifyPlaneRelation of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__classifyPlaneRelation00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__classifyPlaneRelation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* plane = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'classifyPlaneRelation'",NULL);
#endif
  {
   EIntersectionRelation3D tolua_ret = (EIntersectionRelation3D)  self->classifyPlaneRelation(*plane);
   {
#ifdef __cplusplus
    void* tolua_obj = new EIntersectionRelation3D(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EIntersectionRelation3D");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(EIntersectionRelation3D));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"EIntersectionRelation3D");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'classifyPlaneRelation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getCenter of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__getCenter00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__getCenter00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getCenter'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->getCenter();
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::vector3d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector3d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::vector3d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector3d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getCenter'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getExtent of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__getExtent00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__getExtent00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getExtent'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->getExtent();
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::vector3d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector3d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::vector3d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector3d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getExtent'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getEdges of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__getEdges00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__getEdges00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* edges = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getEdges'",NULL);
#endif
  {
   self->getEdges(edges);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getEdges'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isEmpty of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__isEmpty00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__isEmpty00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isEmpty'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isEmpty();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isEmpty'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: repair of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__repair00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__repair00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'repair'",NULL);
#endif
  {
   self->repair();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'repair'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInterpolated of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_aabbox3d_irr_core_aabbox3d_f32__getInterpolated00
static int tolua_aabbox3d_irr_core_aabbox3d_f32__getInterpolated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* other = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
   float d = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInterpolated'",NULL);
#endif
  {
   irr::core::aabbox3d<f32> tolua_ret = (irr::core::aabbox3d<f32>)  self->getInterpolated(*other,d);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::aabbox3d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::aabbox3d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::aabbox3d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::aabbox3d<f32>");
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

/* get function: MinEdge of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__aabbox3d_f32__MinEdge
static int tolua_get_irr__core__aabbox3d_f32__MinEdge(lua_State* tolua_S)
{
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MinEdge'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->MinEdge,"irr::core::vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MinEdge of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__aabbox3d_f32__MinEdge
static int tolua_set_irr__core__aabbox3d_f32__MinEdge(lua_State* tolua_S)
{
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MinEdge'",NULL);
  if (!tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MinEdge = *((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: MaxEdge of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__aabbox3d_f32__MaxEdge
static int tolua_get_irr__core__aabbox3d_f32__MaxEdge(lua_State* tolua_S)
{
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MaxEdge'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->MaxEdge,"irr::core::vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: MaxEdge of class  irr::core::aabbox3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__aabbox3d_f32__MaxEdge
static int tolua_set_irr__core__aabbox3d_f32__MaxEdge(lua_State* tolua_S)
{
  irr::core::aabbox3d<f32>* self = (irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'MaxEdge'",NULL);
  if (!tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->MaxEdge = *((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_aabbox3d_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"irr",0);
  tolua_beginmodule(tolua_S,"irr");
   tolua_module(tolua_S,"core",0);
   tolua_beginmodule(tolua_S,"core");
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"aabbox3d_f32_","irr::core::aabbox3d<f32>","",tolua_collect_irr__core__aabbox3d_f32_);
    #else
    tolua_cclass(tolua_S,"aabbox3d_f32_","irr::core::aabbox3d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"aabbox3d_f32_");
     tolua_function(tolua_S,"new",tolua_aabbox3d_irr_core_aabbox3d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_aabbox3d_irr_core_aabbox3d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_aabbox3d_irr_core_aabbox3d_f32__new00_local);
     tolua_function(tolua_S,"new",tolua_aabbox3d_irr_core_aabbox3d_f32__new01);
     tolua_function(tolua_S,"new_local",tolua_aabbox3d_irr_core_aabbox3d_f32__new01_local);
     tolua_function(tolua_S,".call",tolua_aabbox3d_irr_core_aabbox3d_f32__new01_local);
     tolua_function(tolua_S,"new",tolua_aabbox3d_irr_core_aabbox3d_f32__new02);
     tolua_function(tolua_S,"new_local",tolua_aabbox3d_irr_core_aabbox3d_f32__new02_local);
     tolua_function(tolua_S,".call",tolua_aabbox3d_irr_core_aabbox3d_f32__new02_local);
     tolua_function(tolua_S,"new",tolua_aabbox3d_irr_core_aabbox3d_f32__new03);
     tolua_function(tolua_S,"new_local",tolua_aabbox3d_irr_core_aabbox3d_f32__new03_local);
     tolua_function(tolua_S,".call",tolua_aabbox3d_irr_core_aabbox3d_f32__new03_local);
     tolua_function(tolua_S,".eq",tolua_aabbox3d_irr_core_aabbox3d_f32___eq00);
     tolua_function(tolua_S,"addInternalPoint",tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalPoint00);
     tolua_function(tolua_S,"addInternalBox",tolua_aabbox3d_irr_core_aabbox3d_f32__addInternalBox00);
     tolua_function(tolua_S,"reset",tolua_aabbox3d_irr_core_aabbox3d_f32__reset00);
     tolua_function(tolua_S,"reset",tolua_aabbox3d_irr_core_aabbox3d_f32__reset01);
     tolua_function(tolua_S,"reset",tolua_aabbox3d_irr_core_aabbox3d_f32__reset02);
     tolua_function(tolua_S,"isPointInside",tolua_aabbox3d_irr_core_aabbox3d_f32__isPointInside00);
     tolua_function(tolua_S,"isPointTotalInside",tolua_aabbox3d_irr_core_aabbox3d_f32__isPointTotalInside00);
     tolua_function(tolua_S,"intersectsWithBox",tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithBox00);
     tolua_function(tolua_S,"intersectsWithLine",tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine00);
     tolua_function(tolua_S,"intersectsWithLine",tolua_aabbox3d_irr_core_aabbox3d_f32__intersectsWithLine01);
     tolua_function(tolua_S,"classifyPlaneRelation",tolua_aabbox3d_irr_core_aabbox3d_f32__classifyPlaneRelation00);
     tolua_function(tolua_S,"getCenter",tolua_aabbox3d_irr_core_aabbox3d_f32__getCenter00);
     tolua_function(tolua_S,"getExtent",tolua_aabbox3d_irr_core_aabbox3d_f32__getExtent00);
     tolua_function(tolua_S,"getEdges",tolua_aabbox3d_irr_core_aabbox3d_f32__getEdges00);
     tolua_function(tolua_S,"isEmpty",tolua_aabbox3d_irr_core_aabbox3d_f32__isEmpty00);
     tolua_function(tolua_S,"repair",tolua_aabbox3d_irr_core_aabbox3d_f32__repair00);
     tolua_function(tolua_S,"getInterpolated",tolua_aabbox3d_irr_core_aabbox3d_f32__getInterpolated00);
     tolua_variable(tolua_S,"MinEdge",tolua_get_irr__core__aabbox3d_f32__MinEdge,tolua_set_irr__core__aabbox3d_f32__MinEdge);
     tolua_variable(tolua_S,"MaxEdge",tolua_get_irr__core__aabbox3d_f32__MaxEdge,tolua_set_irr__core__aabbox3d_f32__MaxEdge);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46, 97, 97, 98, 98,111,
    120, 51,100, 32, 61, 32,105,114,114, 46, 99,111,114,101, 46,
     97, 97, 98, 98,111,120, 51,100, 95,102, 51, 50, 95, 10,102,
    117,110, 99,116,105,111,110, 32,105,114,114, 46, 99,111,114,
    101, 46, 97, 97, 98, 98,111,120, 51,100, 95,102, 51, 50, 95,
     58,103,101,116, 69,100,103,101,115, 40, 41, 10,108,111, 99,
     97,108, 32,109,105,100,100,108,101, 32, 61, 32, 40,115,101,
    108,102, 46, 77,105,110, 69,100,103,101, 32, 43, 32,115,101,
    108,102, 46, 77, 97,120, 69,100,103,101, 41, 32, 47, 32,105,
    114,114, 46, 99,111,114,101, 46,118,101, 99,116,111,114, 51,
    100, 40, 50, 44, 50, 44, 50, 41, 10,108,111, 99, 97,108, 32,
    100,105, 97,103, 32, 61, 32,109,105,100,100,108,101, 32, 45,
     32,115,101,108,102, 46, 77, 97,120, 69,100,103,101, 10,108,
    111, 99, 97,108, 32,101,100,103,101,115, 32, 61, 32,123,125,
     10,101,100,103,101,115, 91, 49, 93, 32, 61, 32,105,114,114,
     46, 99,111,114,101, 46,118,101, 99,116,111,114, 51,100, 40,
    109,105,100,100,108,101, 46, 88, 32, 43, 32,100,105, 97,103,
     46, 88, 44, 32,109,105,100,100,108,101, 46, 89, 32, 43, 32,
    100,105, 97,103, 46, 89, 44, 32,109,105,100,100,108,101, 46,
     90, 32, 43, 32,100,105, 97,103, 46, 90, 41, 10,101,100,103,
    101,115, 91, 50, 93, 32, 61, 32,105,114,114, 46, 99,111,114,
    101, 46,118,101, 99,116,111,114, 51,100, 40,109,105,100,100,
    108,101, 46, 88, 32, 43, 32,100,105, 97,103, 46, 88, 44, 32,
    109,105,100,100,108,101, 46, 89, 32, 45, 32,100,105, 97,103,
     46, 89, 44, 32,109,105,100,100,108,101, 46, 90, 32, 43, 32,
    100,105, 97,103, 46, 90, 41, 10,101,100,103,101,115, 91, 51,
     93, 32, 61, 32,105,114,114, 46, 99,111,114,101, 46,118,101,
     99,116,111,114, 51,100, 40,109,105,100,100,108,101, 46, 88,
     32, 43, 32,100,105, 97,103, 46, 88, 44, 32,109,105,100,100,
    108,101, 46, 89, 32, 43, 32,100,105, 97,103, 46, 89, 44, 32,
    109,105,100,100,108,101, 46, 90, 32, 45, 32,100,105, 97,103,
     46, 90, 41, 10,101,100,103,101,115, 91, 52, 93, 32, 61, 32,
    105,114,114, 46, 99,111,114,101, 46,118,101, 99,116,111,114,
     51,100, 40,109,105,100,100,108,101, 46, 88, 32, 43, 32,100,
    105, 97,103, 46, 88, 44, 32,109,105,100,100,108,101, 46, 89,
     32, 45, 32,100,105, 97,103, 46, 89, 44, 32,109,105,100,100,
    108,101, 46, 90, 32, 45, 32,100,105, 97,103, 46, 90, 41, 10,
    101,100,103,101,115, 91, 53, 93, 32, 61, 32,105,114,114, 46,
     99,111,114,101, 46,118,101, 99,116,111,114, 51,100, 40,109,
    105,100,100,108,101, 46, 88, 32, 45, 32,100,105, 97,103, 46,
     88, 44, 32,109,105,100,100,108,101, 46, 89, 32, 43, 32,100,
    105, 97,103, 46, 89, 44, 32,109,105,100,100,108,101, 46, 90,
     32, 43, 32,100,105, 97,103, 46, 90, 41, 10,101,100,103,101,
    115, 91, 54, 93, 32, 61, 32,105,114,114, 46, 99,111,114,101,
     46,118,101, 99,116,111,114, 51,100, 40,109,105,100,100,108,
    101, 46, 88, 32, 45, 32,100,105, 97,103, 46, 88, 44, 32,109,
    105,100,100,108,101, 46, 89, 32, 45, 32,100,105, 97,103, 46,
     89, 44, 32,109,105,100,100,108,101, 46, 90, 32, 43, 32,100,
    105, 97,103, 46, 90, 41, 10,101,100,103,101,115, 91, 55, 93,
     32, 61, 32,105,114,114, 46, 99,111,114,101, 46,118,101, 99,
    116,111,114, 51,100, 40,109,105,100,100,108,101, 46, 88, 32,
     45, 32,100,105, 97,103, 46, 88, 44, 32,109,105,100,100,108,
    101, 46, 89, 32, 43, 32,100,105, 97,103, 46, 89, 44, 32,109,
    105,100,100,108,101, 46, 90, 32, 45, 32,100,105, 97,103, 46,
     90, 41, 10,101,100,103,101,115, 91, 56, 93, 32, 61, 32,105,
    114,114, 46, 99,111,114,101, 46,118,101, 99,116,111,114, 51,
    100, 40,109,105,100,100,108,101, 46, 88, 32, 45, 32,100,105,
     97,103, 46, 88, 44, 32,109,105,100,100,108,101, 46, 89, 32,
     45, 32,100,105, 97,103, 46, 89, 44, 32,109,105,100,100,108,
    101, 46, 90, 32, 45, 32,100,105, 97,103, 46, 90, 41, 10,114,
    101,116,117,114,110, 32,101,100,103,101,115, 10,101,110,100,
    32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_aabbox3d (lua_State* tolua_S) {
 return tolua_aabbox3d_open(tolua_S);
};
#endif

