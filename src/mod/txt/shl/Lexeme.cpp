// ---------------------------------------------------------------------------
// - Lexeme.cpp                                                              -
// - afnix:txt module - lexeme object class implementation                   -
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
#include "Lexeme.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default lexeme

  Lexeme::Lexeme (void) {
    clear ();
  }

  // create a lexeme by value

  Lexeme::Lexeme (const String& lval) {
    clear ();
    d_lval = lval;
  }

  // create a lexeme by value and tag

  Lexeme::Lexeme (const String& lval, const long ltag) {
    clear ();
    d_lval = lval;
    d_ltag = ltag;
  }

  // copy construct a lexeme

  Lexeme::Lexeme (const Lexeme& that) {
    that.rdlock ();
    try {
      d_lval = that.d_lval;
      d_ltag = that.d_ltag;
      d_lsrc = that.d_lsrc;
      d_lidx = that.d_lidx;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Lexeme::repr (void) const {
    return "Lexeme";
  }

  // return a clone of this object

  Object* Lexeme::clone (void) const {
    return new Lexeme (*this);
  }

  // return a literal representattion of this lexeme

  String Lexeme::toliteral (void) const {
    return tostring ();
  }

  // return a string representation of this lexeme
  
  String Lexeme::tostring (void) const {
    rdlock ();
    try {
      String result = d_lval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // assign a lexeme to this one

  Lexeme& Lexeme::operator = (const Lexeme& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    that.rdlock ();
    wrlock ();
    try {
      // assign the lexeme to this one
      d_lval = that.d_lval;
      d_ltag = that.d_ltag;
      d_lsrc = that.d_lsrc;
      d_lidx = that.d_lidx;
      // unlock everything
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // clear this lexeme

  void Lexeme::clear (void) {
    wrlock ();
    try {
      d_lval = "";
      d_ltag = -1;
      d_lsrc = "";
      d_lidx = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the lexeme value

  void Lexeme::setvalue (const String& lval) {
    wrlock ();
    try {
      d_lval = lval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexeme value

  String Lexeme::getvalue (void) const {
    rdlock ();
    try {
      String result = d_lval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the lexeme tag

  void Lexeme::settag (const long ltag) {
    wrlock ();
    try {
      d_ltag = ltag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexeme tag

  long Lexeme::gettag (void) const {
    rdlock ();
    try {
      long result = d_ltag;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the lexeme source

  void Lexeme::setsrc (const String& lsrc) {
    wrlock ();
    try {
      d_lsrc = lsrc;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexeme source

  String Lexeme::getsrc (void) const {
    rdlock ();
    try {
      String result = d_lsrc;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the lexeme index

  void Lexeme::setidx (const long lidx) {
    wrlock ();
    try {
      d_lidx = lidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexeme index

  long Lexeme::getidx (void) const {
    rdlock ();
    try {
      long result = d_lidx;
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
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_SETLTAG = zone.intern ("set-tag");
  static const long QUARK_GETLTAG = zone.intern ("get-tag");
  static const long QUARK_SETLIDX = zone.intern ("set-index");
  static const long QUARK_GETLIDX = zone.intern ("get-index");
  static const long QUARK_SETLVAL = zone.intern ("set-value");
  static const long QUARK_GETLVAL = zone.intern ("get-value");
  static const long QUARK_SETLSRC = zone.intern ("set-source");
  static const long QUARK_GETLSRC = zone.intern ("get-source");

  // create a new object in a generic way

  Object* Lexeme::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Lexeme;
    // check for 1 argument
    if (argc == 1) {
      String lval = argv->getstring (0);
      return new Lexeme (lval);
    }
    throw Exception ("argument-error", "too many arguments with lexeme");
  }

  // return true if the given quark is defined

  bool Lexeme::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Literal::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Lexeme::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETLVAL) return new String  (getvalue ());
      if (quark == QUARK_GETLTAG) return new Integer (gettag   ());
      if (quark == QUARK_GETLSRC) return new String  (getsrc   ());
      if (quark == QUARK_GETLIDX) return new Integer (getidx   ());
      if (quark == QUARK_CLEAR){
	clear ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETLVAL) {
	String lval = argv->getstring (0);
	setvalue (lval);
	return nullptr;
      }
      if (quark == QUARK_SETLTAG) {
	long ltag = argv->getlong (0);
	settag (ltag);
	return nullptr;
      }
      if (quark == QUARK_SETLSRC) {
	String lsrc = argv->getstring (0);
	setsrc (lsrc);
	return nullptr;
      }
      if (quark == QUARK_SETLIDX) {
	long lidx = argv->getlong (0);
	setidx (lidx);
	return nullptr;
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
