// ---------------------------------------------------------------------------
// - Sampler.cpp                                                             -
// - afnix:sps module - data sampler class implementation                    -
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
#include "Real.hpp"
#include "Vector.hpp"
#include "Sampler.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default sampler

  Sampler::Sampler (void) {
    p_lobj = nullptr;
    p_sobj = nullptr;
    reset ();
  }

  // create a sampler by objects

  Sampler::Sampler (Localizer* lobj, Streamable* sobj) {
    Object::iref (p_lobj = lobj);
    Object::iref (p_sobj = sobj);
    reset ();
  }
  
  // delete this sampler

  Sampler::~Sampler (void) {
    Object::dref (p_lobj);
    Object::dref (p_sobj);
  }

  // return the object name

  String Sampler::repr (void) const {
    return "Sampler";
  }

  // reset the sampler

  void Sampler::reset (void) {
    wrlock ();
    try {
      if (p_lobj != nullptr) p_lobj->reset ();
      if (p_sobj != nullptr) {
	p_sobj->reset ();
	p_sobj->begin ();
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the localizer object

  Localizer* Sampler::getlobj (void) const {
    rdlock ();
    try {
      Localizer* result = p_lobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // get the streamable object

  Streamable* Sampler::getsobj (void) const {
    rdlock ();
    try {
      Streamable* result = p_sobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  bool Sampler::validate (const t_real pval) const {
    rdlock ();
    try {
      bool result = (p_lobj == nullptr) ? false : p_lobj->validate (pval);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sample a boolean by a boolean

  bool Sampler::mapbool (const bool pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      bool result = (p_sobj == nullptr) ? false : p_sobj->getbool (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sample a boolean by an integer

  bool Sampler::mapbool (const long pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      bool result = (p_sobj == nullptr) ? false : p_sobj->getbool (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sample a boolean by a real

  bool Sampler::mapbool (const t_real pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      bool result = (p_sobj == nullptr) ? false : p_sobj->getbool (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sample an integer by a boolean

  long Sampler::maplong (const bool pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      long result = (p_sobj == nullptr) ? 0L : p_sobj->getlong (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sample an integer by an integer

  long Sampler::maplong (const long pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      long result = (p_sobj == nullptr) ? 0L : p_sobj->getlong (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sample an integer by a real

  long Sampler::maplong (const t_real pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos = (p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      long result = (p_sobj == nullptr) ? 0L : p_sobj->getlong (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sample a real by a boolean

  t_real Sampler::mapreal (const bool pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos =
	(p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      t_real result =
	(p_sobj == nullptr) ? Math::CV_NAN : p_sobj->getreal (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // sample a real by an integer

  t_real Sampler::mapreal (const long pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos =
	(p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      t_real result =
	(p_sobj == nullptr) ? Math::CV_NAN : p_sobj->getreal (pos);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // sample a real by a real

  t_real Sampler::mapreal (const t_real pval) {
    wrlock ();
    try {
      // get the sampler position
      t_real pos =
	(p_lobj == nullptr) ? Math::CV_NAN : p_lobj->locate (pval);
      // get the streamable by position
      t_real result =
	(p_sobj == nullptr) ? Math::CV_NAN : p_sobj->getreal (pos);
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
  static const long QUARK_ZONE_LENGTH = 7;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET    = zone.intern ("reset");
  static const long QUARK_GETLOBJ  = zone.intern ("get-localizer");
  static const long QUARK_GETSOBJ  = zone.intern ("get-streamable");
  static const long QUARK_MAPREAL  = zone.intern ("map-real");
  static const long QUARK_MAPBOOL  = zone.intern ("map-boolean");
  static const long QUARK_MAPLONG  = zone.intern ("map-integer");
  static const long QUARK_VALIDATE = zone.intern ("validate");
  
  // create a new object in a generic way

  Object* Sampler::mknew (Vector* argv) {
    // get number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Sampler;
    // check for 2 arguments
    if (argc == 2) {
      // get the localizer
      Object* obj = argv->get (0);
      Localizer*  lobj = dynamic_cast <Localizer*> (obj);
      if ((lobj == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid argument as sampler localizer",
			 Object::repr (obj));
      }	
      obj = argv->get (1);
      Streamable* sobj = dynamic_cast <Streamable*> (obj);
      if ((sobj == nullptr) && (obj != nullptr)) {
	throw Exception ("type-error", "invalid argument as sampler streamable",
			 Object::repr (obj));
      }
      return new Sampler (lobj, sobj);
    }
    throw Exception ("argument-error", "too many arguments with sampler");
  }

  // return true if the given quark is defined

  bool Sampler::isquark (const long quark, const bool hflg) const {
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

  Object* Sampler::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0L) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
      if (quark == QUARK_GETLOBJ) {
	rdlock ();
	try {
	  Object* result = getlobj ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETSOBJ) {
	rdlock ();
	try {
	  Object* result = getsobj ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }    
    // dispatch 1 argument
    if (argc == 1L) {
      if (quark == QUARK_MAPBOOL) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Boolean (mapbool (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Boolean (mapbool ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Boolean (mapbool (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for map-boolean",
			 Object::repr (obj));
      }
      if (quark == QUARK_MAPLONG) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Integer (maplong (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Integer (maplong ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Integer (maplong (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for map-integer",
			 Object::repr (obj));
      }
      if (quark == QUARK_MAPREAL) {
	Object* obj = argv->get (0);
	// check for a boolean
	Boolean* bobj = dynamic_cast <Boolean*> (obj);
	if (bobj != nullptr) return new Real (mapreal (bobj->tobool ()));
	// check for an integer
	Integer* iobj = dynamic_cast <Integer*> (obj);
	if (iobj != nullptr) return new Real (mapreal ((long) iobj->tolong ()));
	// check for a real
	Real* lobj = dynamic_cast <Real*> (obj);
	if (lobj != nullptr) return new Real (mapreal (lobj->toreal ()));
	// invalid object
	throw Exception ("type-error", "invalid object for map-real",
			 Object::repr (obj));
      }
      if (quark == QUARK_VALIDATE) {
	t_real pval = argv->getreal (0);
	return new Boolean (validate (pval));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
