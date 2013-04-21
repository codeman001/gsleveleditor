#include "stdafx.h"

using namespace irr;
using namespace irr::core;

/*
** Lua binding: irrString
** Generated automatically by tolua++-1.0.92 on 04/21/13 13:56:35.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
int  tolua_irrString_open (lua_State* tolua_S);


/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_wchar_t (lua_State* tolua_S)
{
 wchar_t* self = (wchar_t*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__string_char_ (lua_State* tolua_S)
{
 irr::core::string<char>* self = (irr::core::string<char>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}

static int tolua_collect_irr__core__string_wchar_t_ (lua_State* tolua_S)
{
 irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*) tolua_tousertype(tolua_S,1,0);
	delete self;
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"wchar_t");
 tolua_usertype(tolua_S,"irr::core::string<char>");
 tolua_usertype(tolua_S,"irr::core::string<wchar_t>");
}

/* method: new of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new00
static int tolua_irrString_irr_core_string_char__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<char>");
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

/* method: new_local of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new00_local
static int tolua_irrString_irr_core_string_char__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<char>");
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

/* method: new of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new01
static int tolua_irrString_irr_core_string_char__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<char>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new01_local
static int tolua_irrString_irr_core_string_char__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<char>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new02
static int tolua_irrString_irr_core_string_char__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int number = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>(number);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<char>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__new02_local
static int tolua_irrString_irr_core_string_char__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int number = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   irr::core::string<char>* tolua_ret = (irr::core::string<char>*)  new irr::core::string<char>(number);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<char>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__delete00
static int tolua_irrString_irr_core_string_char__delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator+ of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char___add00
static int tolua_irrString_irr_core_string_char___add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   irr::core::string<char> tolua_ret = (irr::core::string<char>)  self->operator+(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::string<char>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<char>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::string<char>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<char>");
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

/* method: operator[] of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char___geti00
static int tolua_irrString_irr_core_string_char___geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   char tolua_ret = (char)  self->operator[](index);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char___eq00
static int tolua_irrString_irr_core_string_char___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: operator== of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char___eq01
static int tolua_irrString_irr_core_string_char___eq01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const char str = ((const char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(&str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)str);
  }
 }
 return 2;
tolua_lerror:
 return tolua_irrString_irr_core_string_char___eq00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator< of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char___lt00
static int tolua_irrString_irr_core_string_char___lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.lt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__size00
static int tolua_irrString_irr_core_string_char__size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_str of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__c_str00
static int tolua_irrString_irr_core_string_char__c_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_str'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_str();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_lower of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__make_lower00
static int tolua_irrString_irr_core_string_char__make_lower00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_lower'",NULL);
#endif
  {
   self->make_lower();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_lower'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_upper of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__make_upper00
static int tolua_irrString_irr_core_string_char__make_upper00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_upper'",NULL);
#endif
  {
   self->make_upper();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_upper'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equals_ignore_case of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__equals_ignore_case00
static int tolua_irrString_irr_core_string_char__equals_ignore_case00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equals_ignore_case'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equals_ignore_case(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equals_ignore_case'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equalsn of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__equalsn00
static int tolua_irrString_irr_core_string_char__equalsn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equalsn'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equalsn(*other,len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equalsn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equalsn of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__equalsn01
static int tolua_irrString_irr_core_string_char__equalsn01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const char other = ((const char)  tolua_tonumber(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equalsn'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equalsn(&other,len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)other);
  }
 }
 return 2;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__equalsn00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__append00
static int tolua_irrString_irr_core_string_char__append00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char character = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(character);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'append'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__append01
static int tolua_irrString_irr_core_string_char__append01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(*other);
  }
 }
 return 0;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__append00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__append02
static int tolua_irrString_irr_core_string_char__append02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<char>* other = ((const irr::core::string<char>*)  tolua_tousertype(tolua_S,2,0));
  signed int length = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(*other,length);
  }
 }
 return 0;
tolua_lerror:
 return tolua_irrString_irr_core_string_char__append01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: reserve of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__reserve00
static int tolua_irrString_irr_core_string_char__reserve00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  signed int count = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reserve'",NULL);
#endif
  {
   self->reserve(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reserve'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findFirst of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__findFirst00
static int tolua_irrString_irr_core_string_char__findFirst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char c = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findFirst'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findFirst(c);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findFirst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findFirstChar of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__findFirstChar00
static int tolua_irrString_irr_core_string_char__findFirstChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char c = ((char)  tolua_tonumber(tolua_S,2,0));
  int count = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findFirstChar'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findFirstChar(&c,count);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushnumber(tolua_S,(lua_Number)c);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findFirstChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findNext of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__findNext00
static int tolua_irrString_irr_core_string_char__findNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char c = ((char)  tolua_tonumber(tolua_S,2,0));
  signed int startPos = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findNext'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findNext(c,startPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findNext'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findLast of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__findLast00
static int tolua_irrString_irr_core_string_char__findLast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<char>* self = (const irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char c = ((char)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findLast'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findLast(c);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findLast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: subString of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__subString00
static int tolua_irrString_irr_core_string_char__subString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  signed int begin = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int length = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subString'",NULL);
#endif
  {
   irr::core::string<char> tolua_ret = (irr::core::string<char>)  self->subString(begin,length);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::string<char>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<char>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::string<char>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<char>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replace of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__replace00
static int tolua_irrString_irr_core_string_char__replace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  char toReplace = ((char)  tolua_tonumber(tolua_S,2,0));
  char replaceWith = ((char)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'replace'",NULL);
#endif
  {
   self->replace(toReplace,replaceWith);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: trim of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__trim00
static int tolua_irrString_irr_core_string_char__trim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'trim'",NULL);
#endif
  {
   self->trim();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'trim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: erase of class  irr::core::string<char> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_char__erase00
static int tolua_irrString_irr_core_string_char__erase00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<char>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<char>* self = (irr::core::string<char>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'erase'",NULL);
#endif
  {
   self->erase(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'erase'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new00
static int tolua_irrString_irr_core_string_wchar_t__new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>();
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<wchar_t>");
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

/* method: new_local of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new00_local
static int tolua_irrString_irr_core_string_wchar_t__new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>();
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<wchar_t>");
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

/* method: new of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new01
static int tolua_irrString_irr_core_string_wchar_t__new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>(*other);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<wchar_t>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new01_local
static int tolua_irrString_irr_core_string_wchar_t__new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>(*other);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<wchar_t>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new02
static int tolua_irrString_irr_core_string_wchar_t__new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int number = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>(number);
   tolua_pushusertype(tolua_S,(void*)tolua_ret,"irr::core::string<wchar_t>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__new02_local
static int tolua_irrString_irr_core_string_wchar_t__new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  int number = ((int)  tolua_tonumber(tolua_S,2,0));
  {
   irr::core::string<wchar_t>* tolua_ret = (irr::core::string<wchar_t>*)  new irr::core::string<wchar_t>(number);
   tolua_pushusertype_and_takeownership(tolua_S,(void *)tolua_ret,"irr::core::string<wchar_t>");
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: delete of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__delete00
static int tolua_irrString_irr_core_string_wchar_t__delete00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
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

/* method: operator+ of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t___add00
static int tolua_irrString_irr_core_string_wchar_t___add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'",NULL);
#endif
  {
   irr::core::string<wchar_t> tolua_ret = (irr::core::string<wchar_t>)  self->operator+(*other);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::string<wchar_t>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<wchar_t>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::string<wchar_t>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<wchar_t>");
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

/* method: operator[] of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t___geti00
static int tolua_irrString_irr_core_string_wchar_t___geti00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator[]'",NULL);
#endif
  {
   wchar_t tolua_ret = (wchar_t)  self->operator[](index);
   {
#ifdef __cplusplus
    void* tolua_obj = new wchar_t(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"wchar_t");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(wchar_t));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"wchar_t");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.geti'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t___eq00
static int tolua_irrString_irr_core_string_wchar_t___eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
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

/* method: operator== of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t___eq01
static int tolua_irrString_irr_core_string_wchar_t___eq01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const wchar_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const wchar_t* str = ((const wchar_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(str);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t___eq00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator< of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t___lt00
static int tolua_irrString_irr_core_string_wchar_t___lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.lt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: size of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__size00
static int tolua_irrString_irr_core_string_wchar_t__size00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'size'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->size();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'size'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: c_str of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__c_str00
static int tolua_irrString_irr_core_string_wchar_t__c_str00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'c_str'",NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->c_str();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'c_str'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_lower of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__make_lower00
static int tolua_irrString_irr_core_string_wchar_t__make_lower00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_lower'",NULL);
#endif
  {
   self->make_lower();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_lower'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: make_upper of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__make_upper00
static int tolua_irrString_irr_core_string_wchar_t__make_upper00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'make_upper'",NULL);
#endif
  {
   self->make_upper();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'make_upper'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equals_ignore_case of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__equals_ignore_case00
static int tolua_irrString_irr_core_string_wchar_t__equals_ignore_case00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equals_ignore_case'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equals_ignore_case(*other);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equals_ignore_case'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equalsn of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__equalsn00
static int tolua_irrString_irr_core_string_wchar_t__equalsn00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equalsn'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equalsn(*other,len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'equalsn'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: equalsn of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__equalsn01
static int tolua_irrString_irr_core_string_wchar_t__equalsn01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const wchar_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const wchar_t* other = ((const wchar_t*)  tolua_tousertype(tolua_S,2,0));
  int len = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'equalsn'",NULL);
#endif
  {
   bool tolua_ret = (bool)  self->equalsn(other,len);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__equalsn00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__append00
static int tolua_irrString_irr_core_string_wchar_t__append00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t character = *((wchar_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(character);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'append'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__append01
static int tolua_irrString_irr_core_string_wchar_t__append01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(*other);
  }
 }
 return 0;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__append00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: append of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__append02
static int tolua_irrString_irr_core_string_wchar_t__append02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  const irr::core::string<wchar_t>* other = ((const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,2,0));
  signed int length = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'append'",NULL);
#endif
  {
   self->append(*other,length);
  }
 }
 return 0;
tolua_lerror:
 return tolua_irrString_irr_core_string_wchar_t__append01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: reserve of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__reserve00
static int tolua_irrString_irr_core_string_wchar_t__reserve00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  signed int count = (( signed int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'reserve'",NULL);
#endif
  {
   self->reserve(count);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'reserve'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findFirst of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__findFirst00
static int tolua_irrString_irr_core_string_wchar_t__findFirst00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t c = *((wchar_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findFirst'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findFirst(c);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findFirst'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findFirstChar of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__findFirstChar00
static int tolua_irrString_irr_core_string_wchar_t__findFirstChar00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t* c = ((wchar_t*)  tolua_tousertype(tolua_S,2,0));
  int count = ((int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findFirstChar'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findFirstChar(c,count);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findFirstChar'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findNext of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__findNext00
static int tolua_irrString_irr_core_string_wchar_t__findNext00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t c = *((wchar_t*)  tolua_tousertype(tolua_S,2,0));
  signed int startPos = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findNext'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findNext(c,startPos);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findNext'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: findLast of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__findLast00
static int tolua_irrString_irr_core_string_wchar_t__findLast00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const irr::core::string<wchar_t>* self = (const irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t c = *((wchar_t*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'findLast'",NULL);
#endif
  {
   signed int tolua_ret = ( signed int)  self->findLast(c);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'findLast'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: subString of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__subString00
static int tolua_irrString_irr_core_string_wchar_t__subString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  signed int begin = (( signed int)  tolua_tonumber(tolua_S,2,0));
  signed int length = (( signed int)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'subString'",NULL);
#endif
  {
   irr::core::string<wchar_t> tolua_ret = (irr::core::string<wchar_t>)  self->subString(begin,length);
   {
#ifdef __cplusplus
    void* tolua_obj = new irr::core::string<wchar_t>(tolua_ret);
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<wchar_t>");
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(irr::core::string<wchar_t>));
    tolua_pushusertype_and_takeownership(tolua_S,tolua_obj,"irr::core::string<wchar_t>");
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'subString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: replace of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__replace00
static int tolua_irrString_irr_core_string_wchar_t__replace00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"wchar_t",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,3,"wchar_t",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  wchar_t toReplace = *((wchar_t*)  tolua_tousertype(tolua_S,2,0));
  wchar_t replaceWith = *((wchar_t*)  tolua_tousertype(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'replace'",NULL);
#endif
  {
   self->replace(toReplace,replaceWith);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'replace'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: trim of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__trim00
static int tolua_irrString_irr_core_string_wchar_t__trim00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'trim'",NULL);
#endif
  {
   self->trim();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'trim'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: erase of class  irr::core::string<wchar_t> */
#ifndef TOLUA_DISABLE_tolua_irrString_irr_core_string_wchar_t__erase00
static int tolua_irrString_irr_core_string_wchar_t__erase00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"irr::core::string<wchar_t>",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  irr::core::string<wchar_t>* self = (irr::core::string<wchar_t>*)  tolua_tousertype(tolua_S,1,0);
  int index = ((int)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'erase'",NULL);
#endif
  {
   self->erase(index);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'erase'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_irrString_open (lua_State* tolua_S)
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
    tolua_cclass(tolua_S,"string_char_","irr::core::string<char>","",tolua_collect_irr__core__string_char_);
    #else
    tolua_cclass(tolua_S,"string_char_","irr::core::string<char>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"string_char_");
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_char__new00);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_char__new00_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_char__new00_local);
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_char__new01);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_char__new01_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_char__new01_local);
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_char__new02);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_char__new02_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_char__new02_local);
     tolua_function(tolua_S,"delete",tolua_irrString_irr_core_string_char__delete00);
     tolua_function(tolua_S,".add",tolua_irrString_irr_core_string_char___add00);
     tolua_function(tolua_S,".geti",tolua_irrString_irr_core_string_char___geti00);
     tolua_function(tolua_S,".eq",tolua_irrString_irr_core_string_char___eq00);
     tolua_function(tolua_S,".eq",tolua_irrString_irr_core_string_char___eq01);
     tolua_function(tolua_S,".lt",tolua_irrString_irr_core_string_char___lt00);
     tolua_function(tolua_S,"size",tolua_irrString_irr_core_string_char__size00);
     tolua_function(tolua_S,"c_str",tolua_irrString_irr_core_string_char__c_str00);
     tolua_function(tolua_S,"make_lower",tolua_irrString_irr_core_string_char__make_lower00);
     tolua_function(tolua_S,"make_upper",tolua_irrString_irr_core_string_char__make_upper00);
     tolua_function(tolua_S,"equals_ignore_case",tolua_irrString_irr_core_string_char__equals_ignore_case00);
     tolua_function(tolua_S,"equalsn",tolua_irrString_irr_core_string_char__equalsn00);
     tolua_function(tolua_S,"equalsn",tolua_irrString_irr_core_string_char__equalsn01);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_char__append00);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_char__append01);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_char__append02);
     tolua_function(tolua_S,"reserve",tolua_irrString_irr_core_string_char__reserve00);
     tolua_function(tolua_S,"findFirst",tolua_irrString_irr_core_string_char__findFirst00);
     tolua_function(tolua_S,"findFirstChar",tolua_irrString_irr_core_string_char__findFirstChar00);
     tolua_function(tolua_S,"findNext",tolua_irrString_irr_core_string_char__findNext00);
     tolua_function(tolua_S,"findLast",tolua_irrString_irr_core_string_char__findLast00);
     tolua_function(tolua_S,"subString",tolua_irrString_irr_core_string_char__subString00);
     tolua_function(tolua_S,"replace",tolua_irrString_irr_core_string_char__replace00);
     tolua_function(tolua_S,"trim",tolua_irrString_irr_core_string_char__trim00);
     tolua_function(tolua_S,"erase",tolua_irrString_irr_core_string_char__erase00);
    tolua_endmodule(tolua_S);
    #ifdef __cplusplus
    tolua_cclass(tolua_S,"string_wchar_t_","irr::core::string<wchar_t>","",tolua_collect_irr__core__string_wchar_t_);
    #else
    tolua_cclass(tolua_S,"string_wchar_t_","irr::core::string<wchar_t>","",NULL);
    #endif
    tolua_beginmodule(tolua_S,"string_wchar_t_");
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_wchar_t__new00);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_wchar_t__new00_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_wchar_t__new00_local);
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_wchar_t__new01);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_wchar_t__new01_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_wchar_t__new01_local);
     tolua_function(tolua_S,"new",tolua_irrString_irr_core_string_wchar_t__new02);
     tolua_function(tolua_S,"new_local",tolua_irrString_irr_core_string_wchar_t__new02_local);
     tolua_function(tolua_S,".call",tolua_irrString_irr_core_string_wchar_t__new02_local);
     tolua_function(tolua_S,"delete",tolua_irrString_irr_core_string_wchar_t__delete00);
     tolua_function(tolua_S,".add",tolua_irrString_irr_core_string_wchar_t___add00);
     tolua_function(tolua_S,".geti",tolua_irrString_irr_core_string_wchar_t___geti00);
     tolua_function(tolua_S,".eq",tolua_irrString_irr_core_string_wchar_t___eq00);
     tolua_function(tolua_S,".eq",tolua_irrString_irr_core_string_wchar_t___eq01);
     tolua_function(tolua_S,".lt",tolua_irrString_irr_core_string_wchar_t___lt00);
     tolua_function(tolua_S,"size",tolua_irrString_irr_core_string_wchar_t__size00);
     tolua_function(tolua_S,"c_str",tolua_irrString_irr_core_string_wchar_t__c_str00);
     tolua_function(tolua_S,"make_lower",tolua_irrString_irr_core_string_wchar_t__make_lower00);
     tolua_function(tolua_S,"make_upper",tolua_irrString_irr_core_string_wchar_t__make_upper00);
     tolua_function(tolua_S,"equals_ignore_case",tolua_irrString_irr_core_string_wchar_t__equals_ignore_case00);
     tolua_function(tolua_S,"equalsn",tolua_irrString_irr_core_string_wchar_t__equalsn00);
     tolua_function(tolua_S,"equalsn",tolua_irrString_irr_core_string_wchar_t__equalsn01);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_wchar_t__append00);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_wchar_t__append01);
     tolua_function(tolua_S,"append",tolua_irrString_irr_core_string_wchar_t__append02);
     tolua_function(tolua_S,"reserve",tolua_irrString_irr_core_string_wchar_t__reserve00);
     tolua_function(tolua_S,"findFirst",tolua_irrString_irr_core_string_wchar_t__findFirst00);
     tolua_function(tolua_S,"findFirstChar",tolua_irrString_irr_core_string_wchar_t__findFirstChar00);
     tolua_function(tolua_S,"findNext",tolua_irrString_irr_core_string_wchar_t__findNext00);
     tolua_function(tolua_S,"findLast",tolua_irrString_irr_core_string_wchar_t__findLast00);
     tolua_function(tolua_S,"subString",tolua_irrString_irr_core_string_wchar_t__subString00);
     tolua_function(tolua_S,"replace",tolua_irrString_irr_core_string_wchar_t__replace00);
     tolua_function(tolua_S,"trim",tolua_irrString_irr_core_string_wchar_t__trim00);
     tolua_function(tolua_S,"erase",tolua_irrString_irr_core_string_wchar_t__erase00);
    tolua_endmodule(tolua_S);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_irrString (lua_State* tolua_S) {
 return tolua_irrString_open(tolua_S);
};
#endif

