#include "bbcode_iface.h"
#include <basics/basics.h>
bbcode_tag::bbcode_tag(string name, bool is_unary): unary(is_unary) { bbcode_tags[name] = this; }
bbcode_tag::~bbcode_tag() { }

string bbcode_tag::get_replacement(PurpleConversation*, PurpleAccount*, int, string, string, string) { return "Internal error: invalid tag."; }
string bbcode_tag::get_unary_replacement(PurpleConversation*, PurpleAccount*, int, string) { return "Internal error: invalid tag."; }

bbmap bbcode_tags;
map<string,double> bbcode_tag::vars; ///< Real-valued variables needed by the functions during BB parsing.
map<string,string> bbcode_tag::svars; ///< String-valued variables needed by the functions during BB parsing.

bool bbcode_tag::at(const char *msg, int pos, bbcode_tag::instance &inst) {
  if (msg[pos] != '[')
    return false;
  puts("Looks like we're at a tag...");
  
  int ctsp = pos; // Complete tag starting position
  
  inst.closing = false; // Whether this is a closing tag (denoted by /).
  if (msg[++pos] == '/') {
    inst.closing = true;
    puts("Looks like we're at a closing tag...");
    pos++;
  }
  if (!is_letter(msg[pos])) return false;
  puts("We certainly seem to be at a tag...");
  const int ts = pos;
  while (is_letterd(msg[++pos]));
  
  inst.name = string(msg,ts,pos-ts);
  bbiter bit = bbcode_tags.find(inst.name);
  printf("Yeah, the tag's called '%s'...\n", inst.name.c_str());
  if (bit == bbcode_tags.end()) return false;
  inst.tag = bit->second;
  inst.unary = bit->second->unary;
  inst.arg = "";
  puts("It even exists...");
  
  if (msg[pos] == '=') {
    const int as = ++pos;
    while (msg[pos] and msg[pos] != ']') pos++;
    if (msg[pos] != ']') { return false; }
    inst.arg = string(msg,as,pos-as);
  }
  puts("We have most of a tag...");
  if (msg[pos] != ']')
    { return false; }
  puts("We have a tag!");
  inst.length = pos - ctsp + 1;
  return true;
}
