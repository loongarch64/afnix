// ---------------------------------------------------------------------------
// - Npoint.cpp                                                             -
// - afnix:mth module - numeral point implementation                        -
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
#include "Npoint.hpp"
#include "Algebra.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default point

  Npoint::Npoint (void) {
    p_btab = nullptr;
  }

  // create a point by size

  Npoint::Npoint (const t_long size) : Npi (size) {
    p_btab = (size == 0L) ? nullptr : new t_byte[size*4];
    clear ();
  }

  // create a point by size and type

  Npoint::Npoint (const t_long size,
		  const Numeral::t_numt numt) : Npi (size, numt) {
    p_btab = (size == 0L) ? nullptr : new t_byte[size*4];
    clear ();
  }
  
  // copy construct this point

  Npoint::Npoint (const Npoint& that) {
    that.rdlock ();
    try {
      // copy base class
      Npi::operator = (that);
      // copy locally
      p_btab = nullptr;
      t_long size = d_size * 4;
      if (size > 0LL) {
	p_btab = new t_byte[size];
	Utility::tobcpy (p_btab, size, that.p_btab);
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
	
  // destroy this point

  Npoint::~Npoint (void) {
    delete [] p_btab;
  }

  // assign a point to this one

  Npoint& Npoint::operator = (const Npoint& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean locally
      delete [] p_btab; p_btab = nullptr;
      // assign base class
      Npi::operator = (that);
      // assign locally
      if (d_size > 0) {
	long size = d_size * 4;
	p_btab = new t_byte[size];
	Utility::tobcpy (p_btab, size, that.p_btab);
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

  String Npoint::repr (void) const {
    return "Npoint";
  }

  // return a clone of this object

  Object* Npoint::clone (void) const {
    return new Npoint (*this);
  }
  
  // return the serial did

  t_word Npoint::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Npoint::getsid (void) const {
    return SRL_NVEC_SID;
  }

  // reset this point

  void Npoint::reset (void) {
    wrlock ();
    try {
      // reset base
      Npi::reset ();
      // reset locally
      d_size = 0L;
      delete [] p_btab; p_btab = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this point

  void Npoint::clear (void) {
    wrlock ();
    try {
      for (t_long k = 0L; k < d_size*4; k++) p_btab[k] = nilb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this point

  void Npoint::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
	throw Exception ("rpoint-error", "invalid negatize size in resize");
      }
      if (size == 0)  {
	delete [] p_btab;
	d_size = 0L;
	p_btab = nullptr;
	unlock ();
	return;
      }
      // do nothing if equal
      if (size == d_size) {
	unlock ();
	return;
      }
      // create a new array by size
      t_byte* btab = new t_byte[size*4];
      // check for smaller size
      if (size < d_size) {
	Utility::tobcpy (btab, size*4, p_btab);
      } else {
	Utility::tobcpy (btab, d_size*4, p_btab);
	for (long k = d_size*4; k < size*4; k++) btab[k] = nilb;
      }
      d_size = size;
      delete [] p_btab;
      p_btab = btab;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Npoint::tosize (void) const {
    rdlock ();
    try {
      long result = d_size * 4L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Npoint::tobyte (void) {
    wrlock ();
    try {
      t_byte* result = p_btab;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the viewable data

  const t_byte* Npoint::tobyte (void) const {
    rdlock ();
    try {
      const t_byte* result = p_btab;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // -------------------------------------------------------------------------
  // - no lock section                                                        -
  // -------------------------------------------------------------------------

  // no lock - set a point by position

  void Npoint::nlset (const t_long pos, const Numeral& val) {
    switch (val.getnumt ()) {
    case Numeral::NUMT_SINT:
      if (p_itab != nullptr) p_itab[pos] = (int) val.tolong();
      break;
    case Numeral::NUMT_SFLT:
      if (p_ftab != nullptr) p_ftab[pos] = (float) val.toreal();
      break;
    }
  }

  // no lock - get a point by position
  
  Numeral Npoint::nlget (const t_long pos) const {
    if (p_btab == nullptr) return Numeral (d_numt);
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      return Numeral((t_long) p_itab[pos]);
      break;
    case Numeral::NUMT_SFLT:
      return Numeral((t_real) p_ftab[pos]);
      break;
    }
    return Numeral(d_numt);
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // create a new object in a generic way

  Object* Npoint::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Npoint;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Npoint (size);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with numeral point object");
  }
}

