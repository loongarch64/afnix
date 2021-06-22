// ---------------------------------------------------------------------------
// - Nvi.cpp                                                                 -
// - afnix:mth module - numeral vector interface implementation              -
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

#include "Nvi.hpp"
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

  // create a default vector

  Nvi::Nvi (void) {
    d_numt = Numeral::NUMT_SINT;
  }
  
  // create a vector by size

  Nvi::Nvi (const t_long size) : Avi (size) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create a vector by size and type

  Nvi::Nvi (const t_long size, const Numeral::t_numt numt) : Avi (size) {
    d_numt = numt;
  }
  
  // copy construct this vector

  Nvi::Nvi (const Nvi& that) {
    that.rdlock ();
    try {
      // copy base vector
      Avi::operator = (that);
      // copy locally
      d_numt = that.d_numt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this vector

  Nvi::Nvi (Nvi&& that) noexcept {
    that.wrlock ();
    try {
      // move base vector
      Avi::operator = (static_cast<Avi&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      that.unlock ();
    }
  }

  // assign a vector to this one

  Nvi& Nvi::operator = (const Nvi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base vector
      Avi::operator = (that);
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
  
  // move a vector into this one

  Nvi& Nvi::operator = (Nvi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base vector
      Avi::operator = (static_cast<Avi&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // serialize this object
  
  void Nvi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector size/mode
      Serial::wrlong (d_size, os);
      // write the numeral type
      Serial::wrbyte (d_numt, os);
      // write the vector data
      for (long k = 0; k < d_size; k++) {	
	// get the vector value
	Numeral nval = nlget (k);
	if (nval.iszero () == true) continue;
	// serialize by position
	Serial::wrlong (k,   os);
	nval.wrstream (os);
      }
      Serial::wrlong (-1, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Nvi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the vector
      reset ();
      // get the vector size/mode
      long size = Serial::rdlong (is);
      // get the numeral type
      d_numt = static_cast <Numeral::t_numt>(Serial::rdbyte (is));
      // resize the vector
      resize (size);
      // get the vector data by position
      for (long k = 0; k < size; k++) {
	long idx = Serial::rdlong (is);
	// check for marker
	if (idx == -1) {
	  unlock ();
	  return;
	}
	Numeral nval; nval.rdstream (is);
	nlset (idx, nval);
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("nvi-error", "inconsistent serialized vector");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this vector

  void Nvi::reset (void) {
    wrlock ();
    try {
      d_size = 0LL;
      d_numt = Numeral::NUMT_SINT;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this vector

  void Nvi::clear (void) {
    wrlock ();
    try {
      Numeral zero;
      for (t_long k = 0L; k < d_size; k++) nlset (k, zero);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if this vector is nil

  bool Nvi::isnil (void) const {
    rdlock ();
    try {
      bool result = Algebra::isnil (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a vector by value

  void Nvi::set (const Numeral& val) {
    wrlock ();
    try {
      for (t_long k = 0L; k < d_size; k++) nlset (k, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a vector by position

  void Nvi::set (const t_long pos, const Numeral& val) {
    wrlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      nlset (pos, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a vector by position

  Numeral Nvi::get (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      Numeral result = nlget (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a numeral integer value by position

  t_long Nvi::getlong (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      t_long result = nlget(pos).tolong();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a numeral real value by position

  t_real Nvi::getreal (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      t_real result = nlget(pos).toreal();
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_GETINTG = zone.intern ("get-integer");
  static const long QUARK_GETREAL = zone.intern ("get-real");
  static const long QUARK_CONVERT = zone.intern ("convert");
  
  // return true if the given quark is defined

  bool Nvi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Avi::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Nvi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
        t_long pos = argv->getlong (0);
        return new Numeral (get (pos));
      }
      if (quark == QUARK_GETINTG) {
        t_long pos = argv->getlong (0);
        return new Integer (getlong (pos));
      }
      if (quark == QUARK_GETREAL) {
        t_long pos = argv->getlong (0);
        return new Real (getreal (pos));
      }
      if (quark == QUARK_CONVERT) {
	Object* obj = argv->get (0);
	auto   item = dynamic_cast<Item*>(obj);
	if (item == nullptr) {
	  throw Exception ("type-error", "invalid object with nvector convert",
			   Object::repr (obj));
	}
	Numeral::t_numt numt = convert (Numeral::tonumt(*item));
	return Numeral::toitem (numt);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
        t_long  pos = argv->getlong (0);
	Object* obj = argv->get (1);
	// check for a numeral
	auto nval = dynamic_cast<Numeral*>(obj);
	if (nval != nullptr) {
	  set (pos, *nval);
	  return nullptr;
	}
	// check for an integer numeral
	auto ival = dynamic_cast<Integer*>(obj);
	if (ival != nullptr) {
	  set (pos, ival->tolong ());
	  return nullptr;
	}
	// check for a real numeral
	auto rval = dynamic_cast<Real*>(obj);
	if (rval != nullptr) {
	  set (pos, rval->toreal ());
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for nvi set",
			 Object::repr (obj));
      }
    }
    // call the abstract vector
    return Avi::apply (zobj, nset, quark, argv);
  }
}

