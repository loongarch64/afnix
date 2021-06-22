// ---------------------------------------------------------------------------
// - Rpi.cpp                                                                 -
// - afnix:mth module - real point interface implementation                  -
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

#include "Rpi.hpp"
#include "Real.hpp"
#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a point by size

  Rpi::Rpi (const t_long size) : Api (size) {
  }

  // copy construct this point

  Rpi::Rpi (const Rpi& that) {
    that.rdlock ();
    try {
      Api::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this point

  Rpi::Rpi (Rpi&& that) noexcept {
    that.wrlock ();
    try {
      Api::operator = (static_cast<Api&&>(that));
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      that.unlock ();
    }
  }

  // assign a point to this one

  Rpi& Rpi::operator = (const Rpi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Api::operator = (that);
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

  Rpi& Rpi::operator = (Rpi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      Api::operator = (static_cast<Api&&>(that));
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
  
  void Rpi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the point size
      Serial::wrlong (d_size, os);
      // write the point data
      for (long k = 0; k < d_size; k++) {
	// get the point value
	t_real val = nlget (k);
	if (val == 0.0) continue;
	// write by position
	Serial::wrlong (k,   os);
	Serial::wrreal (val, os);
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

  void Rpi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the point size
      long size = Serial::rdlong (is);
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
	t_real val = Serial::rdreal (is);
	nlset (idx, val);
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("rpi-error", "inconsistent serialized point");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this point
  
  void Rpi::reset (void) {
    wrlock ();
    try {
      d_size = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this point

  void Rpi::clear (void) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) nlset (i, 0.0);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if the point is nil

  bool Rpi::isnil (void) const {
    rdlock ();
    try {
      for (t_long k = 0L; k < d_size; k++) {
	if (Math::acmp (nlget(k), 0.0) == false) {
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

  // compare two points

  bool Rpi::operator == (const Rpi& x) const {
    rdlock ();
    x.rdlock ();
    try {
      // check size first
      if (d_size != x.d_size) {
	throw Exception ("point-error",
			 "incompatible point size with compare");
      }
      // initialize result
      bool result = true;
      // loop in point
      for (t_long i = 0; i < d_size; i++) {
	if (nlget (i) != x.nlget (i)) {
	  result = false;
	  break;
	}
      }
      // unlock and return
      unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // compare two points

  bool Rpi::operator != (const Rpi& x) const {
    return !(*this == x);
  }

  // set a point by value

  void Rpi::set (const t_real val) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) nlset (i, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a point by position

  void Rpi::set (const t_long pos, const t_real val) {
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

  t_real Rpi::get (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid point position");
      }
      t_real result = nlget (pos);
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
  static const long QUARK_EQL = zone.intern ("==");
  static const long QUARK_NEQ = zone.intern ("!=");
  static const long QUARK_SET = zone.intern ("set");
  static const long QUARK_GET = zone.intern ("get");

  // return true if the given quark is defined

  bool Rpi::isquark (const long quark, const bool hflg) const {
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

  // operate this object with another object

  Object* Rpi::oper (t_oper type, Object* object) {
    auto pobj = dynamic_cast <Rpi*> (object);
    switch (type) {
    case Object::OPER_EQL:
      if (pobj != nullptr) return new Boolean (*this == *pobj);
      break;
    case Object::OPER_NEQ:
      if (pobj != nullptr) return new Boolean (*this != *pobj);
      break;
    default:
      break;
    }
    throw Exception ("type-error", "invalid operand with real point",
                     Object::repr (object));
  }

  // apply this object with a set of arguments and a quark
  
  Object* Rpi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
        t_long pos = argv->getlong (0);
        return new Real (get (pos));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
        t_long pos = argv->getlong (0);
        t_real val = argv->getreal (1);
	set (pos, val);
        return nullptr;
      }
    }
    // call the api object
    return Api::apply (zobj, nset, quark, argv);
  }
}
