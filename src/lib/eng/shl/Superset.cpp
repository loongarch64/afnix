// ---------------------------------------------------------------------------
// - Superset.cpp                                                            -
// - afnix engine - superset class implementation                            -
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

#include "Superset.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default superset
  
  Superset::Superset (void) {
    d_glst.reset ();
  }

  // destroy this superset

  Superset::~Superset (void) {
    reset ();
  }

  // return the class name

  String Superset::repr (void) const {
    return "Superset";
  }
  
  // reset this super set 

  void Superset::reset (void) {
    wrlock ();
    try {
      // reset all registered nameset
      long size = length ();
      for (long i = 0; i < size; i++) {
	Nameset* nset = getset (i);
	if (nset != nullptr) nset->reset ();
      }
      // reset the globalset
      Globalset::reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the size of the superset

  long Superset::length (void) const {
    rdlock ();
    try {
      long result = d_glst.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a nameset by index

  Nameset* Superset::getset (const long index) const {
    rdlock ();
    try {
      // get the nameset
      Object*   obj = d_glst.get (index);
      Nameset* nset = dynamic_cast <Nameset*> (obj);
      if (nset == nullptr) {
	throw Exception ("superset-error", "cannot find nameset by index");
      }
      unlock ();
      return nset;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a nameset by quark and register it

  Nameset* Superset::mknset (const long quark) {
    wrlock ();
    if (exists (quark) == true) {
      Nameset* nset = getnset (quark);
      unlock ();
      return nset;
    }
    try {
      Nameset* result = Nameset::mknset (quark);
      d_glst.add (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a nameset by name and register it

  Nameset* Superset::mknset (const String& name) {
    return mknset (name.toquark ());
  }
}
