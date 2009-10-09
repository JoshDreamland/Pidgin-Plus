#include "v8.h"
using namespace v8;

void Trace();
void ClearDebuggingWindow();
bool DebuggingWindowVisible;


struct ChatWnd
{
  Handle<Value> PHandle;
  Handle<Value> Contacts;
  Handle<Value> EditText;
  Handle<Value> EditChangeAllowed;
  Handle<Value> ChatLogEnabled;
  Handle<Value> OverrideFmtEnabled;
  Handle<Value> IsMobileChat;
  
  static Handle<Value> SendMessage(const Arguments& message); //bool (String)
  static Handle<Value> SendFile(const Arguments& path); //bool (String)
  
  static Handle<Value> AddContact(const Arguments& email) { return String::New("TACOS"); } //bool (String)
  static Handle<Value> DisplayInfoMessage(const Arguments& StringDurForce); //void (String, int duration=8, bool forcenow = 0)
  static Handle<Value> ResetInfoMessage(const Arguments& nothing); //void ()
  
  static Handle<Value> EditText_GetCurSelStart(const Arguments& nothing); //int ()
  static Handle<Value> EditText_GetCurSelEnd(const Arguments& nothing); //int ()
  static Handle<Value> EditText_SetCurSel(const Arguments& StartEnd); //void (int Start, int End)
  static Handle<Value> EditText_ReplaceSel(const Arguments& text); //void (String)
  
  static Handle<Value> HistoryText_GetCurSelStart(const Arguments& nothing); //int ()
  static Handle<Value> HistoryText_GetCurSelEnd(const Arguments& nothing); //int ()
  static Handle<Value> HistoryText_GetTextRange(const Arguments& StartEndCodes); //String (int start,int end, bool include_codes)
};

typedef Handle<Value>(*proto)(const Arguments&);

void Plus_JSI_Init(Handle<Context> context)
{
  Handle<ObjectTemplate> chatwnd_object = ObjectTemplate::New();
  chatwnd_object->SetInternalFieldCount(1);
  chatwnd_object->Set(String::New("AddContact"),FunctionTemplate::New((proto)&ChatWnd::AddContact));
  
  Handle<Object> AChatWnd = chatwnd_object->NewInstance();
  context->Global()->Set(String::New("ChatWnd"), AChatWnd);
}

