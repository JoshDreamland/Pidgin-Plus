/**
 * @file plugin_template.cc
 * 
 * This file is almost entirely syntactical doodoo to make happy 
 * the connection between the plugin and pidgin. The goal is to have as
 * little original code in this file as possible so it can be discarded
 * as a waste heap. Comments were left to help the curious navigate.
 *
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
*/

//=============================================================================================
//==- Some includes ===========================================================================
//=============================================================================================

    #ifdef HAVE_CONFIG_H
    # include <config.h>
    #endif

    // config.h may define PURPLE_PLUGINS; protect the definition here so that we
    // don't get complaints about redefinition when it's not necessary.
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


//=============================================================================================
//==- This file's few actual dependencies -====================================================
//=============================================================================================

#include <notify.h>
#include <plugin.h>
#include <version.h>
#include <scripting/v8_implementation.h>
#include "plugin_events.h"
#include <commands_and_macros/bbcode_load.h>



//=============================================================================================
//==- This is where the real code is -=========================================================
//=============================================================================================

// we're adding this here and assigning it in plugin_load because we need
// a valid plugin handle for our call to purple_notify_message() in the
// plugin_action_test_cb() callback function */
PurplePlugin *pidgin_plus_plugin = NULL;

static void cb_pplus_prefs(PurplePluginAction *)
{
  // We presently don't have any preferences to show.
}

// We tell libpurple in the PurplePluginInfo struct to call this function to get a list of
// plugin actions to use for the plugin. This function gives libpurple that list of actions.
static GList *plugin_actions (PurplePlugin *, gpointer)
{
  // This is to be used when Plus! supports enough to justify it
  GList *list = NULL;
  PurplePluginAction *action = purple_plugin_action_new ("Pidgin Plus! Preferences", cb_pplus_prefs);
  list = g_list_append (list, action);
  return list;
}


/*
   Make calls to files that matter.
   I told you this file was a waste heap.
*/
static gboolean plugin_load (PurplePlugin * plugin)
{
  puts("Store handle"); fflush(stdout);
  pidgin_plus_plugin = plugin;   //Assign this here so we have a valid handle later
  puts("Connect signals");
  plus_events_connect_signals(plugin); //Tell Pidgin what events we'll be calling. See plugin_events.cc in this directory.
  puts("Register commands signals");
  plus_commands_register();      //Gain control of Plus!'s commands
  puts("Load BBCode tags");
  load_bbcode();
  
  puts("Start threads");
  //Implement threading for JS
  if (!g_thread_get_initialized())
    g_thread_init(NULL);
  
  puts("Start V8");
  //Initialize V8
  plus_v8_init();
  puts("DONE. Relinquish control to Pidgin.");
  return TRUE;
}

static gboolean plugin_unload(PurplePlugin *plugin)
{
  plus_events_disconnect_signals(plugin);
  plus_commands_free();
  plus_v8_end();
  return true;
}


// For specific notes on the meanings of each of these members, consult the C Plugin Howto
// on the website.
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

//This would do something if anything in this plugin could be loaded and unloaded without freeing global memory <_<"
static void init_plugin (PurplePlugin *) { }

//C++ gives the whole process gas. 
//Before you go making fun of C++, consider that V8 Operates in it.
extern "C"
{
  PURPLE_INIT_PLUGIN (hello_world, init_plugin, info)
}
