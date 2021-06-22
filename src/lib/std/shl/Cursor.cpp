// ---------------------------------------------------------------------------
// - Cursor.cpp                                                              -
// - standard object library - character buffer class implementation         -
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

#include "Ascii.hpp"
#include "Cursor.hpp"
#include "Unicode.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new cursor

  Cursor::Cursor (void) {
    d_cursor = 0;
    d_insert = true;
    d_prtlen = 0;
  }

  // create a new cursor with a predefined size

  Cursor::Cursor (const long size) : Strbuf (size) {
    d_cursor = 0;
    d_insert = true;
    d_prtlen = 0;
  }

  // create a new cursor and initialize it with a c string

  Cursor::Cursor (const char* value) : Strbuf (value) {
    d_cursor = ncclen ();
    d_insert = true;
    d_prtlen = 0;
  }

  // create a new cursor and initialize it with a string

  Cursor::Cursor (const String& value) : Strbuf (value) {
    d_cursor = ncclen ();
    d_insert = true;
    d_prtlen = 0;
  }
  
  // return the class name

  String Cursor::repr (void) const {
    return "Cursor";
  }

  // reset this cursor
  
  void Cursor::reset (void) {
    wrlock ();
    try {
      Strbuf::reset ();
      d_cursor = 0;
      d_prompt = "";
      d_prtlen = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a unicode character in this cursor
  
  void Cursor::add (const t_quad value) {
    wrlock ();
    try {
      long length = ncclen ();
      // if we are at the end - we simply add the character and move the cursor
      if (d_cursor == length) {
	Strbuf::add (value);
	d_cursor = ncclen ();
	unlock ();
	return;
      }
      // if we are in insert mode - we need to shift the buffer
      // prior the insertion else put the character in place
      if (d_insert == true) {
	Strbuf::add (value, d_cursor);
      } else {
	Strbuf::put (value, d_cursor);
      }
      d_cursor++;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this cursor but do not change the prompt
  
  void Cursor::clear (void) {
    wrlock ();
    try {
      Strbuf::reset ();
      d_cursor = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the relative cursor position
  
  long Cursor::getrel (void) const {
    rdlock ();
    try {
      long result = d_cursor;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the absolute cursor position
  
  long Cursor::getabs (void) const {
    rdlock ();
    try {
      long result = d_cursor + d_prtlen;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the remaining distance

  long Cursor::getrem (void) const {
    rdlock ();
    try {
      long result = ncclen () - d_cursor;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the cursor is at bol

  bool Cursor::isbol (void) const {
    rdlock ();
    try {
      bool result = (d_cursor == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the cursor is at eol

  bool Cursor::iseol (void) const {
    rdlock ();
    try {
      bool result = (d_cursor == ncclen ());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the cursor is in the last position
  
  bool Cursor::islst (void) const {
    rdlock ();
    try {
      // get cursor length
      long length = ncclen ();
      // must be at the last character
      bool result = (length == 0) ? false : (d_cursor == (length - 1));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the character can be deleted at cursor position

  bool Cursor::isdel (void) const {
    rdlock ();
    try {
      long length = ncclen ();
      if ((length == 0) || (d_cursor == length)) {
	unlock ();
	return false;
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the cursor prompt 

  void Cursor::setprt (const String& prt) {
    wrlock ();
    try {
      reset ();
      d_prompt = prt;
      d_prtlen = prt.length ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the cursor prompt

  String Cursor::getprt (void) const {
    rdlock ();
    try {
      String result = d_prompt;
      unlock();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the prompt length

  long Cursor::prtlen (void) const {
    rdlock ();
    try {
      long result = d_prtlen;
      unlock();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove a character at the cursor (delete mode)
  
  bool Cursor::chdel (void) {
    wrlock ();
    try {
      // do nothing if empty or cursor is at end
      long length = ncclen ();
      if ((length == 0) || (d_cursor == length)) {
	unlock ();
	return false;
      }
      // delete the character
      Strbuf::chdel (d_cursor);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if the cursor is moved one character left
  
  bool Cursor::movel (void) {
    wrlock ();
    try {
      // do nothing if empty or at the beginning
      long length = ncclen ();
      if ((length == 0) || (d_cursor == 0)) {
	unlock ();
	return false;
      }
      // update cursor position
      d_cursor--;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the cursor is moved one character right
  
  bool Cursor::mover (void) {
    wrlock ();
    try {
      // do nothing if empty or at the end
      long length = ncclen ();
      if ((length == 0) || (d_cursor == length)) {
	unlock ();
	return false;
      }
      // update cursor position
      d_cursor++;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move the cursor to the beginning of the buffer
  
  long Cursor::setcb (void) {
    wrlock ();
    try {
      long result = d_cursor;
      d_cursor    = 0;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // move the cursor to the end of the buffer

  long Cursor::setce (void) {
    wrlock ();
    try {
      long length = ncclen ();
      long result = length - d_cursor;
      d_cursor    = length;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the insert mode flag
  
  void Cursor::setim (const bool mode) {
    wrlock ();
    try {
      d_insert = mode;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the full cursor line

  String Cursor::toline (void) const {
    rdlock ();
    try {
      String result = d_prompt + tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a substring from the cursor to the end

  String Cursor::substr (void) const {
    rdlock ();
    try {
      String result = Strbuf::substr (d_cursor);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a substring from the next cursor to the end

  String Cursor::delstr (void) const {
    rdlock ();
    try {
      String result = Strbuf::substr (d_cursor+1);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
