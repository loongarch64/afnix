// ---------------------------------------------------------------------------
// - Part.cpp                                                                -
// - afnix:csm service -  part class implementation                          -
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

#include "Part.hpp"
#include "Whom.hxx"
#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default part

  Part::Part (void) {
    d_kid.random ();
    p_plst = nullptr;
  }

  // create an part by name

  Part::Part (const String& name) : Taggable (name) {
    d_kid.random ();
    p_plst = nullptr;
  }

  // create an part by name and info

  Part::Part (const String& name, const String& info) : Taggable (name, info) {
    d_kid.random ();
    p_plst = nullptr;
  }

  // copy construct this part

  Part::Part (const Part& that) {
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_kid = that.d_kid;
      Object::iref (p_plst = that.p_plst);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // copy move this part

  Part::Part (Part&& that) noexcept {
    that.wrlock ();
    try {
      // copy move base object
      Taggable::operator = (static_cast<Taggable&&>(that));
      // copy move locally
      d_kid  = that.d_kid;  that.d_kid.reset ();
      p_plst = that.p_plst; that.p_plst = nullptr;
    } catch (...) {
      d_kid.reset ();
    }
    that.unlock ();
  }

  // destroy this part

  Part::~Part (void) {
    Object::dref (p_plst);
  }
  
  // assign an part to this one

  Part& Part::operator = (const Part& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base taggable
      Taggable::operator = (that);
      // copy locally
      d_kid = that.d_kid;
      Object::iref (that.p_plst); Object::dref (p_plst); p_plst = that.p_plst;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // move a part to this one

  Part& Part::operator = (Part&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Taggable::operator = (static_cast<Taggable&&>(that));
      // move locally
      d_kid  = that.d_kid;  that.d_kid.reset ();
      p_plst = that.p_plst; that.p_plst = nullptr;
    } catch (...) {
      d_kid.reset ();
    }
    unlock ();
    that.unlock ();
    return *this;
  }

  // return the part class name

  String Part::repr (void) const {
    return "Part";
  }

  // return a clone of this object

  Object* Part::clone (void) const {
    return new Part (*this);
  }

  // return the serial did

  t_word Part::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Part::getsid (void) const {
    return SRL_PART_SID;
  }
  
  // serialize this part

  void Part::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the taggable
      Taggable::wrstream (os);
      // serialize locally
      d_kid.wrstream (os);
      if (p_plst == nullptr) {
	Serial::wrnilid (os);
      } else {
	p_plst->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this part

  void Part::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the taggable
      Taggable::rdstream (is);
      // deserialize locally
      d_kid.rdstream (is);
      Object::iref (p_plst = dynamic_cast <Plist*> (Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // validate a part by key id

  bool Part::iskid (const Uuid& kid) const {
    rdlock ();
    try {
      bool result = (d_kid == kid);
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // validate a part by key id

  bool Part::iskid (const String& kid) const {
    rdlock ();
    try {
      bool result = (d_kid == kid);
      unlock ();
      return result;
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // set a part kid by uuid

  void Part::setkid (const Uuid& kid) {
    wrlock ();
    try {
      d_kid = kid;
      unlock ();
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // set a part kid by string uuid

  void Part::setkid (const String& kid) {
    rdlock ();
    try {
      d_kid = kid;
      unlock ();
    } catch  (...) {
      unlock ();
      throw;
    }
  }

  // get the part kid

  Uuid Part::getkid (void) const {
    rdlock ();
    try {
      Uuid result = d_kid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // request an information list

  Plist Part::getplst (void) const {
    rdlock ();
    try {
      // get the local plist
      Plist result;
      result.add (PN_PRT_NAME, PI_PRT_NAME, d_name);
      result.add (PN_PRT_INFO, PI_PRT_INFO, d_info);
      result.add (PN_PRT_UUID, PI_PRT_UUID, d_kid.tostring ());
      // merge the local one
      if (p_plst != nullptr) result+= *p_plst;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a property exists in the plist

  bool Part::isprop (const String& name) const {
    rdlock ();
    try {
      bool result = (p_plst == nullptr) ? false : p_plst->exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a plist to the part 

  void Part::addplst (const Plist& plst) {
    wrlock ();
    try {
      // check for a plist
      if (p_plst == nullptr) Object::iref (p_plst = new Plist);
      // add the plist
      *p_plst += plst;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add to the plist by name and value

  void Part::addprop (const String& name, const Literal& lval) {
    wrlock ();
    try {
      // check for a plist
      if (p_plst == nullptr) Object::iref (p_plst = new Plist);
      // add by name and literal
      if (p_plst->exists (name) == true) {
	p_plst->set (name, lval);
      } else {
	p_plst->add (name, lval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add to the plist by name, info and literal value

  void Part::addprop (const String&  name, const String& info,
		      const Literal& lval) {
    wrlock ();
    try {
      // check for a plist
      if (p_plst == nullptr) Object::iref (p_plst = new Plist);
      // add by name and literal
      if (p_plst->exists (name) == true) {	  
	p_plst->set (name, info, lval);
      } else {
	p_plst->add (name, info, lval);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // get a plist literal by name

  Literal* Part::getlval (const String& name) const {
    rdlock ();
    try {
      Literal* result = nullptr;
      if (p_plst != nullptr) result = p_plst->toliteral (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
    
  // get a plist value by name

  String Part::getpval (const String& name) const {
    rdlock ();
    try {
      String result;
      if (p_plst != nullptr) result = p_plst->getpval (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if this part is swappable

  bool Part::isswap (Part* part) const {
    // check for nil first
    if (part == nullptr) return false;
    // lock and check
    rdlock ();
    try {
      // check for valid kid
      bool status = (part->getkid () == d_kid);
      // alright from here
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a subpart by name

  Part* Part::subpart (const String&) const {
    return nullptr;
  }
  
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SWAPP   = zone.intern ("swappable-p");
  static const long QUARK_ISKIDP  = zone.intern ("kid-p");
  static const long QUARK_SETKID  = zone.intern ("set-kid");
  static const long QUARK_GETKID  = zone.intern ("get-kid");
  static const long QUARK_GETPLST = zone.intern ("get-plist");
  static const long QUARK_GETPVAL = zone.intern ("get-property-value");
  static const long QUARK_GETLVAL = zone.intern ("get-property-literal");
  static const long QUARK_ISPROPP = zone.intern ("property-p");
  static const long QUARK_ADDPROP = zone.intern ("add-property");
  static const long QUARK_SUBPART = zone.intern ("subpart");

  // create a new object in a generic way

  Object* Part::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default part
    if (argc == 0) return new Part;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Part (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Part (name, info);
    }
    throw Exception ("argument-error",
                     "too many argument with part constructor");
  }

  // return true if the given quark is defined

  bool Part::isquark (const long quark, const bool hflg) const {
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
  
  Object* Part::apply (Evaluable* zobj, Nameset* nset, const long quark,
		       Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETKID)  return new Uuid  (getkid  ());
      if (quark == QUARK_GETPLST) return new Plist (getplst ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ISKIDP) {
	String kid = argv->getstring (0);
	return new Boolean (iskid (kid));
      }
      if (quark == QUARK_SETKID) {
	String kid = argv->getstring (0);
	setkid (kid);
	return nullptr;
      }      
      if (quark == QUARK_ISPROPP) {
	String name = argv->getstring (0);
	return new Boolean (isprop (name));
      }
      if (quark == QUARK_GETLVAL) {
        String name = argv->getstring (0);
	rdlock ();
	try {
	  Object* result = getlval (name);
	  zobj->post (result);
	  unlock ();	  
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETPVAL) {
        String name = argv->getstring (0);
        return new String (getpval (name));
      }
      if (quark == QUARK_SUBPART) {
	String name = argv->getstring (0);
	rdlock ();
	try {
	  Object* result = subpart (name);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_SWAPP) {
        Object* obj = argv->get (0);
        auto   part = dynamic_cast <Part*> (obj);
        if (part == nullptr) {
	  throw Exception ("type-error", "invalid object with swappable-p",
			   Object::repr (obj));
        }
	return new Boolean (isswap(part));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_ADDPROP) {
        String name = argv->getstring (0);
        Object* obj = argv->get (1);
        auto   lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
        }
        addprop (name, *lobj);
        return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_ADDPROP) {
        String name = argv->getstring (0);
        String info = argv->getstring (1);
        Object* obj = argv->get (2);
        auto   lobj = dynamic_cast <Literal*> (obj);
        if (lobj == nullptr) {
	  throw Exception ("type-error", "invalid object with add",
			   Object::repr (obj));
        }
        addprop (name, info, *lobj);
        return nullptr;
      }
    }
    // call the taggable method
    return Taggable::apply (zobj, nset, quark, argv);
  }
}
