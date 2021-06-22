// ---------------------------------------------------------------------------
// - Property.cpp                                                            -
// - standard object library - property class implementation                 -
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
#include "Stdsid.hxx"
#include "Vector.hpp"
#include "Utility.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Property.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "InputStream.hpp"
#include "OutputStream.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a new default property

  Property::Property (void) {
    p_pval = nullptr;
  }

  // create a new property by name

  Property::Property (const String& name) : Taggable (name) {
    p_pval = nullptr;
  }

  // create a new property by name and boolean value
  
  Property::Property (const String& name, const bool bval) : Taggable (name) { 
    p_pval = new Boolean (bval);
  }

  // create a new property by name and integer value
  
  Property::Property (const String& name,
		      const t_long ival) : Taggable (name) { 
    p_pval = new Integer (ival);
  }

  // create a new property by name and real value
  
  Property::Property (const String& name,
		      const t_real rval) : Taggable (name){ 
    p_pval = new Real (rval);
  }

  // create a new property by name and value
  
  Property::Property (const String& name,
		      const Literal& lval) : Taggable (name){ 
    p_pval = dynamic_cast <Literal*> (lval.clone ());
  }

  // create a new property by name, info and boolean value
  
  Property::Property (const String& name, const String& info,
		      const bool bval) : Taggable (name, info) { 
    p_pval = new Boolean (bval);
  }

  // create a new property by name, info and integer value
  
  Property::Property (const String& name, const String& info, 
		      const t_long  ival) : Taggable (name, info) { 
    p_pval = new Integer (ival);
  }

  // create a new property by name, info and real value
  
  Property::Property (const String& name, const String& info, 
		      const t_real  rval) : Taggable (name, info) { 
    p_pval = new Real (rval);
  }

  // create a new property by name, info and value
  
  Property::Property (const String&  name, const String& info,
		      const Literal& lval) : Taggable (name, info) { 
    p_pval = dynamic_cast <Literal*> (lval.clone ());
  }
  
  // copy constructor for this property class

  Property::Property (const Property& that) {
    that.rdlock ();
    try {
      // copy base object
      Taggable::operator = (that);
      // copy locally
      p_pval = nullptr;
      if (that.p_pval != nullptr) {
	p_pval = dynamic_cast <Literal*> (that.p_pval->clone ());
      }
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this property

  Property::Property (Property&& that) noexcept {
    that.wrlock ();
    try {
      // move base taggable
      Taggable::operator = (static_cast<Taggable&&>(that));
      // copy move locally
      p_pval = that.p_pval; that.p_pval = nullptr;
      that.unlock ();
    } catch (...) {
      d_name.clear();
      d_info.clear();
      p_pval = nullptr;
      that.unlock ();
    } 
  }
  
  // destroy this property

  Property::~Property (void) {
    delete p_pval;
  }

  // assign a property to this one
  
  Property& Property::operator = (const Property& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // assign base object
      Taggable::operator = (that);
      // assign locally
      delete p_pval; p_pval = nullptr;
      if (that.p_pval != nullptr) {
	p_pval = dynamic_cast <Literal*> (that.p_pval->clone ());
      }
      unlock ();
      return *this;
    } catch (...) {
      that.unlock ();
      unlock ();
      throw;
    }
  }
  
  // move a property to this one

  Property& Property::operator = (Property&& that) noexcept {
    // check for self-move
    if (this == &that) return *this;
    // lock and move
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Taggable::operator = (static_cast<Taggable&&>(that));
      // assign locally
      p_pval = that.p_pval; that.p_pval = nullptr;
      unlock ();
      that.unlock ();
    } catch (...) {
      d_name.clear();
      d_info.clear();
      p_pval = nullptr;
      unlock ();
      that.unlock ();
    }      
    return *this;
  }
  
  // return the class name

  String Property::repr (void) const {
    return "Property";
  }

  // return a clone of this object

  Object* Property::clone (void) const {
    return new Property (*this);
  }

  // return the property did

  t_word Property::getdid (void) const {
    return SRL_DEOD_STD;
  }

  // return the property sid

  t_word Property::getsid (void) const {
    return SRL_PROP_SID;
  }

  // serialize this property

  void Property::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize base object
      Taggable::wrstream (os);
      // serialize locally
      if (p_pval == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_pval->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this property

  void Property::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize base
      Taggable::rdstream (is);
      // deserialize locally
      p_pval = dynamic_cast <Literal*> (Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // clear this property

  void Property::clear (void) {
    wrlock ();
    try {
      d_name.clear ();
      d_info.clear ();
      Object::dref (p_pval); p_pval = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the property by boolean value

  void Property::setpval (const bool bval) {
    wrlock ();
    try {
      delete p_pval;
      p_pval = new Boolean (bval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property by integer value

  void Property::setpval (const t_long ival) {
    wrlock ();
    try {
      delete p_pval;
      p_pval = new Integer (ival);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property by real value

  void Property::setpval (const t_real rval) {
    wrlock ();
    try {
      delete p_pval;
      p_pval = new Real (rval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property value

  void Property::setpval (const Literal& lval) {
    wrlock ();
    try {
      delete p_pval;
      p_pval = dynamic_cast <Literal*> (lval.clone ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the property string value

  String Property::getpval (void) const {
    rdlock ();
    try {
      String result = (p_pval == nullptr) ? "" : p_pval->tostring ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the property as a literal

  Literal* Property::toliteral (void) const {
    rdlock ();
    try {
      Literal* result = (p_pval == nullptr)
	? nullptr
	: dynamic_cast<Literal*> (p_pval->clone());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the property value to a boolean

  bool Property::tobool (void) const {
    rdlock ();
    try {
      // check for a boolean literal
      Boolean* bval = dynamic_cast <Boolean*> (p_pval);
      bool   result = (bval == nullptr) ? false : bval->tobool ();
      // fallback to a string literal
      if ((bval == nullptr) && (p_pval != nullptr)) {
	result = Utility::tobool (p_pval->tostring ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the property value to an integer boolean

  bool Property::toboil (void) const {
    rdlock ();
    try {
      // check for a boolean literal
      Boolean* bval = dynamic_cast <Boolean*> (p_pval);
      bool   result = (bval == nullptr) ? false : bval->tobool ();
      // fallback to a string literal
      if ((bval == nullptr) && (p_pval != nullptr)) {
	result = Utility::toboil (p_pval->tostring ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the property value to an integer

  t_long Property::tolong (void) const {
    rdlock ();
    try {
      // check for an integer literal
      Integer* ival = dynamic_cast <Integer*> (p_pval);
      t_long result = (ival == nullptr) ? 0LL : ival->tolong ();
      // fallback to a string literal
      if ((ival == nullptr) && (p_pval != nullptr)) {
	result = Utility::tolong (p_pval->tostring ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // convert the property value to a real

  t_real Property::toreal (void) const {
    rdlock ();
    try {
      // check for a real literal
      Real*    rval = dynamic_cast <Real*> (p_pval);
      t_real result = (rval == nullptr) ? 0.0 : rval->toreal ();
      // fallback to a string literal
      if ((rval == nullptr) && (p_pval != nullptr)) {
	result = Utility::toreal (p_pval->tostring ());
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property name/value by boolean value

  void Property::set (const String& name, const bool bval) {
    wrlock ();
    try {
      d_name = name;
      delete p_pval;
      p_pval = new Boolean (bval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property name/value by integer value

  void Property::set (const String& name, const t_long ival) {
    wrlock ();
    try {
      d_name = name;
      delete p_pval;
      p_pval = new Integer (ival);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property name/value by real value

  void Property::set (const String& name, const t_real rval) {
    wrlock ();
    try {
      d_name = name;
      delete p_pval;
      p_pval = new Real (rval);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the property name/value at once

  void Property::set (const String& name, const Literal& lval) {
    wrlock ();
    try {
      d_name = name;
      delete p_pval;
      p_pval = dynamic_cast <Literal*> (lval.clone ());
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // format the property value with with style

  String Property::format (const Style& lstl) const {
    rdlock ();
    try {
      String result = (p_pval != nullptr) ? p_pval->format (lstl) : getpval ();
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
  static const long QUARK_SET     = zone.intern ("set");
  static const long QUARK_CLEAR   = zone.intern ("clear");
  static const long QUARK_SETPVAL = zone.intern ("set-value");
  static const long QUARK_GETPVAL = zone.intern ("get-value");
  static const long QUARK_GETBVAL = zone.intern ("get-boolean-value");
  static const long QUARK_GETIVAL = zone.intern ("get-integer-value");
  static const long QUARK_GETRVAL = zone.intern ("get-real-value");

  // create a new object in a generic way

  Object* Property::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new Property;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Property (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String   name = argv->getstring (0);
      Object*   obj = argv->get (1);
      Literal* lobj = dynamic_cast <Literal*> (obj);
      if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with property",
			 Object::repr (obj));
      }
      return new Property (name, *lobj);
    }
    // check for 3 arguments
    if (argc == 3) {
      String   name = argv->getstring (0);
      String   info = argv->getstring (1);
      Object*   obj = argv->get (2);
      Literal* lobj = dynamic_cast <Literal*> (obj);
      if (lobj == nullptr) {
	throw Exception ("type-error", "invalid object with property",
			 Object::repr (obj));
      }
      return new Property (name, info, *lobj);
    }	     
    throw Exception ("argument-error", 
		     "too many argument with property constructor");
  }

  // return true if the given quark is defined

  bool Property::isquark (const long quark, const bool hflg) const {
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

  Object* Property::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETPVAL) return new String  (getpval ());
      if (quark == QUARK_GETBVAL) return new Boolean (tobool  ());
      if (quark == QUARK_GETIVAL) return new Integer (tolong  ());
      if (quark == QUARK_GETRVAL) return new Real    (toreal  ());
      if (quark == QUARK_CLEAR) {
	clear ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETPVAL) {
	Object*   obj = argv->get (0);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-value",
			   Object::repr (obj));
	}
	setpval (*lobj);
	return nullptr;
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SET) {
	String   name = argv->getstring (0);
	Object*   obj = argv->get (1);
	Literal* lobj = dynamic_cast <Literal*> (obj);
	if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with set-value",
			   Object::repr (obj));
	}
	set (name, *lobj);
	return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
