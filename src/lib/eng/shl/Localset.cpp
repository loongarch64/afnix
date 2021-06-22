// ---------------------------------------------------------------------------
// - Localset.cpp                                                            -
// - afnix engine - localset class implementation                            -
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

#include "Localset.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default localset

  Localset::Localset (void) {
    Object::iref (p_ntbl = new NameTable);
  }

  // create a localset with a parent nameset

  Localset::Localset (Nameset* nset) {
    Object::iref (p_ntbl = new NameTable);
    setparent (nset);
  }

  // destroy this local set

  Localset::~Localset (void) {
    // protect us
    Object::iref (this);
    // destroy the table
    p_ntbl->reset ();
    Object::dref (p_ntbl);
  }

  // return the class name

  String Localset::repr (void) const {
    return "Localset";
  }

  // reset the localset

  void Localset::reset (void) {
    wrlock ();
    try {
      // protect us before reset
      Object::iref (this);
      // reset the table and parent
      if (p_ntbl != nullptr) p_ntbl->reset ();
      setparent (nullptr);
      /// release and unlock
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a child localset

  Nameset* Localset::dup (void) {
    rdlock ();
    try {
      Nameset* result = new Localset (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind a new object by quark

  void Localset::bind (const long quark, Object* object) {
    wrlock ();
    try {
      p_ntbl->add (quark, object);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the quark exists in the local set

  bool Localset::exists (const long quark) const {
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

  // get an object in this localset by quark

  Object* Localset::get (const long quark) const {
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

  // remove an object by quark in this localset

  void Localset::remove (const long quark) {
    wrlock ();
    try {
      // protect ourself
      Object::iref (this);
      // remove in the table
      if (p_ntbl->exists (quark) == true) p_ntbl->remove (quark);
      // release and unlock
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
