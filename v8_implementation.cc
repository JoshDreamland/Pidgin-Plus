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


#include <string.h>
#include <stdlib.h>

#include <map>
#include <string>
using namespace std;

#include "google_v8/v8-read-only/include/v8.h"

using namespace v8;
extern string tostring(int x);
extern void pidgin_printf(const char*);

struct PPlus_Script
{
  Handle<Script> script;
  //Handle<Context> context;
};

map <string, PPlus_Script> scripts;

//Functions
  bool          ExecuteString(Handle<String> source, string name, bool print_result, bool report_exceptions);
  void          ReportException(TryCatch* handler);
  Handle<Value> Print(const Arguments& args);


HandleScope               handle_to_global_scope; //Nothingness scope
Handle<ObjectTemplate>    global_object_template = ObjectTemplate::New(); //Containing global "this"
Handle<Context>           global_context; //Global Context

int plus_v8_init()
{
  js_functions_initialize(); // see javascript_functions.cc
  global_context = Context::New(NULL, global_object_template);
}

int plus_evaluate_js_line(const char* line)
{
  v8::HandleScope handle_scope;
  ExecuteString(String::New(line), "(shell)", true, true);
  return 0;
}

//If V8 is unloaded here, attempts to 
//reenable it will result in segfault
int plus_v8_end()
{
  return 0;
}

//Until the pidgin devs bother to only load plugins when 
//they are enabled, the V8 engine will remain in memory.

//This can be called when a device to destroy a plugin
//Is not only implemented, but actually effing used.
int plus_v8_wipe()
{
  V8::Dispose();
  return 0;
}


string ValueToStr(Handle<Value> val)
{
  const String::Utf8Value str(val);
  return *str ? *str : "ERROR: Cannot convert output to string";
}

//Returns true if successful
bool load_script(Handle<String> source, string name)
{
  Handle<Script> scr = Script::Compile(source, String::New(name.c_str()));
  if (scr.IsEmpty())
    return 0;
  scripts[name].script = scr;
  return 1;
}
// Executes a string within the current v8 context.
bool ExecuteString(Handle<String> source, string name, bool print_result, bool report_exceptions)
{
  TryCatch try_catch;
  Context::Scope scope(global_context);
  
  //Renew "limited" script resources
  js_resources_renew(); //Like time, and number of print() calls
  
  if (load_script(source,name))
  {
    HandleScope handle_scope;
    Handle<Value> result = scripts[name].script->Run();
    if (result.IsEmpty())
    {
      // Print errors that happened during execution.
      if (report_exceptions)
        ReportException(&try_catch);
      return false;
    }
    else
    {
      if (print_result && !result->IsUndefined())
      {
        String::Utf8Value str(result);
        const char* cstr = ToCString(str,"Result cannot be converted to string");
        pidgin_printf(cstr);
      }
      return true;
    }
  }
  else ReportException(&try_catch);
  return false;
}


void ReportException(TryCatch* try_catch)
{
  HandleScope handle_scope;
  String::Utf8Value exception(try_catch->Exception());
  const char* exception_string = ToCString(exception,"Exception occurred but cannot be converted to string");
  Handle<Message> message = try_catch->Message();
  if (message.IsEmpty())
  {
    // V8 didn't provide any extra information about this error; just
    // print the exception.
    pidgin_printf(exception_string);
  }
  else
  {
    // Print (filename):(line number): (message).
    String::Utf8Value filename(message->GetScriptResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    string eff = string(filename_string) + ", line " + tostring(linenum) + ": " + exception_string;
    // Print line of source code.
    String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    eff += string("\n") + sourceline_string + "\n";
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++)
      eff+=" ";
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++)
      eff += "^";
    pidgin_printf(eff.c_str());
  }
}
