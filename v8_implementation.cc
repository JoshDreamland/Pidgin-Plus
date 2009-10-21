#include <string.h>
#include <stdlib.h>

#include <map>
#include <string>
using namespace std;

#include "google_v8/v8-read-only/include/v8.h"
using namespace v8;
extern string tostring(long double x);
extern void pidgin_printf(const char*);

struct PPlus_Script
{
  Handle<Script> script;
  Handle<Context> context;
};

map <string, PPlus_Script> scripts;


bool ExecuteString(Handle<String> source, Handle<Value> name, bool print_result, bool report_exceptions);
Handle<Value> Print(const Arguments& args);
Handle<Value> Quit(const Arguments& args);
void ReportException(TryCatch* handler);

HandleScope handle_to_global_scope;
Handle<ObjectTemplate> global_object_template = ObjectTemplate::New();;
Handle<Context> GlobalContext = Context::New(NULL, global_object_template);;
Context::Scope context_scope(GlobalContext);

int plus_v8_init()
{
  global_object_template->Set(String::New("print"), FunctionTemplate::New(Print)); //This explains a lot
}

int plus_evaluate_js_line(const char* line)
{
  v8::HandleScope handle_scope;
  ExecuteString(String::New(line), String::New("(shell)"), true, true);
  return 0;
}

int plus_v8_end()
{
  V8::Dispose();
  return 0;
}

// Extracts a C string from a V8 Utf8Value.
const char* ToCString(const String::Utf8Value& value,const char* ifnull = "<string conversion failed>")
{
  return *value ? *value : ifnull;
}


// The callback that is invoked by v8 whenever the JavaScript 'print'
// function is called.  Prints its arguments on stdout separated by
// spaces and ending with a newline.
Handle<Value> Print(const Arguments& args)
{
  bool first = true;
  for (int i = 0; i < args.Length(); i++)
  {
    HandleScope handle_scope;
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    pidgin_printf(cstr);
  }
  return Undefined();
}


// The callback that is invoked by v8 whenever the JavaScript 'quit'
// function is called.  Quits.
Handle<Value> Quit(const Arguments& args)
{
  // If not arguments are given args[0] will yield undefined which
  // converts to the integer value 0.
  int exit_code = args[0]->Int32Value();
  exit(exit_code);
  return Undefined();
}


string ValueToStr(Handle<Value> val)
{
  const String::Utf8Value str(val);
  return *str ? *str : "ERROR: Cannot convert output to string";
}

//Returns true if successful
bool load_script(Handle<String> source, Handle<Value> name)
{
  Handle<Script> scr = Script::Compile(source, name);
  if (scr.IsEmpty())
    return 0;
  scripts[ValueToStr(name)].script = scr;
  return 1;
}
// Executes a string within the current v8 context.
bool ExecuteString(Handle<String> source, Handle<Value> name, bool print_result, bool report_exceptions)
{
  TryCatch try_catch;
  if (load_script(source,name))
  {
    HandleScope handle_scope;
    Handle<Value> result = scripts[ValueToStr(name)].script->Run();
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
