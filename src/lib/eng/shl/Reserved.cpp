// ---------------------------------------------------------------------------
// - Reserved.cpp                                                            -
// - afnix:eng - reserved name class implementation                          -
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

#include "Engsid.hxx"
#include "Nameset.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "Reserved.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default reserved name
  
  Reserved::Reserved (void) {
    d_quark  = 0L;
    d_lnum   = 0L;
    p_object = nullptr;
  }

  // create a reserved with a name and a line number
  
  Reserved::Reserved (const String& name, const long lnum) {
    d_name   = name;
    d_quark  = name.toquark ();
    p_object = nullptr;
    d_lnum   = lnum;
  }

  // copy construct this reserved keyword

  Reserved::Reserved (const Reserved& that) {
    that.rdlock ();
    try {
      d_name   = that.d_name;
      d_quark  = that.d_quark;
      p_object = nullptr;
      d_lnum   = that.d_lnum;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this reserved keyword

  Reserved::~Reserved (void) {
    Object::dref (p_object);
  }

  // return the class name

  String Reserved::repr (void) const {
    return "Reserved";
  }

  // return a clone of this object

  Object* Reserved::clone (void) const {
    return new Reserved (*this);
  }

  // clear this reserved name
  
  void Reserved::clear (void) {
    wrlock ();
    try {
      d_quark  = 0L;
      d_lnum   = 0L;
      Object::dref (p_object); p_object = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation
    
  String Reserved::toliteral (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation

  String Reserved::tostring (void) const {
    rdlock ();
    try {
      String result = d_name;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the serial did

  t_word Reserved::getdid (void) const {
    return SRL_DEOD_ENG;
  }

  // return the serial sid

  t_word Reserved::getsid (void) const {
    return SRL_RESV_SID;
  }
  
  // serialize this reserved name

  void Reserved::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      Integer lnum (d_lnum);
      d_name.wrstream (os);
      lnum.wrstream   (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this reserved name

  void Reserved::rdstream (InputStream& is) {
    wrlock ();
    try {
      String  sval;
      Integer ival;
      sval.rdstream (is);
      ival.rdstream (is);
      d_name  = sval;
      d_lnum  = ival.tolong ();
      d_quark = sval.toquark ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the reserved name quark
    
  long Reserved::toquark (void) const {
    rdlock ();
    try {
      long result = d_quark;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the reserved cached object
    
  Object* Reserved::getobj (void) const {
    rdlock ();
    try {
      Object* result = p_object;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the reserved name line number

  long Reserved::getlnum (void) const {
    rdlock ();
    try {
      long result = d_lnum;
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

  // evaluate this object in the current nameset

  Object* Reserved::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    try {
      // look for the cached object
      if (p_object != nullptr) {
	Object* result = p_object;
	zobj->post (result);
	unlock ();
	return result;
      }
      // try to update in write mode
      unlock ();
      wrlock ();
      if (p_object == nullptr) {
	p_object = Object::iref (nset->eval (zobj, nset, d_quark));
      }
      Object* result = p_object;
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
