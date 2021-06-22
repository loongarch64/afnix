// ---------------------------------------------------------------------------
// - Constant.cpp                                                            -
// - afnix engine - constant object class implementation                     -
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
#include "Constant.hpp"
#include "Evaluable.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                       -
  // -------------------------------------------------------------------------

  // create an empty constant

  Constant::Constant (void) {
    p_lobj = nullptr;
  }

  // create a new constant
 
  Constant::Constant (Literal* lobj) {
    Object::iref (p_lobj = lobj);
  }

  // copy construct this constant

  Constant::Constant (const Constant& that) {
    that.rdlock ();
    try {
      Object::iref (p_lobj = that.p_lobj);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this constant
  
  Constant::~Constant (void) {
    Object::dref (p_lobj);
  }

  // return the class name

  String Constant::repr (void) const {
    return "Constant";
  }

  
  // return a clone of this object

  Object* Constant::clone (void) const {
    return new Constant (*this);
  }

  // clear this constant

  void Constant::clear (void) {
    wrlock ();
    try {
      if (p_lobj != nullptr) p_lobj->clear ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a literal representation of this constant

  String Constant::toliteral (void) const {
    rdlock ();
    try {
      String result;
      if (p_lobj != nullptr) result = p_lobj->toliteral ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return a string representation of this constant

  String Constant::tostring (void) const {
    rdlock ();
    try {
      String result;
      if (p_lobj != nullptr) result = p_lobj->tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the serial did
  
  t_word Constant::getdid (void) const {
    return SRL_DEOD_ENG;
  }
  
  // return the serial sid
  
  t_word Constant::getsid (void) const {
    return SRL_CNST_SID;
  }
  
  // serialize this constant

  void Constant::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      if (p_lobj == nullptr) {
	os.write (nilc);
      } else {
	p_lobj->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this constant

  void Constant::rdstream (InputStream& is) {
    wrlock ();
    try {
      Object*  sobj = Serial::deserialize (is);
      Literal* lobj = dynamic_cast <Literal*> (sobj);
      if (lobj == nullptr) {
	String what = Object::repr (sobj);
	Object::cref (sobj);
	unlock ();
	throw Exception ("serial-error", "invalid object for constant", what);
      }
      Object::iref (p_lobj = lobj);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reduce this object

  Object* Constant::reduce (void) const {
    rdlock ();
    try {
      Object* result = p_lobj;
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

  // evaluate this object

  Object* Constant::eval (Evaluable* zobj, Nameset* nset) {
    rdlock ();
    try {
      Object* result = (p_lobj == nullptr) ? nullptr : p_lobj->clone ();
      zobj->post (result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
}
