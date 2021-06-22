// ---------------------------------------------------------------------------
// - Nmi.cpp                                                                 -
// - afnix:mth module - numeral matrix interface implementation              -
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

#include "Nmi.hpp"
#include "Real.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Algebra.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default matrix

  Nmi::Nmi (void) {
    d_numt = Numeral::NUMT_SINT;
  }
  
  // create a square matrix by size

  Nmi::Nmi (const t_long size) : Ami (size) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create a matrix by size

  Nmi::Nmi (const t_long rsiz, const t_long csiz) : Ami (rsiz, csiz) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create a matrix by size and type

  Nmi::Nmi (const t_long rsiz,
	    const t_long csiz, const Numeral::t_numt numt) : Ami (rsiz, csiz) {
    d_numt = numt;
  }

  // copy construct this matrix

  Nmi::Nmi (const Nmi& that) {
    that.rdlock ();
    try {
      // copy base matrix
      Ami::operator = (that);
      // copy locally
      d_numt = that.d_numt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this matrix

  Nmi::Nmi (Nmi&& that) noexcept {
    that.wrlock ();
    try {
      // copy move base matrix
      Ami::operator = (static_cast<Ami&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      that.unlock ();
    } 
  }

  // assign a matrix to this one

  Nmi& Nmi::operator = (const Nmi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base matrix
      Ami::operator = (that);
      // assign locally
      d_numt = that.d_numt;
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a matrix into this one

  Nmi& Nmi::operator = (Nmi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base matrix
      Ami::operator = (static_cast<Ami&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // serialize this object
  
  void Nmi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the matric row/col size
      Serial::wrlong (d_rsiz, os);
      Serial::wrlong (d_csiz, os);
      // write the numeral type
      Serial::wrbyte (d_numt, os);
      // write the matrix data
      for (t_long row = 0; row < d_rsiz; row++) {
	for (t_long col = 0; col < d_csiz; col++) {
	  // get the matrix value
	  Numeral nval = nlget (row, col);
	  if (nval.iszero () == true) continue;
	  // serialize by position
	  Serial::wrlong (row, os);
	  Serial::wrlong (col, os);
	  nval.wrstream (os);
	}
      }
      // write marker for end
      Serial::wrlong (-1, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Nmi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the matrix row/col size
      t_long rsiz = Serial::rdlong (is);
      t_long csiz = Serial::rdlong (is);
      // get the numeral type
      d_numt = static_cast <Numeral::t_numt>(Serial::rdbyte (is));
      // resize the matrix
      resize (rsiz, csiz);
      // get the matrix data by position
      for (t_long i = 0LL; i < rsiz; i++) {
	for (t_long j = 0; j < csiz; j++) {
	  t_long row = Serial::rdlong (is);
	  // check for marker
	  if (row == -1) {
	    unlock ();
	    return;
	  }
	  t_long col = Serial::rdlong (is);
	  Numeral nval; nval.rdstream (is);
	  nlset (row, col, nval);
	}
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("nmi-error", "inconsistent serialized matrix");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this matrix

  void Nmi::reset (void) {
    wrlock ();
    try {
      d_rsiz = 0LL;
      d_csiz = 0LL;
      d_numt = Numeral::NUMT_SINT;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this matrix

  void Nmi::clear (void) {
    wrlock ();
    try {
      Numeral zero;
      for (long row = 0L; row < d_rsiz; row++) {
	for (long col = 0L; col < d_csiz; col++) {
	  nlset (row, col, zero);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an identity matrix

  void Nmi::eye (void) {
    wrlock ();
    try {
      Algebra::eye (*this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a matrix by position

  void Nmi::set (const t_long row, const t_long col, const Numeral& val) {
    wrlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in set");
      }
      // set in unlocked mode
      nlset (row, col, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a matrix value by position

  Numeral Nmi::get (const t_long row, const t_long col) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in get");
      }
      // get in unlocked mode
      Numeral result = nlget (row, col);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a matrix integer value by position

  t_long Nmi::getlong (const t_long row, const t_long col) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in get");
      }
      t_long result = nlget(row, col).tolong();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a matrix real value by position
  
  t_real Nmi::getreal (const t_long row, const t_long col) const {
    rdlock ();
    try {
      // check for valid position
      if ((row < 0) || (row >= d_rsiz) ||
	  (col < 0) || (col >= d_csiz)) {
	throw Exception ("index-error", "invalid matrix position in get");
      }
      t_real result = nlget(row, col).toreal();
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_GETINTG = zone.intern ("get-integer");
  static const long QUARK_GETREAL = zone.intern ("get-real");

  // return true if the given quark is defined

  bool Nmi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Ami::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Nmi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_GET) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
        return new Numeral (get (row, col));
      }
      if (quark == QUARK_GETINTG) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
        return new Integer (getlong (row, col));
      }
      if (quark == QUARK_GETREAL) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
        return new Real (getreal (row, col));
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
        t_long row = argv->getlong (0);
        t_long col = argv->getlong (1);
	Object* obj = argv->get (2);
	// check for a numeral
	auto nval = dynamic_cast<Numeral*>(obj);
	if (nval != nullptr) {
	  set (row, col, *nval);
	  return nullptr;
	}
	// check for an integer
	auto ival = dynamic_cast<Integer*>(obj);
	if (ival != nullptr) {
	  set (row, col, ival->tolong());
	  return nullptr;
	}
	// check for a real
	auto rval = dynamic_cast<Real*>(obj);
	if (rval != nullptr) {
	  set (row, col, rval->toreal());
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for nmi set",
			 Object::repr (obj));
      }
    }
    // call the ami object
    return Ami::apply (zobj, nset, quark, argv);
  }
}
