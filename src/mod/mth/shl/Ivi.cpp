// ---------------------------------------------------------------------------
// - Ivi.cpp                                                                 -
// - afnix:mth module - integer vector interface implementation              -
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

#include "Ivi.hpp"
#include "Math.hpp"
#include "Mthsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Utility.hpp"
#include "Algebra.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "OutputStream.hpp"
 
namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a vector by size

  Ivi::Ivi (const t_long size) : Avi (size) {
  }
  
  // copy construct this vector

  Ivi::Ivi (const Ivi& that) {
    that.rdlock ();
    try {
      Avi::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this vector

  Ivi::Ivi (Ivi&& that) noexcept {
    that.wrlock ();
    try {
      Avi::operator = (static_cast<Avi&&>(that));
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      that.unlock ();
    }
  }

  // assign a vector to this one

  Ivi& Ivi::operator = (const Ivi& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      Avi::operator = (that);
      that.unlock ();
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a vector into this one

  Ivi& Ivi::operator = (Ivi&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      Avi::operator = (static_cast<Avi&&>(that));
      unlock ();
      that.unlock ();
    } catch (...) {
      d_size = 0L;
      unlock ();
      that.unlock ();
    }
    return *this;
  }
  
  // serialize this object
  
  void Ivi::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // write the vector size/mode
      Serial::wrlong (d_size, os);
      // write the vector data
      for (long k = 0; k < d_size; k++) {
	// get the vector value
	long val = nlget (k);
	if (val == 0L) continue;
	// write by position
	Serial::wrlong (k,   os);
	Serial::wrlong (val, os);
      }
      // write marker for end
      Serial::wrlong (-1, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this object

  void Ivi::rdstream (InputStream& is) {
    wrlock ();
    try {
      // reset the vector
      reset ();
      // get the vector size/mode
      long size = Serial::rdlong (is);
      // resize the vector
      resize (size);
      // get the vector data by position
      for (long k = 0; k < size; k++) {
	long idx = Serial::rdlong (is);
	// check for marker
	if (idx == -1) {
	  unlock ();
	  return;
	}
	long val = Serial::rdlong (is);
	nlset (idx, val);
      }
      // get the remaining marker
      if (Serial::rdlong (is) != -1) {
	throw Exception ("ivi-error", "inconsistent serialized vector");
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this vector

  void Ivi::reset (void) {
    wrlock ();
    try {
      d_size = 0LL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this vector

  void Ivi::clear (void) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) nlset (i, 0);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare two vectors

  bool Ivi::operator == (const Ivi& x) const {
    rdlock ();
    x.rdlock ();
    try {
      bool result = Algebra::eql (*this, x);
      unlock ();
      x.unlock ();
      return result;
    } catch (...) {
      unlock ();
      x.unlock ();
      throw;
    }
  }

  // compare two vectors

  bool Ivi::operator != (const Ivi& x) const {
    return !(*this == x);
  }

  // add this vector by a scalar

  Ivi& Ivi::operator += (const long s) {
    return this->add (*this, s);
  }

  // add this vector with a vector

  Ivi& Ivi::operator += (const Ivi& x) {
    return this->aeq (x);
  }

  // substract this vector by a scalar

  Ivi& Ivi::operator -= (const long s) {
    return this->sub (*this, s);
  }

  // multiply this vector by a scalar

  Ivi& Ivi::operator *= (const long s) {
    return this->mul (*this, s);
  }

  // check if this vector is nil

  bool Ivi::isnil (void) const {
    rdlock ();
    try {
      bool result = Algebra::isnil (*this);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // copy a vector into this one

  Ivi& Ivi::cpy (const Ivi& x) {
    wrlock ();
    try {
      Algebra::cpy (*this, x);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // compare a vector value by position

  bool Ivi::cmp (const t_long pos, const long val) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      bool result = Math::acmp (nlget (pos), val);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a vector by value

  void Ivi::set (const long val) {
    wrlock ();
    try {
      for (t_long i = 0; i < d_size; i++) nlset (i, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a vector by position

  void Ivi::set (const t_long pos, const long val) {
    wrlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      nlset (pos, val);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a vector by position

  long Ivi::get (const t_long pos) const {
    rdlock ();
    try {
      if ((pos < 0) || (pos >= d_size)) {
	throw Exception ("index-error", "invalid vector position");
      }
      long result = nlget (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector with a scalar

  Ivi& Ivi::add (const Ivi& x, const long s) {
    wrlock ();
    try {
      Algebra::add (*this, x, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector with another one

  Ivi& Ivi::add (const Ivi& x, const Ivi& y) {
    wrlock ();
    try {
      Algebra::add (*this, x, y);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a vector with another scaled one

  Ivi& Ivi::add (const Ivi& x, const Ivi& y, const long s) {
    wrlock ();
    try {
      Algebra::add (*this, x, y, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // substract a vector with a scalar

  Ivi& Ivi::sub (const Ivi& x, const long s) {
    wrlock ();
    try {
      Algebra::sub (*this, x, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // substract a vector with another one

  Ivi& Ivi::sub (const Ivi& x, const Ivi& y) {
    wrlock ();
    try {
      Algebra::sub (*this, x, y);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // multiply a vector with a scaled factor

  Ivi& Ivi::mul (const Ivi& x, const long s) {
    wrlock ();
    try {
      Algebra::mul (*this, x, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // multiply a vector with another one

  Ivi& Ivi::mul (const Ivi& x, const Ivi& y) {
    wrlock ();
    try {
      Algebra::mul (*this, x, y);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add equal with a vector

  Ivi& Ivi::aeq (const Ivi& x) {
    wrlock ();
    try {
      Algebra::aeq (*this, x);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add equal with a scaled vector

  Ivi& Ivi::aeq (const Ivi& x, const long s) {
    wrlock ();
    try {
      Algebra::aeq (*this, x, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // rescale equal with a vector

  Ivi& Ivi::req (const Ivi& x, const long s) {
    wrlock ();
    try {
      Algebra::req (*this, x, s);
      unlock ();
      return *this;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // permutate this vector
  
  Ivi* Ivi::permutate (const Cpi& p) const {
    throw Exception ("ivi-error", "unimplemented vector permutate");
  }

  // reverse permutate this vector
  
  Ivi* Ivi::reverse (const Cpi& p) const {
    throw Exception ("ivi-error", "unimplemented vector reverse permutate");
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EQL       = zone.intern ("==");
  static const long QUARK_NEQ       = zone.intern ("!=");
  static const long QUARK_AEQ       = zone.intern ("+=");
  static const long QUARK_SEQ       = zone.intern ("-=");
  static const long QUARK_MEQ       = zone.intern ("*=");
  static const long QUARK_DEQ       = zone.intern ("/=");
  static const long QUARK_SET       = zone.intern ("set");
  static const long QUARK_GET       = zone.intern ("get");
  static const long QUARK_REVERSE   = zone.intern ("reverse");
  static const long QUARK_PERMUTATE = zone.intern ("permutate");

  // return true if the given quark is defined

  bool Ivi::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true){
	unlock ();
	return true;
      }
      bool result = hflg ? Avi::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // operate this object with another object

  Object* Ivi::oper (t_oper type, Object* object) {
    Ivi* vobj = dynamic_cast <Ivi*> (object);
    switch (type) {
    case Object::OPER_EQL:
      if (vobj != nullptr) return new Boolean (*this == *vobj);
      break;
    case Object::OPER_NEQ:
      if (vobj != nullptr) return new Boolean (*this != *vobj);
      break;
    default:
      break;
    }
    throw Exception ("type-error", "invalid operand with integer vector",
		     Object::repr (object));
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Ivi::apply (Evaluable* zobj, Nameset* nset, const long quark,
                      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EQL) return oper (Object::OPER_EQL, argv->get (0));
      if (quark == QUARK_NEQ) return oper (Object::OPER_NEQ, argv->get (0));
      if (quark == QUARK_GET) {
        t_long pos = argv->getlong (0);
        return new Integer (get (pos));
      }
      if (quark == QUARK_AEQ) {
	wrlock ();
	try {
	  Object* obj = argv->get (0);
	  // check for an integer
	  Integer* iobj = dynamic_cast <Integer*> (obj);
	  if (iobj != nullptr) {
	    long rval = iobj->tolong ();
	    *this += rval;
	    zobj->post (this);
	    unlock ();
	    return this;
	  }
	  // check for a vector
	  Ivi* vobj = dynamic_cast <Ivi*> (obj);
	  if (vobj != nullptr) {
	    *this += (*vobj);
	    zobj->post (this);
	    unlock ();
	    return this;
	  }
	  throw Exception ("type-error", "invalid object for vector aeq",
			   Object::repr (obj));
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SEQ) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  *this -= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_MEQ) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  *this *= val;
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_DEQ) {
	wrlock ();
	try {
	  long val = argv->getlong (0);
	  *this *= (1.0 / val);
	  zobj->post (this);
	  unlock ();
	  return this;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_PERMUTATE) {
	Object* obj = argv->get (0);
	Cpi* p = dynamic_cast <Cpi*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid object with permutate",
			   Object::repr (obj));
	}
	return permutate (*p);
      }
      if (quark == QUARK_REVERSE) {
	Object* obj = argv->get (0);
	Cpi* p = dynamic_cast <Cpi*> (obj);
	if (p == nullptr) {
	  throw Exception ("type-error", "invalid object with permutate",
			   Object::repr (obj));
	}
	return reverse (*p);
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
        t_long pos = argv->getlong (0);
        long val = argv->getrint (1);
	set (pos, val);
        return nullptr;
      }
    }
    // call the avi object
    return Avi::apply (zobj, nset, quark, argv);
  }
}

