// ---------------------------------------------------------------------------
// - Globalset.cpp                                                           -
// - afnix engine - global set class implementation                          -
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

#include "Globalset.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new global set

  Globalset::Globalset (void) {
    Object::iref (p_ntbl = new QuarkTable);
  }

  // create a new global set with a parent nameset

  Globalset::Globalset (Nameset* nset) {
    Object::iref (p_ntbl = new QuarkTable);
    setparent (nset);
  }

  // destroy this global set

  Globalset::~Globalset (void) {
    // reset the table
    p_ntbl->reset ();
    // destroy the table
    Object::dref (p_ntbl);
  }

  // return the class name

  String Globalset::repr (void) const {
    return "Globalset";
  }

  // reset this global set

  void Globalset::reset (void) {
    wrlock ();
    try {
      // protect us before reset
      Object::iref (this);
      // reset the table and parent
      if (p_ntbl != nullptr) p_ntbl->reset ();
      setparent (nullptr);
      // release and unlock
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a child global set

  Nameset* Globalset::dup (void) {
    rdlock ();
    try {
      Nameset* result = new Globalset (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind a new object by quark

  void Globalset::bind (const long quark, Object* object) {
    wrlock ();
    try {
      p_ntbl->add (quark, object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the quark exists in the global set

  bool Globalset::exists (const long quark) const {
    // lock and check
    rdlock ();
    try {
      bool result = p_ntbl->exists (quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object by quark locally

  Object* Globalset::get (const long quark) const {
    // lock and search
    rdlock ();
    try {
      Object* result = p_ntbl->get (quark);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an object by quark in this global set

  void Globalset::remove (const long quark) {
    // lock and remove
    wrlock ();
    try {
      // protect ourself
      Object::iref (this);
      // remove in the table
      p_ntbl->remove (quark);
      // release and unlock
      Object::tref (this);
      unlock ();
    } catch(...) {
      unlock ();
      throw;
    }
  }
}
