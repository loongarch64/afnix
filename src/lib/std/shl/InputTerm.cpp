// ---------------------------------------------------------------------------
// - InputTerm.cpp                                                           -
// - standard object library - terminal input stream class implementation    -
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

#include "Error.hpp"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Boolean.hpp"
#include "InputTerm.hpp"
#include "QuarkZone.hpp"
#include "csio.hpp"
#include "ctrm.hpp"
#include "cstr.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this function computes the maximum size of the temporary buffer we must
  // allocate when reading wide characters and special functions
  static long get_mtlen (char** tinfo) {
    if (!tinfo) return 0;
    long max = 0;
    for (long i = 0; i < ITERM_PARMS_MAX; i++) {
      long len = c_strlen (tinfo[i]);
      max = (max < len) ? len : max;
    }
    return max;
  }
  
  // this function check if we have a match between the buffer and the term
  // info sequence. it returns the number of matches
  static long match_tinfo (char** tinfo, char* buffer, long index, 
			   long* match) {
    if (!tinfo) return 0;
    long count = 0;
    for (long i = 0; i < ITERM_PARMS_MAX; i++) {
      if (c_strncmp (tinfo[i], buffer, index) == true) {
	count++;
	*match = i;
      }
    }
    return count;
  }

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return true if we have a wide character
    
  bool InputTerm::iswide (const t_quad w) {
    if ((w & 0x80000000) == 0x80000000) return true;
    return false;
  }

  // return true if we have a backspace character
  
  bool InputTerm::isbs (const t_quad w) {
    if (w == InputTerm::WIDE_BS)    return true;
    if (w == InputTerm::WIDE_STDBS) return true;
    return false;
  }

  // return true if we have a arrow up character
  
  bool InputTerm::isup (const t_quad w) {
    if (w == InputTerm::WIDE_UP)    return true;
    if (w == InputTerm::WIDE_STDUP) return true;
    return false;
  }
  
  // return true if we have a arrow down character
  
  bool InputTerm::isdo (const t_quad w) {
    if (w == InputTerm::WIDE_DO)    return true;
    if (w == InputTerm::WIDE_STDDO) return true;
    return false;
  }
  
  // return true if we have a arrow left character
  
  bool InputTerm::isle (const t_quad w) {
    if (w == InputTerm::WIDE_LE)    return true;
    if (w == InputTerm::WIDE_STDLE) return true;
    return false;
  }
    
  // return true if we have a arrow right character
  
  bool InputTerm::isri (const t_quad w) {
    if (w == InputTerm::WIDE_RI)    return true;
    if (w == InputTerm::WIDE_STDRI) return true;
    return false;
  }
  
  // return true if we have a printable character
  
  bool InputTerm::ispr (const t_quad w) {
    // accept eos/eol
    if (w == InputTerm::WIDE_STDNL) return true;
    if (w == InputTerm::WIDE_STDEF) return true;
    // reject wide characters
    if (InputTerm::iswide (w) == true) return false;
    // úse the unicode validation rule
    return Unicode::isterm (w);
  }

  // return true if we have a delete character

  bool InputTerm::isdel (const t_quad w) {
    if (w == InputTerm::WIDE_DEL)   return true;
    if (w == InputTerm::WIDE_STDDE) return true;
    return false;
  }
  
  // return true if we have an insert character

  bool InputTerm::isins (const t_quad w) {
    if (w == InputTerm::WIDE_STDIM) return true;
    if (w == InputTerm::WIDE_INSCH) return true;
    if (w == InputTerm::WIDE_STDIN) return true;
    return false;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default input stream. By default the input stream is
  // mapped to the default standard input
  
  InputTerm::InputTerm (void)  {
    // set the transcoder and stream modes
    settmod (Encoding::getstm ());
    setemod (Encoding::getsem ());
    // initialize the terminal
    d_sid   = c_stdin ();
    d_eos   = false;
    p_term  = c_talloc (); 
    p_attr  = nullptr;
    p_tinfo = c_tinfo (p_term, true);
    d_mtlen = get_mtlen (p_tinfo);
    d_ieos  = false;
    d_meos  = eolc;
  }
  
  // destroy this terminal class
  
  InputTerm::~InputTerm (void) {
    c_stattr (d_sid, p_attr);
    c_ftattr (p_attr);
    c_tfree  (p_term);
    if (p_tinfo != nullptr) {
      for (long i = 0; i < ITERM_PARMS_MAX; i++) delete [] p_tinfo[i];
      delete [] p_tinfo;
    }
  }

  // return the class name

  String InputTerm::repr (void) const {
    return "InputTerm";
  }

  // return the terminal descriptor

  int InputTerm::getsid (void) const {
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

  // read the next available character

  char InputTerm::read (void) {
    wrlock ();
    try {
      // check for the pushback buffer
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // check if we are at the end of file
      if (d_eos == true) {
	unlock ();
	return eosc;
      }
      // read the next character on the stream
      char byte = nilc;
      long code = c_read (d_sid, &byte,1);
      if (code < 0L) throw Error ("read-error", c_errmsg (code), code);
      // check for ctrl-d against ignore flag - remap to eolc
      if ((byte == eosc) && (d_ieos == true)) {
	char result = d_meos;
	unlock ();
	return result;
      }
      // check for eos
      if ((code == 0) || (byte == eosc)) {
	d_eos = true;
	unlock ();
	return eosc;
      }
      unlock ();
      return byte;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a wide character 

  t_quad InputTerm::wread (void) {
    // local allocation
    long match = 0;
    long count = 0;
    long index = 0;
    char buffer[256];
    // make sure we have allocated enough character for the buffer
    // thanks to Reggie Seagraves for this one
    if (d_mtlen+1 > 256) {
      throw Exception ("internal-error", "wide char buffer is not big enough");
    }
    // read the first character
    wrlock ();
    try {
      // try to read an ascii character
      t_quad data = getu ();
      if (Unicode::isascii (data) == false) {
	unlock ();
	return data;
      }
      // here we have an ascii character - go for the sequence
      buffer[index]   = data;
      buffer[index+1] = nilc;
      // try to match it
      if ((count = match_tinfo (p_tinfo, buffer, 1, &match)) == 0) {
	t_quad result = buffer[0];
	unlock ();
	return result;
      }
      // may be we have a match
      if (count == 1) {
	t_quad result = 0x80000000 | (t_quad) match;
	unlock ();
	return result;
      }
      // now we have a match but it is not complete - let's continue
      // we install a timeout for the read - if we timeout we pushback the 
      // buffer sequence if not we continue to match
      while (++index <= d_mtlen) {
	count = 0;
	if (c_rdwait (d_sid, 500) == false) break;
	buffer[index]   = read ();
	buffer[index+1] = nilc;
	count = match_tinfo (p_tinfo, buffer, index + 1, &match);
	if ((count == 0) || 
	    ((count == 1) && c_strcmp (p_tinfo[match],buffer))) break;
      }      
      // at this stage, we either have a match, or we timed-out. If we timed
      // out count is null and index indicates the amount of characters to 
      // pushback if count is 1, this is the match, else we pushback the 
      // buffer and return the first character
      if (count != 1) {
	pushback (buffer);
	t_quad result = 0x000000FFU & read ();
	unlock ();
	return result;
      }
      t_quad result = 0x80000000U + (t_quad) match;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return true if we are at the eos

  bool InputTerm::iseos (void) const {
    wrlock ();
    try {
      // check for the pushback buffer
      if (d_sbuf.empty () == false) {
	unlock ();
	return false;
      }
      bool result = d_eos;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we can read a character

  bool InputTerm::valid (void) const {
    wrlock ();
    try {
      // check for the pushback buffer
      if (d_sbuf.empty () == false) {
	unlock ();
	return true;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, d_tout);
      if (status == false) {
	unlock ();
	return false;
      }
      // here the eos flag is used
      bool result = !d_eos;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the terminal is a tty
  
  bool InputTerm::istty (void) const {
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
  
  // save the terminal state

  void InputTerm::save (void) {
    wrlock ();
    try {
      p_attr = c_gtattr (d_sid);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // restore the terminal state
  
  void InputTerm::restore (void) {
    wrlock ();
    try {
      c_stattr (d_sid, p_attr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // put the terminal in non canonical mode
  
  bool InputTerm::nocanon (void) {
    wrlock ();
    try {
      bool result = c_stcanon (d_sid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the ignore eos flag

  void InputTerm::setieos (const bool flag) {
    wrlock ();
    try {
      d_ieos = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the eos mapped character

  void InputTerm::setmeos (const char c) {
    wrlock ();
    try {
      d_meos = c;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_SETIEOS = zone.intern ("set-ignore-eos");
  static const long QUARK_SETMEOS = zone.intern ("set-mapped-eos");

  // create a new object in a generic way

  Object* InputTerm::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 0) 
      throw Exception ("argument-error", 
		       "invalid arguments with with input term"); 
    return new InputTerm;
  }

  // return true if the given quark is defined

  bool InputTerm::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? InputStream::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* InputTerm::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETIEOS) {
	bool flag = argv->getbool (0);
	setieos (flag);
	return nullptr;
      }
      if (quark == QUARK_SETMEOS) {
	char c = argv->getbyte (0);
	setmeos (c);
	return nullptr;
      }
    }
    // call the input method
    return InputStream::apply (zobj, nset, quark, argv);
  }
}
