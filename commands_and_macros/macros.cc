/**
 * @file macros.cc
 * @brief Source implementing handlers for text replacement. 
 * 
 * In Pidgin Plus!, "macro" refers to any segment of message that will be replaced with
 * more text later. These include (!N), which is replaced with the name (if available)
 * of the other contact(s) in the chat. Because text replacement accounts for the 
 * majority of the reason we bother having events at this point, filter_outgoing is
 * also defined in this file.Pidgin Plus! Plugin
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
 */

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include <compatibility/codes.h>
#include <scripting/v8_implementation.h>

extern PurplePlugin *pidgin_plus_plugin;
#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
#define chaterror(x) purple_conversation_write(conv, "", x, PURPLE_MESSAGE_NO_LOG, time(NULL))

#include <string.h>

#include <map>
#include <string>
#include <vector>
using namespace std;

#include <gtk_etc_frontend/date_time.h>        //Fetch date/time info
#include <gtk_etc_frontend/gtk_timer_struct.h> //Ping timing
#include <purple_frontend/purple_extension.h>  //Simplify
#include <basics/basics.h>
#include "bbcode_iface.h"
#include "macros.h"

map<string,timestack> conv_pingclocks;

// int d=0;
// static const char * hexa = "0123456789ABCDEF";

static unsigned skip_html(string m, unsigned p)
{
  while (m[p] == '<')
    while (m[p] and m[p++] != '>')
      if (m[p]=='"') while (m[++p] and m[p] != '"') if (m[p]=='\\') p++;
      else if (m[p]=='\'') while (m[++p] and m[p] != '\'') if (m[p]=='\\') p++;
  return p;
}

struct opentaginfo {
  string name, arg;
  int tag_start_pos, tag_end_pos;
  opentaginfo(string n,string a,int sp,int ep): name(n), arg(a), tag_start_pos(sp), tag_end_pos(ep) {}
};

#define macroflags int mflags, int window_type, PurpleAccount *account, string recipient, string message, PurpleConversation *conv
struct macro_basic {
  const char* name;
  virtual string get_replacement(macroflags) = 0;
  macro_basic(const char* n): name(n) {}
  virtual ~macro_basic() {}
};
static map<string, macro_basic*> available_macros;

bool filter_message(int mflags, int window_type, PurpleAccount *account, const char *recipient, char **message, PurpleConversation *conv)
{
  string msg = *message;
  string rs = recipient;
  
  for (unsigned int p = 0; p < msg.length(); p++)
  {
    p = skip_html(msg,p);
    if (msg[p] == '(' and msg[p+1] == '!')
    {
      const unsigned int pstart = p++;
      while (is_letter(msg[++p]));
      if (msg[p] == ')')
      {
        string c = msg.substr(pstart+2,p-pstart-2);
        if (c != "")
        {
          map<string, macro_basic*>:: iterator mac = available_macros.find(c); 
          if (mac != available_macros.end())
          {
            string rw = mac->second->get_replacement(mflags, window_type, account, rs, msg, conv);
            msg.replace(pstart, p-pstart+1, rw);
            p += rw.length() - (p - pstart);
            continue;
          }
        }
      }
      //Didn't break, wasn't a command
      p = pstart+1; //skip the !
    }
  }
  
  //navigate to end of HTML garbage
  unsigned int p = 0;
  while (msg[p] == '<')
    while (msg[p] and msg[p++] != '>') 
      if (msg[p]=='"') while (msg[++p] and msg[p] != '"') if (msg[p]=='\\') p++;
      else if (msg[p]=='\'') while (msg[++p] and msg[p] != '\'') if (msg[p]=='\\') p++;
  
  bool req_auto_reply = 0;
  
  if (msg[p] != '/')
  {
    char* cm = purple_markup_strip_html(msg.c_str());
    if (!strcmp(cm,"Ping? [msgplus]")) //Someone's sending a ping request
    {
      req_auto_reply = 1;
      if (~mflags & MSG_WRITING) // If we're not processing this message for send
      {
        if (not(mflags & (MSG_WRITING|MSG_MINE|MSG_CHAT)) and conv) // If we're receiving this message from someone else, not in a chat
          purple_conv_im_send (PURPLE_CONV_IM(conv), "\x5\x3\x33Pong! [     ]");
        msg = "<font color = \"#00C000\"><i>Ping? [request]</i></color>";
      }
      else conv_pingclocks[recipient].pushtime();
    }
    else if ((!strcmp(cm,"\x5\x3\x33Pong! [     ]") or !strcmp(cm,"\5x5;\3x3;3Pong! [     ]")) and (mflags & MSG_WRITING)) // Writing a pong from somewhere
    {
      req_auto_reply = 1;
      map<string,timestack>::iterator ps = conv_pingclocks.find(recipient);
      if (ps == conv_pingclocks.end()) msg = "<font color = \"#00C000\"><i>Pong! [???]</i></color>";
      else
      msg = (mflags & MSG_MINE) ? "<font color = \"#00C000\"><i>Pong! [resps]</i></color>" :
      "<font color = \"#00C000\"><i>Pong! [" + tostring_time(ps->second.popwithreturn()) + "]</i></color>";
    }
    else if(cm[0] == 'j' and cm[1] == 's' and cm[2] == ':') //Handle "JS:" as a temporary hax
      if (conv != NULL)
      {
        set_receiving_window(conv,window_type);
        plus_evaluate_js_line(cm + 3);
      }
    g_free(cm);
  }
  
  if (!req_auto_reply) // If this isn't an automatic response to /ping or the like
  {
    puts("Handle BBCode");
    // Handle BBCode
    size_t pos; vector< opentaginfo > open_tags; int indx = 0;
    for (bbiter it = bbcode_tags.begin(); it != bbcode_tags.end(); it++)
      it->second->init(); // Initialize the map of bbcode tags.
    bbcode_tag::instance tag;
    for (pos = 0; pos < msg.length(); pos++) {
      if (bbcode_tag::at(msg.c_str(),pos,tag))
      {
        if (tag.unary) continue;
        if (tag.closing) {
          printf("Identified closing [/%s].\n",tag.name.c_str());
          for (int i = indx-1; i >= 0; i--)
            if (open_tags[i].name == tag.name) {
              printf("Found matching [%s]. Replacing...\n",tag.name.c_str());
              string content(msg,open_tags[i].tag_end_pos+1, pos-open_tags[i].tag_end_pos-1);
              string rep = tag.tag->get_replacement(conv,account,mflags,content,open_tags[i].arg,tag.arg);
              msg.replace(open_tags[i].tag_start_pos,pos + tag.length - open_tags[i].tag_start_pos,rep);
              if (i < indx-1)
                pos = open_tags[i].tag_start_pos;
              else
                pos += rep.length() - (pos + tag.length - open_tags[i].tag_start_pos) - 1;
              indx--;
              for (int j = i; j < indx; j++)
                open_tags[j] = open_tags[j+1];
              open_tags.pop_back();
              break;
            } else printf("The next open tag is [%s], not [%s]...\n",open_tags[i].name.c_str(),tag.name.c_str());
          continue;
        }
        printf("Identified opening [%s].\n",tag.name.c_str());
        indx++;
        open_tags.push_back(opentaginfo(tag.name,tag.arg,pos,pos+tag.length-1));
      }
    }
    for (pos = 0; pos < msg.length(); pos++) {
      if (bbcode_tag::at(msg.c_str(),pos,tag)) {
        if (!tag.unary) continue;
        string rep = tag.tag->get_unary_replacement(conv,account,mflags,tag.arg);
        msg.replace(pos,tag.length,rep);
      }
    }
  }
  
  g_free(*message);
  const char *src = msg.c_str();
  int skip_slash = (*src == '/' and (mflags & MSG_MINE));
  *message = g_new(char, msg.length() + !skip_slash); // Keep /me
  memcpy(*message, src + skip_slash, msg.length() + !skip_slash);
  return false;
}

#define wunused() ((void)mflags), ((void)window_type), ((void)recipient), ((void)message), ((void)conv), (void)account
struct macro_NP:    macro_basic { macro_NP():    macro_basic("NP")    {} string get_replacement(macroflags) { return get_my_name (account);  wunused(); } };
struct macro_MP:    macro_basic { macro_MP():    macro_basic("MP")    {} string get_replacement(macroflags) { return get_my_email(account);  wunused(); } };
struct macro_MYPSM: macro_basic { macro_MYPSM(): macro_basic("MYPSM") {} string get_replacement(macroflags) { return get_my_status(account); wunused(); } };
struct macro_N:     macro_basic { macro_N():     macro_basic("N")     {} string get_replacement(macroflags) { return buddy_get_uname (account, recipient); wunused(); } };
struct macro_NN:    macro_basic { macro_NN():    macro_basic("NN")    {} string get_replacement(macroflags) { return buddy_get_alias (account, recipient); wunused(); } }; //Return nick name
struct macro_ME:    macro_basic { macro_ME():    macro_basic("ME")    {} string get_replacement(macroflags) { return buddy_get_alias (account, recipient); wunused(); } }; //Return original sender
struct macro_PSM:   macro_basic { macro_PSM():   macro_basic("PSM")   {} string get_replacement(macroflags) { return buddy_get_status(account, recipient); wunused(); } };
struct macro_M:     macro_basic { macro_M():     macro_basic("M")     {} string get_replacement(macroflags) { return recipient;    wunused(); } };
struct macro_IP:    macro_basic { macro_IP():    macro_basic("IP")    {} string get_replacement(macroflags) { return get_my_ip();  wunused(); } };
struct macro_D:     macro_basic { macro_D():     macro_basic("D")     {} string get_replacement(macroflags) { return get_date_numeric(); wunused(); } };
struct macro_DD:    macro_basic { macro_DD():    macro_basic("DD")    {} string get_replacement(macroflags) { return get_weekday(); wunused(); } };
struct macro_DM:    macro_basic { macro_DM():    macro_basic("DM")    {} string get_replacement(macroflags) { return get_month();   wunused(); } };
struct macro_DY:    macro_basic { macro_DY():    macro_basic("DY")    {} string get_replacement(macroflags) { return get_year();    wunused(); } };
struct macro_T:     macro_basic { macro_T():     macro_basic("T")     {} string get_replacement(macroflags) { return get_time();    wunused(); } };
struct macro_TH:    macro_basic { macro_TH():    macro_basic("TH")    {} string get_replacement(macroflags) { return get_hour();    wunused(); } };
struct macro_TM:    macro_basic { macro_TM():    macro_basic("TM")    {} string get_replacement(macroflags) { return get_minute();  wunused(); } };
struct macro_TS:    macro_basic { macro_TS():    macro_basic("TS")    {} string get_replacement(macroflags) { return get_second();  wunused(); } };
