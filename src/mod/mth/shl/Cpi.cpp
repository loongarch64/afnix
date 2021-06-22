// ---------------------------------------------------------------------------
// - Cpi.cpp                                                                 -
// - afnix:mth module - combinatoric permutation interface implementation    -
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

#include "Cpi.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default permutation

  Cpi::Cpi (void) {
    d_size = 0LL;
  }

  // create a permutation by size

  Cpi::Cpi (const t_long size) {
    // check the size
    if (size < 0) {
      throw Exception ("size-error", "invalid permutation size");
    }
    d_size = size;
  }

  // copy move this permutation

  Cpi::Cpi (Cpi&& that) noexcept : Serial (static_cast<Serial&&>(that)) {
    that.wrlock ();
    try {
      d_size = that.d_size; that.d_size = 0LL;
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      that.unlock ();
    } 
  }

  // move a permutation to this one

  Cpi& Cpi::operator = (Cpi&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move base serial
      Serial::operator = (static_cast<Serial&&>(that));
      // move locally
      d_size = that.d_size; that.d_size = 0LL;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0LL;
      unlock ();
      that.unlock ();
    } 
    return *this;
  }

  
  // serialize this object
  
  void Cpi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the array size
      Serial::wrlong (d_size, os);
      // write the array data
      for (long k = 0; k < d_size; k++) Serial::wrlong (nlget (k), os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Cpi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the permutation
      reset ();
      // get the permutation size
      long size = Serial::rdlong (is);
      resize (size);
      // get the permutation data
      for (long k = 0LL; k < d_size; k++) nlset (k, Serial::rdlong (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this permutation

  void Cpi::reset (void) {
    wrlock ();
    try {
      d_size = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // clear this permutation

  void Cpi::clear (void) {
    wrlock ();
    try {
      for (t_long k = 0; k < d_size; k++) nlset (k, k);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the permutation size

  t_long Cpi::getsize (void) const {
    rdlock ();
    try {
      t_long result = d_size;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the permutation by position

  void Cpi::set (const t_long pos, const t_long val) {
    wrlock ();
    try {
      // check valid bounds
      if ((pos < 0LL) || (pos >= d_size) || (val < 0LL) || (val >= d_size)) {
	throw Exception ("cpi-error", "invalid permutation arguments");
      }
      // set the permutation
      nlset (pos, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the permutation by position

  t_long Cpi::get (const t_long pos) const {
    wrlock ();
    try {
      // check valid bounds
      if ((pos < 0LL) || (pos >= d_size)) {
	throw Exception ("cpi-error", "invalid permutation arguments");
      }
      t_long result = nlget (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the reverse permutation by position

  t_long Cpi::rget (const t_long pos) const {
    wrlock ();
    try {
      // check valid bounds
      if ((pos < 0LL) || (pos >= d_size)) {
	throw Exception ("cpi-error", "invalid permutation arguments");
      }
      t_long result = nlrget (pos);
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
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_REVERSE = zone.intern ("reverse");

  // return true if the given quark is defined

  bool Cpi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Serial::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Cpi::apply (Evaluable* zobj, Nameset* nset, const long quark,
		      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GET) {
	t_long pos = argv->getlong (0);
	return new Integer (get (pos));
      }
      if (quark == QUARK_REVERSE) {
	t_long pos = argv->getlong (0);
	return new Integer (rget (pos));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	t_long pos = argv->getlong (0);
	t_long val = argv->getlong (1);
	set (pos, val);
	return nullptr;
      }
    }
    // call the serial object
    return Serial::apply (zobj, nset, quark, argv);
  }
}

