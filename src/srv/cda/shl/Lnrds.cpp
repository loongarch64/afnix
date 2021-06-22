// ---------------------------------------------------------------------------
// - Lnrds.cpp                                                               -
// - afnix:sps module - linear interpolation streamer class implementation   -
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
#include "Lnrds.hpp"
#include "Vector.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default streamer

  Lnrds::Lnrds (void) {
    p_strm = nullptr;
    reset ();
  }

  // create a streamer by streamable

  Lnrds::Lnrds (Streamable* strm) {
    Object::iref (p_strm = strm);
    reset ();
  }
  
  // delete this streamer

  Lnrds::~Lnrds (void) {
    Object::dref (p_strm);
  }

  // return the object name

  String Lnrds::repr (void) const {
    return "Lnrds";
  }

  // reset the streamer

  void Lnrds::reset (void) {
    wrlock ();
    try {
      // call plugin reset
      if (p_strm != nullptr) p_strm->reset ();
      // reset to position 
      d_spos = begin ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the streamer departure position

  t_real Lnrds::departure (void) const {
    rdlock ();
    try {
      t_real result = Streamable::departure ();
      if (p_strm != nullptr) result = p_strm->departure ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer arrival position

  t_real Lnrds::arrival (void) const {
    rdlock ();
    try {
      t_real result = Streamable::arrival ();
      if (p_strm != nullptr) result = p_strm->arrival ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer position

  t_real Lnrds::locate (void) const {
    rdlock ();
    try {
      t_real result = d_spos;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the streamer to the next position
  
  t_real Lnrds::next (void) {
    wrlock ();
    try {
      d_spos = Streamable::next ();
      if (p_strm != nullptr) d_spos = p_strm->next ();
      unlock ();
      return d_spos;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // move the streamer to the previous position
  
  t_real Lnrds::prev (void) {
    wrlock ();
    try {
      d_spos = Streamable::prev ();
      if (p_strm != nullptr) d_spos = p_strm->prev ();
      unlock ();
      return d_spos;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the streamer position

  t_real Lnrds::move (const t_real pos) {
    wrlock ();
    try {
      d_spos = Streamable::move (pos);
      if (p_strm == nullptr) {
	unlock ();
	return d_spos;
      }
      // check for departure
      t_real dp = departure ();
      if (pos <= dp) {
	d_spos = p_strm->begin ();
	unlock ();
	return d_spos;
      }
      // check for arrival
      t_real ap = arrival ();
      if (pos >= ap) {
	d_spos = p_strm->end ();
	unlock ();
	return d_spos;
      }
      // move the streamable
      p_strm->move (d_spos = pos);
      unlock ();
      return d_spos;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer integer value

  long Lnrds::getlong (void) const {
    rdlock ();
    try {
      // check for valid streamable
      long result = Streamable::getlong ();
      if (p_strm == nullptr) {
	unlock ();
	return result;
      }
      // get the streamable position
      t_real spos = p_strm->locate ();
      // get the reference value
      result = p_strm->getlong ();
      // check for valid position
      if (d_spos == spos) {
	unlock ();
	return result;
      }
      // interpolate with next value
      t_real npos = p_strm->next ();
      long   nval = p_strm->getlong (); p_strm->move (spos);
      result += (long) (((nval - result) / (npos - spos)) * (d_spos - spos));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the streamer real value

  t_real Lnrds::getreal (void) const {
    rdlock ();
    try {
      // check for valid streamable
      t_real result = Streamable::getreal ();
      if (p_strm == nullptr) {
	unlock ();
	return result;
      }
      // get the streamable position
      t_real spos = p_strm->locate ();
      // get the reference value
      result = p_strm->getreal ();
      // check for valid position
      if (d_spos == spos) {
	unlock ();
	return result;
      }
      // interpolate with next value
      t_real npos = p_strm->next ();
      t_real nval = p_strm->getreal (); p_strm->move (spos);
      result += ((nval - result) / (npos - spos)) * (d_spos - spos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // forward the streamer to the next integer

  t_real Lnrds::forward (const long pval) {
    wrlock ();
    try {
      // check for valid streamable
      t_real result = Streamable::forward (pval);
      if (p_strm == nullptr) {
	unlock ();
	return result;
      }
      // forward the streamable
      result = p_strm->forward (pval);
      // get the streamable value
      long sval = getlong ();
      if (sval == pval) {
	unlock ();
	return result;
      }
      // get the streamable position
      t_real spos = locate ();
      // move to the next position
      t_real npos = next ();
      long   nval = getreal ();
      // compute interpolate position
      result = ((pval - sval) / ((nval - sval) / (npos - spos))) + spos;
      // move to interpolated position
      result = Math::isnan (result) ? end () : move (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // forward the streamer to the next real

  t_real Lnrds::forward (const t_real pval) {
    wrlock ();
    try {
      // check for valid streamable
      if (p_strm == nullptr) {
	t_real result = Streamable::forward (pval);
	unlock ();
	return result;
      }
      // forward the streamable
      t_real result = p_strm->forward (pval);
      // get the streamable value
      t_real sval = getreal ();
      if (sval == pval) {
	unlock ();
	return result;
      }
      // get the streamable position
      t_real spos = locate ();
      // move to the next position
      t_real npos = next ();
      t_real nval = getreal ();
      // compute interpolate position
      result = ((pval - sval) / ((nval - sval) / (npos - spos))) + spos;
      // move to interpolated position
      result = Math::isnan (result) ? end () : move (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // backward the streamer to the previous real

  t_real Lnrds::backward (const t_real pval) {
    wrlock ();
    try {
      // check for valid streamable
      if (p_strm == nullptr) {
	t_real result = Streamable::backward (pval);
	unlock ();
	return result;
      }
      // backward the streamable
      t_real result = p_strm->backward (pval);
      // get the streamable value
      t_real sval = getreal ();
      if (sval == pval) {
	unlock ();
	return result;
      }
      // get the streamable position
      t_real spos = locate ();
      // move to the next position
      t_real npos = next ();
      t_real nval = getreal ();
      // compute interpolate position
      result = ((pval - sval) / ((nval - sval) / (npos - spos))) + spos;
      // move to interpolated position
      result = Math::isnan (result) ? begin () : move (result);
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

  // create a new object in a generic way

  Object* Lnrds::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Lnrds;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      Streamable* strm = dynamic_cast <Streamable*> (obj);
      if (strm != nullptr) return new Lnrds (strm);
      throw Exception ("type-error", "invalid argument with linear streamer",
		       Object::repr (obj));
    }
    throw Exception ("argument-error",
		     "too many arguments with linear streamer");
  }
}
