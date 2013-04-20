#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: triangle3d
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:58:23.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_triangle3d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_irr__core__triangle3d_f32_ (lua_State* tolua_S)
{
 irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_plane3d_f32_ (lua_State* tolua_S)
{
 plane3d<f32>* self = (plane3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__vector3d_f32_ (lua_State* tolua_S)
{
 irr::core::vector3d<f32>* self = (irr::core::vector3d<f32>*) tolua_tousertype(tolua_S,1,0);
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
 tolua_usertype(tolua_S,"irr::core::triangle3d<f32>");
 tolua_usertype(tolua_S,"line3d<f32>");
 tolua_usertype(tolua_S,"plane3d<f32>");
 tolua_usertype(tolua_S,"vector3d<f32>");
 tolua_usertype(tolua_S,"irr::core::vector3d<f32>");
 tolua_usertype(tolua_S,"aabbox3d<f32>");
}

/* method: new of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__new00
static int tolua_triangle3d_irr_core_triangle3d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::triangle3d<f32>* tolua_ret = (irr::core::triangle3d<f32>*)  new irr::core::triangle3d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::triangle3d<f32>");
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

/* method: new_local of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__new00_local
static int tolua_triangle3d_irr_core_triangle3d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::triangle3d<f32>* tolua_ret = (irr::core::triangle3d<f32>*)  new irr::core::triangle3d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::triangle3d<f32>");
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

/* method: delete of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__delete00
static int tolua_triangle3d_irr_core_triangle3d_f32__delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'delete'",NULL);
#endif
  delete self;
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'delete'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isTotalInsideBox of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__isTotalInsideBox00
static int tolua_triangle3d_irr_core_triangle3d_f32__isTotalInsideBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  aabbox3d<f32>* box = ((aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isTotalInsideBox'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isTotalInsideBox(*box);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isTotalInsideBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32___eq00
static int tolua_triangle3d_irr_core_triangle3d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::triangle3d<f32>* other = ((irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: closestPointOnTriangle of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__closestPointOnTriangle00
static int tolua_triangle3d_irr_core_triangle3d_f32__closestPointOnTriangle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* p = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'closestPointOnTriangle'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->closestPointOnTriangle(*p);
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
 tolua_error(tolua_S,"#ferror in function 'closestPointOnTriangle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isPointInside of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__isPointInside00
static int tolua_triangle3d_irr_core_triangle3d_f32__isPointInside00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* p = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: isPointInsideFast of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__isPointInsideFast00
static int tolua_triangle3d_irr_core_triangle3d_f32__isPointInsideFast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* p = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isPointInsideFast'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isPointInsideFast(*p);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isPointInsideFast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getIntersectionWithLimitedLine of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLimitedLine00
static int tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLimitedLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"line3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  line3d<f32>* line = ((line3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* outIntersection = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionWithLimitedLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionWithLimitedLine(*line,*outIntersection);
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

/* method: getIntersectionWithLine of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLine00
static int tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getIntersectionOfPlaneWithLine of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionOfPlaneWithLine00
static int tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionOfPlaneWithLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  vector3d<f32>* linePoint = ((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  vector3d<f32>* lineVect = ((vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  vector3d<f32>* outIntersection = ((vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getIntersectionOfPlaneWithLine'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getIntersectionOfPlaneWithLine(*linePoint,*lineVect,*outIntersection);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getIntersectionOfPlaneWithLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getNormal of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__getNormal00
static int tolua_triangle3d_irr_core_triangle3d_f32__getNormal00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getNormal'",NULL);
#endif
  {
   vector3d<f32> tolua_ret = (vector3d<f32>)  self->getNormal();
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
 tolua_error(tolua_S,"#ferror in function 'getNormal'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isFrontFacing of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__isFrontFacing00
static int tolua_triangle3d_irr_core_triangle3d_f32__isFrontFacing00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: getPlane of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__getPlane00
static int tolua_triangle3d_irr_core_triangle3d_f32__getPlane00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getPlane'",NULL);
#endif
  {
   plane3d<f32> tolua_ret = (plane3d<f32>)  self->getPlane();
   {
#ifdef __cplusplus
    void* tolua_obj = new plane3d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"plane3d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(plane3d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"plane3d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getPlane'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: set of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_triangle3d_irr_core_triangle3d_f32__set00
static int tolua_triangle3d_irr_core_triangle3d_f32__set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::triangle3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* a = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* b = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  irr::core::vector3d<f32>* c = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(*a,*b,*c);
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

/* get function: pointA of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__triangle3d_f32__pointA
static int tolua_get_irr__core__triangle3d_f32__pointA(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointA'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->pointA,"vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pointA of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__triangle3d_f32__pointA
static int tolua_set_irr__core__triangle3d_f32__pointA(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointA'",NULL);
  if (!tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pointA = *((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pointB of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__triangle3d_f32__pointB
static int tolua_get_irr__core__triangle3d_f32__pointB(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointB'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->pointB,"vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pointB of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__triangle3d_f32__pointB
static int tolua_set_irr__core__triangle3d_f32__pointB(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointB'",NULL);
  if (!tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pointB = *((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: pointC of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__triangle3d_f32__pointC
static int tolua_get_irr__core__triangle3d_f32__pointC(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointC'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->pointC,"vector3d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: pointC of class  irr::core::triangle3d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__triangle3d_f32__pointC
static int tolua_set_irr__core__triangle3d_f32__pointC(lua_State* tolua_S)
{
  irr::core::triangle3d<f32>* self = (irr::core::triangle3d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'pointC'",NULL);
  if (!tolua_isusertype(tolua_S,2,"vector3d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->pointC = *((vector3d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_triangle3d_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"triangle3d_f32_","irr::core::triangle3d<f32>","",tolua_collect_irr__core__triangle3d_f32_);
    #else
    tolua_cclass(tolua_S,"triangle3d_f32_","irr::core::triangle3d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"triangle3d_f32_");
     tolua_function(tolua_S,"new",tolua_triangle3d_irr_core_triangle3d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_triangle3d_irr_core_triangle3d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_triangle3d_irr_core_triangle3d_f32__new00_local);
     tolua_function(tolua_S,"delete",tolua_triangle3d_irr_core_triangle3d_f32__delete00);
     tolua_function(tolua_S,"isTotalInsideBox",tolua_triangle3d_irr_core_triangle3d_f32__isTotalInsideBox00);
     tolua_function(tolua_S,".eq",tolua_triangle3d_irr_core_triangle3d_f32___eq00);
     tolua_function(tolua_S,"closestPointOnTriangle",tolua_triangle3d_irr_core_triangle3d_f32__closestPointOnTriangle00);
     tolua_function(tolua_S,"isPointInside",tolua_triangle3d_irr_core_triangle3d_f32__isPointInside00);
     tolua_function(tolua_S,"isPointInsideFast",tolua_triangle3d_irr_core_triangle3d_f32__isPointInsideFast00);
     tolua_function(tolua_S,"getIntersectionWithLimitedLine",tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLimitedLine00);
     tolua_function(tolua_S,"getIntersectionWithLine",tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionWithLine00);
     tolua_function(tolua_S,"getIntersectionOfPlaneWithLine",tolua_triangle3d_irr_core_triangle3d_f32__getIntersectionOfPlaneWithLine00);
     tolua_function(tolua_S,"getNormal",tolua_triangle3d_irr_core_triangle3d_f32__getNormal00);
     tolua_function(tolua_S,"isFrontFacing",tolua_triangle3d_irr_core_triangle3d_f32__isFrontFacing00);
     tolua_function(tolua_S,"getPlane",tolua_triangle3d_irr_core_triangle3d_f32__getPlane00);
     tolua_function(tolua_S,"set",tolua_triangle3d_irr_core_triangle3d_f32__set00);
     tolua_variable(tolua_S,"pointA",tolua_get_irr__core__triangle3d_f32__pointA,tolua_set_irr__core__triangle3d_f32__pointA);
     tolua_variable(tolua_S,"pointB",tolua_get_irr__core__triangle3d_f32__pointB,tolua_set_irr__core__triangle3d_f32__pointB);
     tolua_variable(tolua_S,"pointC",tolua_get_irr__core__triangle3d_f32__pointC,tolua_set_irr__core__triangle3d_f32__pointC);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46,116,114,105, 97,110,
    103,108,101, 51,100, 32, 61, 32,105,114,114, 46, 99,111,114,
    101, 46,116,114,105, 97,110,103,108,101, 51,100, 95,102, 51,
     50, 95,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_triangle3d (lua_State* tolua_S) {
 return tolua_triangle3d_open(tolua_S);
};
#endif

