// ---------------------------------------------------------------------------
// - Permute.cpp                                                             -
// - afnix:mth module - combinatoric permutation object implementation       -
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

#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Permute.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default permutation

  Permute::Permute (void) {
    d_size = 0LL;
    p_prow = nullptr;
    p_revp = nullptr;
  }

  // create a permutation by size

  Permute::Permute (const t_long size) : Cpi (size) {
    p_prow = new t_long[d_size];
    p_revp = new t_long[d_size];
    clear ();
  }

  // copy construct this object

  Permute::Permute (const Permute& that) {
    that.rdlock ();
    try {
      p_prow = (p_prow == nullptr) ? nullptr : new t_long[d_size];
      p_revp = (p_revp == nullptr) ? nullptr : new t_long[d_size];
      for (t_long k = 0LL; k < d_size; k++) {
	p_prow[k] = that.p_prow[k];
	p_revp[k] = that.p_revp[k];
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this object

  Permute::~Permute (void) {
    delete [] p_prow;
    delete [] p_revp;
  }

  // assign an object to this one

  Permute& Permute::operator = (const Permute& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      p_prow = (p_prow == nullptr) ? nullptr : new t_long[d_size];
      p_revp = (p_revp == nullptr) ? nullptr : new t_long[d_size];
      for (t_long k = 0LL; k < d_size; k++) {
	p_prow[k] = that.p_prow[k];
	p_revp[k] = that.p_revp[k];
      }
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // return the class name

  String Permute::repr (void) const {
    return "Permute";
  }

  // return a clone of this object

  Object* Permute::clone (void) const {
    return new Permute (*this);
  }

  // return the serial did
  
  t_word Permute::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Permute::getsid (void) const {
    return SRL_PRMT_SID;
  }
  
  // reset this permutation

  void Permute::reset (void) {
    wrlock ();
    try {
      Cpi::reset ();
      delete [] p_prow; p_prow = nullptr;
      delete [] p_revp; p_revp = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this permutation

  void Permute::clear (void) {
    wrlock ();
    try {
      for (t_long k = 0; k < d_size; k++) p_revp[k] = p_prow[k] = k;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // resize this permutation

  void Permute::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
        throw Exception ("permute-error", "invalid negatize size in resize");
      }
      // do nothing if equal
      if (size == d_size) {
        unlock ();
        return;
      }
      // clean old array
      delete [] p_prow;
      delete [] p_revp;
      d_size = 0;
      p_prow = nullptr;
      p_revp = nullptr;
      // reset with null size
      if (size == 0) {
        unlock ();
        return;
      }
      // create a new array by size
      d_size = size;
      p_prow = new t_long[d_size];
      p_revp = new t_long[d_size];
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - no lock section                                                        -
  // -------------------------------------------------------------------------

  // no lock - set the permutation by position

  void Permute::nlset (const t_long pos, const t_long val) {
    if (p_prow != nullptr) p_prow[pos] = val;
    if (p_revp != nullptr) p_revp[val] = pos;
  }

  // no lock - get the permutation by position

  t_long Permute::nlget (const t_long pos) const {
    return (p_prow == nullptr) ? -1L : p_prow[pos];
  }

  // no lock - get the reverse permutation by position

  t_long Permute::nlrget (const t_long pos) const {
    return (p_revp == nullptr) ? -1L : p_revp[pos];
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Permute::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Permute;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Permute (size);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with real permutation object");
  }
}

