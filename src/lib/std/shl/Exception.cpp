// ---------------------------------------------------------------------------
// - Exception.cpp                                                           -
// - standard object library - exception class implementation                -
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

#include "Utility.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default exception

  Exception::Exception (void) {
    d_nlf  = false; 
    d_abf  = false; 
    d_lnum = 0;
    p_obj  = nullptr;
  }
  
  // create a exception with an id
  
  Exception::Exception (const String& eid) {
    d_eid    = eid;
    d_nlf    = false; 
    d_abf    = false; 
    d_lnum   = 0;
    p_obj    = nullptr;
  }

  // create a exception with an id and a reason
  
  Exception::Exception (const String& eid, const String& reason) {
    d_eid    = eid;
    d_reason = reason;
    d_nlf    = false;
    d_abf    = false;
    d_lnum   = 0;
    p_obj    = nullptr;
  }

  // create a exception with an eid, an indicator and a long reason
  
  Exception::Exception (const String& eid, const String& reason,
			const String& name) {
    d_eid    = eid;
    d_reason = reason + " " + name;
    d_nlf    = false;
    d_abf    = false;
    d_lnum   = 0;
    p_obj    = nullptr;
  }

  // create a exception with an id and an object
  
  Exception::Exception (const String& eid, Object* object) {
    d_eid    = eid;
    d_nlf    = false;
    d_abf    = false;
    d_lnum   = 0;
    p_obj    = Object::iref (object);
  }

  // create a exception with a short type, a reason and an object
  
  Exception::Exception (const String& eid, const String& reason,
			Object* object) {
    d_eid    = eid;
    d_reason = reason;
    d_nlf    = false;
    d_abf    = false;
    d_lnum   = 0;
    p_obj    = Object::iref (object);
  }

  // copy constructor for this exception
  
  Exception::Exception (const Exception& that) {
    that.rdlock ();
    try {
      d_eid    = that.d_eid;
      d_reason = that.d_reason;
      d_nlf    = that.d_nlf;
      d_abf    = that.d_abf;
      d_name   = that.d_name;
      d_lnum   = that.d_lnum;
      p_obj    = Object::iref (that.p_obj);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }  
  }

  // destroy this exception

  Exception::~Exception (void) {
    Object::dref (p_obj);
  }

  // assign an exception to this one
  
  Exception& Exception::operator = (const Exception& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_eid    = that.d_eid;
      d_reason = that.d_reason;
      d_nlf    = that.d_nlf;
      d_abf    = that.d_abf;
      d_name   = that.d_name;
      d_lnum   = that.d_lnum;
      Object::dref (p_obj); Object::iref (that.p_obj); p_obj = that.p_obj;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String Exception::repr (void) const {
    return "Exception";
  }

  // return a clone of this object

  Object* Exception::clone (void) const {
    return new Exception (*this);
  }

  // return the exception id

  String Exception::geteid (void) const {
    rdlock ();
    try {
      String result = d_eid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the exception reason

  String Exception::getval (void) const {
    rdlock ();
    try {
      String result = d_reason;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // return the exception object

  Object* Exception::getobj (void) const {
    rdlock ();
    try {
      Object* result = p_obj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the new line flag

  void Exception::setnlf (const bool flag) {
    wrlock ();
    try {
      d_nlf = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the new line flag

  bool Exception::getnlf (void) const {
    rdlock ();
    try {
      bool result = d_nlf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the abort flag

  void Exception::setabf (const bool flag) {
    wrlock ();
    try {
      d_abf = flag;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the abort flag

  bool Exception::getabf (void) const {
    rdlock ();
    try {
      bool result = d_abf;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // update the exception file name - do not touch it if already set

  void Exception::updname (const String& name) {
    wrlock ();
    try {
      if (d_name.length () == 0) d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the exception file name

  void Exception::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the exception file name

  String Exception::getname (void) const {
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

  // update the exception line number

  void Exception::updlnum (const long lnum) {
    wrlock ();
    try {
      if (d_lnum == 0) d_lnum = lnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the exception line number

  void Exception::setlnum (const long lnum) {
    wrlock ();
    try {
      d_lnum = lnum;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the exception line number

  long Exception::getlnum (void) const {
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

  // return an extended exception reason

  String Exception::geteer (void) const {
    rdlock ();
    try {
      // prepare the result
      String result = d_reason;
      // check for a file name
      if (d_name.isnil () == false) {
	// add the file name
	result += " [";
	result += d_name;
	// add the line number
	result += ':';
	result += Utility::tostring (d_lnum);
	result += ']';
      }
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
  static const long QUARK_ZONE_LENGTH = 6;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_EID    = zone.intern ("eid");
  static const long QUARK_NAME   = zone.intern ("name");
  static const long QUARK_LNUM   = zone.intern ("line");
  static const long QUARK_ABOUT  = zone.intern ("about");
  static const long QUARK_REASON = zone.intern ("reason");
  static const long QUARK_OBJECT = zone.intern ("object");

  // return true if the given quark is defined

  bool Exception::isquark (const long quark, const bool hflg) const {
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

  // evaluate an object member by quark

  Object* Exception::eval (Evaluable* zobj, Nameset* nset, const long quark) {
    // check the exception first
    if (quark == QUARK_EID)    return new String  (geteid  ());
    if (quark == QUARK_NAME)   return new String  (getname ());
    if (quark == QUARK_ABOUT)  return new String  (geteer  ());
    if (quark == QUARK_REASON) return new String  (getval  ());
    if (quark == QUARK_OBJECT) {
      rdlock ();
      try {
	Object* result = getobj ();
	zobj->post (result);
	unlock ();
	return result;
      } catch (...) {
	unlock ();
	throw;
      }
    }
    if (quark == QUARK_LNUM) {
      String lnum = Utility::tostring (getlnum ());
      return new String (lnum);
    }
    // evaluate in the object
    return Object::eval (zobj, nset, quark);
  }
}
