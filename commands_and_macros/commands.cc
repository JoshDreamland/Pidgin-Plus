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

#include <map>
#include <string>
using namespace std;

#include "commands.h" //This
#include "../purple_frontend/purple_extension.h"  // Simplify
#include "../plugin_basics/plugin_events.h"      // Link to command event

extern PurplePlugin *pidgin_plus_plugin;
#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
#define chaterror(x) purple_conversation_write(conv, "", x, PURPLE_MESSAGE_NO_LOG, time(NULL))

/// Declares a command class from a name, C string description, and set of purple command flags.
#define declare_command(name, description, mode) \
struct cmd_##name: public pplus_command {\
  cmd_##name (): pplus_command(#name, (description), (PurpleCmdFlag)(mode)) {}            \
  PurpleCmdRet perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *); \
}

declare_command(all,         "Send a message to contacts in all active chats",                       CMDFLAG_BASIC);
declare_command(addcontact,  "Add a new contact with given username/email (depending on protocol)",  CMDFLAG_BASIC);
declare_command(appoffline,  "Set status to invisible on supported protocols",                       CMDFLAG_BASIC);
declare_command(available,   "Set status to available",                                              CMDFLAG_BASIC);
declare_command(away,        "Set status to a supported variant of away",                            CMDFLAG_BASIC);
declare_command(block,       "Block the current buddy(ies) in the conversation, or by name/email",   CMDFLAG_BASIC);
declare_command(blockgrp,    "Block an entire group by name",                                        CMDFLAG_BASIC);
declare_command(brb,         "Set status to 'Be right back' or an available equivalent (Away)",      CMDFLAG_BASIC);
declare_command(busy,        "Set status to busy",                                                   CMDFLAG_BASIC);
declare_command(close,       "Close the current conversation",                                       CMDFLAG_BASIC);
declare_command(emails,      "I'm not sure what /emails does",                                       CMDFLAG_BASIC);
declare_command(invite,      "Invite a contact to this conversation",                                CMDFLAG_BASIC);
declare_command(lock,        "Lock pidgin, if the function is ever implemented",                     CMDFLAG_BASIC);
declare_command(lunch,       "Set status to 'Out to Lunch' on the old MSN protocol",                 CMDFLAG_BASIC);
declare_command(msg,         "Send a message to a contact or individual on the current protocol",    CMDFLAG_BASIC);
declare_command(noicon,      "Remove smilies from outgoing messages on MSN Plus!",                   CMDFLAG_BASIC);
declare_command(online,      "Set status to 'Available'",                                            CMDFLAG_BASIC);
declare_command(onphone,     "Set status to 'In a Call'",                                            CMDFLAG_BASIC);
declare_command(page,        "No idea.",                                                             CMDFLAG_BASIC);
declare_command(phone,       "Starts a phone call, I think",                                         CMDFLAG_BASIC);
declare_command(ping,        "Ping your buddy and record the response time, if he or she has Plus!", CMDFLAG_NOCHAT);
declare_command(profile,     "profile",                                                              CMDFLAG_BASIC);
declare_command(sendfile,    "Send a file to this buddy, if the protocol allows",                    CMDFLAG_BASIC);
declare_command(sendmail,    "Send an email to this buddy, if an address is available",              CMDFLAG_BASIC);
declare_command(signout,     "Sign out of Pidgin",                                                   CMDFLAG_BASIC);
declare_command(unblock,     "Unblock the current buddy(ies) in the conversation, or by name/email", CMDFLAG_BASIC);
declare_command(unblockgrp,  "Unblock an entire group by name",                                      CMDFLAG_BASIC);
declare_command(video,       "This will pretty much never be implemented!",                          CMDFLAG_BASIC);
declare_command(voice,       "This will basically never be implemented!",                            CMDFLAG_BASIC);

PurpleCmdRet cmd_all        :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv;
  string msg; for (gchar** i = args; *i; i++) msg += *i;
  for (GList* allconvs = purple_get_conversations(); allconvs != NULL; allconvs = allconvs->next)
    purple_conv_im_send (PURPLE_CONV_IM((PurpleConversation*)allconvs->data),msg.c_str()); 
  for (GList* allconvs = purple_get_chats(); allconvs != NULL; allconvs = allconvs->next)
    purple_conv_chat_send (PURPLE_CONV_CHAT((PurpleConversation*)allconvs->data),msg.c_str()); 
  return PURPLE_CMD_RET_OK;
}

PurpleCmdRet cmd_addcontact :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Unimplemented.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_appoffline :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_INVISIBLE,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_available  :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_away       :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_AWAY,"");
  all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_block      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  if (!purple_privacy_deny_add(conv->account, conv->name, false)) {
    chaterror("Plus! - Failed to block buddy.");
    return PURPLE_CMD_RET_FAILED;
  }
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_blockgrp   :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented: Working with groups in pidgin? Yikes.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_brb        :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_AWAY,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_busy       :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_close      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  purple_conversation_destroy(conv);
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_emails     :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Unimplemented: Iterating things in pidgin? Yikes.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_invite     :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  if (generic_protocol_invite_contact(conv))
    return PURPLE_CMD_RET_OK;
  chaterror("Plus! failed to invite buddy! (Not implemented?)");
  return PURPLE_CMD_RET_FAILED;
}
PurpleCmdRet cmd_lock       :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Unimplemented: Pidgin lacks a lock mechanism, and GTK sucks.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_lunch      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_EXTENDED_AWAY,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_msg        :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Unimplemented: Dunno what this does in MSG Plus!");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_noicon     :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  // TODO: Prepend 0x08 to the outgoing message
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_online     :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_AVAILABLE,"");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_onphone    :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)conv, (void)args;
  all_accounts_set_status(PURPLE_STATUS_UNAVAILABLE,"On the phone");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_page       :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - What does this even do?");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_phone      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented. May be implemented soon using PRPL's voice support.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_ping       :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  purple_conv_im_send(PURPLE_CONV_IM(conv), "Ping? [msgplus]");
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_profile    :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented. Hover your cursor on the user's icon.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_sendfile   :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_sendmail   :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_signout    :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_unblock    :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  if (!purple_privacy_deny_remove(conv->account, conv->name, false)) {
    chaterror("Plus! - Failed to unblock buddy.");
    return PURPLE_CMD_RET_FAILED;
  }
  return PURPLE_CMD_RET_OK;
}
PurpleCmdRet cmd_unblockgrp :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Plus! - Not implemented: Working with groups in pidgin? Yikes.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_video      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Not implemented. Unlikely to be implemented.");
  return PURPLE_CMD_RET_CONTINUE;
}
PurpleCmdRet cmd_voice      :: perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *, void *) {
  (void)cmd, (void)args;
  chaterror("Not implemented. May be implemented soon using PRPL's voice support.");
  return PURPLE_CMD_RET_CONTINUE;
}

/// Map type for storing commands
typedef map<string, pplus_command*> rc_map;
typedef rc_map::iterator   rc_iter;
typedef rc_map::value_type rc_pair;

/// A map of all commands we've registered
static  rc_map registered_commands;


pplus_command::pplus_command(const char* n, const char *d, PurpleCmdFlag u): name(n), description(d), usage(u) {
  pair<rc_iter, bool> ins = registered_commands.insert(rc_pair(name, this));
  if (!ins.second) {
    delete ins.first->second;
    ins.first->second = this;
  }
  else {
    purple_cmd_register(
      name, "S", PURPLE_CMD_P_PLUGIN,
      usage, NULL, PURPLE_CMD_FUNC(execute_command),
      description,
      NULL
    );
  }
}

pplus_command::~pplus_command() {
  rc_iter rit = registered_commands.find(name);
  if (rit != registered_commands.end() && rit->second == this)
    rit->second = NULL;
}

PurpleCmdRet pplus_command::perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *uu1, void *uu2) { return PURPLE_CMD_RET_FAILED; }

/// Purple callback function to execute a command.
PurpleCmdRet execute_command(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *uu1, void *uu2)
{
  string c = cmd;
  rc_iter cmdp = registered_commands.find(c);
  if (cmdp != registered_commands.end()  && cmdp->second)
    return cmdp->second->perform(conv, cmd, args, uu1, uu2);
  chaterror("Command not registered...");
  return PURPLE_CMD_RET_FAILED;
}

/// Register all built-in commands.
void plus_commands_register()
{
  new cmd_all;
  new cmd_addcontact;
  new cmd_appoffline;
  new cmd_available;
  new cmd_away;
  new cmd_block;
  new cmd_blockgrp;
  new cmd_brb;
  new cmd_busy;
  new cmd_close;
  new cmd_emails;
  new cmd_invite;
  new cmd_lock;
  new cmd_lunch;
  new cmd_msg;
  new cmd_noicon;
  new cmd_online;
  new cmd_onphone;
  new cmd_page;
  new cmd_phone;
  new cmd_ping;
  new cmd_profile;
  new cmd_sendfile;
  new cmd_sendmail;
  new cmd_signout;
  new cmd_unblock;
  new cmd_unblockgrp;
  new cmd_video;
  new cmd_voice;
}

/// Unregister all built-in commands.
void plus_commands_free()
{
  for (rc_iter it = registered_commands.begin(); it != registered_commands.end(); ++it)
    delete it->second;
  registered_commands.clear();
}
