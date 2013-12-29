/**
 * @file  v8_implementations.cc
 * @brief Implement functions for interfacing with Google V8.
 * 
 * This file is another unoriginal waste heap, for
 * the most part. It handles most of the initialization of
 * Google V8. The rest is done implicitly in v8_shared.cc
 *
 * @section License
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
**/

#include <string.h>
#include <stdlib.h>
#include <stdint.h> // For some reason, explicit cast from void* to int is forbidden when void* is larger.

#include <map>
#include <string>
#include <glib.h>
#include <sys/time.h>
#include <unistd.h>
using namespace std;

//Assuming those who want to compile this have 
//checked out V8 as per my well-hidden instruction
#include "v8_shared.h"

using namespace v8;
extern string tostring(int x);
extern void pidgin_printf(const char*);

//Include the file that actually does things
#include "js_functions.h"
#include "v8_implementation.h"

map <string, PPlus_Script> scripts;

//Functions
static bool ExecuteString(string source, string name, bool print_result, bool report_exceptions);
static void ReportException(TryCatch* handler);


int plus_v8_init()
{
  v8::V8::InitializeICU();
  plus_v8_global = new plus_v8_instance();
  js_functions_initialize();
  plus_v8_global->begin();
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
  HandleScope hs(plus_v8_global->isolate);
  ExecuteString(line, "(shell)", true, true);
  return 0;
}

int plus_v8_end() {
  // If V8 is unloaded here, any attempt to reenable it will result in segfault
  return 0;
}

intptr_t CALL_ID = 0;

/// Static callback to kill the running V8 thread if it's been open for too long.
static gpointer kill_v8_if_it_takes_too_long(gpointer data)
{
  usleep(1000000); // GNU only. Sorry, MSVS fans: go fuck yourselves.
  if (CALL_ID == intptr_t(data))
    V8::TerminateExecution();
  return NULL;
}

//Until the pidgin devs bother to only load plugins when 
//they are enabled, the V8 engine will remain in memory.

/// Destroy the V8 context, permanently.
/// This can be called when some device to destroy a pidgin plugin is not only implemented, but actually used.
int plus_v8_wipe() {
  V8::Dispose();
  return 0;
}

//Returns true if successful
static bool load_script(string source, string name)
{
  Handle<Script> scr = Script::Compile(GV8::String(source.c_str()), GV8::String(name.c_str()));
  if (scr.IsEmpty())
    return 0;
  scripts[name].script = scr;
  return 1;
}
// Executes a string within the current v8 context.
bool ExecuteString(string source, string name, bool print_result, bool report_exceptions)
{
  TryCatch try_catch;
  Context::Scope scope(plus_v8_global->context);
  
  //Renew "limited" script resources
  js_resources_renew(); //Like time, and number of print() calls
  
  if (load_script(source,name))
  {
    //Thread a killer and run the code.
    Handle<Value> result;
    try {
      g_thread_new("kill-v8-if-it-takes-too-long", kill_v8_if_it_takes_too_long, (gpointer)CALL_ID);
      result = scripts[name].script->Run();
      CALL_ID++;
    } catch(int x) { result = GV8::String("A system exception was thrown."); }
      catch(const char* x) { result = GV8::String(x); }
    
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
        string cstr = ToCString(str, "Result cannot be converted to string");
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
    string effabbr = eff;
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
    pidgin_printf(effabbr.c_str());
  }
}
