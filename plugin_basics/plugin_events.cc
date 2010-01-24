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
  @file plugin_events.cc
  @summary This file makes calls to more complicated functions
    when a relevant event is invoked in Pidgin, such as when a
    message is sent or received. This file also contains code
    to register all the purple events and Plus! functions with
    Pidgin so the plugin can work. Those two functions can be
    found at the bottom, and are called in plugin_template.cc.
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


/*
    Handle simple events
*/

void sending_im_msg(PurpleAccount *account, const char *receiver,char **message)
{
  if (filter_outgoing(true,pct_im,account,receiver,message))
  {
    g_free(*message);
    *message = NULL;
  }
}

static gboolean writing_im_msg(PurpleAccount *account, const char *who, char **message, PurpleConversation *conv, PurpleMessageFlags flags)
{
  return filter_outgoing(false,pct_im,account,who,message,conv,flags);
}


void sending_chat_msg(PurpleAccount *account, char **message, int id)
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
  
  if (filter_outgoing(true,pct_chat,account,usernames.c_str(),message))
  {
    g_free(*message);
    *message = NULL;
  }
}

#define msgbox(STRHERE) purple_notify_message (pidgin_plus_plugin, PURPLE_NOTIFY_MSG_INFO, "Info", STRHERE, NULL, NULL, NULL)
static gboolean writing_chat_msg(PurpleAccount *account, const char *who, char **message, PurpleConversation *conv, PurpleMessageFlags flags)
{
  PurpleConvChat* cc = purple_conversation_get_chat_data(conv);
  GList* u = purple_conv_chat_get_users(cc);
  string usernames;
  for (; u != NULL; u = u->next)
  {
    usernames += purple_conv_chat_cb_get_name((PurpleConvChatBuddy*)u->data);
    usernames += ", ";
  }
  return filter_outgoing(false,pct_chat,account,usernames.c_str(),message,conv,flags);
}




/*
    Handle Plus!'s large array of commands
*/

static PurpleCmdRet slash_command_handler(PurpleConversation *conv, const gchar *cmd, gchar **args,	gchar *error, void *data)
{
  execute_command(conv,cmd,args,error,data);
  return PURPLE_CMD_RET_OK;
}



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


void plus_commands_register()
{
  for (int i = 0; i < NUM_MSGPLUS_COMMANDS; i++)
  {
    //printf("%d of %d: 0x%8X and 0x%8X\r\n",i,NUM_MSGPLUS_COMMANDS,(unsigned)command_string_msgplus[i],(unsigned)command_description_msgplus[i]);
    COMMANDS_MSGPLUS[i] = purple_cmd_register(command_string_msgplus[i], "w", PURPLE_CMD_P_PLUGIN,(PurpleCmdFlag)(PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS),	
                         NULL, PURPLE_CMD_FUNC(slash_command_handler), command_description_msgplus[i], NULL);
  }
}
void plus_commands_free()
{
  for (int i=0; i<NUM_MSGPLUS_COMMANDS; i++)
    purple_cmd_unregister(COMMANDS_MSGPLUS[i]);
}
