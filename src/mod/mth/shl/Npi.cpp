// ---------------------------------------------------------------------------
// - Npi.cpp                                                                 -
// - afnix:mth module - numeral point interface implementation              -
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

#include "Npi.hpp"
#include "Real.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Npi::Npi (void) {
    d_numt = Numeral::NUMT_SINT;
  }
  
  // create a point by size

  Npi::Npi (const t_long size) : Api (size) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create a point by size and type

  Npi::Npi (const t_long size, const Numeral::t_numt numt) : Api (size) {
    d_numt = numt;
  }
  
  // copy construct this point

  Npi::Npi (const Npi& that) {
    that.rdlock ();
    try {
      // copy base point
      Api::operator = (that);
      // copy locally
      d_numt = that.d_numt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this point

  Npi::Npi (Npi&& that) noexcept {
    that.wrlock ();
    try {
      // move base point
      Api::operator = (static_cast<Api&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      that.unlock ();
    }
  }

  // assign a point to this one

  Npi& Npi::operator = (const Npi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base point
      Api::operator = (that);
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
  
  // move a point into this one

  Npi& Npi::operator = (Npi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base point
      Api::operator = (static_cast<Api&&>(that));
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
  
  void Npi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the point size/mode
      Serial::wrlong (d_size, os);
      // write the numeral type
      Serial::wrbyte (d_numt, os);
      // write the point data
      for (long k = 0; k < d_size; k++) {	
	// get the point value
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

  void Npi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the point
      reset ();
      // get the point size/mode
      long size = Serial::rdlong (is);
      // get the numeral type
      d_numt = static_cast <Numeral::t_numt>(Serial::rdbyte (is));
      // resize the point
      resize (size);
      // get the point data by position
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
	throw Exception ("npi-error", "inconsistent serialized point");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this point

  void Npi::reset (void) {
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

  // clear this point

  void Npi::clear (void) {
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

  // check if the point is nil

  bool Npi::isnil (void) const {
    rdlock ();
    try {
      for (t_long k = 0L; k < d_size; k++) {
	if (nlget(k).iszero () == false) {
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
  
  // set a point by value

  void Npi::set (const Numeral& val) {
    wrlock ();
    try {
      for (t_long k = 0L; k < d_size; k++) nlset (k, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a point by position

  void Npi::set (const t_long pos, const Numeral& val) {
    wrlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid point position");
      }
      nlset (pos, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a point by position

  Numeral Npi::get (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid point position");
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

  t_long Npi::getlong (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid point position");
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

  t_real Npi::getreal (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid point position");
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_GETINTG = zone.intern ("get-integer");
  static const long QUARK_GETREAL = zone.intern ("get-real");
  
  // return true if the given quark is defined

  bool Npi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Api::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Npi::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
        t_long  pos = argv->getlong (0);
	Object* obj = argv->get (1);
	auto val = dynamic_cast<Numeral*>(obj);
	if (val == nullptr) {
	  throw Exception ("type-error", "invalid object for npi set",
			   Object::repr (obj));
	}
	set (pos, *val);
        return nullptr;
      }
    }
    // call the abstract point
    return Api::apply (zobj, nset, quark, argv);
  }
}

