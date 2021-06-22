// ---------------------------------------------------------------------------
// - Rmd.cpp                                                                 -
// - afnix:mth module - real matrix datum class implementation               -
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

#include "Rmd.hpp"
#include "Real.hpp"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default datum

  Rmd::Rmd (void) {
    d_row = 0L;
    d_col = 0L;
    d_val = 0.0;
  }
  
  // create a datum by values

  Rmd::Rmd (const t_long row, const t_long col, const t_real val) {
    set (row, col, val);
  }

  // copy construct this object

  Rmd::Rmd (const Rmd& that) {
    that.rdlock ();
    try {
      d_row = that.d_row;
      d_col = that.d_col;
      d_val = that.d_val;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the class name
  
  String Rmd::repr (void) const {
    return "Rmd";
  }
  
  // return a clone of this object
  
  Object* Rmd::clone (void) const {
    return new Rmd (*this);
  }

  // assign an object to this one

  Rmd& Rmd::operator = (const Rmd& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_row = that.d_row;
      d_col = that.d_col;
      d_val = that.d_val;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // set the datum by values

  void Rmd::set (const t_long row, const t_long col, const t_real val) {
    wrlock ();
    try {
      d_row = row;
      d_col = col;
      d_val = val;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the datum matrix row

  void Rmd::setrow (const t_long row) {
    wrlock ();
    try {
      d_row = row;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the datum matrix row

  t_long Rmd::getrow (void) const {
    rdlock ();
    try {
      t_long result = d_row;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the datum matrix column

  void Rmd::setcol (const t_long col) {
    wrlock ();
    try {
      d_col = col;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the datum matrix column

  t_long Rmd::getcol (void) const {
    rdlock ();
    try {
      t_long result = d_col;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the datum matrix value

  void Rmd::setval (const t_real val) {
    wrlock ();
    try {
      d_val = val;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the datum matrix value

  t_real Rmd::getval (void) const {
    rdlock ();
    try {
      t_real result = d_val;
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETALL = zone.intern ("set");
  static const long QUARK_SETROW = zone.intern ("set-row");
  static const long QUARK_GETROW = zone.intern ("get-row");
  static const long QUARK_SETCOL = zone.intern ("set-col");
  static const long QUARK_GETCOL = zone.intern ("get-col");
  static const long QUARK_SETVAL = zone.intern ("set-val");
  static const long QUARK_GETVAL = zone.intern ("get-val");

  // create a new object in a generic way
  
  Object* Rmd::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rmd;
    // check for 3 arguments
    if (argc == 2) {
      t_long row = argv->getlong (0);
      t_long col = argv->getlong (1);
      t_real val = argv->getrint (2);
      return new Rmd (row, col, val);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real matrix datum");
  }

  // return true if the given quark is defined

  bool Rmd::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true){
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Rmd::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETROW) return new Integer (getrow ());
      if (quark == QUARK_GETCOL) return new Integer (getcol ());
      if (quark == QUARK_GETVAL) return new Real    (getval ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETROW) {
	t_long row = argv->getlong (0);
	setrow (row);
	return nullptr;
      }
      if (quark == QUARK_SETCOL) {
	t_long col = argv->getlong (0);
	setcol (col);
	return nullptr;
      }
      if (quark == QUARK_SETVAL) {
	t_real val = argv->getrint (0);
	setval (val);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETALL) {
	t_long row = argv->getlong (0);
	t_long col = argv->getlong (1);
	t_real val = argv->getrint (2);
	set (row, col, val);
	return nullptr;
      }
    }
    // call the object
    return Object::apply (zobj, nset, quark, argv);
  }
}
