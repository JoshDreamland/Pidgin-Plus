/*
 * Pidgin Plus! Plugin
 *
 * Copyright (C) 2009 Josh Ventura
 *
 * Pidgin Plus! is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <www.gnu.org/licenses>
 *
 */

#include <vector>
using namespace std;
#include "../v8_shared.h"
#include "glib-2.0/glib.h"

struct jsFuncDesc
{
  const char* name;
  Handle<Value> (*jsFunc)(const Arguments& args);
};
struct jsClassDesc
{
  const char* name;
  struct jsSubClass_basic* jsSubClass;
};
struct jsObject_basic
{
  const char* name; //   //   // Name of this object as it appears in the JavaScript
  Handle<ObjectTemplate> me; // A handle to this object in the JavaScript
  jsFuncDesc *myFunctions;  // Array of function names and codes
  jsClassDesc *myClasses;  // Array of class names and instances
  
  jsObject_basic(const char*,jsFuncDesc*,jsClassDesc*); // This will take care of the redundant parts for you.
};
struct jsSubClass_basic
{
  Handle<ObjectTemplate> me; // A handle to this object in the JavaScript
  virtual void construct();
};


#define EO_FUNC  { NULL, NULL }
#define EO_CLASS { NULL, NULL }

#define Add_Function(name,func) { #name, func },
#define Add_Class(name,class)   { #name, &class },

#define Begin_JavaScript_Functions struct NAME: jsObject_basic {
#define End_JavaScript_Functions   NAME(); }; _js_instit(NAME);
#define _js_instit(n) _js_instit1(n)
#define _js_instit1(n) n _inst_##n

#define Begin_Function_List  _egin_Function_L(NAME)
#define _egin_Function_L(n) _egin_Function_1(n)
#define _egin_Function_1(n) static jsFuncDesc n##_Funcs[] = {
#define End_Function_List   EO_FUNC };

#define Begin_Class_List _egin_Class_L(NAME)
#define _egin_Class_L(n) _egin_Class_1(n)
#define _egin_Class_1(n) static jsClassDesc n##_Classes[] = {
#define End_Class_List   EO_CLASS };

#define Link_Class(class, member) struct class::_jss_##member class::member;

#define Begin_SubClass(name) struct _jss_##name: jsSubClass_basic {
#define End_SubClass(name) }; static _jss_##name name;

#define Finalize_JavaScript_Class() _w_fin_class(NAME)
#define _w_fin_class(n)  _w_fin_class1(n)
#define _w_fin_class1(n) n::n(): jsObject_basic(#n, n##_Funcs, n##_Classes) {}; jsObject_basic* jsob_##n = &_inst_##n
