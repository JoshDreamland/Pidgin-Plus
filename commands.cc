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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* config.h may define PURPLE_PLUGINS; protect the definition here so that we
 * don't get complaints about redefinition when it's not necessary. */
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include <glib.h>

/* This will prevent compiler errors in some instances and is better explained in the
 * how-to documents on the wiki */
#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>


#include <string>
using namespace std;

#include "macros.h"
#include "commands.h"
#include "purple_extension.h"

extern PurplePlugin *pidgin_plus_plugin;
#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
#define chaterror(x) purple_conversation_write(conv, "", x, PURPLE_MESSAGE_NO_LOG, time(NULL))




const char* command_string_msgplus[] = {
  "addcontact","appearoffline","available","away",
  "block","blockgrp","brb","busy","close", "emails","invite",
  "lock","lunch","msg", "noicon","online","onphone","page",
  "phone", "ping","profile","sendfile","sendmail","signout",
  "unblock","unblockgrp","video","voice"
};
#define CMDFLAG_BASIC PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS
#define CMDFLAG_NOCHAT PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS

const int NUM_MSGPLUS_COMMANDS = sizeof(command_string_msgplus) / sizeof(char*);

const char* command_description_msgplus[] = {
  "Add a new contact with given username/email (depending on protocol)","Set status to invisible on supported protocols","Set status to available","Set status to a supported variant of away",
  "Block the current user(s) in the conversation, or by name/email","Block an entire group by name","Set status to 'Be right back' or an available equivalent","Set status to busy","Close the current conversation",
  "I'm not sure what /emails does","Invite a contact to this conversation","Lock pidgin, if the function is ever implemented","Set status to 'Out to Lunch' on old MSN protocol","No idea what /msg does",
  "Remove smilies from outgoing messages on MSN Plus!","Set status to 'Available'","Set status to 'In a Call'","No idea.","Starts a phone call, I think",
  "profile","sendfile","sendmail","signout",
  "unblock","unblockgrp","video","voice"
};

enum {
  cmd_addcontact, cmd_appearoffline, cmd_available, cmd_away, 
  cmd_block, cmd_blockgrp, cmd_brb, cmd_busy, cmd_close, cmd_emails, cmd_invite, 
  cmd_lock, cmd_lunch, cmd_msg,  cmd_noicon, cmd_online, cmd_onphone, cmd_page, 
  cmd_phone, cmd_ping, cmd_profile, cmd_sendfile,  cmd_sendmail, cmd_signout, 
  cmd_unblock, cmd_unblockgrp, cmd_video, cmd_voice 
};



PurpleCmdId COMMANDS_MSGPLUS[NUM_MSGPLUS_COMMANDS];

bool execute_command(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *error, void *data)
{
  string c = cmd;
  int jump_to_which = -1;
  
  for (int i = 0; i < NUM_MSGPLUS_COMMANDS; i++)
  if (c == command_string_msgplus[i]) { jump_to_which = i; break; }
  
  string message = "";
  switch (jump_to_which)
  {
    case -1: break;
    case cmd_addcontact:
        chaterror("Unimplemented.");
      break;
    case cmd_appearoffline:
        all_accounts_set_status(PURPLE_STATUS_INVISIBLE,"");
      break;
    case cmd_available:
        all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
      break;
    case cmd_away:
        all_accounts_set_status(PURPLE_STATUS_AWAY,"");
        all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
      break;
    case cmd_block:
        if (!purple_privacy_deny_add(conv->account, conv->name, false))
          chaterror("Failed to block user. Sorry.");
      break;
    case cmd_blockgrp:
        chaterror("Not implemented: Groups in pidgin? Yikes.");
      break;
    case cmd_brb:
        all_accounts_set_status(PURPLE_STATUS_AWAY,"");
      break;
    case cmd_busy:
        all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"");
      break;
    case cmd_close:
        //pidgin_conv_window_destroy(conv->win);
        chaterror("Not implemented: Conv has no tie to Window");
      break;
    case cmd_emails:
        chaterror("Unimplemented: Iterating things in pidgin? Yikes.");
      break;
    case cmd_invite:
        chaterror("Unimplemented: Chats in pidgin? Impossible. Seriously, like two protocols support it. And only one's built-in. Forget it.");
      break;
    case cmd_lock:
        chaterror("Unimplemented: Pidgin lacks a lock function, and GTK sucks.");
      break;
    case cmd_lunch:
        all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
      break;
    case cmd_msg:
        chaterror("Unimplemented: Dunno what this does.");
      break;
    case cmd_noicon:
        
      break;
    case cmd_online:
        all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
      break;
    case cmd_onphone:
        all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"On the phone");
      break;
    case cmd_page:
        
      break;
    case cmd_phone:
        chaterror("Not implemented. Are you KIDDING me?");
      break;
    case cmd_ping:
        purple_conv_im_send(PURPLE_CONV_IM(conv), "Ping? [msgplus]");
      break;
    case cmd_profile:
        
      break;
    case cmd_sendfile:
        
      break;
    case cmd_sendmail:
        
      break;
    case cmd_signout:
        
      break;
    case cmd_unblock:
        if (!purple_privacy_deny_remove(conv->account, conv->name, false))
          chaterror("Failed to unblock user. Sorry.");
      break;
    case cmd_unblockgrp:
        
      break;
    case cmd_video:
        
      break;
    case cmd_voice:
        
      break;
  }
}
