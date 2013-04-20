#include "stdafx.h"
using namespace irr;
using namespace irr::core;

/*
** Lua binding: matrix4
** Generated automatically by tolua++-1.0.92 on 04/20/13 17:58:23.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_matrix4_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_irr__core__vector3d_f32_ (lua_State* tolua_S)
{
 irr::core::vector3d<f32>* self = (irr::core::vector3d<f32>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__matrix4 (lua_State* tolua_S)
{
 irr::core::matrix4* self = (irr::core::matrix4*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"irr::core::plane3d<f32>");
 tolua_usertype(tolua_S,"irr::core::aabbox3d<f32>");
 tolua_usertype(tolua_S,"irr::core::matrix4");
 tolua_usertype(tolua_S,"irr::core::vector3d<f32>");
 tolua_usertype(tolua_S,"irr::core::rect<s32>");
}

/* method: new of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_new00
static int tolua_matrix4_irr_core_matrix4_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::matrix4* tolua_ret = (irr::core::matrix4*)  new irr::core::matrix4();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::matrix4");
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

/* method: new_local of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_new00_local
static int tolua_matrix4_irr_core_matrix4_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::matrix4* tolua_ret = (irr::core::matrix4*)  new irr::core::matrix4();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::matrix4");
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

/* method: delete of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_delete00
static int tolua_matrix4_irr_core_matrix4_delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator== of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4__eq00
static int tolua_matrix4_irr_core_matrix4__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::matrix4* other = ((irr::core::matrix4*)  tolua_tousertype(tolua_S,2,0));
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

/* method: operator* of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4__mul00
static int tolua_matrix4_irr_core_matrix4__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::matrix4* other = ((irr::core::matrix4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'",NULL);
#endif
  {
   irr::core::matrix4 tolua_ret = (irr::core::matrix4)  self->operator*(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::matrix4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::matrix4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
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

/* method: makeIdentity of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_makeIdentity00
static int tolua_matrix4_irr_core_matrix4_makeIdentity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'makeIdentity'",NULL);
#endif
  {
   self->makeIdentity();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'makeIdentity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isIdentity of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_isIdentity00
static int tolua_matrix4_irr_core_matrix4_isIdentity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isIdentity'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isIdentity();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isIdentity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setTranslation of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setTranslation00
static int tolua_matrix4_irr_core_matrix4_setTranslation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* translation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setTranslation'",NULL);
#endif
  {
   self->setTranslation(*translation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setTranslation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTranslation of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_getTranslation00
static int tolua_matrix4_irr_core_matrix4_getTranslation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTranslation'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->getTranslation();
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
 tolua_error(tolua_S,"#ferror in function 'getTranslation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInverseTranslation of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setInverseTranslation00
static int tolua_matrix4_irr_core_matrix4_setInverseTranslation00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* translation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInverseTranslation'",NULL);
#endif
  {
   self->setInverseTranslation(*translation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInverseTranslation'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotationRadians of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setRotationRadians00
static int tolua_matrix4_irr_core_matrix4_setRotationRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* rotation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotationRadians'",NULL);
#endif
  {
   self->setRotationRadians(*rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotationRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRotationDegrees of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setRotationDegrees00
static int tolua_matrix4_irr_core_matrix4_setRotationDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* rotation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRotationDegrees'",NULL);
#endif
  {
   self->setRotationDegrees(*rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRotationDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRotationDegrees of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_getRotationDegrees00
static int tolua_matrix4_irr_core_matrix4_getRotationDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRotationDegrees'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->getRotationDegrees();
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
 tolua_error(tolua_S,"#ferror in function 'getRotationDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInverseRotationRadians of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setInverseRotationRadians00
static int tolua_matrix4_irr_core_matrix4_setInverseRotationRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* rotation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInverseRotationRadians'",NULL);
#endif
  {
   self->setInverseRotationRadians(*rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInverseRotationRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setInverseRotationDegrees of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setInverseRotationDegrees00
static int tolua_matrix4_irr_core_matrix4_setInverseRotationDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* rotation = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setInverseRotationDegrees'",NULL);
#endif
  {
   self->setInverseRotationDegrees(*rotation);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setInverseRotationDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScale of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_setScale00
static int tolua_matrix4_irr_core_matrix4_setScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* scale = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScale'",NULL);
#endif
  {
   self->setScale(*scale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScale of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_getScale00
static int tolua_matrix4_irr_core_matrix4_getScale00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScale'",NULL);
#endif
  {
   irr::core::vector3d<f32> tolua_ret = (irr::core::vector3d<f32>)  self->getScale();
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
 tolua_error(tolua_S,"#ferror in function 'getScale'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: inverseTranslateVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_inverseTranslateVect00
static int tolua_matrix4_irr_core_matrix4_inverseTranslateVect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* vect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'inverseTranslateVect'",NULL);
#endif
  {
   self->inverseTranslateVect(*vect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inverseTranslateVect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: inverseRotateVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_inverseRotateVect00
static int tolua_matrix4_irr_core_matrix4_inverseRotateVect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* vect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'inverseRotateVect'",NULL);
#endif
  {
   self->inverseRotateVect(*vect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'inverseRotateVect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: rotateVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_rotateVect00
static int tolua_matrix4_irr_core_matrix4_rotateVect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* vect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'rotateVect'",NULL);
#endif
  {
   self->rotateVect(*vect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'rotateVect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformVect00
static int tolua_matrix4_irr_core_matrix4_transformVect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* vect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformVect'",NULL);
#endif
  {
   self->transformVect(*vect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transformVect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformVect01
static int tolua_matrix4_irr_core_matrix4_transformVect01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* in = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* out = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformVect'",NULL);
#endif
  {
   self->transformVect(*in,*out);
  }
 }
 return 0;
tolua_lerror:
 return tolua_matrix4_irr_core_matrix4_transformVect00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: translateVect of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_translateVect00
static int tolua_matrix4_irr_core_matrix4_translateVect00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* vect = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'translateVect'",NULL);
#endif
  {
   self->translateVect(*vect);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'translateVect'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformPlane of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformPlane00
static int tolua_matrix4_irr_core_matrix4_transformPlane00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* plane = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformPlane'",NULL);
#endif
  {
   self->transformPlane(*plane);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transformPlane'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformPlane of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformPlane01
static int tolua_matrix4_irr_core_matrix4_transformPlane01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::plane3d<f32>* in = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::plane3d<f32>* out = ((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformPlane'",NULL);
#endif
  {
   self->transformPlane(*in,*out);
  }
 }
 return 0;
tolua_lerror:
 return tolua_matrix4_irr_core_matrix4_transformPlane00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformBox of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformBox00
static int tolua_matrix4_irr_core_matrix4_transformBox00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* box = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformBox'",NULL);
#endif
  {
   self->transformBox(*box);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transformBox'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: transformBoxEx of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_transformBoxEx00
static int tolua_matrix4_irr_core_matrix4_transformBoxEx00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::aabbox3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::aabbox3d<f32>* box = ((irr::core::aabbox3d<f32>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'transformBoxEx'",NULL);
#endif
  {
   self->transformBoxEx(*box);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'transformBoxEx'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: multiplyWith1x4Matrix of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_multiplyWith1x4Matrix00
static int tolua_matrix4_irr_core_matrix4_multiplyWith1x4Matrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float matrix = ((  float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'multiplyWith1x4Matrix'",NULL);
#endif
  {
   self->multiplyWith1x4Matrix(&matrix);
   tolua_pushnumber(tolua_S,(lua_Number)matrix);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'multiplyWith1x4Matrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: makeInverse of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_makeInverse00
static int tolua_matrix4_irr_core_matrix4_makeInverse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'makeInverse'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->makeInverse();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'makeInverse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getInverse of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_getInverse00
static int tolua_matrix4_irr_core_matrix4_getInverse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::matrix4* out = ((irr::core::matrix4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getInverse'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->getInverse(*out);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getInverse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixPerspectiveFovRH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovRH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovRH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float fieldOfViewRadians = ((  float)  tolua_tonumber(tolua_S,2,0));
   float aspectRatio = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixPerspectiveFovRH'",NULL);
#endif
  {
   self->buildProjectionMatrixPerspectiveFovRH(fieldOfViewRadians,aspectRatio,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixPerspectiveFovRH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixPerspectiveFovLH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovLH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovLH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float fieldOfViewRadians = ((  float)  tolua_tonumber(tolua_S,2,0));
   float aspectRatio = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixPerspectiveFovLH'",NULL);
#endif
  {
   self->buildProjectionMatrixPerspectiveFovLH(fieldOfViewRadians,aspectRatio,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixPerspectiveFovLH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixPerspectiveRH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveRH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveRH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float widthOfViewVolume = ((  float)  tolua_tonumber(tolua_S,2,0));
   float heightOfViewVolume = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixPerspectiveRH'",NULL);
#endif
  {
   self->buildProjectionMatrixPerspectiveRH(widthOfViewVolume,heightOfViewVolume,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixPerspectiveRH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixPerspectiveLH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveLH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveLH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float widthOfViewVolume = ((  float)  tolua_tonumber(tolua_S,2,0));
   float heightOfViewVolume = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixPerspectiveLH'",NULL);
#endif
  {
   self->buildProjectionMatrixPerspectiveLH(widthOfViewVolume,heightOfViewVolume,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixPerspectiveLH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixOrthoLH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoLH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoLH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float widthOfViewVolume = ((  float)  tolua_tonumber(tolua_S,2,0));
   float heightOfViewVolume = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixOrthoLH'",NULL);
#endif
  {
   self->buildProjectionMatrixOrthoLH(widthOfViewVolume,heightOfViewVolume,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixOrthoLH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildProjectionMatrixOrthoRH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoRH00
static int tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoRH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
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
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
   float widthOfViewVolume = ((  float)  tolua_tonumber(tolua_S,2,0));
   float heightOfViewVolume = ((  float)  tolua_tonumber(tolua_S,3,0));
   float zNear = ((  float)  tolua_tonumber(tolua_S,4,0));
   float zFar = ((  float)  tolua_tonumber(tolua_S,5,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildProjectionMatrixOrthoRH'",NULL);
#endif
  {
   self->buildProjectionMatrixOrthoRH(widthOfViewVolume,heightOfViewVolume,zNear,zFar);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildProjectionMatrixOrthoRH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildCameraLookAtMatrixLH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixLH00
static int tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixLH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* position = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* target = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  irr::core::vector3d<f32>* upVector = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildCameraLookAtMatrixLH'",NULL);
#endif
  {
   self->buildCameraLookAtMatrixLH(*position,*target,*upVector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildCameraLookAtMatrixLH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildCameraLookAtMatrixRH of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixRH00
static int tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixRH00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,4,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32>* position = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::vector3d<f32>* target = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,3,0));
  irr::core::vector3d<f32>* upVector = ((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildCameraLookAtMatrixRH'",NULL);
#endif
  {
   self->buildCameraLookAtMatrixRH(*position,*target,*upVector);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildCameraLookAtMatrixRH'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildShadowMatrix of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildShadowMatrix00
static int tolua_matrix4_irr_core_matrix4_buildShadowMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::vector3d<f32>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"irr::core::plane3d<f32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::vector3d<f32> light = *((irr::core::vector3d<f32>*)  tolua_tousertype(tolua_S,2,0));
  irr::core::plane3d<f32> plane = *((irr::core::plane3d<f32>*)  tolua_tousertype(tolua_S,3,0));
   float point = ((  float)  tolua_tonumber(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildShadowMatrix'",NULL);
#endif
  {
   self->buildShadowMatrix(light,plane,point);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildShadowMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: buildNDCToDCMatrix of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_buildNDCToDCMatrix00
static int tolua_matrix4_irr_core_matrix4_buildNDCToDCMatrix00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::rect<s32>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::rect<s32>* area = ((irr::core::rect<s32>*)  tolua_tousertype(tolua_S,2,0));
   float zScale = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'buildNDCToDCMatrix'",NULL);
#endif
  {
   self->buildNDCToDCMatrix(*area,zScale);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'buildNDCToDCMatrix'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: interpolate of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_interpolate00
static int tolua_matrix4_irr_core_matrix4_interpolate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
  irr::core::matrix4* b = ((irr::core::matrix4*)  tolua_tousertype(tolua_S,2,0));
   float time = ((  float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'interpolate'",NULL);
#endif
  {
   irr::core::matrix4 tolua_ret = (irr::core::matrix4)  self->interpolate(*b,time);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::matrix4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::matrix4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'interpolate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getTransposed of class  irr::core::matrix4 */
#ifndef TOLUA_DISABLE_tolua_matrix4_irr_core_matrix4_getTransposed00
static int tolua_matrix4_irr_core_matrix4_getTransposed00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::matrix4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::matrix4* self = (irr::core::matrix4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getTransposed'",NULL);
#endif
  {
   irr::core::matrix4 tolua_ret = (irr::core::matrix4)  self->getTransposed();
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::matrix4(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::matrix4));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::matrix4");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getTransposed'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_matrix4_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"matrix4","irr::core::matrix4","",tolua_collect_irr__core__matrix4);
    #else
    tolua_cclass(tolua_S,"matrix4","irr::core::matrix4","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"matrix4");
     tolua_function(tolua_S,"new",tolua_matrix4_irr_core_matrix4_new00);
     tolua_function(tolua_S,"new_local",tolua_matrix4_irr_core_matrix4_new00_local);
     tolua_function(tolua_S,".call",tolua_matrix4_irr_core_matrix4_new00_local);
     tolua_function(tolua_S,"delete",tolua_matrix4_irr_core_matrix4_delete00);
     tolua_function(tolua_S,".eq",tolua_matrix4_irr_core_matrix4__eq00);
     tolua_function(tolua_S,".mul",tolua_matrix4_irr_core_matrix4__mul00);
     tolua_function(tolua_S,"makeIdentity",tolua_matrix4_irr_core_matrix4_makeIdentity00);
     tolua_function(tolua_S,"isIdentity",tolua_matrix4_irr_core_matrix4_isIdentity00);
     tolua_function(tolua_S,"setTranslation",tolua_matrix4_irr_core_matrix4_setTranslation00);
     tolua_function(tolua_S,"getTranslation",tolua_matrix4_irr_core_matrix4_getTranslation00);
     tolua_function(tolua_S,"setInverseTranslation",tolua_matrix4_irr_core_matrix4_setInverseTranslation00);
     tolua_function(tolua_S,"setRotationRadians",tolua_matrix4_irr_core_matrix4_setRotationRadians00);
     tolua_function(tolua_S,"setRotationDegrees",tolua_matrix4_irr_core_matrix4_setRotationDegrees00);
     tolua_function(tolua_S,"getRotationDegrees",tolua_matrix4_irr_core_matrix4_getRotationDegrees00);
     tolua_function(tolua_S,"setInverseRotationRadians",tolua_matrix4_irr_core_matrix4_setInverseRotationRadians00);
     tolua_function(tolua_S,"setInverseRotationDegrees",tolua_matrix4_irr_core_matrix4_setInverseRotationDegrees00);
     tolua_function(tolua_S,"setScale",tolua_matrix4_irr_core_matrix4_setScale00);
     tolua_function(tolua_S,"getScale",tolua_matrix4_irr_core_matrix4_getScale00);
     tolua_function(tolua_S,"inverseTranslateVect",tolua_matrix4_irr_core_matrix4_inverseTranslateVect00);
     tolua_function(tolua_S,"inverseRotateVect",tolua_matrix4_irr_core_matrix4_inverseRotateVect00);
     tolua_function(tolua_S,"rotateVect",tolua_matrix4_irr_core_matrix4_rotateVect00);
     tolua_function(tolua_S,"transformVect",tolua_matrix4_irr_core_matrix4_transformVect00);
     tolua_function(tolua_S,"transformVect",tolua_matrix4_irr_core_matrix4_transformVect01);
     tolua_function(tolua_S,"translateVect",tolua_matrix4_irr_core_matrix4_translateVect00);
     tolua_function(tolua_S,"transformPlane",tolua_matrix4_irr_core_matrix4_transformPlane00);
     tolua_function(tolua_S,"transformPlane",tolua_matrix4_irr_core_matrix4_transformPlane01);
     tolua_function(tolua_S,"transformBox",tolua_matrix4_irr_core_matrix4_transformBox00);
     tolua_function(tolua_S,"transformBoxEx",tolua_matrix4_irr_core_matrix4_transformBoxEx00);
     tolua_function(tolua_S,"multiplyWith1x4Matrix",tolua_matrix4_irr_core_matrix4_multiplyWith1x4Matrix00);
     tolua_function(tolua_S,"makeInverse",tolua_matrix4_irr_core_matrix4_makeInverse00);
     tolua_function(tolua_S,"getInverse",tolua_matrix4_irr_core_matrix4_getInverse00);
     tolua_function(tolua_S,"buildProjectionMatrixPerspectiveFovRH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovRH00);
     tolua_function(tolua_S,"buildProjectionMatrixPerspectiveFovLH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveFovLH00);
     tolua_function(tolua_S,"buildProjectionMatrixPerspectiveRH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveRH00);
     tolua_function(tolua_S,"buildProjectionMatrixPerspectiveLH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixPerspectiveLH00);
     tolua_function(tolua_S,"buildProjectionMatrixOrthoLH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoLH00);
     tolua_function(tolua_S,"buildProjectionMatrixOrthoRH",tolua_matrix4_irr_core_matrix4_buildProjectionMatrixOrthoRH00);
     tolua_function(tolua_S,"buildCameraLookAtMatrixLH",tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixLH00);
     tolua_function(tolua_S,"buildCameraLookAtMatrixRH",tolua_matrix4_irr_core_matrix4_buildCameraLookAtMatrixRH00);
     tolua_function(tolua_S,"buildShadowMatrix",tolua_matrix4_irr_core_matrix4_buildShadowMatrix00);
     tolua_function(tolua_S,"buildNDCToDCMatrix",tolua_matrix4_irr_core_matrix4_buildNDCToDCMatrix00);
     tolua_function(tolua_S,"interpolate",tolua_matrix4_irr_core_matrix4_interpolate00);
     tolua_function(tolua_S,"getTransposed",tolua_matrix4_irr_core_matrix4_getTransposed00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_matrix4 (lua_State* tolua_S) {
 return tolua_matrix4_open(tolua_S);
};
#endif

