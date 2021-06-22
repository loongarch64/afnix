// ---------------------------------------------------------------------------
// - Rtransform.cpp                                                          -
// - afnix:mth module - real transform class implementation                  -
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
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Rtransform.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // set a 3 dimensional rotation x-axis rmatrix 
  static inline void geo_rotx (Rmatrix3& m, const t_real a) {
    m.eye ();
    m.set (1, 1,  Math::cos (a)); m.set (1, 2, -Math::sin (a));
    m.set (2, 1,  Math::sin (a)); m.set (2, 2,  Math::cos (a));
  }
  
  // set a 3 dimensional rotation y-axis rmatrix 
  static inline void geo_roty (Rmatrix3& m, const t_real a) {
    m.eye ();
    m.set (0, 0,  Math::cos (a)); m.set (0, 2, Math::sin (a));
    m.set (2, 0, -Math::sin (a)); m.set (2, 2, Math::cos (a));
  }
  
  // set a 3 dimensional rotation z-axis rmatrix 
  static inline void geo_rotz (Rmatrix3& m, const t_real a) {
    m.eye ();
    m.set (0, 0,  Math::cos (a)); m.set (0, 1, -Math::sin (a));
    m.set (1, 0,  Math::sin (a)); m.set (1, 1,  Math::cos (a));
  }
  
  // -------------------------------------------------------------------------
  // - public section                                                        -
  // -------------------------------------------------------------------------

  // create a vector from a point

  Rvector2 Rtransform::tovector (const Rpoint2& p) {
    Rvector2 result;
    for (long k = 0L; k < 2L; k++) result.nlset (k, p.get (k));
    return result;
  }

  // create a vector from a point

  Rvector3 Rtransform::tovector (const Rpoint3& p) {
    Rvector3 result;
    for (long k = 0L; k < 3L; k++) result.nlset (k, p.get (k));
    return result;
  }

  // create a vector from a point

  Rvector4 Rtransform::tovector (const Rpoint4& p) {
    Rvector4 result;
    for (long k = 0L; k < 4L; k++) result.nlset (k, p.get (k));
    return result;
  }

  // create a vector from two points

  Rvector2 Rtransform::tovector (const Rpoint2& o, const Rpoint2& p) {
    Rvector2 result;
    for (long k = 0L; k < 2L; k++) {
      result.nlset (k, p.get (k) - o.get (k));
    }
    return result;
  }
  
  // create a vector from two points

  Rvector3 Rtransform::tovector (const Rpoint3& o, const Rpoint3& p) {
    Rvector3 result;
    for (long k = 0L; k < 3L; k++) {
      result.nlset (k, p.get (k) - o.get (k));
    }
    return result;
  }
  
  // create a vector from two points

  Rvector4 Rtransform::tovector (const Rpoint4& o, const Rpoint4& p) {
    Rvector4 result;
    for (long k = 0L; k < 4L; k++) {
      result.nlset (k, p.get (k) - o.get (k));
    }
    return result;
  }
  
  // translate a point with a vector

  Rpoint2 Rtransform::translate (const Rpoint2& p, const Rvector2& v) {
    Rpoint2 result;
    result.set (0, p.get (0) + v.get (0));
    result.set (1, p.get (1) + v.get (1));
    return result;
  }

  // translate a point with a vector

  Rpoint3 Rtransform::translate (const Rpoint3& p, const Rvector3& v) {
    Rpoint3 result;
    result.set (0, p.get (0) + v.get (0));
    result.set (1, p.get (1) + v.get (1));
    result.set (2, p.get (2) + v.get (2));
    return result;
  }
  
  // translate a point with a vector

  Rpoint4 Rtransform::translate (const Rpoint4& p, const Rvector4& v) {
    Rpoint4 result;
    result.set (0, p.get (0) + v.get (0));
    result.set (1, p.get (1) + v.get (1));
    result.set (2, p.get (2) + v.get (2));
    result.set (3, p.get (3) + v.get (3));
    return result;
  }
  
  // scale a vector by a real

  Rvector2 Rtransform::scale (const t_real s, const Rvector2& v) {
    Rvector2 result;
    for (long k = 0L; k < 2L; k++) result.nlset (k, s * v.get (k));
    return result;
  }

  // scale a vector by a real

  Rvector3 Rtransform::scale (const t_real s, const Rvector3& v) {
    Rvector3 result;
    for (long k = 0L; k < 3L; k++) result.nlset (k, s * v.get (k));
    return result;
  }

  // scale a vector by a real

  Rvector4 Rtransform::scale (const t_real s, const Rvector4& v) {
    Rvector4 result;
    for (long k = 0L; k < 4L; k++) result.nlset (k, s * v.get (k));
    return result;
  }

  // normalize a vector

  Rvector2 Rtransform::normalize (const Rvector2& v) {
    t_real s = v.norm ();
    return (s == 0.0) ? Rvector2() : scale (1.0 / s, v);
  }
  
  // normalize a vector

  Rvector3 Rtransform::normalize (const Rvector3& v) {
    t_real s = v.norm ();
    return (s == 0.0) ? Rvector3() : scale (1.0 / s, v);
  }
  
  // normalize a vector

  Rvector4 Rtransform::normalize (const Rvector4& v) {
    t_real s = v.norm ();
    return (s == 0.0) ? Rvector4() : scale (1.0 / s, v);
  }

  // compute the vector cross product

  Rvector3 Rtransform::cross (const Rvector3& vl, const Rvector3& vr) {
    Rvector3 result;
    result.set (0, vl.get(1)*vr.get(2) - vl.get(2)*vr.get(1));
    result.set (1, vl.get(2)*vr.get(0) - vl.get(0)*vr.get(2));
    result.set (2, vl.get(0)*vr.get(1) - vl.get(1)*vr.get(0));
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // return the class name

  String Rtransform::repr (void) const {
    return "Rtransform";
  }

  // create a new rotation rmatrix

  Rmatrix2 Rtransform::newrot2 (const t_real a){
    rdlock ();
    try {
      Rmatrix2 m;
      setrot2 (m, a);
      unlock ();
      return m;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a 2 dimensional rotation rmatrix
  
  Rmatrix2& Rtransform::setrot2 (Rmatrix2& m, const t_real a) {
    rdlock ();
    try {
      m.eye ();
      m.set (0, 0,  Math::cos (a)); m.set (0, 1, -Math::sin (a));
      m.set (1, 0,  Math::sin (a)); m.set (1, 1,  Math::cos (a));
      unlock ();
      return m;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // create a new rotation rmatrix

  Rmatrix3 Rtransform::newrot3 (const t_real a[3]){
    rdlock ();
    try {
      Rmatrix3 m;
      setrot3 (m, a);
      unlock ();
      return m;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a 3 dimensional rotation rmatrix
  
  Rmatrix3& Rtransform::setrot3 (Rmatrix3& m, const t_real a[3]) {
    rdlock ();
    try {
      // set rotation
      Rmatrix3 mx; geo_rotx (mx, a[0]);
      Rmatrix3 my; geo_roty (my, a[1]);
      Rmatrix3 mz; geo_rotz (mz, a[2]);
      m = mx * my * mz;
      unlock ();
      return m;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ROTATE  = zone.intern ("rotate");
  
  // create a new object in a generic way

  Object* Rtransform::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Rtransform;
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with with Rtransform"); 
  }

  // return true if the given quark is defined
  
  bool Rtransform::isquark (const long quark, const bool hflg) const {
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

  Object* Rtransform::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ROTATE) {
	// get the angle
	t_real a = argv->getreal (0);
	// create the rmatrix
	return new Rmatrix2 (Rtransform::newrot2 (a));
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_ROTATE) {
	// get the rmatrix
	Object* obj = argv->get (0);
	auto m = dynamic_cast<Rmatrix2*>(obj);
	if (m == nullptr) {
	  throw Exception ("type-error", "invalid object as rmatrix 2",
			   Object::repr (obj));
	}
	// get the angle
	t_real a = argv->getreal (1);
	// rotate the rmatrix
	Rtransform::setrot2 (*m, a);
	if (zobj != nullptr) zobj->post (m);
	return m;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ROTATE) {
	// get the angle
	t_real xa = argv->getreal (0);
	t_real ya = argv->getreal (1);
	t_real za = argv->getreal (2);
	// create the rmatrix
	t_real a[3] = {xa, ya, za};
	return new Rmatrix3 (Rtransform::newrot3 (a));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
