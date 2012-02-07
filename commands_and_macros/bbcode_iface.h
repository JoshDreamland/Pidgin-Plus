/**
 * @file   bbcode_iface.h
 * @brief  Header declaring an interface for adding BBCode tags.
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
 *
**/

#include <map>
#include <string>
#include <account.h>
#include <conversation.h>
using std::string;
using std::map;


struct bbcode_tag {
  static map<string,double> vars; ///< Real-valued variables needed by the functions during BB parsing.
  static map<string,string> svars; ///< String-valued variables needed by the functions during BB parsing.
  bool unary; ///< True if this tag stands alone. Ie, true if this tag does not have a corresponding end tag.
  virtual void init() = 0; ///< Called before parse begins to allow resetting variables
  
  /** Called when the tag is encountered for tags with a start and end.
      @param conv     The conversation in which the tag is being handled.
      @param account  The account from which the tag is being handled, for differentiating behavior by protocol.
      @param content  The text enclosed between the start and end tag.
      @param arg      The argument specified to the tag by means of [tagname=arg], or an empty string if no argument was given.
      @return Returns the string with which to replace \p content.
  **/
  virtual string get_replacement(PurpleConversation *conv, PurpleAccount *account, string content, string arg = "");
  /** Called when the tag is encountered for tags with a start and end.
      @param conv     The conversation in which the tag is being handled.
      @param account  The account from which the tag is being handled, for differentiating behavior by protocol.
      @param arg      The argument specified to the tag by means of [tagname=arg], or an empty string if no argument was given.
      @return Returns the string with which to replace \p content.
  **/
  virtual string get_unary_replacement(PurpleConversation *conv, PurpleAccount *account, string arg = "");
  
  /// Structure detailing a tag as used in 
  struct instance {
    string name; ///< The name of the tag of which this is an instance.
    string arg; ///< Any argument given to this tag.
    bbcode_tag *tag; ///< The actual tag we represent.
    short length; ///< the length of this tag.
    bool closing; ///< True if this is the end tag.
    bool unary; ///< True if the tag represented stands alone.
  };
  static instance def_inst; ///< A default structure for writing values to, in case the caller is uninterested.
  
  /** Return whether there is a BBCode tag at the given position in a given string, and if so, populates info
      about it in the given bbcode_tag::instance.
      @param msg  The message string in which to check for a BBCode tag.
      @param pos  The position at which to conduct the check.
      @param inst The position at which to conduct the check.
      @return Returns true if there was a valid tag at the given position in the given string.
  **/
  static bool at(const char* msg, int pos, instance &tag_inst = def_inst); 
  
  bbcode_tag(string name, bool unary = false); ///< Add self to the list of bbtags.
  virtual ~bbcode_tag(); ///< Virtual destructor.
};

typedef map<string, bbcode_tag*> bbmap; ///< Map type for storing supported BBCode tags.
typedef bbmap::iterator bbiter; ///< Iterator type for bbmap.
extern bbmap bbcode_tags; ///< Map of all BBCode tags by tag name.
