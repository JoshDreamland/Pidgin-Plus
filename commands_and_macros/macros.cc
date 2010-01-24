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
   @file macros.cc
   @summary In Pidgin Plus!, "macro" refers to any segment
     of message that will be replaced by more text later.
     These include (!N), which is replaced with the name
     (if available) of the other contact(s) in the chat.
     Because text replacement accounts for the majority of
     the reason we bother having events at this point,
     filter_outgoing is also defined in this file.
 */

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>


extern PurplePlugin *pidgin_plus_plugin;
#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
#define chaterror(x) purple_conversation_write(conv, "", x, PURPLE_MESSAGE_NO_LOG, time(NULL))

#include <string.h>

#include <map>
#include <string>
using namespace std;

#include "../gtk_etc_frontend/date_time.h"        //Fetch date/time info
#include "../gtk_etc_frontend/gtk_timer_struct.h" //Ping timing
#include "../purple_frontend/purple_extension.h"  //Simplify

#include "../basics/basics.h"

bool command_interpret(string m,unsigned int p,PurpleAccount *account)
{ 
  unsigned int cs = p;
  for (p++; is_letterd(m[p]); p++);
  string c = m.substr(cs+1,p-cs-1);
  
  while (m[p] == ' ') p++; //Move to end of spaces following /away... but keep tabs and newlines
  
  if (c == "all")
  {
    m.erase(cs,p-cs);
    GList* conv = purple_get_conversations();
    for (; conv != NULL; conv = conv->next)
      purple_conv_im_send (PURPLE_CONV_IM((PurpleConversation*)conv->data),m.c_str());
    return true;
  }
  
  msgbox(("Unknown function "+c+".").c_str());
  return true;
}

map<string,timestack> conv_pingclocks;

int d=0;
const char * hexa = "0123456789ABCDEF";

extern int plus_evaluate_js_line(const char* line);
bool filter_outgoing(bool me_sending, int window_type, PurpleAccount *account, const char *receiver,char **message,PurpleConversation *conv = NULL, PurpleMessageFlags flags = PURPLE_MESSAGE_SEND)
{
  unsigned int f;
  string m = *message;
  string rs = receiver;
  
  for (unsigned int p = 0; p < m.length(); p++)
  {
    while (m[p] == '<')
      while (m[p] and m[p++] != '>') 
        if (m[p]=='"') while (m[++p] and m[p] != '"') if (m[p]=='\\') p++;
        else if (m[p]=='\'') while (m[++p] and m[p] != '\'') if (m[p]=='\\') p++;
    if (m[p] == '(' and m[p+1] == '!')
    {
      const unsigned int pstart = p++;
      while (is_letter(m[++p]));
      if (m[p] == ')')
      {
        string c = m.substr(pstart+2,p-pstart-2);
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
          else if (c == "NN")
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
            m.replace(pstart, p-pstart+1,rw);
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
  while (m[p] == '<')
    while (m[p] and m[p++] != '>') 
      if (m[p]=='"') while (m[++p] and m[p] != '"') if (m[p]=='\\') p++;
      else if (m[p]=='\'') while (m[++p] and m[p] != '\'') if (m[p]=='\\') p++;
  
  bool req_auto_reply = 0;
  
  if (m[p] == '/')
  {
    if (m[p+1] == '/')
        m.erase(p,1);
    else if (me_sending)
    {
      if (command_interpret(m,p,account))
        return true;
    }
  }
  else 
  {
    char* cm = purple_markup_strip_html(m.c_str());
    if (!strcmp(cm,"Ping? [msgplus]")) //Someone's sending a ping request
    {
      req_auto_reply = 1;
      if (!me_sending)
      {
        if ((flags & PURPLE_MESSAGE_RECV) and conv != NULL)
          purple_conv_im_send (PURPLE_CONV_IM(conv), "\x5\x3\x33Pong! [     ]");
        m = "<font color = \"#00C000\"><i>Ping? [request]</i></color>";
      }
      else conv_pingclocks[receiver].pushtime();
    }
    else if ((!strcmp(cm,"\x5\x3\x33Pong! [     ]") or !strcmp(cm,"\5x5;\3x3;3Pong! [     ]")) and !me_sending) //Got a pong that isn't being sent
    {
      req_auto_reply = 1;
      map<string,timestack>::iterator ps = conv_pingclocks.find(receiver);
      if (ps == conv_pingclocks.end()) m = "<font color = \"#00C000\"><i>Pong! [???]</i></color>";
      else
      m = (flags & PURPLE_MESSAGE_RECV) ?
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
  
  if (!req_auto_reply)
  {
    if (!me_sending) //This is local, being written to our chat window
    {
      bool bold=0,ital=0,undr=0;
      int lbold=-1, lital=-1, lundr=-1;
      for (unsigned int i=0; i < m.length(); i++)
      {
        if (m[i] == 0x02)
            m.replace(i,m.substr(i+1,3) == "x2;" ? 4:1, (bold = !bold) ? "<b>":"</b>");
        else if (m[i] == 0x05)
            m.replace(i,m.substr(i+1,3) == "x5;" ? 4:1, (ital = !ital) ? "<i>":"</i>");
        else if (m[i] == 0x1f)
            m.replace(i,m.substr(i+1,4) == "x1f;" ? 5:1, (undr = !undr) ? "<u>":"</u>");
        else if (i < m.length() - 3)
        {
          if (lpos(m.substr(i,3),1) == "[b]")
            lbold = i;
          else if (lbold != -1 and lpos(m.substr(i,4),2) == "[/b]")
          {
            m.replace(lbold, 3, bold ? "" : "<b>"), bold=1;
            m.replace(i, 4, !bold ? "":"</b>"), bold=0;
            lbold = -1;
          }
          else if (lpos(m.substr(i,3),1) == "[i]")
            lital = i;
          else if (lital != -1 and lpos(m.substr(i,4),2) == "[/i]")
          {
            m.replace(lital, 3, ital ? "" : "<i>"), ital=1;
            m.replace(i, 4, !ital ? "":"</i>"), ital=0;
            lital = -1;
          }
          else if (lpos(m.substr(i,3),1) == "[u]")
            lundr = i;
          else if (lundr != -1 and lpos(m.substr(i,4),2) == "[/u]")
          {
            m.replace(lundr, 3, undr ? "" : "<u>"), undr=1;
            m.replace(i, 4, !undr ? "":"</u>"), undr=0;
            lundr = -1;
          }
        }
      }
    }
  }
  
  g_free(*message);
  *message = g_new(char,m.length() + 1);
  memcpy(*message, m.c_str(), m.length() + 1);
  return false;
}
