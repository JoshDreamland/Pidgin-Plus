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
  @file  plugin_events.cc
  @brief File for handling event firing.
  
  This file makes calls to more complicated functions when a relevant event
  is invoked in Pidgin, such as when a message is sent or received. This file
  also contains code to register all the purple events with Pidgin so the plugin
  can work. Those two functions can be found at the bottom, and are called in
  plugin_template.cc.
*/

#include <notify.h>
#include <plugin.h>
#include <version.h>

#include <cmds.h>
#include <privacy.h>
#include <savedstatuses.h>
#include <conversation.h>

#include "../commands_and_macros/macros.h"
#include "../commands_and_macros/commands.h"
#include "../purple_frontend/purple_extension.h"

#include "plugin_events.h"

//=================================================================================================
//===- Handle simple events -======================================================================
//=================================================================================================

static void sending_im_msg(PurpleAccount *account, const char *receiver,char **message)
{
  if (filter_message(MSG_MINE,pct_im,account,receiver,message))
  {
    g_free(*message);
    *message = NULL;
  }
}

/// This callback is invoked upon actually printing a message to the window of a one-on-one IM conversation.
/// @param account  The account from which the message is being sent.
/// @param who      The name of the recipient of the message.
/// @param message  The contents of the message, as a pointer to a string. This is to allow the message to be modified.
/// @param conv     The PurpleConversation of the IM to which the message is being sent.
/// @param flags    The purple flags affiliated with this message. These are all but completely irrelevant to us.
static gboolean writing_im_msg(PurpleAccount *account, const char *who, char **message, PurpleConversation *conv, PurpleMessageFlags flags)
{
  return filter_message((flags&PURPLE_MESSAGE_SEND)? MSG_WRITING|MSG_MINE : MSG_WRITING,pct_im,account,who,message,conv);
}


static void sending_chat_msg(PurpleAccount *account, char **message, int id)
{
  PurpleConversation* c = purple_find_chat (purple_account_get_connection(account), id);
  PurpleConvChat* cc = purple_conversation_get_chat_data(c);
  GList* u = purple_conv_chat_get_users(cc);
  string usernames;
  for (; u != NULL; u = u->next)
  {
    usernames += purple_conv_chat_cb_get_name((PurpleConvChatBuddy*)u->data);
    usernames += ", ";
  }
  
  if (filter_message(MSG_MINE|MSG_CHAT,pct_chat,account,usernames.c_str(),message))
  {
    g_free(*message);
    *message = NULL;
  }
}

#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
static gboolean writing_chat_msg(PurpleAccount *account, const char *, char **message, PurpleConversation *conv, PurpleMessageFlags flags)
{
  PurpleConvChat* cc = purple_conversation_get_chat_data(conv);
  GList* u = purple_conv_chat_get_users(cc);
  string usernames;
  for (; u != NULL; u = u->next)
  {
    usernames += purple_conv_chat_cb_get_name((PurpleConvChatBuddy*)u->data);
    usernames += ", ";
  }
  return filter_message((flags&PURPLE_MESSAGE_SEND)? MSG_WRITING|MSG_CHAT|MSG_MINE : MSG_WRITING|MSG_CHAT,pct_chat,account,usernames.c_str(),message,conv);
}



/*
    Handle Plus!'s large array of commands
    AKA Slash commands: anything given by /command, such as /all, /ping, or /online.
    See commands.h / commands.cc
*/


/*
    Handle conveying this information to Pidgin
*/

void plus_events_connect_signals(PurplePlugin * plugin)
{
  purple_signal_connect(purple_conversations_get_handle(), "writing-im-msg",   plugin, PURPLE_CALLBACK(writing_im_msg), NULL);
  purple_signal_connect(purple_conversations_get_handle(), "sending-im-msg",   plugin, PURPLE_CALLBACK(sending_im_msg), NULL);
  purple_signal_connect(purple_conversations_get_handle(), "sending-chat-msg", plugin, PURPLE_CALLBACK(sending_chat_msg), NULL);
  purple_signal_connect(purple_conversations_get_handle(), "writing-chat-msg", plugin, PURPLE_CALLBACK(writing_chat_msg), NULL);
}
void plus_events_disconnect_signals(PurplePlugin * plugin)
{
  purple_signal_disconnect(purple_conversations_get_handle(), "writing-im-msg",   plugin, PURPLE_CALLBACK(writing_im_msg));
  purple_signal_disconnect(purple_conversations_get_handle(), "sending-im-msg",   plugin, PURPLE_CALLBACK(sending_im_msg));
  purple_signal_disconnect(purple_conversations_get_handle(), "sending-chat-msg", plugin, PURPLE_CALLBACK(sending_chat_msg));
  purple_signal_disconnect(purple_conversations_get_handle(), "writing-chat-msg", plugin, PURPLE_CALLBACK(writing_chat_msg));
}
