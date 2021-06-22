// ---------------------------------------------------------------------------
// - Rpoint.cpp                                                              -
// - afnix:mth module - real point implementation                            -
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

#include "Real.hpp"
#include "Vector.hpp"
#include "Rpoint.hpp"
#include "Mthsid.hxx"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Rpoint::Rpoint (void) {
    p_ptab = nullptr;
  }

  // create a point by size

  Rpoint::Rpoint (const t_long size) : Rpi (size) {
    p_ptab = (size == 0L) ? nullptr : new t_real[size];
    clear ();
  }

  // copy construct this point

  Rpoint::Rpoint (const Rpoint& that) {
    that.rdlock ();
    try {
      d_size = that.d_size;
      p_ptab = nullptr;
      if (d_size > 0) {
	p_ptab = new t_real[d_size];
	for (t_long i = 0; i < d_size; i++) p_ptab[i] = that.p_ptab[i];
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this point

  Rpoint::~Rpoint (void) {
    delete [] p_ptab;
  }

  // return the class name

  String Rpoint::repr (void) const {
    return "Rpoint";
  }

  // return a clone of this object

  Object* Rpoint::clone (void) const {
    return new Rpoint (*this);
  }

  // assign a point to this one

  Rpoint& Rpoint::operator = (const Rpoint& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      delete [] p_ptab;
      d_size = that.d_size;
      p_ptab = nullptr;
      if (d_size > 0) {
	p_ptab = new t_real[d_size];
	for (t_long i = 0; i < d_size; i++) p_ptab[i] = that.p_ptab[i];
      }
      // unlock and return
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the serial did

  t_word Rpoint::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Rpoint::getsid (void) const {
    return SRL_RPNT_SID;
  }
  
  // resize this point

  void Rpoint::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
	throw Exception ("rpoint-error", "invalid negatize size in resize");
      }
      if (size == 0) {
	delete [] p_ptab;
	d_size = 0;
	p_ptab = nullptr;
	unlock ();
	return;
      }
      // do nothing if equal
      if (size == d_size) {
	unlock ();
	return;
      }
      // create a new array by size
      t_real* ptab = new t_real[size];
      // check for smaller size
      if (size < d_size) {
	for (long k = 0; k < size; k++) {
	  ptab[k] = (p_ptab == nullptr) ? 0.0 : p_ptab[k];
	}
      } else {
	for (long k = 0; k < d_size; k++) {
	  ptab[k] = (p_ptab == nullptr) ? 0.0 : p_ptab[k];
	}
	for (long k = d_size; k < size; k++) ptab[k] = 0.0;
      }
      d_size = size;
      delete [] p_ptab;
      p_ptab = ptab;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this point

  void Rpoint::reset (void) {
    wrlock ();
    try {
      delete [] p_ptab;
      d_size = 0;
      p_ptab = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this point

  void Rpoint::clear (void) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) p_ptab[i] = 0.0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - no lock section                                                        -
  // -------------------------------------------------------------------------

  // no lock - set a point by position

  void Rpoint::nlset (const t_long pos, const t_real val) {
    if (p_ptab != nullptr) p_ptab[pos] = val;
  }

  // no lock - get a point by position
  
  t_real Rpoint::nlget (const t_long pos) const {
    return (p_ptab == nullptr) ? 0.0 : p_ptab[pos];
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Rpoint::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Rpoint;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Rpoint (size);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real point object");
  }
}

