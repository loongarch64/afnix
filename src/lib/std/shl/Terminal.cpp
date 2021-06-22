// ---------------------------------------------------------------------------
// - Terminal.cpp                                                            -
// - afnix standard library - terminal class implementation                  -
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

#include "Vector.hpp"
#include "Boolean.hpp"
#include "Terminal.hpp"
#include "Character.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default prompts
  static const bool  DEF_TERM_NPRT = false;
  static const char* DEF_TERM_PRT1 = "(axi) ";
  static const char* DEF_TERM_PRT2 = "(...) ";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default terminal

  Terminal::Terminal (void) {
    // prompt initialize
    d_nprt = DEF_TERM_NPRT;
    d_prt1 = DEF_TERM_PRT1;
    d_prt2 = DEF_TERM_PRT2;
    // save the input stream state
    save ();
  }

  // destroy this terminal

  Terminal::~Terminal (void) {
    // restore terminal state
    restore ();
  }

  // return the class name

  String Terminal::repr (void) const {
    return "Terminal";
  }

  // insert a character in this terminal

  void Terminal::insert (const t_quad c) {
    wrlock ();
    try {
      // check if we are at the eol
      if (d_lbuf.iseol () == true) {
	// insert the character in the buffer
	d_lbuf.add (c);
	// insert the character in the terminal
	OutputTerm::insert (c);
	// check if we need to move the cursor to the next line
	long cols = getcols ();
	long cpos = d_lbuf.getabs ();
	if ((cols != 0) && ((cpos % cols) == 0)) {
	  if (OutputTerm::mvbol () == false) OutputTerm::movel (cols);
	  OutputTerm::moved (1);
	}
	unlock ();
	return;
      }
      // if we are not in insert mode - just replace the character
      if (d_insert == false) {
	d_lbuf.add (c);
	OutputTerm::insert (c);
	// check if we need to move the cursor to the next line
	long cols = getcols ();
	long cpos = d_lbuf.getabs ();
	if ((cols != 0) && ((cpos % cols) == 0)) {
	  if (OutputTerm::mvbol () == false) OutputTerm::movel (cols);
	  OutputTerm::moved (1);
	}
	unlock ();
	return;
      }
      // here we are in the middle of the line - start by killing the line at
      // the cursor position - then insert the character and refresh the rest
      // of the line - we also need to restore the original cursor position
      // first save the context
      long   crem = d_lbuf.getrem ();
      String sstr = d_lbuf.substr ();
      // kill at the cursor position
      kbchr ();
      // insert the character, then the line
      insert (c);
      OutputTerm::insert (sstr);
      // restore the cursor position
      for (long i = 0; i < crem; i++) {
	if (movel () == false) break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // delete a character in place

  void Terminal::rmchr (void) {
    wrlock ();
    try {
      // check if we operate at the eol
      if (d_lbuf.iseol () ==  true) {
	unlock ();
	return;
      }
      // check if we are at the last character
      if (d_lbuf.islst () ==  true) {
	// delete the buffer character
	if (d_lbuf.chdel () == true) {
	  // delete the terminal character
	  OutputTerm::chdel ();
	}
	unlock ();
	return;
      }
      // here we are in the middle of the line - start by saving the buffer
      // in the next position, kill the line at the cursor and restore
      // the remaining buffer
      if (d_lbuf.isdel () == true) {
	// save the buffer contents
	String sstr = d_lbuf.delstr ();
	// kill at the cursor position
	kbchr ();
	// insert the line
	OutputTerm::insert (sstr);
	// restore the cursor position
	long slen = sstr.length ();
	for (long i = 0; i < slen; i++) {
	  if (movel () == false) break;
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // delete a character in backspace mode

  void Terminal::bkchr (void) {
    wrlock ();
    try {
      // move to the left and delete
      if (movel () == true) rmchr ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move to the beginning of the line

  bool Terminal::mvbol (void) {
    wrlock ();
    try {
      // get the number of move
      long num  = d_lbuf.getrel ();
      // loop for the move
      for (long i = 0; i < num; i++) {
	if (movel () == false) {
	  unlock ();
	  return false;
	}
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move to the end of the line

  bool Terminal::mveol (void) {
    wrlock ();
    try {
      // get the number of moves
      long num  = d_lbuf.getrem ();
      // loop for the move
      for (long i = 0; i < num; i++) {
	if (mover () == false) {
	  unlock ();
	  return false;
	}
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the terminal cursor to the left

  bool Terminal::movel (void) {
    wrlock ();
    try {
      // move the line cursor
      if (d_lbuf.movel () == false) {
	unlock ();
	return false;
      }
      // update the terminal 
      long cols = getcols ();
      long cpos = d_lbuf.getabs () + 1;
      if ((cols != 0) && ((cpos % cols) == 0)) {
	OutputTerm::moveu (1);
	OutputTerm::mover (cols);
      } else {
	OutputTerm::movel (1);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the terminal cursor to the right

  bool Terminal::mover (void) {
    wrlock ();
    try {
      // update the line buffer
      if (d_lbuf.mover () ==  false) {
	unlock ();
	return false;
      }
      // update the terminal cursor
      long cols = getcols ();
      long cpos = d_lbuf.getabs () - 1;
      if ((cols > 0) && ((cpos % cols) == (cols - 1))) {
	OutputTerm::movel (cols-1);
	OutputTerm::moved (1);
      } else {
	OutputTerm::mover (1);
      }
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // kill all characters from the current position

  void Terminal::kbchr (void) {
    wrlock ();
    try {
      // get the remaining characters
      long num = d_lbuf.getrem ();
      // move to the eol
      mveol ();
      // remove the characters
      for (long i = 0; i < num; i++) bkchr ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // kill all characters in the buffer

  void Terminal::kachr (void) {
    wrlock ();
    try {
      // move to the eol
      mveol ();
      // get the length
      long num = d_lbuf.length ();
      // remove the characters
      for (long i = 0; i < num; i++) bkchr ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // replace a buffer line with another one

  void Terminal::rline (const String& line) {
    wrlock ();
    try {
      // kill existing buffer
      kachr ();
      // reset buffer
      d_lbuf.clear ();
      // update the terminal
      OutputTerm::insert (line);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // refresh the terminal

  void Terminal::rfrsh (void) {
    wrlock ();
    try {
      if (OutputTerm::clear () == true) {
	// get the string buffer
	String line = d_lbuf.tostring ();
	// clear the buffer
	d_lbuf.clear ();
	// write the prompt
	OutputStream::write (d_lbuf.getprt ());
	// insert the line
	OutputTerm::insert (line);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the insert mode

  void Terminal::setim (void) {
    wrlock ();
    try {
      d_insert = !d_insert;
      d_lbuf.setim (d_insert);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a line and return a string

  String Terminal::readline (bool pflg) {
    wrlock ();
    try {
      // reset the readline buffer
      d_lbuf.reset ();
      // check for the eos
      if (d_eos == true) return eosq;
      // turn off canonical mode and reset error mode
      bool canon = nocanon ();
      temode (false);
      // ok, print the prompt
      if (d_nprt == false) {
	if (pflg == true) {
	  OutputStream::write (d_prt1);
	  d_lbuf.setprt (d_prt1);
	} else {
	  OutputStream::write (d_prt2);
	  d_lbuf.setprt (d_prt2);
	}
      }

      // loop until we have an eol/eos
      while (true) {
	t_quad w = wread ();
	// check for backspace
	if ((canon == true ) && isbs (w) == true) {
	  bkchr ();
	  continue;
	}
	// check for delete
	if ((canon == true)  && isdel (w) == true) {
	  rmchr ();
	  continue;
	}
	// check for insert
	if ((canon == true) && (isins (w) == true)) {
	  setim ();
	  continue;
	}
	// check for move to begin line
	if ((canon == true) && (w == WIDE_STDBL)) {
	  mvbol ();
	  continue;
	}
	// check for move to end of line
	if ((canon == true) && (w == WIDE_STDEL)) {
	  mveol ();
	  continue;
	}
	// check for kill everything
	if ((canon == true) && (w == WIDE_STDKB)) {
	  kachr ();
	  continue;
	}
	// check for kill to eol
	if ((canon == true) && (w == WIDE_STDKL)) {
	  kbchr ();
	  continue;
	}
	// check for refresh
	if ((canon == true) && (w == WIDE_STDRF)) {
	  rfrsh ();
	  continue;
	}
	// check for move to left
	if ((canon == true) && (isle (w) == true)) {
	  movel ();
	  continue;
	}
	// check for move to right
	if ((canon == true)  && (isri (w) == true)) {
	  mover ();
	  continue;
	}
	// check for a move down
	if ((canon == true) && (isdo (w) == true)) {
	  if (d_cilo.istop () == true) {
	    kachr ();
	    continue;
	  }
	  String* data = dynamic_cast <String*> (d_cilo.getup ());
	  if (data == nullptr)
	    kachr ();
	  else
	    rline (*data);
	  continue;
	}
	// check for a move up
	if ((canon == true) && (isup (w) == true)) {
	  if (d_cilo.isbottom () == true) continue;
	  String* data = dynamic_cast <String*> (d_cilo.getdown ());
	  if (data == nullptr)
	    kachr ();
	  else
	    rline (*data);
	  continue;
	}

	// make sure the cursor is reset to the end for a eol/eos
	if (w == WIDE_STDNL) d_lbuf.setce ();
	if (w == WIDE_STDEF) d_lbuf.setce ();
	// check for printable - if not we reject - we accept eol/eos as well
	if (ispr (w) == false) continue;
	
	// process normal character
	if ((w == WIDE_STDNL) || (w == WIDE_STDEF)) {
	  if (d_lbuf.length () > 0) {
	    d_cilo.add (new String (d_lbuf.tostring()));
	  }
	  d_lbuf.add (w);
	  write (eolc);
	  break;
	}
	// insert the character in the terminal
	insert (w);
      }
      // this is it - return the string buffer
      if (canon == true) restore ();
      String result = d_lbuf.tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a credential as a string

  String Terminal::readcred (void) {
    wrlock ();
    try {
      // reset the readline buffer
      d_lbuf.reset ();
      // check for the eos
      if (d_eos == true) return eosq;
      // turn off canonical mode and reset error mode
      bool canon = nocanon ();
      temode (false);
      // loop until we have an eol/eos
      while (true) {
	// read a character
	t_quad w = wread ();
	// make sure the cursor is reset to the end for a eol/eos
	if (w == WIDE_STDNL) d_lbuf.setce ();
	if (w == WIDE_STDEF) d_lbuf.setce ();
	// check for printable or reject - we accept eol/eos as well
	if (ispr (w) == false) continue;
	// process normal character
	if ((w == WIDE_STDNL) || (w == WIDE_STDEF)) {
	  write (eolc);
	  break;
	}
	// insert the character in the buffer
	d_lbuf.add (w);
      }
      // this is it - return the string buffer
      if (canon == true) restore ();
      String result = d_lbuf.tostring ();
      // clean the buffer for security
      d_lbuf.reset ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the no prompt flag

  void Terminal::setnp (const bool value) {
    wrlock ();
    try {
      d_nprt = value;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the primary prompt

  void Terminal::setpp (const String& value) {
    wrlock ();
    try {
      d_prt1 = value;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the seconday prompt

  void Terminal::setsp (const String& value) {
    wrlock ();
    try {
      d_prt2 = value;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the no prompt flag

  bool Terminal::getnp (void) const {
    rdlock ();
    try {
      bool result = d_nprt;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the primary prompt

  String Terminal::getpp (void) const {
    rdlock ();
    try {
      String result = d_prt1;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the secondary prompt

  String Terminal::getsp (void) const {
    rdlock ();
    try {
      String result = d_prt2;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETNP    = zone.intern ("set-no-prompt");
  static const long QUARK_GETNP    = zone.intern ("get-no-prompt");
  static const long QUARK_GETPP    = zone.intern ("get-primary-prompt");
  static const long QUARK_GETSP    = zone.intern ("get-secondary-prompt");
  static const long QUARK_SETPP    = zone.intern ("set-primary-prompt");
  static const long QUARK_SETSP    = zone.intern ("set-secondary-prompt");
  static const long QUARK_READLINE = zone.intern ("read-line");
  static const long QUARK_READCRED = zone.intern ("read-credential");

  // create a new object in a generic way

  Object* Terminal::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    if (argc != 0) {
      throw Exception ("argument-error", "invalid arguments with terminal"); 
    }
    return new Terminal;
  }

  // return true if the given quark is defined

  bool Terminal::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? InputTerm::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? OutputTerm::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Terminal::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_READCRED) return new String  (readcred ());
      if (quark == QUARK_READLINE) return new String  (readline (true));
      if (quark == QUARK_GETNP)    return new Boolean (getnp ());
      if (quark == QUARK_GETPP)    return new String  (getpp ());
      if (quark == QUARK_GETSP)    return new String  (getsp ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_READLINE) {
	bool flag = argv->getbool (0);
	String* result = new String (readline (flag));
	return result;
      }
      if (quark == QUARK_SETNP) {
	bool val = argv->getbool (0);
	setnp (val);
	return nullptr;
      }
      if (quark == QUARK_SETPP) {
	String val = argv->getstring (0);
	setpp (val);
	return nullptr;
      }
      if (quark == QUARK_SETSP) {
	String val = argv->getstring (0);
	setsp (val);
	return nullptr;
      }
    }

    // check the input terminal stream
    if (InputTerm::isquark (quark, true) == true) {
      return InputTerm::apply (zobj, nset, quark, argv);
    }
    // default to the output terminal stream
    return OutputTerm::apply(zobj, nset, quark, argv);
  }
}
