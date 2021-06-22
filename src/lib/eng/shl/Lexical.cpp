// ---------------------------------------------------------------------------
// - Lexical.cpp                                                             -
// - afnix engine - lexical name class implementation                        -
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

#include "Engsid.hxx"
#include "Vector.hpp"
#include "Unicode.hpp"
#include "Lexical.hpp"
#include "Nameset.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the nil quark
  static const long QUARK_NIL = String::intern ("nil");

  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // return true if the character is a valid lexical one

  bool Lexical::valid (const t_quad c) {
    // check as combining
    if (Unicode::iscan (c) == true) return true;
    // check special characters
    if (c == '.') return true;
    if (c == '+') return true;
    if (c == '-') return true;
    if (c == '*') return true;
    if (c == '/') return true;
    if (c == '!') return true;
    if (c == '=') return true;
    if (c == '.') return true;
    if (c == '>') return true;
    if (c == '<') return true;
    if (c == '?') return true;
    return false;
  }

  // return true if the name is valid lexical one

  bool Lexical::valid (const String& name) {
    long len = name.length ();
    // check special case
    if (len == 0) return false;
    // check for bigger name
    for (long i = 0; i < len; i++) {
      if (Lexical::valid (name[i]) == false) return false;
    }
    return true;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default lexical
 
  Lexical::Lexical (void) {
    d_lnum  = 0L;
    d_quark = 0L;
  }

  // create a lexical with a name

  Lexical::Lexical (const String& name) {
    if (Lexical::valid (name) == false) 
      throw Exception ("syntax-error","invalid lexical name", name);
    d_name  = name;
    d_quark = name.toquark ();
    d_lnum  = 0L;
  }

  // create a lexical with a name and a line number

  Lexical::Lexical (const String& name, const long lnum) {
    if (Lexical::valid (name) == false) {
      throw Exception ("syntax-error","invalid lexical name", name);
    }
    d_name  = name;
    d_quark = name.toquark ();
    d_lnum  = lnum;
  }
  
  // copy constructor for this lexical

  Lexical::Lexical (const Lexical& that) {
    that.rdlock ();
    try {
      d_name  = that.d_name;
      d_quark = that.d_quark;
      d_lnum  = that.d_lnum;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Lexical::repr (void) const {
    return "Lexical";
  }

  // return a clone of this object

  Object* Lexical::clone (void) const {
    return new Lexical (*this);
  }

  // clear this lexical name

  void Lexical::clear (void) {
    wrlock ();
    try {
      d_name  = "";
      d_quark = 0L;
      d_lnum  = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this lexical

  String Lexical::toliteral (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return a string representation of this lexical

  String Lexical::tostring (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the serial did

  t_word Lexical::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Lexical::getsid (void) const {
    return SRL_LEXL_SID;
  }
  
  // serialize this lexical

  void Lexical::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      d_name.wrstream (os);
      Serial::wrlong (d_lnum, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this lexical

  void Lexical::rdstream (InputStream& is) {
    wrlock ();
    try {
      String sval; sval.rdstream (is); d_name = sval;
      d_lnum  = Serial::rdlong (is);
      d_quark = sval.toquark ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexical line number

  long Lexical::getlnum (void) const {
    rdlock ();
    try {
      long result = d_lnum;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the lexical quark

  long Lexical::toquark (void) const {
    rdlock ();
    try {
      long result = d_quark;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the lexical is the nil quark

  bool Lexical::isnil (void) const {
    rdlock ();
    try {
      bool result = (d_quark == QUARK_NIL);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                         -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the lexical quarks
  static const long QUARK_MAP = zone.intern ("map");

  // create a new object in a generic way

  Object* Lexical::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Lexical;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Lexical (name);
    }
    // illegal arguments
    throw Exception ("argument-error", "too many arguments with lexical");
  }

  // return true if the given quark is defined

  bool Lexical::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }      
    bool result = hflg ? Literal::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // set this lexical name as a const symbol

  Object* Lexical::cdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Object* result = nset->cdef (zobj, nset, d_quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set this lexical name with an object

  Object* Lexical::vdef (Evaluable* zobj, Nameset* nset, Object* object) {
    wrlock ();
    try {
      Object* result = nset->vdef (zobj, nset, d_quark, object);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // unset this lexical name by quark

  Object* Lexical::udef (Evaluable* zobj, Nameset* nset) {
    wrlock ();
    try {
      Object* result = nset->udef (zobj, nset, d_quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // evaluate this lexical in the current environment

  Object* Lexical::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    try {
      Object* result = nset->eval (zobj, nset, d_quark);
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this lexical with a set of arguments and a quark

  Object* Lexical::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_MAP) {
	Object* result = (nset == nullptr) ? nullptr : nset->find (d_quark);
	zobj->post (result);
	return result;
      }
    }
    // call the literal method
    return Literal::apply (zobj, nset, quark, argv);
  }
}
