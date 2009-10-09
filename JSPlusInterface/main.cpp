#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#include <map>
#include <string>
using namespace std;

#include "v8.h"
using namespace v8;

#include "plus_structures.h"

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

int main(int argc, char* argv[])
{
  V8::SetFlagsFromCommandLine(&argc, argv, true);
  HandleScope handle_scope;
  // Create a template for the global object.
  Handle<ObjectTemplate> global = ObjectTemplate::New();
  // Bind the global 'print' function to the C++ Print callback.
  global->Set(String::New("print"), FunctionTemplate::New(Print)); //This explains a lot
  global->Set(String::New("quit"), FunctionTemplate::New(Quit));
  
  // Create a new execution environment containing the built-in functions
  Handle<Context> GlobalContext = Context::New(NULL, global);
  
  Plus_JSI_Init(GlobalContext);
  
  // Enter the newly created execution environment.
  Context::Scope context_scope(GlobalContext);
  
  printf("V8 version %s\n", V8::GetVersion());
  while (true)
  {
    char strb[4096]; printf("> ");
    char *str = fgets(strb, 4096, stdin);
    if (str == NULL) break;
    v8::HandleScope handle_scope;
    ExecuteString(String::New(str), String::New("(shell)"), true, true);
  }
  
  V8::Dispose(); //What the FACK is dispose()?
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
    if (first)
      first = false;
    else
      printf(" ");
    String::Utf8Value str(args[i]);
    const char* cstr = ToCString(str);
    printf("%s", cstr);
  }
  printf("\n");
  fflush(stdout);
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
        printf("%s\n", cstr);
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
    printf("%s\n", exception_string);
  }
  else
  {
    // Print (filename):(line number): (message).
    String::Utf8Value filename(message->GetScriptResourceName());
    const char* filename_string = ToCString(filename);
    int linenum = message->GetLineNumber();
    printf("%s:%i: %s\n", filename_string, linenum, exception_string);
    // Print line of source code.
    String::Utf8Value sourceline(message->GetSourceLine());
    const char* sourceline_string = ToCString(sourceline);
    printf("%s\n", sourceline_string);
    // Print wavy underline (GetUnderline is deprecated).
    int start = message->GetStartColumn();
    for (int i = 0; i < start; i++)
      printf(" ");
    int end = message->GetEndColumn();
    for (int i = start; i < end; i++)
      printf("^");
    printf("\n");
  }
}
