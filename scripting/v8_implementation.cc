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

/**
  @file v8_implementations.cc
  @summary This file is another unoriginal waste heap, for
    the most part. It handles most of the initialization of
    Google V8. The rest is done implicitly in v8_shared.cc
*/

#include <string.h>
#include <stdlib.h>

#include <map>
#include <string>
#include <glib/glib.h>
using namespace std;

//Assuming those who want to compile this have 
//checked out V8 as per my well-hidden instruction
#include "v8_shared.h"

using namespace v8;
extern string tostring(int x);
extern void pidgin_printf(const char*);

//Include the file that actually does things
#include "js_functions.h"


map <string, PPlus_Script> scripts;

//Functions
  bool          ExecuteString(Handle<String> source, string name, bool print_result, bool report_exceptions);
  void          ReportException(TryCatch* handler);
  Handle<Value> Print(const Arguments& args);


int plus_v8_init()
{
  js_functions_initialize(); // see javascript_functions.cc
  global_context = Context::New(NULL, global_object_template);
  return 0;
}

string lastmessages[3];
int plus_evaluate_js_line(const char* line)
{
  if (lastmessages[2] == lastmessages[1] and lastmessages[1] == lastmessages[0] and lastmessages[0] == line)
    line = "\"Fuck you.\"";
  else {
    lastmessages[2] = lastmessages[1];
    lastmessages[1] = lastmessages[0];
    lastmessages[0] = line;
  }
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

//Kill the running V8 thread if it's been going on too long
int CALL_ID = 0;    
gpointer kill_v8_if_it_takes_too_long(gpointer data)
{
  usleep(1000000); //Unix only. Sorry Windows fans, go fuck yourselves.
  if (CALL_ID == int(data))
    V8::TerminateExecution();
  return NULL;
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
    
    //Thread a killer and run the code.
    Handle<Value> result;
    try {
      g_thread_create(kill_v8_if_it_takes_too_long,(gpointer)CALL_ID,false,NULL);
      result = scripts[name].script->Run();
      CALL_ID++;
    } catch(int x) { result = String::New("A system exception was thrown."); }
      catch(const char* x) { result = String::New(x); }
    
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
        string cstr = ToCString(str,"Result cannot be converted to string");
        if (cstr.length() > 512)
        {
          cstr.erase(511);
          cstr[508] = cstr[509] = cstr[510] = '.';
        }
        pidgin_printf(cstr.c_str());
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
