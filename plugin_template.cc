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

#include "macros.h"
#include "commands.h"

/* we're adding this here and assigning it in plugin_load because we need
 * a valid plugin handle for our call to purple_notify_message() in the
 * plugin_action_test_cb() callback function */
PurplePlugin *pidgin_plus_plugin = NULL;

/* This function is the callback for the plugin action we added. All we're
 * doing here is displaying a message. When the user selects the plugin
 * action, this function is called. */
static void
plugin_action_test_cb (PurplePluginAction * action)
{
  
}




void sending_im_msg(PurpleAccount *account, const char *receiver,char **message)
{
  if (filter_outgoing(true,account,receiver,message))
  {
    g_free(*message);
    *message = NULL;
  }
}

static gboolean writing_im_msg(PurpleAccount *account, const char *who, char **message, PurpleConversation *conv, PurpleMessageFlags flags)
{
  return filter_outgoing(false,account,who,message,conv,flags);
}





/* we tell libpurple in the PurplePluginInfo struct to call this function to
 * get a list of plugin actions to use for the plugin.  This function gives
 * libpurple that list of actions. */
static GList *plugin_actions (PurplePlugin * plugin, gpointer context)
{
  GList *list = NULL;
  PurplePluginAction *action = NULL;
  
  action = purple_plugin_action_new ("Plugin Action Test", plugin_action_test_cb);
  list = g_list_append (list, action);
  
  return list;
}

static PurpleCmdRet slash_command_handler(PurpleConversation *conv, const gchar *cmd, gchar **args,	gchar *error, void *data)
{
  execute_command(conv,cmd,args,error,data);
  return PURPLE_CMD_RET_OK;
}

int plus_v8_end();
int plus_v8_init();
static gboolean plugin_load (PurplePlugin * plugin)
{
  pidgin_plus_plugin = plugin; /* assign this here so we have a valid handle later */
  purple_signal_connect(purple_conversations_get_handle(), "writing-im-msg", plugin, PURPLE_CALLBACK(writing_im_msg), NULL);
  purple_signal_connect(purple_conversations_get_handle(), "sending-im-msg", plugin, PURPLE_CALLBACK(sending_im_msg), NULL);
  
  for (int i = 0; i < NUM_MSGPLUS_COMMANDS; i++)
  {
    printf("%d of %d: 0x%8X and 0x%8X\r\n",i,NUM_MSGPLUS_COMMANDS,(unsigned)command_string_msgplus[i],(unsigned)command_description_msgplus[i]);
    COMMANDS_MSGPLUS[i] = purple_cmd_register(command_string_msgplus[i], "w", PURPLE_CMD_P_PLUGIN,(PurpleCmdFlag)(PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS),	
                         NULL, PURPLE_CMD_FUNC(slash_command_handler), command_description_msgplus[i], NULL);
  }
  plus_v8_init();
  return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin)
{
  for (int i=0; i<NUM_MSGPLUS_COMMANDS; i++)
    purple_cmd_unregister(COMMANDS_MSGPLUS[i]);
  plus_v8_end();
}


/* For specific notes on the meanings of each of these members, consult the C Plugin Howto
 * on the website. */
static PurplePluginInfo info = {
  PURPLE_PLUGIN_MAGIC,
  PURPLE_MAJOR_VERSION,
  PURPLE_MINOR_VERSION,
  PURPLE_PLUGIN_STANDARD,
  NULL, //PIDGIN_PLUGIN_TYPE
  0, NULL, PURPLE_PRIORITY_DEFAULT,

  (char*)"gtk-joshdreamland-pidginplus", (char*)"Pidgin Plus!", (char*)"0.0.0:0001",
  (char*)"Pidgin Plus!", (char*)"Pidgin Plus! Extension-- A plugin designed to bring the features of MSN Plus! to Pidgin.", 
  (char*)"Josh Ventura <JoshV@zoominternet.net>", (char*)"http://pidginplus.com/",

  plugin_load,  plugin_unload,  NULL, //Load, unload, destroy

  NULL,  NULL,  NULL, //PidginPluginUiInfo, PurplePluginLoaderInfo, PurplePluginUiInfo
  plugin_actions,
  
  NULL, NULL, NULL, NULL //Not 2.0.0
};


static void init_plugin (PurplePlugin *plugin)
{
  
}


extern "C"
{
  PURPLE_INIT_PLUGIN (hello_world, init_plugin, info)
}
