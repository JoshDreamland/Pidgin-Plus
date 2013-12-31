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

#define CMDFLAG_BASIC     (PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_CHAT | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS)
#define CMDFLAG_NOCHAT    (PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS)

struct pplus_command {
  const char* name;         ///< The name of this command, as the user will type it.
  const char* description; ///< The description of this command, as displayed when queried through /help.
  PurpleCmdFlag usage;    ///< The allowed usage of this command; probably either the CMDFLAG_BASIC or CMDFLAG_NOCHAT macro.
  PurpleCmdId pcid;      ///< The Purple command ID assigned to this command when it was registered.
  
  /// Perform this command, whatever it may do. Defaults are declared in the corresponding source.
  /// @return This function should return one of PURPLE_CMD_RET_{OK, FAILED, CONTINUE}
  virtual PurpleCmdRet perform(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *error, void *data);
  virtual ~pplus_command();
  
  pplus_command(const char* n, const char *d, PurpleCmdFlag u);
};

PurpleCmdRet execute_command(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar *error, void *data);
