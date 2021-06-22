// ---------------------------------------------------------------------------
// - Transform.cpp                                                           -
// - afnix:svg service - svg transformation attribute class implementation   -
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
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "Transform.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the transformation mode
  enum t_svgm {
    SVG_MATRX_MODE,
    SVG_TRANS_MODE,
    SVG_SCALE_MODE,
    SVG_ROTAT_MODE,
    SVG_SKEWX_MODE,
    SVG_SKEWY_MODE
  };

  // the transformation structure
  struct s_svgt {
    // the transformation mode
    t_svgm d_mode;
    // the transformation data
    t_real d_data[6];
    // the translate or rotation point
    long   d_trpt[2];
    // the rotation point flag
    bool   d_rotp;
    // the next transformation
    s_svgt* p_next;
    // create a translate transformation
    s_svgt (const long tx) {
      // set transformation data
      d_mode = SVG_TRANS_MODE;
      d_trpt[0] = tx;
      d_trpt[1] = 0;
      // clear other data
      for (long i = 0; i < 6; i++) d_data[i] = 0.0;
      d_rotp = false;
      p_next = nullptr;
    }
    // create a translate transformation
    s_svgt (const long tx, const long ty) {
      // set transformation data
      d_mode = SVG_TRANS_MODE;
      d_trpt[0] = tx;
      d_trpt[1] = ty;
      // clear other data
      for (long i = 0; i < 6; i++) d_data[i] = 0.0;
      d_rotp = false;
      p_next = nullptr;
    }
    // create a transformation by mode
    s_svgt (const t_svgm mode, const t_real tval) {
      // check for matrix or translate
      if ((mode == SVG_MATRX_MODE) || (mode == SVG_TRANS_MODE)) {
	throw Exception ("svg-error","invalid transformation mode");
      }
      // check for scale
      if (mode == SVG_SCALE_MODE) {
	// set transformation data
	d_mode = mode;
	d_data[0] = tval;
	d_data[1] = tval;
	// clear other data
	for (long i = 2; i < 6; i++) d_data[i] = 0.0;
	d_trpt[0] = 0;
	d_trpt[1] = 0;
	d_rotp = false;
	p_next = nullptr;
      }
      // check for rotate, skew-x or skew-y
      if ((mode == SVG_ROTAT_MODE) || (mode == SVG_SKEWX_MODE) ||
	  (mode == SVG_SKEWY_MODE)) {
	// set transformation data
	d_mode = mode;
	d_data[0] = tval;
	// clear other data
	for (long i = 1; i < 6; i++) d_data[i] = 0.0;
	d_trpt[0] = 0;
	d_trpt[1] = 0;
	d_rotp = false;
	p_next = nullptr;
      }
    }
    // create a scaling transformation
    s_svgt (const t_real sx, const t_real sy) {
      // set transformation data
      d_mode = SVG_SCALE_MODE;
      d_data[0] = sx;
      d_data[1] = sy;
      // clear other data
      for (long i = 2; i < 6; i++) d_data[i] = 0.0;
      d_trpt[0] = 0;
      d_trpt[1] = 0;
      d_rotp    = false;
      p_next    = nullptr;
    }
    // create a rotation by angle and point
    s_svgt (const t_real tval, const long px, const long py) {
      // set transformation data
      d_mode    = SVG_ROTAT_MODE;
      d_data[0] = tval;
      d_trpt[0] = px;
      d_trpt[1] = py;
      d_rotp    = true;
      // clear other data
      for (long i = 1; i < 6; i++) d_data[i] = 0.0;
      p_next = nullptr;
    }
    // create a matrix transformation
    s_svgt (const t_real a, const t_real b, const t_real c,
	    const t_real d, const t_real e, const t_real f) {
      // set transformation data
      d_mode    = SVG_MATRX_MODE;
      d_data[0] = a;
      d_data[1] = b;
      d_data[2] = c;
      d_data[3] = d;
      d_data[4] = e;
      d_data[5] = f;
      // clear other data
      d_trpt[0] = 0;
      d_trpt[1] = 0;
      d_rotp    = false;
      p_next    = nullptr;
    }
    // destroy this transformation
    ~s_svgt (void) {
      if (p_next != nullptr) delete p_next;
    }
    // return a transformation string representation
    String tostring (void) const {
      // prepare result
      String result;
      // check for a matrix mode
      if (d_mode == SVG_MATRX_MODE) {
	result = "matrix(";
	result += Utility::tostring (d_data[0]); result += ", ";
	result += Utility::tostring (d_data[1]); result += ", ";
	result += Utility::tostring (d_data[2]); result += ", ";
	result += Utility::tostring (d_data[3]); result += ", ";
	result += Utility::tostring (d_data[4]); result += ", ";
	result += Utility::tostring (d_data[5]); result += ')';
      }
      // check for a translation mode
      if (d_mode == SVG_TRANS_MODE) {
	result = "translate(";
	result += Utility::tostring (d_trpt[0]); result += ", ";
	result += Utility::tostring (d_trpt[1]); result += ')';
      }
      // check for a scaling mode
      if (d_mode == SVG_SCALE_MODE) {
	result = "scale(";
	result += Utility::tostring (d_data[0]); result += ", ";
	result += Utility::tostring (d_data[1]); result += ')';
      }
      // check for a rotation mode
      if (d_mode == SVG_ROTAT_MODE) {
	result = "rotate(";
	result += Utility::tostring (d_data[0]);
	if (d_rotp == true) {
	  result += ", ";
	  result += Utility::tostring (d_trpt[0]); result += ", ";
	  result += Utility::tostring (d_trpt[1]);
	}
	result += ')';
      }
      // check for a skew-x mode
      if (d_mode == SVG_SKEWX_MODE) {
	result = "skewX(";
	result += Utility::tostring (d_data[0]); result += ')';
      }
      // check for a skew-y mode
      if (d_mode == SVG_SKEWY_MODE) {
	result = "skewY(";
	result += Utility::tostring (d_data[0]); result += ')';
      }
      // here it is
      return result;
    }
  };
  
  // this procedure update the svg translation list
  static s_svgt* svgt_add_elem (s_svgt* root, s_svgt* elem) {
    // check for nil first
    if (root == nullptr) return elem;
    // set root pointer
    s_svgt* sptr = root;
    // fin last element
    while (sptr->p_next != nullptr) sptr = sptr->p_next;
    // update last element
    sptr->p_next = elem;
    return root;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a null transform

  Transform::Transform (void) {
    p_tlst = nullptr;
  }

  // destroy this transform object

  Transform::~Transform (void) {
    delete p_tlst;
  }

  // return the class name

  String Transform::repr (void) const {
    return "Transform";
  }

  // reset this transformation

  void Transform::reset (void) {
    wrlock ();
    try {
      delete p_tlst;
      p_tlst = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the transformation is nil

  bool Transform::isnil (void) const {
    rdlock ();
    try {
      bool result = (p_tlst == nullptr);
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of the transformation

  String Transform::tostring (void) const {
    rdlock ();
    try {
      // prepare result
      String result;
      // loop in the list
      s_svgt* tlst = p_tlst;
      while (tlst != nullptr) {
	result += tlst->tostring ();
	if (tlst->p_next != nullptr) result += ' ';
	tlst = tlst->p_next;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a translation transformation

  void Transform::addtr (const long tx) {
    wrlock ();
    try {
      // create a translation element
      s_svgt* tptr = new s_svgt (tx);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a translation transformation

  void Transform::addtr (const long tx, const long ty) {
    wrlock ();
    try {
      // create a translation element
      s_svgt* tptr = new s_svgt (tx, ty);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a scaling transformation

  void Transform::addsc (const t_real sx) {
    wrlock ();
    try {
      // create a scaling element
      s_svgt* tptr = new s_svgt (SVG_SCALE_MODE, sx);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a scaling transformation

  void Transform::addsc (const t_real sx, const t_real sy) {
    wrlock ();
    try {
      // create a scaling element
      s_svgt* tptr = new s_svgt (sx, sy);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a single factor rotation

  void Transform::addrt (const t_real ra) {
    wrlock ();
    try {
      // create a rotation element
      s_svgt* tptr = new s_svgt (SVG_ROTAT_MODE, ra);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a rotation by point

  void Transform::addrt (const t_real ra, const long px, const long py) {
    wrlock ();
    try {
      // create a rotation element
      s_svgt* tptr = new s_svgt (ra, px, py);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a skew-x transformation

  void Transform::addsx (const t_real sa) {
    wrlock ();
    try {
      // create a rotation element
      s_svgt* tptr = new s_svgt (SVG_SKEWX_MODE, sa);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a skew-y transformation

  void Transform::addsy (const t_real sa) {
    wrlock ();
    try {
      // create a rotation element
      s_svgt* tptr = new s_svgt (SVG_SKEWY_MODE, sa);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a matrix transformation

  void Transform::addmt (const t_real a, const t_real b, const t_real c,
			    const t_real d, const t_real e, const t_real f) {
    wrlock ();
    try {
      // create a rotation element
      s_svgt* tptr = new s_svgt (a, b, c, d, e, f);
      // update the translation list
      p_tlst = svgt_add_elem (p_tlst, tptr);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 9;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_NILP  = zone.intern ("nil-p");
  static const long QUARK_RESET = zone.intern ("reset");
  static const long QUARK_TOSTR = zone.intern ("to-string");
  static const long QUARK_ADDSX = zone.intern ("add-skew-x");
  static const long QUARK_ADDSY = zone.intern ("add-skew-y");
  static const long QUARK_ADDMT = zone.intern ("add-matrix");
  static const long QUARK_ADDRT = zone.intern ("add-rotate");
  static const long QUARK_ADDSC = zone.intern ("add-scaling");
  static const long QUARK_ADDTR = zone.intern ("add-translate");

  // create a new object in a generic way

  Object* Transform::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Transform;
    // wrong arguments
    throw Exception ("argument-error", 
		     "too many arguments with svg transform object");
  }
  
  // return true if the given quark is defined
  
  bool Transform::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* Transform::apply (Evaluable* zobj, Nameset* nset, const long quark,
			       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_NILP)  return new Boolean (isnil ());
      if (quark == QUARK_TOSTR) return new String  (tostring ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADDTR) {
	long tx = argv->getlong (0);
	addtr (tx);
	return nullptr;
      }
      if (quark == QUARK_ADDSC) {
	t_real sx = argv->getreal (0);
	addsc (sx);
	return nullptr;
      }
      if (quark == QUARK_ADDRT) {
	t_real ra = argv->getreal (0);
	addrt (ra);
	return nullptr;
      }
      if (quark == QUARK_ADDSX) {
	t_real sa = argv->getreal (0);
	addsx (sa);
	return nullptr;
      }
      if (quark == QUARK_ADDSY) {
	t_real sa = argv->getreal (0);
	addsy (sa);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDTR) {
	long tx = argv->getlong (0);
	long ty = argv->getlong (1);
	addtr (tx, ty);
	return nullptr;
      }
      if (quark == QUARK_ADDSC) {
	t_real sx = argv->getreal (0);
	t_real sy = argv->getreal (1);
	addsc (sx, sy);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADDRT) {
	t_real ra = argv->getreal (0);
	long   px = argv->getlong (1);
	long   py = argv->getlong (2);
	addrt (ra, px, py);
	return nullptr;
      }
    }
    // dispatch 6 arguments
    if (argc == 3) {
      if (quark == QUARK_ADDMT) {
	t_real a = argv->getreal (0);
	t_real b = argv->getreal (1);
	t_real c = argv->getreal (2);
	t_real d = argv->getreal (3);
	t_real e = argv->getreal (4);
	t_real f = argv->getreal (5);
	addmt (a, b, c, d, e, f);
	return nullptr;
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
