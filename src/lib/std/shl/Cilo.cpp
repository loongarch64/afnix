// ---------------------------------------------------------------------------
// - Cilo.cpp                                                                -
// - standard object library - circular cilo class implementation            -
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

#include "Cilo.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default cilo

  Cilo::Cilo (void) {
    d_size = 64;
    d_sidx = 0;
    d_eidx = 0;
    d_tidx = 0;
    d_full = false;
    p_cilo = new Object*[d_size];
    for (long i = 0; i < d_size; i++) p_cilo[i] = nullptr;
  }

  // create a cilo with a predefined size

  Cilo::Cilo (const long size) {
    d_size = size;
    d_sidx = 0;
    d_eidx = 0;
    d_tidx = 0;
    d_full = false;
    p_cilo = new Object*[d_size];
    for (long i = 0; i < d_size; i++) p_cilo[i] = nullptr;
  }

  // copy constructor for this cilo

  Cilo::Cilo (const Cilo& that) {
    that.rdlock ();
    d_size = that.d_size;
    d_sidx = that.d_sidx;
    d_eidx = that.d_eidx;
    d_tidx = that.d_tidx;
    d_full = that.d_full;
    p_cilo = new Object*[d_size];
    for (long i = 0; i < d_size; i++) {
      p_cilo[i] = Object::iref (that.p_cilo[i]);
    }
    that.unlock ();
  }

  // destroy this circular cilo

  Cilo::~Cilo (void) {
    for (long i = 0; i < d_size; i++) Object::dref (p_cilo[i]);
    delete [] p_cilo;
  }

  // return the class name

  String Cilo::repr (void) const {
    return "Cilo";
  }

  // add an object in this cilo

  void Cilo::add (Object* object) {
    wrlock ();
    try {
      Object::iref (object);
      Object::dref (p_cilo[d_eidx]);
      p_cilo[d_eidx] = object;
      if (d_full == true) {
	d_eidx = (d_eidx + 1) % d_size;
	d_sidx = (d_sidx + 1) % d_size;
      } else {
	if (++d_eidx >= d_size) {
	  d_eidx = d_eidx % d_size;
	  d_sidx++;
	  d_full = true;
	}
      }
      d_tidx = d_eidx;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we are at the top

  bool Cilo::istop (void) const {
    rdlock ();
    try {
      bool result = (d_tidx == d_eidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we are at the bottom

  bool Cilo::isbottom (void) const {
    rdlock ();
    try {
      bool result = (d_tidx == d_sidx);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the traverse cursor up object

  Object* Cilo::getup (void) {
    wrlock ();
    try {
      if (d_tidx == d_eidx) {
	unlock ();
	return nullptr;
      }
      d_tidx = (d_tidx + 1) % d_size;
      Object* result = p_cilo[d_tidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the traverse cursor down object

  Object* Cilo::getdown (void) {
    wrlock ();
    try {
      if (d_tidx == d_sidx) {
	unlock ();
	return nullptr;
      }
      d_tidx = (d_tidx == 0) ? d_size - 1 : d_tidx - 1;
      Object* result = p_cilo[d_tidx];
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
