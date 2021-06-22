// ---------------------------------------------------------------------------
// - Nvector.cpp                                                             -
// - afnix:mth module - numeral vector implementation                        -
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
#include "Nvector.hpp"
#include "Algebra.hpp"
#include "Utility.hpp"
#include "Exception.hpp"
 
namespace afnix {
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // generate a random vector by size

  Nvector* Nvector::random (const t_long   size, 
			   const Numeral& nmin, const Numeral& nmax) {
    // create a vector by size
    Nvector* result = new Nvector(size);
    // fill the vector
    Algebra::random (*result, nmin, nmax);
    // done
    return  result;
  }
    
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default vector

  Nvector::Nvector (void) {
    p_btab = nullptr;
  }

  // create a vector by size

  Nvector::Nvector (const t_long size) : Nvi (size) {
    p_btab = nullptr;
    preset ();
  }
  
  // create a vector by size and type

  Nvector::Nvector (const t_long size,
		    const Numeral::t_numt numt) : Nvi (size, numt) {
    p_btab = nullptr;
    preset ();
  }
  
  // copy construct this vector

  Nvector::Nvector (const Nvector& that) {
    that.rdlock ();
    try {
      // copy base class
      Nvi::operator = (that);
      // copy locally
      p_btab = nullptr;
      t_long size = d_size * Numeral::tobsiz(d_numt);
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
	
  // destroy this vector

  Nvector::~Nvector (void) {
    delete [] p_btab;
  }

  // assign a vector to this one

  Nvector& Nvector::operator = (const Nvector& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // clean locally
      delete [] p_btab; p_btab = nullptr;
      // assign base class
      Nvi::operator = (that);
      // assign locally
      if (d_size > 0) {
	long size = d_size * Numeral::tobsiz(d_numt);
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

  String Nvector::repr (void) const {
    return "Nvector";
  }

  // return a clone of this object

  Object* Nvector::clone (void) const {
    return new Nvector (*this);
  }
  
  // return the serial did

  t_word Nvector::getdid (void) const {
    return SRL_DEOD_MTH;
  }

  // return the serial sid

  t_word Nvector::getsid (void) const {
    return SRL_NVEC_SID;
  }

  // reset this vector

  void Nvector::reset (void) {
    wrlock ();
    try {
      // reset base
      Nvi::reset ();
      // reset locally
      d_size = 0L;
      delete [] p_btab; p_btab = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this vector

  void Nvector::clear (void) {
    wrlock ();
    try {
      t_long size = d_size * Numeral::tobsiz (d_numt);
      for (t_long k = 0L; k < size; k++) p_btab[k] = nilb;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // resize this vector

  void Nvector::resize (const t_long size) {
    wrlock ();
    try {
      if (size < 0) {
	throw Exception ("rvector-error", "invalid negatize size in resize");
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
      t_long  bsiz = size * Numeral::tobsiz (d_numt);
      t_byte* btab = new t_byte[bsiz];
      // check for smaller size
      if (size < d_size) {
	Utility::tobcpy (btab, bsiz, p_btab);
      } else {
	t_long osiz = d_size * Numeral::tobsiz (d_numt);
	Utility::tobcpy (btab, osiz, p_btab);
	for (long k = osiz; k < bsiz; k++) btab[k] = nilb;
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

  // preset this vector

  void Nvector::preset (void) {
    wrlock ();
    try {
      delete [] p_btab;
      t_long size = d_size * Numeral::tobsiz (d_numt);
      p_btab = (size == 0LL) ? nullptr : new t_byte[size];
      clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable size

  long Nvector::tosize (void) const {
    rdlock ();
    try {
      long result = d_size * Numeral::tobsiz (d_numt);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the viewable data

  t_byte* Nvector::tobyte (void) {
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

  const t_byte* Nvector::tobyte (void) const {
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

  // convert a vector into another type

  Numeral::t_numt Nvector::convert (const Numeral::t_numt numt) {
    wrlock ();
    try {
      // check for same type
      if (d_numt == numt) {
	unlock ();
	return numt;
      }
      // check for integer
      if (d_numt == Numeral::NUMT_SINT) {
	if (numt == Numeral::NUMT_SFLT) {
	  for (long k = 0L; k < d_size; k++) p_ftab[k] = (float) p_itab[k];
	}
      }
      // check for float
      if (d_numt == Numeral::NUMT_SFLT) {
	if (numt == Numeral::NUMT_SINT) {
	  for (long k = 0L; k < d_size; k++) p_itab[k] = (int) p_ftab[k];
	}
      }
      Numeral::t_numt result = d_numt;
      d_numt = numt;
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

  // no lock - set a vector by position

  void Nvector::nlset (const t_long pos, const Numeral& val) {
    switch (d_numt) {
    case Numeral::NUMT_SINT:
      if (p_itab != nullptr) p_itab[pos] = (int) val.tolong();
      break;
    case Numeral::NUMT_SFLT:
      if (p_ftab != nullptr) p_ftab[pos] = (float) val.toreal();
      break;
    }
  }

  // no lock - get a vector by position
  
  Numeral Nvector::nlget (const t_long pos) const {
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

  Object* Nvector::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new Nvector;
    // check for 1 argument
    if (argc == 1) {
      t_long size = argv->getlong (0);
      return new Nvector (size);
    }
    // check for 2 arguments
    if (argc == 2) {
      t_long size = argv->getlong (0);
      Object* obj = argv->get (1);
      auto   item = dynamic_cast<Item*>(obj);
      if (item == nullptr) {
	throw Exception ("type-error", "invalid object with nvector",
			 Object::repr (obj));
      }
      Numeral::t_numt numt = Numeral::tonumt(*item);
      return new Nvector (size, numt);
    }
    // invalid arguments
    throw Exception ("argument-error", 
		     "invalid arguments with numeral vector object");
  }
}

