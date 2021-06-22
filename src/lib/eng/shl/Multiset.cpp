// ---------------------------------------------------------------------------
// - Multiset.cpp                                                            -
// - afnix engine - multiset class implementation                            -
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

#include "Multiset.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default multiset

  Multiset::Multiset (void) {
    p_ptbl = new NameTable;
    p_stbl = nullptr;
    p_xset = nullptr;
    d_slok = false;
    Object::iref (p_ptbl);
    //symcst (QUARK_THIS, this);
  }

  // create a multiset with a secondary table

  Multiset::Multiset (const bool sflg) {
    p_ptbl = new NameTable;
    p_stbl = sflg ? new NameTable : nullptr;
    p_xset = nullptr;
    d_slok = false;
    Object::iref (p_ptbl);
    Object::iref (p_stbl);
  }

  // create a multiset with a parent nameset

  Multiset::Multiset (Nameset* nset) {
    p_ptbl = new NameTable;
    p_stbl = nullptr;
    p_xset = nullptr;
    d_slok = false;
    Object::iref (p_ptbl);
    setparent (nset);
  }

  // create a multiset with a parent and extra namesets

  Multiset::Multiset (Nameset* nset, Nameset* xset) {
    p_ptbl = new NameTable;
    p_stbl = nullptr;
    p_xset = nullptr;
    d_slok = false;
    Object::iref (p_ptbl);
    linkset (nset, xset);
  }

  // destroy this multiset

  Multiset::~Multiset (void) {
    // protect us
    Object::iref (this);
    // destroy everything
    if (p_ptbl != nullptr) p_ptbl->reset ();
    if (p_stbl != nullptr) p_stbl->reset ();
    Object::dref (p_ptbl);
    Object::dref (p_stbl);
    // clean locally
    Object::dref (p_xset);
  }

  // return the class name

  String Multiset::repr (void) const {
    return "Multiset";
  }

  // reset the multiset

  void Multiset::reset (void) {
    wrlock ();
    try {
      // protect us before reset
      Object::iref (this);
      // reset local tables
      if (p_stbl != nullptr) p_stbl->reset ();
      if (p_ptbl != nullptr) p_ptbl->reset ();
      // remove extra and parent namesets
      setxset   (nullptr);
      setparent (nullptr);
      /// release and unlock
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a child multiset

  Nameset* Multiset::dup (void) {
    rdlock ();
    try {
      Nameset* result = new Multiset (this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind a new object by quark

  void Multiset::bind (const long quark, Object* object) {
    wrlock ();
    try {
      if ((p_stbl != nullptr) && (d_slok == false)) {
	p_stbl->add (quark, object);
      } else {
	p_ptbl->add (quark, object);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the quark exists in the multiset

  bool Multiset::exists (const long quark) const {
    rdlock ();
    try {
      // check in the secondary table
      bool result = (p_stbl == nullptr) ? false : p_stbl->exists (quark);
      // check in the primary table
      if (result == false) {
	result = p_ptbl->exists (quark);
      }
      // check in the extra nameset
      if (result == false) {
	if (p_xset != nullptr) result = p_xset->exists (quark);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get an object in this multiset by quark

  Object* Multiset::get (const long quark) const {
    rdlock ();
    try {
      // check in the secondary table
      Object* result = (p_stbl == nullptr) ? nullptr : p_stbl->get (quark);
      // check in the primary table
      if (result == nullptr) {
	result = p_ptbl->get (quark);
      }
      // check in the extra nameset
      if ((result == nullptr) && (p_xset != nullptr)) {
	result = p_xset->get (quark);
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remove an object by quark in this multiset

  void Multiset::remove (const long quark) {
    wrlock ();
    try {
      // protect ourself
      Object::iref (this);
      // check in the secondary table
      if ((p_stbl != nullptr) && (d_slok == false)) {
	if (p_stbl->exists (quark) == true) {
	  p_stbl->remove (quark);
	  Object::tref (this);
	  unlock ();
	  return;
	}
      }
      // check in the primary table
      if (p_ptbl->exists (quark) == true) {
	p_ptbl->remove (quark);
	Object::tref (this);
	unlock ();
	return;
      }
      // check in the extra nameset
      if (p_xset != nullptr) {
	if (p_xset->exists (quark) == true) p_xset->remove (quark);
      }
      Object::tref (this);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // link the nameset with this multiset

  void Multiset::linkset (Nameset* pset, Nameset* xset) {
    wrlock ();
    try {
      setxset   (xset);
      setparent (pset);
      setslok   (true);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the extra nameset

  void Multiset::setxset (Nameset* xset) {
    wrlock ();
    try {
      Object::iref (xset);
      Object::dref (p_xset);
      p_xset = xset;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the secondary locking flag

  void Multiset::setslok (const bool slok) {
    wrlock ();
    try {
      d_slok = slok;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the secondary lock flag
  
  bool Multiset::getslok (void) const {
    rdlock ();
    try {
      bool result = d_slok;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
