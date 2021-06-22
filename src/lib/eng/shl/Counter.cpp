// ---------------------------------------------------------------------------
// - Counter.cpp                                                             -
// - afnix engine - counter class implementation                             -
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

#include "Vector.hpp"
#include "Engsid.hxx"
#include "Counter.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default counter

  Counter::Counter (void) {
    d_sval = 0;
    d_eval = 0;
    d_cdir = true;
    d_cval = 0;
  }

  // create a new counter by value

  Counter::Counter (const t_long cval) {
    if (cval < 0) {
      d_sval = cval;
      d_eval = 0;
    } else {
      d_sval = 0;
      d_eval = cval;
    }
    d_cdir = true;
    d_cval = d_sval;
  }

  // create a new counter by start and end values

  Counter::Counter (const t_long sval, const t_long eval) {
    d_sval = sval;
    d_eval = eval;
    d_cdir = (sval < eval) ? true : false;
    d_cval = sval;
  }

  // copy construct this counter

  Counter::Counter (const Counter& that) {
    that.rdlock ();
    try {
      d_sval = that.d_sval;
      d_eval = that.d_eval;
      d_cdir = that.d_cdir;
      d_cval = that.d_cval;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a counter to this one

  Counter& Counter::operator = (const Counter& that) {
    wrlock ();
    that.rdlock ();
    try {
      d_sval = that.d_sval;
      d_eval = that.d_eval;
      d_cdir = that.d_cdir;
      d_cval = that.d_cval;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // pre-increment operator

  t_long Counter::operator ++ (void) {
    wrlock ();
    try {
      t_long result = d_cval;
      if (step () == true) result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // post-increment operator

  t_long Counter::operator ++ (int) {
    wrlock ();
    try {
      t_long result = d_cval;
      step ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  
  // return the class name

  String Counter::repr (void) const {
    return "Counter";
  }

  // return a clone of this object

  Object* Counter::clone (void) const {
    return new Counter (*this);
  }

  // clear this counter

  void Counter::clear (void) {
    wrlock ();
    try {
      d_sval = 0LL;
      d_eval = 0LL;
      d_cdir = true;
      d_cval = 0L;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this counter

  String Counter::toliteral (void) const {
    return tostring ();
  }

  // get a string representation on this counter

  String Counter::tostring (void) const {
    rdlock ();
    try {
      String result = Utility::tostring (d_cval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the serial did

  t_word Counter::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Counter::getsid (void) const {
    return SRL_CNTR_SID;
  }
  
  // serialize this counter

  void Counter::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Serial::wrlong (d_sval, os);
      Serial::wrlong (d_eval, os);
      Serial::wrbool (d_cdir, os);
      Serial::wrlong (d_cval, os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this counter

  void Counter::rdstream (InputStream& is) {
    wrlock ();
    try {
      // read the counter stream
      Integer sval; sval.rdstream (is);
      Integer eval; eval.rdstream (is);
      Boolean cdir; cdir.rdstream (is);
      Integer cval; cval.rdstream (is);
      // map the integer values
      d_sval = Serial::rdlong (is);
      d_eval = Serial::rdlong (is);
      d_cdir = Serial::rdbool (is);
      d_cval = Serial::rdlong (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset this counter

  void Counter::reset (void) {
    wrlock ();
    try {
      d_cval = d_sval;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the counter is valid

  bool Counter::valid (void) const {
    rdlock ();
    try {
      bool result = d_cdir ? (d_cval < d_eval) : (d_cval > d_eval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // step the counter and return true if done

  bool Counter::step (void) {
    wrlock ();
    try {
      // check if we can step
      bool result = d_cdir ? (d_cval < d_eval) : (d_cval > d_eval);
      // step the counter if possible
      if (result == true) d_cval = d_cdir ? d_cval + 1LL : d_cval - 1LL;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the counter value

  t_long Counter::get (void) const {
    rdlock ();
    try {
      t_long result = d_cval;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GET     = zone.intern ("get");
  static const long QUARK_INCR    = zone.intern ("++");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_STEPP   = zone.intern ("step-p");
  static const long QUARK_VALIDP  = zone.intern ("valid-p");
  
  // create a new object in a generic way

  Object* Counter::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check 0 argument
    if (argc == 0) return new Counter;
    // check 1 argument
    if (argc == 1) {
      t_long cval = argv->getlong (0);
      return new Counter (cval);
    }
    // check for 2 arguments
    if (argc == 2) {
      t_long sval = argv->getlong (0);
      t_long eval = argv->getlong (1);
      return new Counter (sval, eval);
    }
    throw Exception ("argument-error", "too many argument for counter");
  }

  // return true if the given quark is defined
  
  bool Counter::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Literal::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }
  
  // apply this object class with a set of arguments and a quark
  
  Object* Counter::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GET)     return new Integer (get     ());
      if (quark == QUARK_STEPP)   return new Boolean (step    ());
      if (quark == QUARK_VALIDP)  return new Boolean (valid   ());
      if (quark == QUARK_INCR)    return new Integer ((*this)++);
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // apply these arguments with the object
    return Literal::apply (zobj, nset, quark, argv);
  }
}
