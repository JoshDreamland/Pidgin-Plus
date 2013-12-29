/**
 * @file   codes.cc
 * @brief  Source filling in special unicode strings.
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

#include "codes.h"

const char* pplus_unicode_doublestrike    = "��Թℂ��Լ��Ծℍ��Ձ��Ճ��ĕ��ęℚℝ��Ջ��Ս��Տ��Ĥ��Փ��Օ��՗��ՙ��՛��՝��՟��ա��գ��ե��է��թ��ի";
const char* pplus_unicode_fraktur         = "��խ��կ��ձ��ճ��յ��շ��չ��ջ��ս��տ��ց��փ��օ��և��։��֋��֍��֏��֑��֓��֕��֗��֙��֛��֝��֟";
const char* pplus_unicode_sansserif       = "��֡��֣��֥��֧��֩��֫��֭��֯��ֱ��ֳ��ֵ��ַ��ֹ��ֻ��ֽ��ֿ��ׁ��׃��ׅ��ׇ��׉��׋��׍��׏��ב��ד";
const char* pplus_unicode_sansbold        = "��ו��ח��י��כ��ם��ן��ס��ף��ץ��ק��ש��׫��׭��ׯ��ױ��׳��׵��׷��׹��׻��׽��׿��؁��؃��؅��؇";
const char* pplus_unicode_sansitalic      = "��؉��؋��؍��؏��ؑ��ؓ��ؕ��ؗ��ؙ��؛��؝��؟��ء��أ��إ��ا��ة��ث��ح��د��ر��س��ص��ط��ع��ػ";
const char* pplus_unicode_sansbolditalic  = "��ؽ��ؿ��ف��ك��م��ه��ى��ً��ٍ��ُ��ّ��ٓ��ٕ��ٗ��ٙ��ٛ��ٝ��ٟ��١��٣��٥��٧��٩��٫��٭��ٯ";
const char* pplus_unicode_monospace       = "��ٱ��ٳ��ٵ��ٷ��ٹ��ٻ��ٽ��ٿ��ځ��ڃ��څ��ڇ��ډ��ڋ��ڍ��ڏ��ڑ��ړ��ڕ��ڗ��ڙ��ڛ��ڝ��ڟ��ڡ��ڣ";
const char* pplus_unicode_script          = "��ӑ��ӓ��ӕ��ӗ��ә��ӛ��ӝ��ӟ��ӡ��ӣ��ӥ��ӧ��ө��ӫ��ӭ��ӯ��ӱ��ӳ��ӵ��ӷ��ӹ��ӻ��ӽ��ӿ��ԁ��ԃ";
const char* pplus_unicode_serifbold       = "��Ё��Ѓ��Ѕ��Ї��Љ��Ћ��Ѝ��Џ��Б��Г��Е��З��Й��Л��Н��П��С��У��Х��Ч��Щ��Ы��Э��Я��б��г";
const char* pplus_unicode_serifitalic     = "��е��з��й��л��н��п��с��у��х��ч��щ��ы��э��я��ё��ѓ��Ď��ї��љ��ћ��ѝ��џ��ѡ��ѣ��ѥ��ѧ";
const char* pplus_unicode_serifbolditalic = "��ѩ��ѫ��ѭ��ѯ��ѱ��ѳ��ѵ��ѷ��ѹ��ѻ��ѽ��ѿ��ҁ��҃��҅��҇��҉��ҋ��ҍ��ҏ��ґ��ғ��ҕ��җ��ҙ��қ";
const char* pplus_unicode_braille         = "⠁⠃⠉⠙⠑⠋⠛⠓⠊⠚⠅⠇⠍⠝⠕⠏⠟⠗⠎⠞⠥⠧⠺⠭⠽⠵⠁⠃⠉⠙⠑⠋⠛⠓⠊⠚⠅⠇⠍⠝⠕⠏⠟⠗⠎⠞⠥⠧⠺⠭⠽⠵";
const char* pplus_unicode_circled         = "ⒶⒷⒸⒹⒺⒻⒼⒽⒾⒿⓀⓁⓂⓃⓄⓅⓆⓇⓈⓉⓊⓋⓌⓍⓎⓏⓐⓑⓒⓓⓔⓕⓖⓗⓘⓙⓚⓛⓜⓝⓞⓟⓠⓡⓢⓣⓤⓥⓦⓧⓨⓩ";
const char* pplus_smilies = "��؂��؄��؆��؈��؊��،��؎��ؐ��ؓ��ؖ��ؚ��؝��ؠ��آ��ؤ��ب��ت��ح��ر��س��ض��ظ��غ��ؼ��ؾ��ـ";

const string msgplus_bold = "\x02";
const string msgplus_italic = "\x05";
const string msgplus_underline = "\x1f";

#define MISSING 0
int msgplus_colors[128] = {
  /*00: */ 0xffffff, /*01: */ 0x000000, /*02: */ 0x00007f, /*03: */ 0x009300, /*04: */ 0xff0000, 
  /*05: */ 0x7f0000, /*06: */ 0x9c009c, /*07: */ 0xfc7f00, /*08: */ 0xffff00, /*09: */ 0x00fc00,
  /*10: */ 0x009393, /*11: */ 0x00ffff, /*12: */ 0x2020fc, /*13: */ 0xff00ff, /*14: */ 0x7f7f7f, 
  /*15: */ 0xd2d2d2, /*16: */ 0xe7e6e4, /*17: */ MISSING,  /*18: */ 0xffdea4, /*19: */ 0xffaeb9,
  /*20: */ 0xffa8ff, /*21: */ 0xb4b4fc, /*22: */ 0xbafbe5, /*23: */ 0xc1ffa3, /*24: */ 0xfafda2,
  /*26: */ MISSING,  /*27: */ MISSING,  /*27: */ 0xf9c152, /*28: */ 0xff6d66, /*29: */ 0xff62ff,
  /*30: */ 0x6c6cff, /*31: */ 0x68ffc3, /*32: */ 0x8eff67, /*33: */ 0xf9ff57, /*34: */ MISSING,
  /*35: */ MISSING,  /*36: */ 0xffa022, /*37: */ 0xf92611, /*38: */ 0xff20ff, /*39: */ 0x202bff,
  /*40: */ 0x1effa5, /*41: */ 0x60f913, /*42: */ MISSING,  /*43: */ MISSING,  /*44: */ MISSING,
  /*45: */ 0xd98812, /*46: */ 0xeb0505, /*47: */ 0xde00de, /*48: */ 0x0000d3, /*49: */ 0x03cc88, 
  /*50: */ 0x59d80d, /*51: */ 0xd4c804, /*52: */ MISSING,  /*49: */ MISSING,  /*54: */ MISSING,
  /*55: */ 0x9b0008, /*56: */ MISSING,  /*57: */ 0x01038c, /*58: */ 0x01885f, /*59: */ MISSING,
  /*60: */ MISSING,  /*61: */ 0x473400, /*62: */ 0x4d0000, /*63: */ 0x5f0162, /*64: */ 0x000047,
  /*65: */ 0x06502f, /*66: */ 0x1c5300, /*67: */ 0x544d05,
  
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
