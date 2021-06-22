// ---------------------------------------------------------------------------
// - Saveas.cpp                                                             -
// - afnix:sps module - saveas class implementation                         -
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
#include "Saveas.hpp"
#include "QuarkZone.hpp"
#include "OutputFile.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a saveas by name

  Saveas::Saveas (const String& name) {
    setname (name);
  }

  // create a saveas by name and info

  Saveas::Saveas (const String& name, const String& info) {
    setname (name);
    setinfo (info);
  }

  // copy construct this saveas

  Saveas::Saveas (const Saveas& that) {
    that.rdlock ();
    try {
      // copy base infoable
      Taggable::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // assign a saveas into this one

  Saveas& Saveas::operator = (const Saveas& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base infoable
      Taggable::operator = (that);
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // serialize this saveas

  void Saveas::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Taggable::wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this saveas

  void Saveas::rdstream (InputStream& is) {
    wrlock ();
    try {
      Taggable::rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // save the object

  void Saveas::save (void) const {
    saveas (getname ());
  }

  // save a object by name
  
  void Saveas::saveas (const String& name) const {
    OutputFile os (name);
    serialize (os);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SAVE   = zone.intern ("save");
  static const long QUARK_SAVEAS = zone.intern ("saveas");

  // return true if the given quark is defined

  bool Saveas::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Saveas::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_SAVE ) {
	save ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SAVEAS) {
	String name = argv->getstring (0);
	saveas (name);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
