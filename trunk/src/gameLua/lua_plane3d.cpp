#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: plane3d
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:01:27.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_plane3d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_f32 (lua_State* tolua_S)
{
 f32* self = (f32*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__plane3d_f32_ (lua_State* tolua_S)
{
 irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_vector3d_f32_ (lua_State* tolua_S)
{
 vector3d<f32>* self = (vector3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"irr::core::plane3d<f32>");
 tolua_usertype(tolua_S,"f32");
 tolua_usertype(tolua_S,"vector3d<f32>");
}

/* method: new of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new00
static int tolua_plane3d_irr_core_plane3d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::plane3d<f32>");
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

/* method: new_local of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new00_local
static int tolua_plane3d_irr_core_plane3d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::plane3d<f32>");
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

/* method: new of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new01
static int tolua_plane3d_irr_core_plane3d_f32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  vector3d<f32>* MPoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* Normal = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*MPoint,*Normal);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new01_local
static int tolua_plane3d_irr_core_plane3d_f32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  vector3d<f32>* MPoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* Normal = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*MPoint,*Normal);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new02
static int tolua_plane3d_irr_core_plane3d_f32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 px = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 py = *((f32*)  tolua_tousertype(tolua_S,3,0));
  f32 pz = *((f32*)  tolua_tousertype(tolua_S,4,0));
  f32 nx = *((f32*)  tolua_tousertype(tolua_S,5,0));
  f32 ny = *((f32*)  tolua_tousertype(tolua_S,6,0));
  f32 nz = *((f32*)  tolua_tousertype(tolua_S,7,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(px,py,pz,nx,ny,nz);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new02_local
static int tolua_plane3d_irr_core_plane3d_f32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,5,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,6,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,7,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,8,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 px = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 py = *((f32*)  tolua_tousertype(tolua_S,3,0));
  f32 pz = *((f32*)  tolua_tousertype(tolua_S,4,0));
  f32 nx = *((f32*)  tolua_tousertype(tolua_S,5,0));
  f32 ny = *((f32*)  tolua_tousertype(tolua_S,6,0));
  f32 nz = *((f32*)  tolua_tousertype(tolua_S,7,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(px,py,pz,nx,ny,nz);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new03
static int tolua_plane3d_irr_core_plane3d_f32__new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::plane3d<f32>* other = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new03_local
static int tolua_plane3d_irr_core_plane3d_f32__new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::plane3d<f32>* other = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new04
static int tolua_plane3d_irr_core_plane3d_f32__new04(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  vector3d<f32>* point1 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* point2 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* point3 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*point1,*point2,*point3);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new03(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__new04_local
static int tolua_plane3d_irr_core_plane3d_f32__new04_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  vector3d<f32>* point1 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* point2 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* point3 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
  {
   irr::core::plane3d<f32>* tolua_ret = (irr::core::plane3d<f32>*)  new irr::core::plane3d<f32>(*point1,*point2,*point3);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::plane3d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__new03_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32___eq00
static int tolua_plane3d_irr_core_plane3d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* other = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: setPlane of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__setPlane00
static int tolua_plane3d_irr_core_plane3d_f32__setPlane00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* point = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* nvector = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlane'",NULL);
#endif
  {
   self->setPlane(*point,*nvector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setPlane'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlane of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__setPlane01
static int tolua_plane3d_irr_core_plane3d_f32__setPlane01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* nvect = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  f32 d = *((f32*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlane'",NULL);
#endif
  {
   self->setPlane(*nvect,d);
  }
 }
 return 0;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__setPlane00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setPlane of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__setPlane02
static int tolua_plane3d_irr_core_plane3d_f32__setPlane02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* point1 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* point2 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* point3 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setPlane'",NULL);
#endif
  {
   self->setPlane(*point1,*point2,*point3);
  }
 }
 return 0;
tolua_lerror:
 return tolua_plane3d_irr_core_plane3d_f32__setPlane01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntersectionWithLine of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLine00
static int tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* linePoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* lineVect = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* outIntersection = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionWithLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionWithLine(*linePoint,*lineVect,*outIntersection);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersectionWithLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getKnownIntersectionWithLine of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getKnownIntersectionWithLine00
static int tolua_plane3d_irr_core_plane3d_f32__getKnownIntersectionWithLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* linePoint1 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* linePoint2 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getKnownIntersectionWithLine'",NULL);
#endif
  {
   f32 tolua_ret = (f32)  self->getKnownIntersectionWithLine(*linePoint1,*linePoint2);
   {
#ifdef __cplusplus
    void* tolua_obj = new f32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f32");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getKnownIntersectionWithLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntersectionWithLimitedLine of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLimitedLine00
static int tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLimitedLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* linePoint1 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* linePoint2 = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* outIntersection = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionWithLimitedLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionWithLimitedLine(*linePoint1,*linePoint2,*outIntersection);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersectionWithLimitedLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: classifyPointRelation of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__classifyPointRelation00
static int tolua_plane3d_irr_core_plane3d_f32__classifyPointRelation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* point = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'classifyPointRelation'",NULL);
#endif
  {
   irr::core::EIntersectionRelation3D tolua_ret = (irr::core::EIntersectionRelation3D)  self->classifyPointRelation(*point);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'classifyPointRelation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: recalculateD of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__recalculateD00
static int tolua_plane3d_irr_core_plane3d_f32__recalculateD00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* MPoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'recalculateD'",NULL);
#endif
  {
   self->recalculateD(*MPoint);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'recalculateD'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getMemberPoint of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getMemberPoint00
static int tolua_plane3d_irr_core_plane3d_f32__getMemberPoint00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getMemberPoint'",NULL);
#endif
  {
   vector3d<f32> tolua_ret = (vector3d<f32>)  self->getMemberPoint();
   {
#ifdef __cplusplus
    void* tolua_obj = new vector3d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"vector3d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(vector3d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"vector3d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getMemberPoint'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: existsIntersection of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__existsIntersection00
static int tolua_plane3d_irr_core_plane3d_f32__existsIntersection00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* other = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'existsIntersection'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->existsIntersection(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'existsIntersection'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntersectionWithPlane of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlane00
static int tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlane00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* other = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* outLinePoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* outLineVect = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionWithPlane'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionWithPlane(*other,*outLinePoint,*outLineVect);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersectionWithPlane'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntersectionWithPlanes of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlanes00
static int tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlanes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* o1 = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::plane3d<f32>* o2 = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* outPoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionWithPlanes'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionWithPlanes(*o1,*o2,*outPoint);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersectionWithPlanes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFrontFacing of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__isFrontFacing00
static int tolua_plane3d_irr_core_plane3d_f32__isFrontFacing00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* lookDirection = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isFrontFacing'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isFrontFacing(*lookDirection);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isFrontFacing'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDistanceTo of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_plane3d_irr_core_plane3d_f32__getDistanceTo00
static int tolua_plane3d_irr_core_plane3d_f32__getDistanceTo00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* point = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDistanceTo'",NULL);
#endif
  {
   f32 tolua_ret = (f32)  self->getDistanceTo(*point);
   {
#ifdef __cplusplus
    void* tolua_obj = new f32(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f32");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f32));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f32");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDistanceTo'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Normal of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__plane3d_f32__Normal
static int tolua_get_irr__core__plane3d_f32__Normal(lua_State* tolua_S)
{
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Normal'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Normal,"vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Normal of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__plane3d_f32__Normal
static int tolua_set_irr__core__plane3d_f32__Normal(lua_State* tolua_S)
{
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Normal'",NULL);
  if (!tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Normal = *((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: D of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__plane3d_f32__D
static int tolua_get_irr__core__plane3d_f32__D(lua_State* tolua_S)
{
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'D'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->D,"f32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: D of class  irr::core::plane3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__plane3d_f32__D
static int tolua_set_irr__core__plane3d_f32__D(lua_State* tolua_S)
{
  irr::core::plane3d<f32>* self = (irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'D'",NULL);
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->D = *((f32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_plane3d_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"irr",0);
  tolua_beginmodule(tolua_S,"irr");
   tolua_module(tolua_S,"core",0);
   tolua_beginmodule(tolua_S,"core");
    tolua_constant(tolua_S,"ISREL3D_FRONT",irr::core::ISREL3D_FRONT);
    tolua_constant(tolua_S,"ISREL3D_BACK",irr::core::ISREL3D_BACK);
    tolua_constant(tolua_S,"ISREL3D_PLANAR",irr::core::ISREL3D_PLANAR);
    tolua_constant(tolua_S,"ISREL3D_SPANNING",irr::core::ISREL3D_SPANNING);
    tolua_constant(tolua_S,"ISREL3D_CLIPPED",irr::core::ISREL3D_CLIPPED);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"plane3d_f32_","irr::core::plane3d<f32>","",tolua_collect_irr__core__plane3d_f32_);
    #else
    tolua_cclass(tolua_S,"plane3d_f32_","irr::core::plane3d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"plane3d_f32_");
     tolua_function(tolua_S,"new",tolua_plane3d_irr_core_plane3d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_plane3d_irr_core_plane3d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_plane3d_irr_core_plane3d_f32__new00_local);
     tolua_function(tolua_S,"new",tolua_plane3d_irr_core_plane3d_f32__new01);
     tolua_function(tolua_S,"new_local",tolua_plane3d_irr_core_plane3d_f32__new01_local);
     tolua_function(tolua_S,".call",tolua_plane3d_irr_core_plane3d_f32__new01_local);
     tolua_function(tolua_S,"new",tolua_plane3d_irr_core_plane3d_f32__new02);
     tolua_function(tolua_S,"new_local",tolua_plane3d_irr_core_plane3d_f32__new02_local);
     tolua_function(tolua_S,".call",tolua_plane3d_irr_core_plane3d_f32__new02_local);
     tolua_function(tolua_S,"new",tolua_plane3d_irr_core_plane3d_f32__new03);
     tolua_function(tolua_S,"new_local",tolua_plane3d_irr_core_plane3d_f32__new03_local);
     tolua_function(tolua_S,".call",tolua_plane3d_irr_core_plane3d_f32__new03_local);
     tolua_function(tolua_S,"new",tolua_plane3d_irr_core_plane3d_f32__new04);
     tolua_function(tolua_S,"new_local",tolua_plane3d_irr_core_plane3d_f32__new04_local);
     tolua_function(tolua_S,".call",tolua_plane3d_irr_core_plane3d_f32__new04_local);
     tolua_function(tolua_S,".eq",tolua_plane3d_irr_core_plane3d_f32___eq00);
     tolua_function(tolua_S,"setPlane",tolua_plane3d_irr_core_plane3d_f32__setPlane00);
     tolua_function(tolua_S,"setPlane",tolua_plane3d_irr_core_plane3d_f32__setPlane01);
     tolua_function(tolua_S,"setPlane",tolua_plane3d_irr_core_plane3d_f32__setPlane02);
     tolua_function(tolua_S,"getIntersectionWithLine",tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLine00);
     tolua_function(tolua_S,"getKnownIntersectionWithLine",tolua_plane3d_irr_core_plane3d_f32__getKnownIntersectionWithLine00);
     tolua_function(tolua_S,"getIntersectionWithLimitedLine",tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithLimitedLine00);
     tolua_function(tolua_S,"classifyPointRelation",tolua_plane3d_irr_core_plane3d_f32__classifyPointRelation00);
     tolua_function(tolua_S,"recalculateD",tolua_plane3d_irr_core_plane3d_f32__recalculateD00);
     tolua_function(tolua_S,"getMemberPoint",tolua_plane3d_irr_core_plane3d_f32__getMemberPoint00);
     tolua_function(tolua_S,"existsIntersection",tolua_plane3d_irr_core_plane3d_f32__existsIntersection00);
     tolua_function(tolua_S,"getIntersectionWithPlane",tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlane00);
     tolua_function(tolua_S,"getIntersectionWithPlanes",tolua_plane3d_irr_core_plane3d_f32__getIntersectionWithPlanes00);
     tolua_function(tolua_S,"isFrontFacing",tolua_plane3d_irr_core_plane3d_f32__isFrontFacing00);
     tolua_function(tolua_S,"getDistanceTo",tolua_plane3d_irr_core_plane3d_f32__getDistanceTo00);
     tolua_variable(tolua_S,"Normal",tolua_get_irr__core__plane3d_f32__Normal,tolua_set_irr__core__plane3d_f32__Normal);
     tolua_variable(tolua_S,"D",tolua_get_irr__core__plane3d_f32__D,tolua_set_irr__core__plane3d_f32__D);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46,112,108, 97,110,101,
     51,100, 32, 61, 32,105,114,114, 46, 99,111,114,101, 46,112,
    108, 97,110,101, 51,100, 95,102, 51, 50, 95,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_plane3d (lua_State* tolua_S) {
 return tolua_plane3d_open(tolua_S);
};
#endif

