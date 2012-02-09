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
   @file commands.cc
   
   This file breaks down the large list of Plus! commands into a few sets,
   an array of command names, an array of help texts for Pidgin, an enum of
   their IDs (which each correspond to an item from the first two arrays),
   and finally a function containing a switch statement to execute the
   appropriate one on call.
   
   This mechanism makes it possible to submit the whole
   list of commands to Pidgin using a for loop instead
   of bloated, redundant code.
*/

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>


#include <string>
using namespace std;

#include "commands.h" //This
#include "../purple_frontend/purple_extension.h"  // Simplify
#include "../plugin_basics/plugin_events.h"      // Link to command event

extern PurplePlugin *pidgin_plus_plugin;
#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
#define chaterror(x) purple_conversation_write(conv, "", x, PURPLE_MESSAGE_NO_LOG, time(NULL))


#define CMDFLAG_BASIC     (PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS)
#define CMDFLAG_NOCHAT    (PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS)

/// Give us an easy system by which to identify our commands.
enum {
  cmd_all, cmd_addcontact, cmd_appearoffline, cmd_available, cmd_away, 
  cmd_block, cmd_blockgrp, cmd_brb, cmd_busy, cmd_close, cmd_emails, cmd_invite, 
  cmd_lock, cmd_lunch, cmd_msg,  cmd_noicon, cmd_online, cmd_onphone, cmd_page, 
  cmd_phone, cmd_ping, cmd_profile, cmd_sendfile,  cmd_sendmail, cmd_signout, 
  cmd_unblock, cmd_unblockgrp, cmd_video, cmd_voice,
  NUM_MSGPLUS_COMMANDS
};

/// Structure detailing a command.
struct command_info {
  const char* name;         ///< The name of this command, as the user will type it.
  const char* description; ///< The description of this command, as displayed when queried through /help.
  PurpleCmdFlag usage;    ///< The allowed usage of this command; probably either the CMDFLAG_BASIC or CMDFLAG_NOCHAT macro.
};

/// An array of command names.
command_info commands[NUM_MSGPLUS_COMMANDS] = {
  { "all",           "Send a message to contacts in all active chats",                       (PurpleCmdFlag)CMDFLAG_BASIC },
  { "addcontact",    "Add a new contact with given username/email (depending on protocol)",  (PurpleCmdFlag)CMDFLAG_BASIC },
  { "appearoffline", "Set status to invisible on supported protocols",                       (PurpleCmdFlag)CMDFLAG_BASIC },
  { "available",     "Set status to available",                                              (PurpleCmdFlag)CMDFLAG_BASIC },
  { "away",          "Set status to a supported variant of away",                            (PurpleCmdFlag)CMDFLAG_BASIC },
  { "block",         "Block the current buddy(ies) in the conversation, or by name/email",   (PurpleCmdFlag)CMDFLAG_BASIC },
  { "blockgrp",      "Block an entire group by name",                                        (PurpleCmdFlag)CMDFLAG_BASIC },
  { "brb",           "Set status to 'Be right back' or an available equivalent (Away)",      (PurpleCmdFlag)CMDFLAG_BASIC },
  { "busy",          "Set status to busy",                                                   (PurpleCmdFlag)CMDFLAG_BASIC },
  { "close",         "Close the current conversation",                                       (PurpleCmdFlag)CMDFLAG_BASIC },
  { "emails",        "I'm not sure what /emails does",                                       (PurpleCmdFlag)CMDFLAG_BASIC },
  { "invite",        "Invite a contact to this conversation",                                (PurpleCmdFlag)CMDFLAG_BASIC },
  { "lock",          "Lock pidgin, if the function is ever implemented",                     (PurpleCmdFlag)CMDFLAG_BASIC },
  { "lunch",         "Set status to 'Out to Lunch' on the old MSN protocol",                 (PurpleCmdFlag)CMDFLAG_BASIC },
  { "msg",           "Send a message to a contact or individual on the current protocol",    (PurpleCmdFlag)CMDFLAG_BASIC },
  { "noicon",        "Remove smilies from outgoing messages on MSN Plus!",                   (PurpleCmdFlag)CMDFLAG_BASIC },
  { "online",        "Set status to 'Available'",                                            (PurpleCmdFlag)CMDFLAG_BASIC },
  { "onphone",       "Set status to 'In a Call'",                                            (PurpleCmdFlag)CMDFLAG_BASIC },
  { "page",          "No idea.",                                                             (PurpleCmdFlag)CMDFLAG_BASIC },
  { "phone",         "Starts a phone call, I think",                                         (PurpleCmdFlag)CMDFLAG_BASIC },
  { "ping",          "Ping your buddy and record the response time, if he or she has Plus!", (PurpleCmdFlag)CMDFLAG_BASIC },
  { "profile",       "profile",                                                              (PurpleCmdFlag)CMDFLAG_BASIC },
  { "sendfile",      "Send a file to this buddy, if the protocol allows",                    (PurpleCmdFlag)CMDFLAG_BASIC },
  { "sendmail",      "Send an email to this buddy, if an address is available",              (PurpleCmdFlag)CMDFLAG_BASIC },
  { "signout",       "Sign out of Pidgin",                                                   (PurpleCmdFlag)CMDFLAG_BASIC },
  { "unblock",       "Unblock the current buddy(ies) in the conversation, or by name/email", (PurpleCmdFlag)CMDFLAG_BASIC },
  { "unblockgrp",    "Unblock an entire group by name",                                      (PurpleCmdFlag)CMDFLAG_BASIC },
  { "video",         "This will pretty much never be implemented!",                          (PurpleCmdFlag)CMDFLAG_BASIC },
  { "voice",         "This will basically never be implemented!",                            (PurpleCmdFlag)CMDFLAG_BASIC }
};

/// Array 
PurpleCmdId COMMANDS_MSGPLUS[NUM_MSGPLUS_COMMANDS];

/// Purple callback function to execute a command.
int execute_command(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *)
{
  string c = cmd;
  int jump_to_which = -1;
  
  for (int i = 0; i < NUM_MSGPLUS_COMMANDS; i++)
  if (c == commands[i].name) { jump_to_which = i; break; }
  
  string message = "";
  switch (jump_to_which)
  {
    case -1: break;
    case cmd_addcontact:
        chaterror("Unimplemented.");
      return false;
    case cmd_all: {
          string msg; for (gchar** i = args; *i; i++) msg += *i;
          for (GList* allconvs = purple_get_conversations(); allconvs != NULL; allconvs = allconvs->next)
            purple_conv_im_send (PURPLE_CONV_IM((PurpleConversation*)allconvs->data),msg.c_str()); 
          for (GList* allconvs = purple_get_chats(); allconvs != NULL; allconvs = allconvs->next)
            purple_conv_chat_send (PURPLE_CONV_CHAT((PurpleConversation*)allconvs->data),msg.c_str()); 
        }
      return true;
    case cmd_appearoffline:
        all_accounts_set_status(PURPLE_STATUS_INVISIBLE,"");
      return true;
    case cmd_available:
        all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
      return true;
    case cmd_away:
        all_accounts_set_status(PURPLE_STATUS_AWAY,"");
        all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
      return true;
    case cmd_block:
        if (!purple_privacy_deny_add(conv->account, conv->name, false))
          return 2;
      return true;
    case cmd_blockgrp:
        chaterror("Not implemented: Working with groups in pidgin? Yikes.");
      return false;
    case cmd_brb:
        all_accounts_set_status(PURPLE_STATUS_AWAY,"");
      return true;
    case cmd_busy:
        all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"");
      return true;
    case cmd_close:
        purple_conversation_destroy(conv);
      return true;
    case cmd_emails:
        chaterror("Unimplemented: Iterating things in pidgin? Yikes.");
      return false;
    case cmd_invite:
      return generic_protocol_invite_contact(conv);
    case cmd_lock:
        chaterror("Unimplemented: Pidgin lacks a lock mechanism, and GTK sucks.");
      return false;
    case cmd_lunch:
        all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
      return true;
    case cmd_msg:
        chaterror("Unimplemented: Dunno what this does.");
      return false;
    case cmd_noicon:
        //Prepend 0x08 to the outgoing message
      return true;
    case cmd_online:
        all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
      return true;
    case cmd_onphone:
        all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"On the phone");
      return true;
    case cmd_page:
        chaterror("What does this even do?");
      return false;
    case cmd_phone:
        chaterror("Not implemented. Are you KIDDING me?");
      return true;
    case cmd_ping:
        purple_conv_im_send(PURPLE_CONV_IM(conv), "Ping? [msgplus]");
      return true;
    case cmd_profile:
        
      return false;
    case cmd_sendfile:
        
      return false;
    case cmd_sendmail:
        
      return false;
    case cmd_signout:
        
      return false;
    case cmd_unblock:
        if (!purple_privacy_deny_remove(conv->account, conv->name, false))
          return 2;
      return true;
    case cmd_unblockgrp:
        
      return false;
    case cmd_video:
        
      return false;
    case cmd_voice:
        
      return false;
    default:
      chaterror("Unrecognized command... not handled in switch.");
      return false;
  }
  return false;
}

/// Register all built-in commands.
void plus_commands_register()
{
  for (int i = 0; i < NUM_MSGPLUS_COMMANDS; i++)
  {
    printf("--> Register /%s\n",commands[i].name);
    COMMANDS_MSGPLUS[i] = purple_cmd_register(
      commands[i].name, //
      "S",
      PURPLE_CMD_P_PLUGIN,
      commands[i].usage,	
      NULL,
      PURPLE_CMD_FUNC(slash_command_handler),
      commands[i].description,
      NULL
    );
  }
}

/// Unregister all built-in commands.
void plus_commands_free()
{
  for (int i=0; i<NUM_MSGPLUS_COMMANDS; i++)
    purple_cmd_unregister(COMMANDS_MSGPLUS[i]);
}
