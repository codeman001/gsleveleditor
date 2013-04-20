#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: vector2d
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:01:28.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_vector2d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_f64 (lua_State* tolua_S)
{
 f64* self = (f64*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__vector2d_f32_ (lua_State* tolua_S)
{
 irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_f32 (lua_State* tolua_S)
{
 f32* self = (f32*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"f32");
 tolua_usertype(tolua_S,"irr::core::vector2d<f32>");
 tolua_usertype(tolua_S,"f64");
}

/* method: new of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new00
static int tolua_vector2d_irr_core_vector2d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::vector2d<f32>");
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

/* method: new_local of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new00_local
static int tolua_vector2d_irr_core_vector2d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::vector2d<f32>");
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

/* method: new of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new01
static int tolua_vector2d_irr_core_vector2d_f32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 nx = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 ny = *((f32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>(nx,ny);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::vector2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new01_local
static int tolua_vector2d_irr_core_vector2d_f32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 nx = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 ny = *((f32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>(nx,ny);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::vector2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new02
static int tolua_vector2d_irr_core_vector2d_f32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::vector2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__new02_local
static int tolua_vector2d_irr_core_vector2d_f32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::vector2d<f32>* tolua_ret = (irr::core::vector2d<f32>*)  new irr::core::vector2d<f32>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::vector2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___sub00
static int tolua_vector2d_irr_core_vector2d_f32___sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator-();
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
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___add00
static int tolua_vector2d_irr_core_vector2d_f32___add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator+(*other);
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
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___sub01
static int tolua_vector2d_irr_core_vector2d_f32___sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator-(*other);
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
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32___sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___mul00
static int tolua_vector2d_irr_core_vector2d_f32___mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator*(*other);
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
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___mul01
static int tolua_vector2d_irr_core_vector2d_f32___mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  f32 v = *((f32*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator*(v);
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
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32___mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___div00
static int tolua_vector2d_irr_core_vector2d_f32___div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator/(*other);
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
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___div01
static int tolua_vector2d_irr_core_vector2d_f32___div01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  f32 v = *((f32*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->operator/(v);
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
tolua_lerror:
 return tolua_vector2d_irr_core_vector2d_f32___div00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32___eq00
static int tolua_vector2d_irr_core_vector2d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: set of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__set00
static int tolua_vector2d_irr_core_vector2d_f32__set00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  f32* nx = ((f32*)  tolua_tousertype(tolua_S,2,0));
  f32* ny = ((f32*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'set'",NULL);
#endif
  {
   self->set(*nx,*ny);
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

/* method: getLength of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getLength00
static int tolua_vector2d_irr_core_vector2d_f32__getLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getLength'",NULL);
#endif
  {
   f64 tolua_ret = (f64)  self->getLength();
   {
#ifdef __cplusplus
    void* tolua_obj = new f64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#endif
   }
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

/* method: dotProduct of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__dotProduct00
static int tolua_vector2d_irr_core_vector2d_f32__dotProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dotProduct'",NULL);
#endif
  {
   f32 tolua_ret = (f32)  self->dotProduct(*other);
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
 tolua_error(tolua_S,"#ferror in function 'dotProduct'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDistanceFrom of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getDistanceFrom00
static int tolua_vector2d_irr_core_vector2d_f32__getDistanceFrom00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDistanceFrom'",NULL);
#endif
  {
   f64 tolua_ret = (f64)  self->getDistanceFrom(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new f64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDistanceFrom'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateBy of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__rotateBy00
static int tolua_vector2d_irr_core_vector2d_f32__rotateBy00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f64",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  f64 degrees = *((f64*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* center = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateBy'",NULL);
#endif
  {
   self->rotateBy(degrees,*center);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateBy'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: normalize of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__normalize00
static int tolua_vector2d_irr_core_vector2d_f32__normalize00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'normalize'",NULL);
#endif
  {
   irr::core::vector2d<f32>& tolua_ret = (irr::core::vector2d<f32>&)  self->normalize();
   tolua_pushusertype(tolua_S,(void*)&tolua_ret,"irr::core::vector2d<f32>");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'normalize'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngleTrig of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getAngleTrig00
static int tolua_vector2d_irr_core_vector2d_f32__getAngleTrig00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngleTrig'",NULL);
#endif
  {
   f64 tolua_ret = (f64)  self->getAngleTrig();
   {
#ifdef __cplusplus
    void* tolua_obj = new f64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAngleTrig'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngle of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getAngle00
static int tolua_vector2d_irr_core_vector2d_f32__getAngle00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngle'",NULL);
#endif
  {
   f64 tolua_ret = (f64)  self->getAngle();
   {
#ifdef __cplusplus
    void* tolua_obj = new f64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getAngle'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getAngleWith of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getAngleWith00
static int tolua_vector2d_irr_core_vector2d_f32__getAngleWith00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* b = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getAngleWith'",NULL);
#endif
  {
   f64 tolua_ret = (f64)  self->getAngleWith(*b);
   {
#ifdef __cplusplus
    void* tolua_obj = new f64(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(f64));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"f64");
#endif
   }
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

/* method: getInterpolated of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__getInterpolated00
static int tolua_vector2d_irr_core_vector2d_f32__getInterpolated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* other = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  f32 d = *((f32*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInterpolated'",NULL);
#endif
  {
   irr::core::vector2d<f32> tolua_ret = (irr::core::vector2d<f32>)  self->getInterpolated(*other,d);
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
 tolua_error(tolua_S,"#ferror in function 'getInterpolated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: interpolate of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_vector2d_irr_core_vector2d_f32__interpolate00
static int tolua_vector2d_irr_core_vector2d_f32__interpolate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector2d<f32>* a = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector2d<f32>* b = ((irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,3,0));
  f32 t = *((f32*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'interpolate'",NULL);
#endif
  {
   self->interpolate(*a,*b,t);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'interpolate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: X of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__vector2d_f32__X
static int tolua_get_irr__core__vector2d_f32__X(lua_State* tolua_S)
{
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'X'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->X,"f32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: X of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__vector2d_f32__X
static int tolua_set_irr__core__vector2d_f32__X(lua_State* tolua_S)
{
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'X'",NULL);
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->X = *((f32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Y of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__vector2d_f32__Y
static int tolua_get_irr__core__vector2d_f32__Y(lua_State* tolua_S)
{
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Y'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Y,"f32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Y of class  irr::core::vector2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__vector2d_f32__Y
static int tolua_set_irr__core__vector2d_f32__Y(lua_State* tolua_S)
{
  irr::core::vector2d<f32>* self = (irr::core::vector2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Y'",NULL);
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Y = *((f32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_vector2d_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"vector2d_f32_","irr::core::vector2d<f32>","",tolua_collect_irr__core__vector2d_f32_);
    #else
    tolua_cclass(tolua_S,"vector2d_f32_","irr::core::vector2d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"vector2d_f32_");
     tolua_function(tolua_S,"new",tolua_vector2d_irr_core_vector2d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_vector2d_irr_core_vector2d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_vector2d_irr_core_vector2d_f32__new00_local);
     tolua_function(tolua_S,"new",tolua_vector2d_irr_core_vector2d_f32__new01);
     tolua_function(tolua_S,"new_local",tolua_vector2d_irr_core_vector2d_f32__new01_local);
     tolua_function(tolua_S,".call",tolua_vector2d_irr_core_vector2d_f32__new01_local);
     tolua_function(tolua_S,"new",tolua_vector2d_irr_core_vector2d_f32__new02);
     tolua_function(tolua_S,"new_local",tolua_vector2d_irr_core_vector2d_f32__new02_local);
     tolua_function(tolua_S,".call",tolua_vector2d_irr_core_vector2d_f32__new02_local);
     tolua_function(tolua_S,".sub",tolua_vector2d_irr_core_vector2d_f32___sub00);
     tolua_function(tolua_S,".add",tolua_vector2d_irr_core_vector2d_f32___add00);
     tolua_function(tolua_S,".sub",tolua_vector2d_irr_core_vector2d_f32___sub01);
     tolua_function(tolua_S,".mul",tolua_vector2d_irr_core_vector2d_f32___mul00);
     tolua_function(tolua_S,".mul",tolua_vector2d_irr_core_vector2d_f32___mul01);
     tolua_function(tolua_S,".div",tolua_vector2d_irr_core_vector2d_f32___div00);
     tolua_function(tolua_S,".div",tolua_vector2d_irr_core_vector2d_f32___div01);
     tolua_function(tolua_S,".eq",tolua_vector2d_irr_core_vector2d_f32___eq00);
     tolua_function(tolua_S,"set",tolua_vector2d_irr_core_vector2d_f32__set00);
     tolua_function(tolua_S,"getLength",tolua_vector2d_irr_core_vector2d_f32__getLength00);
     tolua_function(tolua_S,"dotProduct",tolua_vector2d_irr_core_vector2d_f32__dotProduct00);
     tolua_function(tolua_S,"getDistanceFrom",tolua_vector2d_irr_core_vector2d_f32__getDistanceFrom00);
     tolua_function(tolua_S,"rotateBy",tolua_vector2d_irr_core_vector2d_f32__rotateBy00);
     tolua_function(tolua_S,"normalize",tolua_vector2d_irr_core_vector2d_f32__normalize00);
     tolua_function(tolua_S,"getAngleTrig",tolua_vector2d_irr_core_vector2d_f32__getAngleTrig00);
     tolua_function(tolua_S,"getAngle",tolua_vector2d_irr_core_vector2d_f32__getAngle00);
     tolua_function(tolua_S,"getAngleWith",tolua_vector2d_irr_core_vector2d_f32__getAngleWith00);
     tolua_function(tolua_S,"getInterpolated",tolua_vector2d_irr_core_vector2d_f32__getInterpolated00);
     tolua_function(tolua_S,"interpolate",tolua_vector2d_irr_core_vector2d_f32__interpolate00);
     tolua_variable(tolua_S,"X",tolua_get_irr__core__vector2d_f32__X,tolua_set_irr__core__vector2d_f32__X);
     tolua_variable(tolua_S,"Y",tolua_get_irr__core__vector2d_f32__Y,tolua_set_irr__core__vector2d_f32__Y);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46,118,101, 99,116,111,
    114, 50,100, 32, 61, 32,105,114,114, 46, 99,111,114,101, 46,
    118,101, 99,116,111,114, 50,100, 95,102, 51, 50, 95,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_vector2d (lua_State* tolua_S) {
 return tolua_vector2d_open(tolua_S);
};
#endif

