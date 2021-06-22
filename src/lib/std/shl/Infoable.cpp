// ---------------------------------------------------------------------------
// - Infoable.cpp                                                            -
// - standard object library - infoable class implementation                 -
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
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Infoable.hpp"
#include "QuarkZone.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a infoable by name

  Infoable::Infoable (const String& name) {
    d_name = name;
  }

  // create a infoable by name and info

  Infoable::Infoable (const String& name, const String& info) {
    d_name = name;
    d_info = info;
  }
  
  // copy construct this infoable

  Infoable::Infoable (const Infoable& that) {
    that.rdlock ();
    try {
      d_name = that.d_name;
      d_info = that.d_info;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this infoable

  Infoable::Infoable (Infoable&& that) noexcept {
    that.wrlock ();
    try {
      // move base nameable
      Nameable::operator = (static_cast<Nameable&&>(that));
      // copy move locally
      d_name = that.d_name; that.d_name.clear();
      d_info = that.d_info; that.d_info.clear();
    } catch (...) {
      d_name.clear();
      d_info.clear();
    }
    that.unlock ();
  }
  
  // assign a infoable into this one

  Infoable& Infoable::operator = (const Infoable& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign locally
      d_name = that.d_name;
      d_info = that.d_info;
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

  // move this infoable into this one

  Infoable& Infoable::operator = (Infoable&& that) noexcept {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // move base nameable
      Nameable::operator = (static_cast<Nameable&&>(that));
      // move locally
      d_name = that.d_name; that.d_name.clear();
      d_info = that.d_info; that.d_info.clear();
    } catch (...) {
      d_name.clear();
      d_info.clear();
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // get the infoable name

  String Infoable::getname (void) const {
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

  // set the infoable name

  void Infoable::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the infoable name and info at once

  void Infoable::setname (const String& name, const String& info) {
    wrlock ();
    try {
      d_name = name;
      d_info = info;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the infoable info

  String Infoable::getinfo (void) const {
    rdlock ();
    try {
      String result = d_info;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the infoable info

  void Infoable::setinfo (const String& info) {
    wrlock ();
    try {
      d_info = info;
      unlock ();
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
  static const long QUARK_SETNAME = zone.intern ("set-name");
  static const long QUARK_SETINFO = zone.intern ("set-info");
  static const long QUARK_SETNNFO = zone.intern ("set-name-info");
  static const long QUARK_GETINFO = zone.intern ("get-info");

  // return true if the given quark is defined

  bool Infoable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Infoable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETINFO) return new String  (getinfo ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETINFO) {
	String info = argv->getstring (0);
	setinfo (info);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETNNFO) {
	String name = argv->getstring (0);
	String info = argv->getstring (1);
	setname (name, info);
	return nullptr;
      }
    }
    // check the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
