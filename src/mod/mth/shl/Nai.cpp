// ---------------------------------------------------------------------------
// - Nai.cpp                                                                 -
// - afnix:mth module - numeral array interface implementation               -
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

#include "Nai.hpp"
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

  // create a default array

  Nai::Nai (void) {
    d_numt = Numeral::NUMT_SINT;
  }
  
  // create an array by size

  Nai::Nai (const t_long size) : Aai (size) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create an array by size

  Nai::Nai (const t_long size, const long strd) : Aai (size, strd) {
    d_numt = Numeral::NUMT_SINT;
  }

  // create a array by size and type

  Nai::Nai (const t_long size,
	    const long strd, const Numeral::t_numt numt) : Aai (size, strd) {
    d_numt = numt;
  }
  
  // create a array by stride and type

  Nai::Nai (const long strd, const Numeral::t_numt numt) : Aai (0LL, strd) {
    d_numt = numt;
  }
  
  // copy construct this array

  Nai::Nai (const Nai& that) {
    that.rdlock ();
    try {
      // copy base array
      Aai::operator = (that);
      // copy locally
      d_numt = that.d_numt;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this array

  Nai::Nai (Nai&& that) noexcept {
    that.wrlock ();
    try {
      // copy move base array
      Aai::operator = (static_cast<Aai&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      d_strd = 1L;
      that.unlock ();
    } 
  }

  // assign a array to this one

  Nai& Nai::operator = (const Nai& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base array
      Aai::operator = (that);
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
  
  // move a array into this one

  Nai& Nai::operator = (Nai&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base array
      Aai::operator = (static_cast<Aai&&>(that));
      // move locally
      d_numt = that.d_numt; that.d_numt = Numeral::NUMT_SINT;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      d_strd = 1L;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // serialize this object
  
  void Nai::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the array size
      Serial::wrlong (d_size, os);
      Serial::wrlong (d_strd, os);
      // write the numeral type
      Serial::wrbyte (d_numt, os);
      // write the array data
      for (t_long aidx = 0LL; aidx < d_size; aidx++) {
	for (t_long sidx = 0L; sidx < d_strd; sidx++) {
	  // get the array value
	  Numeral nval = nlget (aidx, sidx);
	  if (nval.iszero () == true) continue;
	  // serialize by position
	  Serial::wrlong (aidx, os);
	  Serial::wrlong (sidx, os);
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

  void Nai::rdstream (InputStream& is) {
    wrlock ();
    try {
      // get the array row/col size
      t_long size = Serial::rdlong (is);
      d_strd = Serial::rdlong (is);
      // get the numeral type
      d_numt = static_cast <Numeral::t_numt>(Serial::rdbyte (is));
      // resize the array
      resize (size);
      // get the array data by position
      for (t_long i = 0LL; i < size; i++) {
	for (long j = 0L; j < d_strd; j++) {
	  t_long aidx = Serial::rdlong (is);
	  // check for marker
	  if (aidx == -1LL) {
	    unlock ();
	    return;
	  }
	  long sidx = Serial::rdlong (is);
	  Numeral nval; nval.rdstream (is);
	  nlset (aidx, sidx, nval);
	}
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("nai-error", "inconsistent serialized array");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this array

  void Nai::reset (void) {
    wrlock ();
    try {
      d_size = 0LL;
      d_strd = 1L;
      d_numt = Numeral::NUMT_SINT;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this array

  void Nai::clear (void) {
    wrlock ();
    try {
      Numeral zero;
      for (t_long aidx = 0LL; aidx < d_size; aidx++) {
	for (long sidx = 0L; sidx < d_strd; sidx++) {
	  nlset (aidx, sidx, zero);
	}
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set an array by position

  void Nai::set (const t_long aidx, const long sidx, const Numeral& nval) {
    wrlock ();
    try {
      // check for valid position
      if ((aidx < 0) || (aidx >= d_size) ||
	  (sidx < 0) || (sidx >= d_strd)) {
	throw Exception ("index-error", "invalid array position in set");
      }
      // set in unlocked mode
      nlset (aidx, sidx, nval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a array value by position

  Numeral Nai::get (const t_long aidx, const long sidx) const {
    rdlock ();
    try {
      // check for valid position
      if ((aidx < 0) || (aidx >= d_size) ||
	  (sidx < 0) || (sidx >= d_strd)) {
	throw Exception ("index-error", "invalid array position in get");
      }
      // get in unlocked mode
      Numeral result = nlget (aidx, sidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a array integer value by position

  t_long Nai::getlong (const t_long aidx, const long sidx) const {
    rdlock ();
    try {
      // check for valid position
      if ((aidx < 0) || (aidx >= d_size) ||
	  (sidx < 0) || (sidx >= d_strd)) {
	throw Exception ("index-error", "invalid array position in get");
      }
      t_long result = nlget(aidx, sidx).tolong();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a array real value by position
  
  t_real Nai::getreal (const t_long aidx, const long sidx) const {
    rdlock ();
    try {
      // check for valid position
      if ((aidx < 0) || (aidx >= d_size) ||
	  (sidx < 0) || (sidx >= d_strd)) {
	throw Exception ("index-error", "invalid array position in get");
      }
      t_real result = nlget(aidx, sidx).toreal();
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

  bool Nai::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Aai::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* Nai::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_GET) {
        t_long aidx = argv->getlong (0);
        long   sidx = argv->getlong (1);
        return new Numeral (get (aidx, sidx));
      }
      if (quark == QUARK_GETINTG) {
        t_long aidx = argv->getlong (0);
        long   sidx = argv->getlong (1);
        return new Integer (getlong (aidx, sidx));
      }
      if (quark == QUARK_GETREAL) {
        t_long aidx = argv->getlong (0);
        long   sidx = argv->getlong (1);
        return new Real (getreal (aidx, sidx));
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
        t_long aidx = argv->getlong (0);
        long   sidx = argv->getlong (1);
	Object* obj = argv->get (2);
	// check for a numeral
	auto nval = dynamic_cast<Numeral*>(obj);
	if (nval != nullptr) {
	  set (aidx, sidx, *nval);
	  return nullptr;
	}
	// check for an integer
	auto ival = dynamic_cast<Integer*>(obj);
	if (ival != nullptr) {
	  set (aidx, sidx, ival->tolong());
	  return nullptr;
	}
	// check for a real
	auto rval = dynamic_cast<Real*>(obj);
	if (rval != nullptr) {
	  set (aidx, sidx, rval->toreal());
	  return nullptr;
	}
	throw Exception ("type-error", "invalid object for nai set",
			 Object::repr (obj));
      }
    }
    // call the aai object
    return Aai::apply (zobj, nset, quark, argv);
  }
}
