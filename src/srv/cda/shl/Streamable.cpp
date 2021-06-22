// ---------------------------------------------------------------------------
// - Streamable.cpp                                                          -
// - afnix:cda service - streamable interface implementation                 -
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
#include "Math.hpp"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Streamable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default streamable

  Streamable::Streamable (void) {
  }
  
  // copy move this streamable

  Streamable::Streamable (Streamable&& that) noexcept :
    Object (static_cast<Object&&>(that)) {
  }
  
  // move this streamable into this one

  Streamable& Streamable::operator = (Streamable&& that) noexcept {
    Object::operator = (static_cast<Object&&>(that));
    return *this;
  }

  // get the streamer departure position

  t_real Streamable::departure (void) const {
    rdlock ();
    try {
      t_real result = 0.0;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the streamer arrival position

  t_real Streamable::arrival (void) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer position

  t_real Streamable::locate (void) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the streamer to the departure position

  t_real Streamable::begin (void) {
    wrlock ();
    try {
      // get the departure position
      t_real dp = departure ();
      // move to departure
      t_real result = move (dp);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // move the streamer to the arrival position

  t_real Streamable::end (void) {
    wrlock ();
    try {
      // get the end position
      t_real ap = arrival ();
      // move to arrival
      t_real result = move (ap);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the streamer to the next position

  t_real Streamable::next (void) {
    wrlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer to the next position

  t_real Streamable::next (const t_real incr) {
    wrlock ();
    try {
      // compute move position
      t_real pos = locate () + incr;
      // move to the desired position
      t_real result = move (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer to the previous position

  t_real Streamable::prev (void) {
    wrlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer to the previous position

  t_real Streamable::prev (const t_real decr) {
    wrlock ();
    try {
      // compute move position
      t_real pos = locate () - decr;
      // move to the desired position
      t_real result = move (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer to a position

  t_real Streamable::move (const t_real pos) {
    wrlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer boolean value

  bool Streamable::getbool (void) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer boolean value by position

  bool Streamable::getbool (const t_real pos) {
    wrlock ();
    try {
      move (pos);
      bool result = getbool ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer integer value

  long Streamable::getlong (void) const {
    rdlock ();
    try {
      long result = 0L;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the stream integer value by position

  long Streamable::getlong (const t_real pos) {
    wrlock ();
    try {
      move (pos);
      long result = getlong ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the stream real value

  t_real Streamable::getreal (void) const {
    rdlock ();
    try {
      t_real result = Math::CV_NAN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the stream real value by position

  t_real Streamable::getreal (const t_real pos) {
    wrlock ();
    try {
      move (pos);
      t_real result = getreal ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // forward the streamer to the next boolean

  t_real Streamable::forward (const bool pval) {
    wrlock ();
    try {
      // move until end
      while (next () < arrival ()) {
	if (getbool () == pval) break;
      }
      // locate the position
      t_real result = locate ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // forward the streamer to the next integer

  t_real Streamable::forward (const long pval) {
    wrlock ();
    try {
      // get the streamable value
      long sval = getlong ();
      // move until end
      while (next () < arrival ()) {
	// get the forward value
	long fval = getlong ();
	// check for equality
	if (fval == pval) break;
	// check for ascending
	if ((fval > sval) && (fval > pval)) {
	  prev ();
	  break;
	}
	// check for descending
	if ((fval < sval) && (fval < pval)) {
	  prev ();
	  break;
	}
	// update streamable value
	sval = fval;
      }
      // locate the position
      t_real result = locate ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // forward the streamer to the next real

  t_real Streamable::forward (const t_real pval) {
    wrlock ();
    try {
      // get the streamable value
      t_real sval = getreal ();
      // move until end
      while (next () < arrival ()) {
	// get the forward value
	t_real fval = getreal ();
	// check for equality
	if (fval == pval) break;
	// check for ascending
	if ((fval > sval) && (fval > pval)) {
	  prev ();
	  break;
	}
	// check for descending
	if ((fval < sval) && (fval < pval)) {
	  prev ();
	  break;
	}
	// update streamable value
	sval = fval;
      }
      // locate the position
      t_real result = locate ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // backward the streamer to the previous boolean

  t_real Streamable::backward (const bool pval) {
    wrlock ();
    try {
      // move until departure
      while (prev () > departure ()) {
	if (getbool () == pval) break;
      }
      // locate the position
      t_real result = locate ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // backward the streamer to the previous integer

  t_real Streamable::backward (const long pval) {
    wrlock ();
    try {
      // get the streamable value
      long sval = getlong ();
      // move until departure
      while (prev () > departure ()) {
	// get the backward value
	long bval = getlong ();
	// check for equality
	if (bval == pval) break;
	// check for descending
	if ((sval > bval) && (pval > bval)) break;
	// check for ascending
	if ((sval < bval) && (pval < bval)) break;
	// update streamable value
	sval = bval;
      }
      // locate the position
      t_real result = locate ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // backward the streamer to the previous real

  t_real Streamable::backward (const t_real pval) {
    wrlock ();
    try {
      // get the streamable value
      t_real sval = getreal ();
      // move until departure
      while (prev () > departure ()) {
	// get the backward value
	t_real bval = getreal ();
	// check for equality
	if (bval == pval) break;
	// check for descending
	if ((sval > bval) && (pval > bval)) break;
	// check for ascending
	if ((sval < bval) && (pval < bval)) break;
	// update streamable value
	sval = bval;
      }
      // locate the position
      t_real result = locate ();
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
  static const long QUARK_ZONE_LENGTH = 14;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_END     = zone.intern ("end");
  static const long QUARK_NEXT    = zone.intern ("next");
  static const long QUARK_PREV    = zone.intern ("previous");
  static const long QUARK_MOVE    = zone.intern ("move");
  static const long QUARK_FWARD   = zone.intern ("forward");
  static const long QUARK_BWARD   = zone.intern ("backward");
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_BEGIN   = zone.intern ("begin");
  static const long QUARK_LOCATE  = zone.intern ("locate");
  static const long QUARK_DEPART  = zone.intern ("departure");
  static const long QUARK_ARRIVAL = zone.intern ("arrival");
  static const long QUARK_GETREAL = zone.intern ("get-real");
  static const long QUARK_GETBOOL = zone.intern ("get-boolean");
  static const long QUARK_GETLONG = zone.intern ("get-integer");

  // return true if the given quark is defined

  bool Streamable::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Object::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Streamable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_END)     return new Real (end   ());
      if (quark == QUARK_NEXT)    return new Real (next  ());
      if (quark == QUARK_PREV)    return new Real (prev  ());
      if (quark == QUARK_BEGIN)   return new Real (begin ());
      if (quark == QUARK_DEPART)  return new Real (departure ());
      if (quark == QUARK_LOCATE)  return new Real (locate ());
      if (quark == QUARK_ARRIVAL) return new Real (arrival ());
      if (quark == QUARK_GETBOOL) return new Boolean (getbool ());
      if (quark == QUARK_GETLONG) return new Integer (getlong ());
      if (quark == QUARK_GETREAL) return new Real (getreal ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_NEXT) {
	t_real incr = argv->getreal(0);
	return new Real (next (incr));
      }
      if (quark == QUARK_PREV) {
	t_real decr = argv->getreal(0);
	return new Real (prev (decr));
      }
      if (quark == QUARK_MOVE) {
	t_real pos = argv->getreal(0);
	return new Real (move (pos));
      }
      if (quark == QUARK_GETREAL) {
	t_real pos = argv->getreal(0);
	return new Real (getreal (pos));
      }
      if (quark == QUARK_GETBOOL) {
	t_real pos = argv->getreal(0);
	return new Boolean (getbool (pos));
      }
      if (quark == QUARK_GETLONG) {
	t_real pos = argv->getreal(0);
	return new Integer (getlong (pos));
      }
      if (quark == QUARK_FWARD) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Real (forward (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Real (forward ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Real (forward (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for forward",
			 Object::repr (obj));
      }
      if (quark == QUARK_BWARD) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Real (backward (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Real (backward ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Real (backward (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for backward",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
