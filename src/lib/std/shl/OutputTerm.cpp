// ---------------------------------------------------------------------------
// - OutputTerm.cpp                                                          -
// - standard object library - terminal output stream class implementation   -
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
#include "Error.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "OutputTerm.hpp"
#include "csio.hpp"
#include "ctrm.hpp"
#include "cstr.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default output stream. By default the output stream is
  // mapped to the default standard output

  OutputTerm::OutputTerm (void)  {
    // set the terminal structure
    p_term = c_talloc ();
    // set the tarnscoder and stream modes
    settmod (Encoding::getstm ());
    setemod (Encoding::getsem ());
    // initialize the terminal
    d_sid    = c_stdout ();
    p_tinfo  = c_tinfo (p_term, false);
    d_insert = true;
  }

  // create a new default output stream according to the mode.
  
  OutputTerm::OutputTerm (t_mode mode)  {
    // set the terminal structure
    p_term = c_talloc ();
    // set the tarnscoder and stream modes
    settmod (Encoding::getstm ());
    setemod (Encoding::getsem ());
    // set from mode
    switch (mode) {
    case OutputTerm::OUTPUT:
      d_sid    = c_stdout ();
      break;
    case OutputTerm::ERROR:
      d_sid    = c_stderr ();
      break;
    }
    p_tinfo  = c_tinfo (p_term, false);
    d_insert = true;
  }

  // destroy this output terminal

  OutputTerm::~OutputTerm (void) {
    c_tfree (p_term);
    if (p_tinfo != nullptr) {
      for (long i = 0; i < OTERM_PARMS_MAX; i++) delete [] p_tinfo[i];
      delete [] p_tinfo;
    }
  }

  // return the class name

  String OutputTerm::repr (void) const {
    return "OutputTerm";
  }

  // return the stream descriptor

  int OutputTerm::getsid (void) const {
    rdlock ();
    try {
      int result = d_sid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }    
  }

  // return true if we have a tty

  bool OutputTerm::istty (void) const {
    rdlock ();
    try {
      bool result = c_istty (d_sid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write one character to the output stream
  
  long OutputTerm::write (const char value) {
    wrlock ();
    try {
      long code = c_write (d_sid, &value, 1);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character string to the output stream
  
  long OutputTerm::write (const char* data) {
    wrlock ();
    try {
      long size = c_strlen (data);
      if (size == 0) {
	unlock ();
	return 0;
      }
      // write the string
      long code = c_write (d_sid, data, size);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a string with the terminal in error mode

  void OutputTerm::error (const String& value) {
    wrlock ();
    try {
      c_temode      (d_sid, p_tinfo, true);
      OutputStream::write (value);
      c_temode      (d_sid, p_tinfo, false);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the number of columns

  long OutputTerm::getcols (void)const {
    rdlock ();
    try {
      long result = istty () ? c_getcols (d_sid) : 0;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set or reset the terminal error mode

  void OutputTerm::temode (const bool mode) {
    wrlock ();
    try {
      c_temode (d_sid, p_tinfo, mode);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear the screen
  
  bool OutputTerm::clear (void) {
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_CLEAR_SCREEN, true) == false) {
	unlock ();
	return false;
      }
      c_tparm (d_sid,p_tinfo, OTERM_CLEAR_SCREEN);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // insert a character based on the mode

  void OutputTerm::insert (const char c) {
    wrlock ();
    try {
      // check that we have terminal capabilities
      if (p_tinfo == nullptr) {
	write (c);
	unlock ();
	return;
      }
      // check for mode and perform insert
      if (d_insert == true) {
	if (c_tpvld (p_tinfo, OTERM_INSERT_CHAR, true) == true) {
	  c_tparm (d_sid, p_tinfo, OTERM_INSERT_CHAR);
	  write (c);
	} else {
	  c_tparm (d_sid, p_tinfo, OTERM_IMODE_START);
	  write (c);
	  c_tparm (d_sid, p_tinfo, OTERM_IMODE_END);
	}
      } else {
	write (c);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // insert a character buffer

  void OutputTerm::insert (const char* s) {
    // check for size
    long size = Ascii::strlen (s);
    if (size == 0) return;
    wrlock ();
    try {
      // check that we have terminal capabilities
      if (p_tinfo == nullptr) {
	write (s);
	unlock ();
	return;
      }
      // check for mode and perform insert
      if (d_insert == true) {
	if (c_tpvld (p_tinfo, OTERM_INSERT_CHAR, true) == true) {
	  c_tparm (d_sid, p_tinfo, OTERM_INSERT_CHAR);
	  write (s);
	} else {
	  c_tparm (d_sid, p_tinfo, OTERM_IMODE_START);
	  write (s);
	  c_tparm (d_sid, p_tinfo, OTERM_IMODE_END);
	}
      } else {
	write (s);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // insert an unicode character
  
  void OutputTerm::insert (const t_quad value) {
    wrlock ();
    try {
      // check for byte mode
      if (d_emod == Encoding::EMOD_BYTE) {
	// encode from the trancoder
	insert (decode (value));
	unlock ();
	return;
      }
      // check for utf-8 mode
      if (d_emod == Encoding::EMOD_UTF8) {
	// encode in a character buffer
	char* cbuf = Unicode::encode (d_emod, value);
	// insert the buffer
	insert (cbuf);
	// clean and unlock 
	delete [] cbuf;
	unlock ();
	return;
      }
      throw Exception ("insert-error", "invalid unicode character to insert");
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // insert a unicode buffer

  void OutputTerm::insert (const t_quad* s) {
    // check for size
    long size = Unicode::strlen (s);
    if (size == 0) return;
    // insert buffer
    wrlock ();
    try {
      for (long i = 0; i < size; i++) insert (s[i]);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // insert a string

  void OutputTerm::insert (const String& s) {
    wrlock ();
    try {
      t_quad* buf = s.toquad ();
      insert (buf);
      delete [] buf;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // delete one character in position
  
  bool OutputTerm::chdel (void) {
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_DELETE_CHAR, true) == false) {
	unlock ();
	return false;
      }
      c_tparm (d_sid, p_tinfo, OTERM_DELETE_CHAR);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move to the beginning of line by a number
  
  bool OutputTerm::mvbol (void) {
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_MOVE_BOL, true) == false) {
	unlock ();
	return false;
      }
      c_tparm (d_sid, p_tinfo, OTERM_MOVE_BOL);
      unlock ();
      return true;;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move to the left by a number
  
  bool OutputTerm::movel (const long num) {
    if (num <= 0) return false;
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_MOVE_LEFT, true) == false) {
	unlock ();
	return false;
      }
      for (long i = 0; i < num; i++) {
	c_tparm (d_sid, p_tinfo, OTERM_MOVE_LEFT);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move to the right by a number
  
  bool OutputTerm::mover (const long num) {
    if (num <= 0) return false;
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_MOVE_RIGHT, true) == false) {
	unlock ();
	return false;
      }
      for (long i = 0; i < num; i++) {
	c_tparm (d_sid, p_tinfo, OTERM_MOVE_RIGHT);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move up by a number
  
  bool OutputTerm::moveu (const long num) {
    if (num <= 0) return false;
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_MOVE_UP, true) == false) {
	unlock ();
	return false;
      }
      for (long i = 0; i < num; i++) {
	c_tparm (d_sid, p_tinfo, OTERM_MOVE_UP);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // move down by a number
  
  bool OutputTerm::moved (const long num) {
    if (num <= 0) return false;
    wrlock ();
    try {
      if (c_tpvld (p_tinfo, OTERM_MOVE_DOWN, true) == false) {
	unlock ();
	return false;
      }
      for (long i = 0; i < num; i++) {
	c_tparm (d_sid,p_tinfo, OTERM_MOVE_DOWN);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new output term in a generic way

  Object* OutputTerm::mkout (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 0) 
      throw Exception ("argument-error", 
		       "invalid arguments with with output term"); 
    return new OutputTerm (OutputTerm::OUTPUT);
  }

  // create a new error term in a generic way

  Object* OutputTerm::mkerr (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 0) 
      throw Exception ("argument-error", 
		       "invalid arguments with with error term"); 
    return new OutputTerm (OutputTerm::ERROR);
  }

  // return true if the given quark is defined

  bool OutputTerm::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      bool result = hflg ? OutputStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this output term with a set of arguments and a quark

  Object* OutputTerm::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    return OutputStream::apply (zobj, nset, quark, argv);
  }
}
