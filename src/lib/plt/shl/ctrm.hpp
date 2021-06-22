// ---------------------------------------------------------------------------
// - ctrm.hpp                                                                -
// - standard platform library - c terminal native functions                 -
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

#ifndef  AFNIX_CTRM_HPP
#define  AFNIX_CTRM_HPP

#ifndef  AFNIX_CCNF_HPP
#include "ccnf.hpp"
#endif

namespace afnix {
  /// This file contains the basic procedures used to model a terminal
  /// behavior. This model is based on accessing a capability database
  /// that reports what the terminal can do and how it does it.
  /// @author amaury darsch

  /// check if the stream id is a terminal
  /// @param sid the stream id
  bool c_istty (const int sid);

  /// get the current terminal attributes
  /// @param sid the terminal id
  /// @return an opaque data structure
  void* c_gtattr (const int sid);

  /// free the terminal data structure attributes
  /// @param ptr the terminal data structure
  void c_ftattr (void* ptr);

  /// set the terminal attribute
  /// @param sid the terminal id
  /// @param attr the terminal attributes 
  void c_stattr (const int sid, void* attr);

  /// set the terminal in non canonical mode - disable echo
  /// @param sid the terminal id
  /// @return true if the operation succeds
  bool c_stcanon (const int sid);

  /// allocate a new terminal structure
  /// @return an opaque terminal structure
  void* c_talloc (void);

  /// free a terminal structure
  /// @param term the terminal to free
  void c_tfree (void* term);

  /// return the terminal capabilities
  /// @param term the terminal structure
  /// @param iflag the input/output terminal mode
  char** c_tinfo (void* term, const bool iflag);

  /// get the terminal capabilities flag
  /// @param term the terminal structure
  bool* c_tbool (void* term);

  /// @return true if a terminal parameter is valid
  bool c_tpvld (char** tinfo, const long index, const bool mode);

  /// send an data parameter to the standard output
  /// @param sid the output terminal id
  /// @param tinfo the terminal capabilities
  /// @param index the capability index
  void c_tparm (const int sid, char** tinfo, const long index);

  /// turn on/off the terminal error mode
  /// @param sid the output terminal id
  /// @param tinfo the terminal capabilities
  /// @param mode set error mode if true
  void c_temode (const int sid, char** tinfo, const bool mode);

  /// get the number of columns for a given tty
  /// @param sid the stream id
  long c_getcols (const int sid);
}

// ---------------------------------------------------------------------------
// - key section                                                             -
// ---------------------------------------------------------------------------

// input terminal capability entries
#define ITERM_BACKSPACE     0        // backspace key
#define ITERM_DELETE        1        // del key
#define ITERM_ARROW_UP      2        // arrow up key
#define ITERM_ARROW_DOWN    3        // arrow down key
#define ITERM_ARROW_LEFT    4        // arrow left key
#define ITERM_ARROW_RIGHT   5        // arrow right key
#define ITERM_INSERT_KEY    6        // insert key
#define ITERM_STD_UP        7        // standard arrow up key
#define ITERM_STD_DOWN      8        // standard arrow down key
#define ITERM_STD_LEFT      9        // standard arrow left key
#define ITERM_STD_RIGHT    10        // standard arrow right key
#define ITERM_STD_DELETE   11        // standard delete key
#define ITERM_STD_INSERT   12        // standard insert key
#define ITERM_PARMS_MAX    13        // max number of inputs entries

// output terminal capability entries
#define OTERM_DELETE_CHAR   0        // delete character
#define OTERM_MOVE_LEFT     1        // move left one character
#define OTERM_MOVE_RIGHT    2        // move right one character
#define OTERM_MOVE_UP       3        // move left one character
#define OTERM_MOVE_DOWN     4        // move right one character
#define OTERM_MOVE_BOL      5        // move beginning of line
#define OTERM_INSERT_CHAR   6        // insert character
#define OTERM_IMODE_START   7        // enter insert mode
#define OTERM_IMODE_END     8        // exit insert mode
#define OTERM_SETFG_MODE    9        // set the foreground color
#define OTERM_RESET_COLOR  10        // reset color mode
#define OTERM_CLEAR_SCREEN 11        // clear screen and move cursor up
#define OTERM_PARMS_MAX    12        // max number of output entries

// terminal boolean capabilities
#define BTERM_AUTO_WRAP     1        // terminal can wrap
#define BTERM_PARMS_MAX     1        // max number of entries

// terminal default keys
#define XTERM_INSERT_KEY   "\033[2~" // insert key
#define XTERM_DELETE_KEY   "\033[3~" // delete key
#define XTERM_ARROW_UP     "\033[A"  // arrow up
#define XTERM_ARROW_DOWN   "\033[B"  // arrow down
#define XTERM_ARROW_RIGHT  "\033[C"  // arrow right
#define XTERM_ARROW_LEFT   "\033[D"  // arrow left

#endif
