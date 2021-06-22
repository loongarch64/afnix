// ---------------------------------------------------------------------------
// - Narray.cpp                                                              -
// - afnix:mth module - numeral array array implementation                   -
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

#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Narray.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure computes the minimum of two integers
  static inline t_long min (const t_long x, const t_long y) {
    return (x < y) ? x : y;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default array

  Narray::Narray (void) {
    p_bblk = nullptr;
  }

  // create a square array by size

  Narray::Narray (const t_long size) : Nai (size) {
    p_bblk = nullptr;
    resize (size);
  }

  // create a array by size

  Narray::Narray (const t_long size, const long strd) : Nai (size, strd) {
    p_bblk = nullptr;
    resize (size);    
  }

  // create a array by stride and type

  Narray::Narray (const long strd,
		  const Numeral::t_numt numt) : Nai (strd, numt) {
    p_bblk = nullptr;
  }

  // create a array by size and type

  Narray::Narray (const t_long size, const long strd,
		  const Numeral::t_numt numt) : Nai (size, strd, numt) {
    p_bblk = nullptr;
    resize (size);    
  }

  // copy construct this array

  Narray::Narray (const Narray& that) {
    that.rdlock ();
    try {
      // copy base class
      Nai::operator = (that);
      // copy locally
      p_bblk = nullptr;
      t_long size = d_size * d_strd * Numeral::tobsiz (d_numt);
      if (size > 0LL) {
	p_bblk = new t_byte[size];
	Utility::tobcpy (p_bblk, size, that.p_bblk);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this array

  Narray::~Narray (void) {
    delete [] p_bblk;
  }

  // assign a array to this one

  Narray& Narray::operator = (const Narray& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete the old array
      delete [] p_bblk; p_bblk = nullptr;
      // assign base class
      Nai::operator = (that);
      // assign locally
      t_long size = d_size * d_strd * Numeral::tobsiz (d_numt);
      if (size > 0LL) {
	p_bblk = new t_byte[size];
	Utility::tobcpy (p_bblk, size, that.p_bblk);
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
  
  // return the class name

  String Narray::repr (void) const {
    return "Narray";
  }

  // return a clone of this object

  Object* Narray::clone (void) const {
    return new Narray (*this);
  }

  // return the serial did

  t_word Narray::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Narray::getsid (void) const {
    return SRL_NARY_SID;
  }

  // reset this array

  void Narray::reset (void) {
    wrlock ();
    try {
      // reset base class
      Nai::reset ();
      // reset locally
      delete [] p_bblk; p_bblk = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this array

  void Narray::clear (void) {
    wrlock ();
    try {
      t_long size = d_size * d_strd * Numeral::tobsiz (d_numt);
      Utility::tonull (p_bblk, size);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this array

  void Narray::resize (const t_long size) {
    wrlock ();
    try {
      // check for valid size
      if (size < 0) {
	throw Exception ("narray-error", "invalid resize parameters");
      }
      // check for nil size
      if (size == 0LL) {
	delete [] p_bblk; p_bblk = nullptr;
	d_size = 0LL;
	unlock ();
	return;
      }
      // normal resize array allocation
      t_long  bsiz = size * d_strd * Numeral::tobsiz (d_numt);
      t_byte* bblk = new t_byte[bsiz];
      Utility::tonull (bblk, bsiz);
      // check for original resize
      if ((p_bblk == nullptr) && (size > 0LL)) {
	p_bblk = bblk; d_size = size;
	unlock ();
	return;
      }
      // compute mininum sizes
      t_long msiz = min(d_size, size) * d_strd * Numeral::tobsiz (d_numt);
      // copy the block
      Utility::tobcpy (bblk, msiz, p_bblk);
      // update array and unlock
      delete [] p_bblk; p_bblk = bblk; d_size = size;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Narray::tosize (void) const {
    rdlock ();
    try {
      long result = d_size * d_strd * Numeral::tobsiz (d_numt);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Narray::tobyte (void) {
    wrlock ();
    try {
      t_byte* result = p_bblk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  const t_byte* Narray::tobyte (void) const {
    rdlock ();
    try {
      const t_byte* result = p_bblk;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - no lock section                                                       -
  // -------------------------------------------------------------------------

  // no lock - set a array by position

  void Narray::nlset (const t_long aidx, const long sidx, const Numeral& val) {
    t_long nidx = aidx * d_strd + sidx;
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      if (p_iblk != nullptr) p_iblk[nidx] = (int) val.tolong();
      break;
    case Numeral::NUMT_SFLT:
      if (p_fblk != nullptr) p_fblk[nidx] = (float) val.toreal();
      break;
    }
  }

  // set a array by position

  Numeral Narray::nlget (const t_long aidx, const long sidx) const {
    if (p_bblk == nullptr) return Numeral (d_numt);
    t_long nidx = aidx * d_strd + sidx;
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      return Numeral((t_long) p_iblk[nidx]);
      break;
    case Numeral::NUMT_SFLT:
      return Numeral((t_real) p_fblk[nidx]);
      break;
    }
    return Numeral(d_numt);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Narray::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Narray;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Narray (size);
    }
    // check for 2 arguments
    if (argc == 2) {
      t_long size = argv->getlong (0);
      long   strd = argv->getlong (1);
      return new Narray (size, strd);
    }
    // check for 3 arguments
    if (argc == 3) {
      t_long size = argv->getlong (0);
      long   strd = argv->getlong (1);
      Object* obj = argv->get (2);
      auto   item = dynamic_cast<Item*>(obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object with nblobk",
			 Object::repr (obj));
      }
      Numeral::t_numt numt = Numeral::tonumt(*item);
      return new Narray (size, strd, numt);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with narray object");
  }
}
