// ---------------------------------------------------------------------------
// - Scanner.cpp                                                             -
// - afnix:txt module - scanner object class implementation                  -
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
#include "Scanner.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputTerm.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // scanner context
  struct s_sctx {
    // the status flag
    bool d_status;
    // the scanned string
    String d_lexeme;
    // simple constructor
    s_sctx (void) {
      reset ();
    }
    // reset this context
    void reset (void) {
      d_status = false;
      d_lexeme = "";
    }
    // return true if the context is valid
    bool valid (void) const {
      return d_status;
    }
    // return the lexeme value
    String getlval (void) const {
      return d_status ? d_lexeme : "";
    }
    // scan an input stream
    void scan (InputStream& is, const String& ps, const Pattern& pat) {
      reset ();
      d_lexeme = pat.match (&is, ps);
      if (d_lexeme.length () > 0) d_status = true;
    }
  };

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default scanner

  Scanner::Scanner (void) {
    d_mmin = false;
  }

  // return the class name

  String Scanner::repr (void) const {
    return "Scanner";
  }

  // return the scanner length

  long Scanner::length (void) const {
    rdlock ();
    try {
      long result = d_vpat.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a pattern to the scanner

  void Scanner::add (Pattern* pat) {
    wrlock ();
    try {
      // get the index
      long index = d_vpat.length ();
      // set the tag
      pat->settag (index);
      // add it into the scanner
      d_vpat.add (pat);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a pattern by index

  Pattern* Scanner::get (const long index) const {
    rdlock ();
    try {
      Pattern* pat = dynamic_cast <Pattern*> (d_vpat.get (index));
      unlock ();
      return pat;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check a string with the scanner

  Lexeme* Scanner::check (const String& s) const {
    rdlock ();
    try {
      // iterate with the patterns
      long len = length ();
      for (long i = 0; i < len; i++) {
	// get the pattern
	Pattern* pat = get (i);
	if (pat == nullptr) continue;
	// check the regex against the string
	if (pat->check (s) == false) continue;
	// build a lexeme since we have a match
	Lexeme* lexm = new Lexeme (s, pat->gettag ());
	unlock ();
	return lexm;
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // scan an input stream and return a lexeme
 
  Lexeme* Scanner::scan (InputStream& is) const {
    wrlock ();
    try {
      // check for patterns
      long slen = length ();
      if (slen == 0) {
	unlock ();
	return nullptr;
      }
      // create a scanner context
      s_sctx* sctx = new s_sctx[slen];
      try {
	// the lexeme index
	long lidx = -1;
	for (long i = 0; i < slen; i++) {
	  // get the pattern
	  Pattern* pat = get (i);
	  if (pat == nullptr) continue;
	  // get the initial value
	  String init = (lidx == -1) ? "" : sctx[lidx].getlval ();
	  // scan for good value
	  sctx[i].scan (is, init, *pat); 
	  if (sctx[i].valid () == true) {
	    lidx = i;
	    if (d_mmin == true) break;
	  }
	}
	// if a lexeme was found, the value is the index
	Lexeme* lexm = nullptr;
	if (lidx != -1) {
	  String lval = sctx[lidx].getlval ();
	  long   ltag = get(lidx)->gettag ();
	  lexm = new Lexeme (lval, ltag);
	}
	// no match found
	delete [] sctx;
	unlock ();
	return lexm;
      } catch (...) {
	delete [] sctx;
	throw;
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the scanner supported quarks
  static const long QUARK_ADD    = zone.intern ("add");
  static const long QUARK_GET    = zone.intern ("get");
  static const long QUARK_SCAN   = zone.intern ("scan");
  static const long QUARK_CHECK  = zone.intern ("check");
  static const long QUARK_LENGTH = zone.intern ("length");

  // create a new object in a generic way

  Object* Scanner::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Scanner;
    throw Exception ("argument-error", "too many arguments with scanner");
  }

  // return true if the given quark is defined

  bool Scanner::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Scanner::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // process generic quark
    if (quark == QUARK_ADD) {
      for (long i = 0; i < argc; i++) {
	Object*  obj = argv->get (i);
	Pattern* pat = dynamic_cast <Pattern*> (obj);
	if (pat == nullptr) {
	  throw Exception ("type-error", "invalid object to add ", 
			   Object::repr (obj));
	}
	add (pat);
      }
      return nullptr;
    }
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH) return new Integer (length ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	rdlock ();
	try {
	  long index   = argv->getlong (0);
	  Pattern* pat = get (index);
	  zobj->post (pat);
	  unlock ();
	  return pat;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_CHECK) {
	rdlock ();
	try {
	  String sval = argv->getstring (0);
	  Lexeme* lexm = check (sval);
	  zobj->post (lexm);
	  unlock ();
	  return lexm;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SCAN) {
	Object* obj = argv->get (0);
	InputStream* is = dynamic_cast <InputStream*> (obj);
	if (is == nullptr) {
	  throw Exception ("type-error", "invalid object to scan ", 
			   Object::repr (obj));
	}
	wrlock ();
	try {
	  Lexeme* lexm = scan (*is);
	  zobj->post (lexm);
	  unlock ();
	  return lexm;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
