#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: dimension2d
** Generated automatically by tolua++-1.0.92 on 04/20/13 16:09:21.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_dimension2d_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_irr__core__dimension2d_s32_ (lua_State* tolua_S)
{
 irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__dimension2d_f32_ (lua_State* tolua_S)
{
 irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"f32");
 tolua_usertype(tolua_S,"irr::core::dimension2d<f32>");
 tolua_usertype(tolua_S,"s32");
 tolua_usertype(tolua_S,"irr::core::dimension2d<s32>");
}

/* method: new of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new00
static int tolua_dimension2d_irr_core_dimension2d_f32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<f32>");
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

/* method: new_local of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new00_local
static int tolua_dimension2d_irr_core_dimension2d_f32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<f32>");
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

/* method: new of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new01
static int tolua_dimension2d_irr_core_dimension2d_f32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 width = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 height = *((f32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>(width,height);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_f32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new01_local
static int tolua_dimension2d_irr_core_dimension2d_f32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"f32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"f32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  f32 width = *((f32*)  tolua_tousertype(tolua_S,2,0));
  f32 height = *((f32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>(width,height);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_f32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new02
static int tolua_dimension2d_irr_core_dimension2d_f32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::dimension2d<f32>* other = ((irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_f32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32__new02_local
static int tolua_dimension2d_irr_core_dimension2d_f32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::dimension2d<f32>* other = ((irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::dimension2d<f32>* tolua_ret = (irr::core::dimension2d<f32>*)  new irr::core::dimension2d<f32>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<f32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_f32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_f32___eq00
static int tolua_dimension2d_irr_core_dimension2d_f32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::dimension2d<f32>* other = ((irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,2,0));
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

/* get function: Width of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__dimension2d_f32__Width
static int tolua_get_irr__core__dimension2d_f32__Width(lua_State* tolua_S)
{
  irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Width'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Width,"f32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Width of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__dimension2d_f32__Width
static int tolua_set_irr__core__dimension2d_f32__Width(lua_State* tolua_S)
{
  irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Width'",NULL);
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Width = *((f32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Height of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__dimension2d_f32__Height
static int tolua_get_irr__core__dimension2d_f32__Height(lua_State* tolua_S)
{
  irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Height'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Height,"f32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Height of class  irr::core::dimension2d<f32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__dimension2d_f32__Height
static int tolua_set_irr__core__dimension2d_f32__Height(lua_State* tolua_S)
{
  irr::core::dimension2d<f32>* self = (irr::core::dimension2d<f32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Height'",NULL);
  if (!tolua_isusertype(tolua_S,2,"f32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Height = *((f32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new00
static int tolua_dimension2d_irr_core_dimension2d_s32__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<s32>");
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

/* method: new_local of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new00_local
static int tolua_dimension2d_irr_core_dimension2d_s32__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<s32>");
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

/* method: new of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new01
static int tolua_dimension2d_irr_core_dimension2d_s32__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  s32 width = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 height = *((s32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>(width,height);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<s32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_s32__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new01_local
static int tolua_dimension2d_irr_core_dimension2d_s32__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"s32",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"s32",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  s32 width = *((s32*)  tolua_tousertype(tolua_S,2,0));
  s32 height = *((s32*)  tolua_tousertype(tolua_S,3,0));
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>(width,height);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<s32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_s32__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new02
static int tolua_dimension2d_irr_core_dimension2d_s32__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::dimension2d<s32>* other = ((irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::dimension2d<s32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_s32__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32__new02_local
static int tolua_dimension2d_irr_core_dimension2d_s32__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::dimension2d<s32>* other = ((irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::dimension2d<s32>* tolua_ret = (irr::core::dimension2d<s32>*)  new irr::core::dimension2d<s32>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::dimension2d<s32>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_dimension2d_irr_core_dimension2d_s32__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_dimension2d_irr_core_dimension2d_s32___eq00
static int tolua_dimension2d_irr_core_dimension2d_s32___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::dimension2d<s32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,1,0);
  irr::core::dimension2d<s32>* other = ((irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,2,0));
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

/* get function: Width of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__dimension2d_s32__Width
static int tolua_get_irr__core__dimension2d_s32__Width(lua_State* tolua_S)
{
  irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Width'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Width,"s32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Width of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__dimension2d_s32__Width
static int tolua_set_irr__core__dimension2d_s32__Width(lua_State* tolua_S)
{
  irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Width'",NULL);
  if (!tolua_isusertype(tolua_S,2,"s32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Width = *((s32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: Height of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_get_irr__core__dimension2d_s32__Height
static int tolua_get_irr__core__dimension2d_s32__Height(lua_State* tolua_S)
{
  irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Height'",NULL);
#endif
  tolua_pushusertype(tolua_S,(void*)&self->Height,"s32");
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: Height of class  irr::core::dimension2d<s32> */
#ifndef TOLUA_DISABLE_tolua_set_irr__core__dimension2d_s32__Height
static int tolua_set_irr__core__dimension2d_s32__Height(lua_State* tolua_S)
{
  irr::core::dimension2d<s32>* self = (irr::core::dimension2d<s32>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'Height'",NULL);
  if (!tolua_isusertype(tolua_S,2,"s32",0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->Height = *((s32*)  tolua_tousertype(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_dimension2d_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"dimension2d_f32_","irr::core::dimension2d<f32>","",tolua_collect_irr__core__dimension2d_f32_);
    #else
    tolua_cclass(tolua_S,"dimension2d_f32_","irr::core::dimension2d<f32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"dimension2d_f32_");
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_f32__new00);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_f32__new00_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_f32__new00_local);
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_f32__new01);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_f32__new01_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_f32__new01_local);
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_f32__new02);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_f32__new02_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_f32__new02_local);
     tolua_function(tolua_S,".eq",tolua_dimension2d_irr_core_dimension2d_f32___eq00);
     tolua_variable(tolua_S,"Width",tolua_get_irr__core__dimension2d_f32__Width,tolua_set_irr__core__dimension2d_f32__Width);
     tolua_variable(tolua_S,"Height",tolua_get_irr__core__dimension2d_f32__Height,tolua_set_irr__core__dimension2d_f32__Height);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"dimension2d_s32_","irr::core::dimension2d<s32>","",tolua_collect_irr__core__dimension2d_s32_);
    #else
    tolua_cclass(tolua_S,"dimension2d_s32_","irr::core::dimension2d<s32>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"dimension2d_s32_");
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_s32__new00);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_s32__new00_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_s32__new00_local);
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_s32__new01);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_s32__new01_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_s32__new01_local);
     tolua_function(tolua_S,"new",tolua_dimension2d_irr_core_dimension2d_s32__new02);
     tolua_function(tolua_S,"new_local",tolua_dimension2d_irr_core_dimension2d_s32__new02_local);
     tolua_function(tolua_S,".call",tolua_dimension2d_irr_core_dimension2d_s32__new02_local);
     tolua_function(tolua_S,".eq",tolua_dimension2d_irr_core_dimension2d_s32___eq00);
     tolua_variable(tolua_S,"Width",tolua_get_irr__core__dimension2d_s32__Width,tolua_set_irr__core__dimension2d_s32__Width);
     tolua_variable(tolua_S,"Height",tolua_get_irr__core__dimension2d_s32__Height,tolua_set_irr__core__dimension2d_s32__Height);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);

  { /* begin embedded lua code */
   int top = lua_gettop(tolua_S);
   static unsigned char B[] = {
    10,105,114,114, 46, 99,111,114,101, 46,100,105,109,101,110,
    115,105,111,110, 50,100, 32, 61, 32,105,114,114, 46, 99,111,
    114,101, 46,100,105,109,101,110,115,105,111,110, 50,100, 95,
    102, 51, 50, 95, 10,105,114,114, 46, 99,111,114,101, 46,100,
    105,109,101,110,115,105,111,110, 50,100,102, 51, 50, 32, 61,
     32,105,114,114, 46, 99,111,114,101, 46,100,105,109,101,110,
    115,105,111,110, 50,100, 95,102, 51, 50, 95, 10,105,114,114,
     46, 99,111,114,101, 46,100,105,109,101,110,115,105,111,110,
     50,100,115, 51, 50, 32, 61, 32,105,114,114, 46, 99,111,114,
    101, 46,100,105,109,101,110,115,105,111,110, 50,100, 95,115,
     51, 50, 95,32
   };
   tolua_dobuffer(tolua_S,(char*)B,sizeof(B),"tolua: embedded Lua code 1");
   lua_settop(tolua_S, top);
  } /* end of embedded lua code */

 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_dimension2d (lua_State* tolua_S) {
 return tolua_dimension2d_open(tolua_S);
};
#endif

