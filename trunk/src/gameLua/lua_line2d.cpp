#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: line2d
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:58:23.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_line2d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_irr__core__vector2d_f32_ (lua_State* tolua_S)
{
 irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_vector2d_f32_ (lua_State* tolua_S)
{
 vector2d<f32>* self = (vector2d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__line2d_f32_ (lua_State* tolua_S)
{
 irr::core::line2d<f32>* self = (irr::core::line2d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"vector2d<f32>");
 tolua_usertype(tolua_S,"irr::core::vector2d<f32>");
 tolua_usertype(tolua_S,"irr::core::line2d<f32>");
}

/* method: new of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new00
static int tolua_line2d_irr_core_line2d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::line2d<f32>");
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

/* method: new_local of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new00_local
static int tolua_line2d_irr_core_line2d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::line2d<f32>");
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

/* method: new of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new01
static int tolua_line2d_irr_core_line2d_f32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
   float xa = ((  float)  tolua_tonumber(tolua_S,2,0));
   float ya = ((  float)  tolua_tonumber(tolua_S,3,0));
   float xb = ((  float)  tolua_tonumber(tolua_S,4,0));
   float yb = ((  float)  tolua_tonumber(tolua_S,5,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(xa,ya,xb,yb);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new01_local
static int tolua_line2d_irr_core_line2d_f32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
   float xa = ((  float)  tolua_tonumber(tolua_S,2,0));
   float ya = ((  float)  tolua_tonumber(tolua_S,3,0));
   float xb = ((  float)  tolua_tonumber(tolua_S,4,0));
   float yb = ((  float)  tolua_tonumber(tolua_S,5,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(xa,ya,xb,yb);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new02
static int tolua_line2d_irr_core_line2d_f32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* Start = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* End = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(*Start,*End);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new02_local
static int tolua_line2d_irr_core_line2d_f32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* Start = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* End = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(*Start,*End);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new03
static int tolua_line2d_irr_core_line2d_f32__new03(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::line2d<f32>* other = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new02(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__new03_local
static int tolua_line2d_irr_core_line2d_f32__new03_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::line2d<f32>* other = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::line2d<f32>* tolua_ret = (irr::core::line2d<f32>*)  new irr::core::line2d<f32>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::line2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__new02_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32___add00
static int tolua_line2d_irr_core_line2d_f32___add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* point = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   irr::core::line2d<f32> tolua_ret = (irr::core::line2d<f32>)  self->operator+(*point);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::line2d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::line2d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::line2d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::line2d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32___sub00
static int tolua_line2d_irr_core_line2d_f32___sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* point = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   irr::core::line2d<f32> tolua_ret = (irr::core::line2d<f32>)  self->operator-(*point);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::line2d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::line2d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::line2d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::line2d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32___eq00
static int tolua_line2d_irr_core_line2d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::line2d<f32>* other = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: setLine of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__setLine00
static int tolua_line2d_irr_core_line2d_f32__setLine00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
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
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
   float xa = ((  float)  tolua_tonumber(tolua_S,2,0));
   float ya = ((  float)  tolua_tonumber(tolua_S,3,0));
   float xb = ((  float)  tolua_tonumber(tolua_S,4,0));
   float yb = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLine'",NULL);
#endif
  {
   self->setLine(xa,ya,xb,yb);
   tolua_pushnumber(tolua_S,(lua_Number)xa);
   tolua_pushnumber(tolua_S,(lua_Number)ya);
   tolua_pushnumber(tolua_S,(lua_Number)xb);
   tolua_pushnumber(tolua_S,(lua_Number)yb);
  }
 }
 return 4;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setLine'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLine of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__setLine01
static int tolua_line2d_irr_core_line2d_f32__setLine01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* nstart = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* nend = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLine'",NULL);
#endif
  {
   self->setLine(*nstart,*nend);
  }
 }
 return 0;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__setLine00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setLine of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__setLine02
static int tolua_line2d_irr_core_line2d_f32__setLine02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::line2d<f32>* line = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setLine'",NULL);
#endif
  {
   self->setLine(*line);
  }
 }
 return 0;
tolua_lerror:
 return tolua_line2d_irr_core_line2d_f32__setLine01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: getLength of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__getLength00
static int tolua_line2d_irr_core_line2d_f32__getLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLength'",NULL);
#endif
  {
    double tolua_ret = (  double)  self->getLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVector of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__getVector00
static int tolua_line2d_irr_core_line2d_f32__getVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVector'",NULL);
#endif
  {
   vector2d<f32> tolua_ret = (vector2d<f32>)  self->getVector();
   {
#ifdef __cplusplus
    void* tolua_obj = new vector2d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"vector2d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(vector2d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"vector2d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: intersectWith of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__intersectWith00
static int tolua_line2d_irr_core_line2d_f32__intersectWith00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::line2d<f32>* l = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* out = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'intersectWith'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->intersectWith(*l,*out);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'intersectWith'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getUnitVector of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__getUnitVector00
static int tolua_line2d_irr_core_line2d_f32__getUnitVector00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getUnitVector'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->getUnitVector();
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::vector2d<f32>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector2d<f32>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::vector2d<f32>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::vector2d<f32>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getUnitVector'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngleWith of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_line2d_irr_core_line2d_f32__getAngleWith00
static int tolua_line2d_irr_core_line2d_f32__getAngleWith00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::line2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::line2d<f32>* l = ((irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngleWith'",NULL);
#endif
  {
    double tolua_ret = (  double)  self->getAngleWith(*l);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAngleWith'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: start of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__line2d_f32__start
static int tolua_get_irr__core__line2d_f32__start(lua_State* tolua_S)
{
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'start'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->start,"irr::core::vector2d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: start of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__line2d_f32__start
static int tolua_set_irr__core__line2d_f32__start(lua_State* tolua_S)
{
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'start'",NULL);
  if (!tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->start = *((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: end of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__line2d_f32__end
static int tolua_get_irr__core__line2d_f32__end(lua_State* tolua_S)
{
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'end'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->end,"irr::core::vector2d<f32>");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: end of class  irr::core::line2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__line2d_f32__end
static int tolua_set_irr__core__line2d_f32__end(lua_State* tolua_S)
{
  irr::core::line2d<f32>* self = (irr::core::line2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'end'",NULL);
  if (!tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->end = *((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_line2d_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"line2d_f32_","irr::core::line2d<f32>","",tolua_collect_irr__core__line2d_f32_);
    #else
    tolua_cclass(tolua_S,"line2d_f32_","irr::core::line2d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"line2d_f32_");
     tolua_function(tolua_S,"new",tolua_line2d_irr_core_line2d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_line2d_irr_core_line2d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_line2d_irr_core_line2d_f32__new00_local);
     tolua_function(tolua_S,"new",tolua_line2d_irr_core_line2d_f32__new01);
     tolua_function(tolua_S,"new_local",tolua_line2d_irr_core_line2d_f32__new01_local);
     tolua_function(tolua_S,".call",tolua_line2d_irr_core_line2d_f32__new01_local);
     tolua_function(tolua_S,"new",tolua_line2d_irr_core_line2d_f32__new02);
     tolua_function(tolua_S,"new_local",tolua_line2d_irr_core_line2d_f32__new02_local);
     tolua_function(tolua_S,".call",tolua_line2d_irr_core_line2d_f32__new02_local);
     tolua_function(tolua_S,"new",tolua_line2d_irr_core_line2d_f32__new03);
     tolua_function(tolua_S,"new_local",tolua_line2d_irr_core_line2d_f32__new03_local);
     tolua_function(tolua_S,".call",tolua_line2d_irr_core_line2d_f32__new03_local);
     tolua_function(tolua_S,".add",tolua_line2d_irr_core_line2d_f32___add00);
     tolua_function(tolua_S,".sub",tolua_line2d_irr_core_line2d_f32___sub00);
     tolua_function(tolua_S,".eq",tolua_line2d_irr_core_line2d_f32___eq00);
     tolua_function(tolua_S,"setLine",tolua_line2d_irr_core_line2d_f32__setLine00);
     tolua_function(tolua_S,"setLine",tolua_line2d_irr_core_line2d_f32__setLine01);
     tolua_function(tolua_S,"setLine",tolua_line2d_irr_core_line2d_f32__setLine02);
     tolua_function(tolua_S,"getLength",tolua_line2d_irr_core_line2d_f32__getLength00);
     tolua_function(tolua_S,"getVector",tolua_line2d_irr_core_line2d_f32__getVector00);
     tolua_function(tolua_S,"intersectWith",tolua_line2d_irr_core_line2d_f32__intersectWith00);
     tolua_function(tolua_S,"getUnitVector",tolua_line2d_irr_core_line2d_f32__getUnitVector00);
     tolua_function(tolua_S,"getAngleWith",tolua_line2d_irr_core_line2d_f32__getAngleWith00);
     tolua_variable(tolua_S,"start",tolua_get_irr__core__line2d_f32__start,tolua_set_irr__core__line2d_f32__start);
     tolua_variable(tolua_S,"end",tolua_get_irr__core__line2d_f32__end,tolua_set_irr__core__line2d_f32__end);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46,108,105,110,101, 50,
    100, 32, 61, 32,105,114,114, 46, 99,111,114,101, 46,108,105,
    110,101, 50,100, 95,102, 51, 50, 95,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_line2d (lua_State* tolua_S) {
 return tolua_line2d_open(tolua_S);
};
#endif

