/**
 * @file   simplified_flags.h
 * @brief  Purple's message flag system is unhelpful. This provides an alternative.
 * 
 * @section License
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
**/

enum {
  MSG_MINE = 1,    ///< Set if the message was sent by us. Otherwise, it is from our partner.
  MSG_WRITING = 2, ///< Set if this message is to be written to the chat. Otherwise, it is being processed before send.
  MSG_CHAT = 4     ///< Set if the message originated in a chat window. Otherwise, it originated in an IM window.
};
