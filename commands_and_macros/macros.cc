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

bool filter_outgoing(bool me_sending, int window_type, PurpleAccount *account, const char *receiver,char **message,PurpleConversation *conv, PurpleMessageFlags flags)
{
  string msg = *message;
  string rs = receiver;
  
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
          bool r=0;
          string rw="";
          
          if (c == "NP")
            r=1, rw = get_my_name(account);
          else if (c == "MP")
            r=1, rw = get_my_email(account);
          else if (c == "IP")
            r=1, rw = get_my_ip();
          else if (c == "MYPSM")
            r=1, rw = get_my_status(account);
          else if (c == "N")
            r=1, rw = buddy_get_uname(account,rs);
          else if (c == "NN") //Return nick name
            r=1, rw = buddy_get_alias(account,rs);
          else if (c == "ME") //Return original sender
            r=1, rw = buddy_get_alias(account,rs);
          else if (c == "PSM")
            r=1, rw = buddy_get_status(account,rs);
          else if (c == "M")
            r=1, rw = rs;
          else if (c == "D")
            r=1, rw = get_date_numeric();
          else if (c == "DD")
            r=1, rw = get_weekday();
          else if (c == "DM")
            r=1, rw = get_month();
          else if (c == "DY")
            r=1, rw = get_year();
          else if (c == "T")
            r=1, rw = get_time();
          else if (c == "TH")
            r=1, rw = get_hour();
          else if (c == "TM")
            r=1, rw = get_minute();
          else if (c == "TS")
            r=1, rw = get_second();
          
          if (r)
          {
            msg.replace(pstart, p-pstart+1,rw);
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
      if (!me_sending)
      {
        if ((flags & PURPLE_MESSAGE_RECV) and conv != NULL)
          purple_conv_im_send (PURPLE_CONV_IM(conv), "\x5\x3\x33Pong! [     ]");
        msg = "<font color = \"#00C000\"><i>Ping? [request]</i></color>";
      }
      else conv_pingclocks[receiver].pushtime();
    }
    else if ((!strcmp(cm,"\x5\x3\x33Pong! [     ]") or !strcmp(cm,"\5x5;\3x3;3Pong! [     ]")) and !me_sending) //Got a pong that isn't being sent
    {
      req_auto_reply = 1;
      map<string,timestack>::iterator ps = conv_pingclocks.find(receiver);
      if (ps == conv_pingclocks.end()) msg = "<font color = \"#00C000\"><i>Pong! [???]</i></color>";
      else
      msg = (flags & PURPLE_MESSAGE_RECV) ?
      "<font color = \"#00C000\"><i>Pong! [" + tostring_time(ps->second.popwithreturn()) + "]</i></color>" : "<font color = \"#00C000\"><i>Pong! [resps]</i></color>";
    }
    else if(cm[0] == 'j' and cm[1] == 's' and cm[2] == ':') //Handle "JS:" as a temporary hax
      if (conv != NULL)
      {
        set_receiving_window(conv,window_type);
        plus_evaluate_js_line(cm + 3);
      }
    g_free(cm);
  }
  
  if (!req_auto_reply && !me_sending) // If this isn't an automatic response to /ping or the like, and it's not being sent to the network
  {
    puts("Handle BBCode");
    // Handle BBCode
    size_t pos, indx = 0; vector< opentaginfo > open_tags;
    for (bbiter it = bbcode_tags.begin(); it != bbcode_tags.end(); it++)
      it->second->init(); // Initialize the map of bbcode tags.
    bbcode_tag::instance tag;
    for (pos = 0; pos < msg.length(); pos++) {
      if (bbcode_tag::at(msg.c_str(),pos,tag))
      {
        if (tag.unary) continue;
        if (tag.closing) {
          printf("Identified closing [/%s].\n",tag.name.c_str());
          for (int i = indx; i >= 0; i--)
          if (open_tags[i].name == tag.name) {
            printf("Found matching [%s]. Replacing...\n",tag.name.c_str());
            string content(msg,open_tags[i].tag_end_pos+1, pos-open_tags[i].tag_end_pos-1);
            string rep = bbcode_tags[open_tags[i].name]->get_replacement(conv,account,content,open_tags[i].arg);
            msg.replace(open_tags[i].tag_start_pos,pos + tag.length - open_tags[i].tag_start_pos,rep);
            break;
          }
          continue;
        }
        printf("Identified opening [%s].\n",tag.name.c_str());
        open_tags.push_back(opentaginfo(tag.name,tag.arg,pos,pos+tag.length-1));
      }
    }
  }
  
  g_free(*message);
  *message = g_new(char,msg.length() + 1);
  memcpy(*message, msg.c_str(), msg.length() + 1);
  return false;
}
