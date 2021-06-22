// ---------------------------------------------------------------------------
// - Palette.cpp                                                             -
// - afnix:svg service - color palette class implementation                  -
// ---------------------------------------------------------------------------
// - This program is free software;  you can redistribute it  and/or  modify -
// - it provided that this copyright notice is kept intact.                  -
// -                                                                         -
// - This program  is  distributed in  the hope  that it will be useful, but -
// - without  any  warranty;  without  even   the   implied    warranty   of -
// - merchantability or fitness for a particular purpose.  In no event shall -
// - the copyright holder be liable for any  direct, indirect, incidental or -
// - special damages arising in any way out of the use of this software.     -
// ---------------------------------------------------------------------------
// - copyright (c) 1999-2021 amaury darsch                                   -
// ---------------------------------------------------------------------------

#include "Palette.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the color palette size
  static long  PLT_CLR_SIZE = 109;
  // the color palette table name
  static char const*const PLT_CLR_NAME[] = {
    "red",            "green",                "blue",           "gray",
    "brown",          "burlywood",            "cadetblue",      "chartreuse",
    "chocolate",      "coral",                "cornflowerblue", "cornsilk",
    "crimson",        "cyan",                 "darkblue",       "darkcyan",
    "darkgoldenrod",  "darkgray",             "darkgreen",      "darkkhaki",
    "darkmagenta",    "darkolivegreen",       "darkorange",     "darkorchid",
    "darkred",        "darksalmon",           "darkseagreen",   "darkslateblue",
    "darkslategray",  "darkturquoise",        "darkviolet",     "deeppink",
    "deepskyblue",    "dimgray",              "dodgerblue",     "firebrick",
    "floralwhite",    "forestgreen",          "gainsboro",      "ghostwhite",
    "gold",           "goldenrod",            "gray",           "greenyellow",
    "honeydew",       "hotpink",              "indianred",      "indigo",
    "ivory",          "khaki",                "lavender",       "lavenderblush",
    "lawngreen",      "lemonchiffon",         "lightblue",      "lightcoral",
    "lightcyan",      "lightgoldenrodyellow", "lightgreen",     "lightgrey",
    "lightpink",      "lightsalmon",          "lightseagreen",  "lightskyblue",
    "lightslategray", "lightsteelblue",       "lightyellow",    "limegreen",
    "linen",          "magenta",              "mediumblue",     "mediumorchid",
    "mediumpurple",   "midnightblue",         "mistyrose",      "moccasin",
    "oldlace",        "orange",               "orchid",         "peachpuff",
    "peru",           "pink",                 "plum",           "purple",
    "rosybrown",      "royalblue",            "salmon",         "sandybrown",
    "seagreen",       "sienna",               "skyblue",        "slateblue",
    "steelblue",      "tan",                  "thistle",        "tomato",
    "violet",         "wheat",                "whitesmoke",     "yellow",
    "aliceblue",      "antiquewhite",         "aquamarine",     "azure",
    "beige",          "bisque",               "blanchedalmond", "blueviolet",
    "yellowgreen"
  };
  // the color palette table code
  static char const*const PLT_CLR_CODE[] = {
    "#ff0000",        "#00ff00",              "#0000ff",        "#777777",
    "#a52a2a",        "#deb887",              "#5f9ea0",        "#7fff00",
    "#d2691e",        "#ff7f50",              "#6495ed",        "#fff8dc",
    "#dc143c",        "#00ffff",              "#00008b",        "#008b8b",
    "#b8860b",        "#a9a9a9",              "#006400",        "#bdb76b",
    "#8b008b",        "#556b2f",              "#ff8c00",        "#9932cc",
    "#8b0000",        "#e9967a",              "#8fbc8f",        "#483d8b",
    "#2f4f4f",        "#00ced1",              "#9400d3",        "#ff1493",
    "#00bfff",        "#696969",              "#1e90ff",        "#b22222",
    "#fffaf0",        "#228b22",              "#dcdcdc",        "#f8f8ff",
    "#ffd700",        "#daa520",              "#808080",        "#adff2f",
    "#f0fff0",        "#ff69b4",              "#cd5c5c",        "#4b0082",
    "#fffff0",        "#f0e68c",              "#e6e6fa",        "#fff0f5",
    "#7cfc00",        "#fffacd",              "#add8e6",        "#f08080",
    "#e0ffff",        "#fafad2",              "#90ee90",        "#d3d3d3",
    "#ffb6c1",        "#ffa07a",              "#20b2aa",        "#87cefa",
    "#778899",        "#b0c4de",              "#ffffe0",        "#32cd32",
    "#faf0e6",        "#ff00ff",              "#0000cd",        "#ba55d3",
    "#9370db",        "#191970",              "#ffe4e1",        "#ffe4b5",
    "#fdf5e6",        "#ffa500",              "#da70d6",        "#ffdab9",
    "#cd853f",        "#ffc0cb",              "#dda0dd",        "#800080",
    "#bc8f8f",        "#4169e1",              "#fa8072",        "#f4a460",
    "#2e8b57",        "#a0522d",              "#87ceeb",        "#6a5acd",
    "#4682b4",        "#d2b48c",              " #d8bfd8",       "#ff6347",
    "#ee82ee",        "#f5deb3",              "#f5f5f5",        "#ffff00",
    "#f0f8ff",        "#faebd7",              "#7fffd4",        "#f0ffff",
    "#f5f5dc",        "#ffe4c4",              "#ffebcd",        "#8a2be2",
    "#9acd32"
  };

  // -------------------------------------------------------------------------
  // - public section                                                       -
  // -------------------------------------------------------------------------

  // get a color name by index
  String Palette::getname (const long index) {
    // check for valid range
    if ((index < 0) || (index >= PLT_CLR_SIZE)) {
      throw Exception ("palette-error", "invalid color index");
    }
    String result = PLT_CLR_NAME[index];
    return result;
  }

  // get a color rgb by index

  String Palette::getrgb (const long index) {
    // check for valid range
    if ((index < 0) || (index >= PLT_CLR_SIZE)) {
      throw Exception ("palette-error", "invalid color index");
    }
    String result = PLT_CLR_CODE[index];
    return result;
  }
}


