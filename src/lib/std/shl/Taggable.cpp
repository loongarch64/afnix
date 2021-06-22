// ---------------------------------------------------------------------------
// - Taggable.cpp                                                            -
// - standard object library - taggable class implementation                 -
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
#include "Taggable.hpp"
#include "QuarkZone.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a taggable by name

  Taggable::Taggable (const String& name) {
    setname (name);
  }

  // create a taggable by name and info

  Taggable::Taggable (const String& name, const String& info) {
    setname (name);
    setinfo (info);
  }
  
  // copy construct this taggable

  Taggable::Taggable (const Taggable& that) {
    that.rdlock ();
    try {
      // copy base infoable
      Infoable::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this taggable

  Taggable::Taggable (Taggable&& that) noexcept {
    that.wrlock ();
    try {
      // move base infoable
      Infoable::operator = (static_cast<Infoable&&>(that));
    } catch (...) {}
    that.unlock ();
  }
  
  // assign a taggable into this one

  Taggable& Taggable::operator = (const Taggable& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base infoable
      Infoable::operator = (that);
      // unlock and here
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move this taggable into this one

  Taggable& Taggable::operator = (Taggable&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move base infoable
      Infoable::operator = (static_cast<Infoable&&>(that));
    } catch (...) {}
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // serialize this taggable

  void Taggable::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      d_name.wrstream (os);
      d_info.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this taggable

  void Taggable::rdstream (InputStream& is) {
    wrlock ();
    try {
      d_name.rdstream (is);
      d_info.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tag

  String Taggable::totag (void) const {
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

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_TOTAG = zone.intern ("to-tag");

  // return true if the given quark is defined

  bool Taggable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Infoable::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Serial::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Taggable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOTAG) return new String (totag ());
    }
    // check the infoable method
    if (Infoable::isquark (quark, true) == true) {
      return Infoable::apply (zobj, nset, quark, argv);
    }
    // call the serial method
    return Serial::apply (zobj, nset, quark, argv);
  }
}
