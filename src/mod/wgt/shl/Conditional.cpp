// ---------------------------------------------------------------------------
// - Conditional.cpp                                                         -
// - afnix:wgt module - conditional widget class implementation              -
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
#include "Wgtsid.hxx"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Conditional.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default conditional

  Conditional::Conditional (void) {
    d_actv = false;
    p_llhs = nullptr;
    p_lrhs = nullptr;
  }

  // create a new conditional by name

  Conditional::Conditional (const String& name) : Taggable (name) {
    d_actv = false;
    p_llhs = nullptr;
    p_lrhs = nullptr;
  }

  // create a new conditional by name and info
  
  Conditional::Conditional (const String& name,
			    const String& info) : Taggable (name, info) { 
    d_actv = false;
    p_llhs = nullptr;
    p_lrhs = nullptr;
  }
  
  // copy construct this conditional

  Conditional::Conditional (const Conditional& that) {
    that.rdlock ();
    try {
      // copy base object
      Taggable::operator = (that);
      // copy locally
      d_actv = that.d_actv;
      d_oper = that.d_oper;
      Object::iref (p_llhs = that.p_llhs);
      Object::iref (p_lrhs = that.p_lrhs);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this conditional

  Conditional::Conditional (Conditional&& that) noexcept {
    that.wrlock ();
    try {
      // move base taggable
      Taggable::operator = (static_cast<Taggable&&>(that));
      // copy move locally
      d_actv = that.d_actv; that.d_actv = false;
      d_oper = that.d_oper; that.d_oper.clear ();
      p_llhs = that.p_llhs; that.p_llhs = nullptr;
      p_lrhs = that.p_lrhs; that.p_lrhs = nullptr;
      that.unlock ();
    } catch (...) {
      d_actv = false;
      d_oper.clear ();
      Object::dref (p_llhs);
      Object::dref (p_lrhs);
      that.unlock ();
    } 
  }

  // destroy this conditional

  Conditional::~Conditional (void) {
    Object::dref (p_llhs);
    Object::dref (p_lrhs);
  }
  
  // assign a conditional to this one

  Conditional& Conditional::operator = (const Conditional& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Taggable::operator = (that);
      // assign locally
      d_actv = that.d_actv;
      d_oper = that.d_oper;
      Object::iref (p_llhs = that.p_llhs);
      Object::iref (p_lrhs = that.p_lrhs);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  //  move a conditional to this one

  Conditional& Conditional::operator = (Conditional&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // move base taggable
      Taggable::operator = (static_cast<Taggable&&>(that));
      // copy move locally
      d_actv = that.d_actv; that.d_actv = false;
      d_oper = that.d_oper; that.d_oper.clear ();
      p_llhs = that.p_llhs; that.p_llhs = nullptr;
      p_lrhs = that.p_lrhs; that.p_lrhs = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_actv = false;
      d_oper.clear ();
      Object::dref (p_llhs);
      Object::dref (p_lrhs);
      unlock ();
      that.unlock ();
    }
    return *this;
  }

  // return the class name

  String Conditional::repr (void) const {
    return "Conditional";
  }

  // return a clone of this object

  Object* Conditional::clone (void) const {
    return new Conditional (*this);
  }

  // return the conditional did

  t_word Conditional::getdid (void) const {
    return SRL_DEOD_WGT;
  }

  // return the conditional sid

  t_word Conditional::getsid (void) const {
    return SRL_COND_SID;
  }

  // serialize this conditional

  void Conditional::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Taggable::wrstream (os);
      // serialize locally
      Serial::wrbool (d_actv, os);
      d_oper.wrstream (os);
      if (p_llhs == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_llhs->serialize (os);
      }
      if (p_lrhs == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_lrhs->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this conditional

  void Conditional::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base object
      Taggable::rdstream (is);
      // deserialize locally
      d_actv = Serial::rdbool (is);
      d_oper.rdstream (is);
      p_llhs = dynamic_cast <Literal*> (Serial::deserialize (is));
      p_lrhs = dynamic_cast <Literal*> (Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the activate flag

  void Conditional::setactv (const bool actv) {
    wrlock ();
    try {
      d_actv = actv;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the conditional activate flag

  bool Conditional::getactv (void) const {
    rdlock ();
    try {
      bool result = d_actv;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the conditional expression

  void Conditional::set (const String& oper, Literal* llhs) {
    wrlock ();
    try {
      d_oper = oper;
      Object::iref (llhs);
      Object::dref (p_llhs); p_llhs = llhs;
      Object::dref (p_lrhs); p_lrhs = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the conditional expression

  void Conditional::set (const String& oper, Literal* llhs, Literal* lrhs) {
    wrlock ();
    try {
      d_oper = oper;
      Object::iref (llhs); Object::dref (p_llhs); p_llhs = llhs;
      Object::iref (lrhs); Object::dref (p_lrhs); p_lrhs = lrhs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the conditional operator

  String Conditional::getoper (void) const {
    rdlock ();
    try {
      String result = d_oper;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the conditional lhs

  void Conditional::setllhs (Literal* llhs) {
    rdlock ();
    try {
      Object::iref (llhs); Object::dref (p_llhs); p_llhs = llhs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the conditional lhs

  Literal* Conditional::getllhs (void) const {
    rdlock ();
    try {
      Literal* result = p_llhs;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the conditional rhs

  void Conditional::setlrhs (Literal* lrhs) {
    rdlock ();
    try {
      Object::iref (lrhs); Object::dref (p_lrhs); p_lrhs = lrhs;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the conditional rhs

  Literal* Conditional::getlrhs (void) const {
    rdlock ();
    try {
      Literal* result = p_lrhs;
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
  static const long QUARK_ZONE_LENGTH = 8;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_SETACTV = zone.intern ("set-active");
  static const long QUARK_GETACTV = zone.intern ("get-active");
  static const long QUARK_GETOPER = zone.intern ("get-operator");
  static const long QUARK_SETLLHS = zone.intern ("set-lhs");
  static const long QUARK_GETLLHS = zone.intern ("get-lhs");
  static const long QUARK_SETLRHS = zone.intern ("set-rhs");
  static const long QUARK_GETLRHS = zone.intern ("get-rhs");

  // create a new object in a generic way

  Object* Conditional::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Conditional;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Conditional (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Conditional (name, info);
    }
    throw Exception ("argument-error", 
		     "too many argument with conditional constructor");
  }

  // return true if the given quark is defined

  bool Conditional::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Taggable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Conditional::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETOPER) return new String  (getoper ());
      if (quark == QUARK_GETACTV) return new Boolean (getactv  ());
      if (quark == QUARK_GETLLHS) {
	rdlock ();
	try {
	  Object* result = getllhs ();
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETLRHS) {
	rdlock ();
	try {
	  Object* result = getlrhs ();
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
    if (argc == 1) {
      if (quark == QUARK_SETACTV) {
	bool actv = argv->getbool (0);
	setactv (actv);
	return nullptr;
      }
      if (quark == QUARK_SETLLHS) {
	Object*   obj = argv->get (0);
	Literal* llhs = dynamic_cast <Literal*> (obj);
	if ((llhs == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error",
			   "invalid object with conditional set-lhs",
			   Object::repr (obj));
	}
	setllhs (llhs);
	return nullptr;
      }
      if (quark == QUARK_SETLRHS) {
	Object*   obj = argv->get (0);
	Literal* lrhs = dynamic_cast <Literal*> (obj);
	if ((lrhs == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error",
			   "invalid object with conditional set-rhs",
			   Object::repr (obj));
	}
	setlrhs (lrhs);
	return nullptr;
      }
    }
    // dispatch 2 argument
    if (argc == 2) {
      if (quark == QUARK_SET) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* llhs = dynamic_cast <Literal*> (obj);
	if ((llhs == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object with conditional set",
			   Object::repr (obj));
	}
	set (name, llhs);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SET) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* llhs = dynamic_cast <Literal*> (obj);
	if ((llhs == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object with conditional set",
			   Object::repr (obj));
	}
	obj = argv->get (2);
	Literal* lrhs = dynamic_cast <Literal*> (obj);
	if ((lrhs == nullptr) && (obj != nullptr)) {
	  throw Exception ("type-error", "invalid object with conditional set",
			   Object::repr (obj));
	}
	set (name, llhs, lrhs);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
