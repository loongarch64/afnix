// ---------------------------------------------------------------------------
// - Nblock.cpp                                                              -
// - afnix:mth module - numeral block matrix implementation                  -
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
#include "Nblock.hpp"
#include "Utility.hpp"
#include "Algebra.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure computes the minimum of two integers
  static inline t_long min (const t_long x, const t_long y) {
    return (x < y) ? x : y;
  }

  // this procedure compute a block size by geomtry and padding
  static t_long nbk_tosize (const long rsiz, const long csiz, const long lpad,
			    const long bpad, const Numeral::t_numt numt,
			    const bool cord) {
    t_long size = bpad;
    if (cord == true) {
      size += csiz * (rsiz + lpad) * Numeral::tobsiz (numt);
    } else {
      size += rsiz * (csiz + lpad) * Numeral::tobsiz (numt);
    }
    return size;
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a square random block by size

  Nblock* Nblock::random (const t_long   size, const Numeral& rmin,
			  const Numeral& rmax) {
    // create a block by size
    Nblock* result = new Nblock (size);
    // fill the block
    Algebra::random (*result, rmin, rmax);
    // here it is
    return result;
  }

  // generate a random block by size

  Nblock* Nblock::random (const t_long   rsiz, const t_long   csiz,
			  const Numeral& rmin, const Numeral& rmax) {
    // create a block by size
    Nblock* result = new Nblock (rsiz, csiz);
    // fill the block
    Algebra::random (*result, rmin, rmax);
    // here it is
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default block

  Nblock::Nblock (void) {
    p_bblk = nullptr;
    d_cord = false;
    d_lpad = 0L;
    d_bpad = 0L;
  }

  // create a square block by size

  Nblock::Nblock (const t_long size) : Nmi (size) {
    p_bblk = nullptr;
    d_cord = false;
    d_lpad = 0L;
    d_bpad = 0L;
    resize (size, size);
  }

  // create a block by size

  Nblock::Nblock (const t_long rsiz, const t_long csiz) : Nmi (rsiz, csiz) {
    p_bblk = nullptr;
    d_cord = false;
    d_lpad = 0L;
    d_bpad = 0L;
    resize (rsiz, csiz);    
  }

  // create a block by size and type

  Nblock::Nblock (const t_long rsiz, const t_long csiz,
		  const Numeral::t_numt numt) : Nmi (rsiz, csiz, numt) {
    p_bblk = nullptr;
    d_cord = false;
    d_lpad = 0L;
    d_bpad = 0L;
    resize (rsiz, csiz);    
  }

  // create a block by size, type and order flag

  Nblock::Nblock (const t_long size, const Numeral::t_numt numt,
		  const bool   cord) : Nmi (size, size, numt) {
    p_bblk = nullptr;
    d_cord = cord;
    d_lpad = 0L;
    d_bpad = 0L;
    resize (size, size);    
  }
  
  // copy construct this block

  Nblock::Nblock (const Nblock& that) {
    that.rdlock ();
    try {
      // copy base class
      Nmi::operator = (that);
      // copy locally
      p_bblk = nullptr;
      d_cord = that.d_cord;
      d_lpad = that.d_lpad;
      d_bpad = that.d_bpad;
      t_long size = nbk_tosize (d_rsiz, d_csiz, d_lpad, d_bpad, d_numt, d_cord);
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
	
  // destroy this block

  Nblock::~Nblock (void) {
    delete [] p_bblk;
  }

  // assign a block to this one

  Nblock& Nblock::operator = (const Nblock& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // delete the old block
      delete [] p_bblk; p_bblk = nullptr;
      // assign base class
      Nmi::operator = (that);
      // assign locally
      d_cord = that.d_cord;
      d_lpad = that.d_lpad;
      d_bpad = that.d_bpad;
      t_long size = nbk_tosize (d_rsiz, d_csiz, d_lpad, d_bpad, d_numt, d_cord);
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

  String Nblock::repr (void) const {
    return "Nblock";
  }

  // return a clone of this object

  Object* Nblock::clone (void) const {
    return new Nblock (*this);
  }

  // return the serial did

  t_word Nblock::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Nblock::getsid (void) const {
    return SRL_NBLK_SID;
  }

  // reset this block

  void Nblock::reset (void) {
    wrlock ();
    try {
      // reset base class
      Nmi::reset ();
      // reset locally
      d_lpad = 0L;
      d_bpad = 0L;
      delete [] p_bblk; p_bblk = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this block

  void Nblock::clear (void) {
    wrlock ();
    try {
      t_long size = nbk_tosize (d_rsiz, d_csiz, d_lpad, d_bpad, d_numt, d_cord);
      for (t_long k = 0LL; k < size; k++) p_bblk[k] = nilb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this block

  void Nblock::resize (const t_long rsiz, const t_long csiz) {
    wrlock ();
    try {
      // check for valid size
      if ((rsiz < 0) || (csiz < 0)) {
	throw Exception ("nblock-error", "invalid resize parameters");
      }
      // check for nil size
      if ((rsiz == 0LL) || (csiz == 0LL)) {
	delete [] p_bblk; p_bblk = nullptr;
	d_rsiz = rsiz; d_csiz = csiz;
	unlock ();
	return;
      }
      // normal resize block allocation
      t_long  size = nbk_tosize (rsiz, csiz, d_lpad, d_bpad, d_numt, d_cord);
      t_byte* bblk = new t_byte[size];
      for (long k = 0LL; k < size; k++) bblk[k] = nilb;
      //  check for original resize
      if ((p_bblk == nullptr) && (rsiz > 0LL) && (csiz > 0LL)) {
	p_bblk = bblk; d_rsiz = rsiz; d_csiz = csiz;
	unlock ();
	return;
      }      
      // compute mininum sizes
      t_long rmin = min (d_rsiz, rsiz);
      t_long cmin = min (d_csiz, csiz);
      // copy the integer array
      if (d_numt == Numeral::NUMT_SINT) {
	auto iblk = reinterpret_cast<int*>(bblk);
	if (d_cord == false) {
	  for (t_long i = 0LL; i < rmin; i++) {
	    for (t_long j = 0LL; j < cmin; j++) {
	      long lpos = i * (csiz   + d_lpad) + j;
	      long rpos = i * (d_csiz + d_lpad) + j;
	      iblk[lpos] = p_iblk[rpos];
	    }
	  }
	} else {
	  for (t_long j = 0LL; j < cmin; j++) {
	    for (t_long i = 0LL; i < rmin; i++) {
	      long lpos = j * (rsiz   + d_lpad) + i;
	      long rpos = j * (d_rsiz + d_lpad) + i;
	      iblk[lpos] = p_iblk[rpos];
	    }
	  }
	}	  
      }
      if (d_numt == Numeral::NUMT_SFLT) {
	auto fblk = reinterpret_cast<float*>(bblk);
	if (d_cord == false) {
	  for (t_long i = 0LL; i < rmin; i++) {
	    for (t_long j = 0LL; j < cmin; j++) {
	      long lpos = i * (csiz   + d_lpad) + j;
	      long rpos = i * (d_csiz + d_lpad) + j;
	      fblk[lpos] = p_fblk[rpos];
	    }
	  }
	} else {
	  for (t_long j = 0LL; j < cmin; j++) {
	    for (t_long i = 0LL; i < rmin; i++) {
	      long lpos = j * (rsiz   + d_lpad) + i;
	      long rpos = j * (d_rsiz + d_lpad) + i;
	      fblk[lpos] = p_fblk[rpos];
	    }
	  }
	}
      }
      // update block and unlock
      delete [] p_bblk; p_bblk = bblk; d_rsiz = rsiz; d_csiz = csiz;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Nblock::tosize (void) const {
    rdlock ();
    try {
      long result = nbk_tosize (d_rsiz, d_csiz, d_lpad, d_bpad, d_numt, d_cord);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Nblock::tobyte (void) {
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

  const t_byte* Nblock::tobyte (void) const {
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

  // no lock - set a block by position

  void Nblock::nlset (const t_long row, const t_long col, const Numeral& val) {
    t_long pos = d_cord
      ? col * (d_rsiz + d_lpad) + row
      : row * (d_csiz + d_lpad) + col;
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      if (p_iblk != nullptr) p_iblk[pos] = (int) val.tolong();
      break;
    case Numeral::NUMT_SFLT:
      if (p_fblk != nullptr) p_fblk[pos] = (float) val.toreal();
      break;
    }
  }

  // set a block by position

  Numeral Nblock::nlget (const t_long row, const t_long col) const {
    if (p_bblk == nullptr) return Numeral (d_numt);
    t_long pos = d_cord
      ? col * (d_rsiz + d_lpad) + row
      : row * (d_csiz + d_lpad) + col;
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      return Numeral((t_long) p_iblk[pos]);
      break;
    case Numeral::NUMT_SFLT:
      return Numeral((t_real) p_fblk[pos]);
      break;
    }
    return Numeral(d_numt);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Nblock::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Nblock;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Nblock (size);
    }
    // check for 2 arguments
    if (argc == 2) {
      t_long rsiz = argv->getlong (0);
      t_long csiz = argv->getlong (1);
      return new Nblock (rsiz, csiz);
    }
    // check for 3 arguments
    if (argc == 3) {
      t_long rsiz = argv->getlong (0);
      t_long csiz = argv->getlong (1);
      Object* obj = argv->get (2);
      auto   item = dynamic_cast<Item*>(obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object with nblobk",
			 Object::repr (obj));
      }
      Numeral::t_numt numt = Numeral::tonumt(*item);
      return new Nblock (rsiz, csiz, numt);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with nblock object");
  }
}
