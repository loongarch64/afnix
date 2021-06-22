// ---------------------------------------------------------------------------
// - Collection.cpp                                                          -
// - afnix:csm service - part collection class implementation                -
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

#include "Csmsid.hxx"
#include "Vector.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "Collection.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil collection

  Collection::Collection (void) {
    p_cset = nullptr;
  }

  // create a collection by name

  Collection::Collection (const String& name) : Part (name) {
    p_cset = nullptr;
  }

  // create a collection by name and info

  Collection::Collection (const String& name,
			  const String& info) : Part (name, info) {
    p_cset = nullptr;
  }

  // copy construct this collection

  Collection::Collection (const Collection& that) {
    that.rdlock ();
    try {
      // copy base part
      Part::operator = (that);
      // copy locally
      Object::iref (p_cset = that.p_cset);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // copy move this part

  Collection::Collection (Collection&& that) noexcept :
    Part (static_cast<Part&&>(that)) {
    that.wrlock ();
    try {
      p_cset = that.p_cset; that.p_cset = nullptr;
    } catch (...) {
      p_cset = nullptr;
    }
    that.unlock ();
  }
  
  // destroy this collection

  Collection::~Collection (void) {
    Object::dref (p_cset);
  }

  // copy construct this collection

  Collection& Collection::operator = (const Collection& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base part
      Part::operator = (that);
      // copy locally
      Object::iref (that.p_cset); Object::dref (p_cset); p_cset = that.p_cset;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // move a collection to this one

  Collection& Collection::operator = (Collection&& that) noexcept {
    // check for self move
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.wrlock ();
    try {
      // assign base object
      Part::operator = (static_cast<Part&&>(that));
      // move locally
      Object::dref (p_cset); p_cset = that.p_cset; that.p_cset = nullptr;
    } catch (...) {
      p_cset = nullptr;
    }
    unlock ();
    that.unlock ();
    return *this;
  }
  
  // return the collection class name

  String Collection::repr (void) const {
    return "Collection";
  }

  // return the serial did

  t_word Collection::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Collection::getsid (void) const {
    return SRL_COLN_SID;
  }
  
  // serialize this collection

  void Collection::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the part
      Part::wrstream (os);
      // serialize locally
      if (p_cset == nullptr) {
        Serial::wrnilid (os);
      } else {
        p_cset->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this collection

  void Collection::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the part
      Part::rdstream (is);
      // deserialize locally
      Object::iref (p_cset = dynamic_cast <Set*> (Serial::deserialize (is)));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a subpart by name

  Part* Collection::subpart (const String& name) const {
    rdlock ();
    try {
      // get the collection length
      long clen = (p_cset == nullptr) ? 0L : p_cset->length ();
      // loop in the set
      for (long k = 0L; k < clen; k++) {
	auto part = dynamic_cast<Part*> (p_cset->get(k));
	if ((part != nullptr) && (part->getname () == name)) {
	  unlock ();
	  return part;
	}
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // reset this collection
  
  void Collection::reset (void) {
    wrlock ();
    try {
      Object::dref (p_cset); p_cset = nullptr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of parts in the collection

  long Collection::length (void) const {
    rdlock ();
    try {
      long result = (p_cset == nullptr) ? 0L : p_cset->length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // return true if the collection is empty

  bool Collection::empty (void) const {
    rdlock ();
    try {
      bool result = (p_cset == nullptr) ? true : p_cset->empty ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a part exists in this collection

  bool Collection::exists (const Uuid& kid) const {
    rdlock ();
    try {
      // get the set length
      long slen = (p_cset == nullptr) ? 0L : p_cset->length ();
      // loop in the set
      for (long k = 0L; k < slen; k++) {
	// get the part
	Object* obj = p_cset->get (k);
	auto part = dynamic_cast <Part*> (obj);
	if (part == nullptr) {
	  throw Exception ("internal-error", "invalid object in collection",
			   Object::repr (obj));
	}
	// check for valid key id
	if (part->iskid (kid) == true) {
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // check if a part exists in this collection

  bool Collection::exists (const String& kid) const {
    rdlock ();
    try {
      // create the uuid by string
      Uuid uuid = kid;
      // check for existence
      bool result = exists (uuid);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
      
  // add a part in this collection

  Uuid Collection::addpart (Part* part) {
    // check for nil
    if (part == nullptr) {
      throw Exception ("collection-error", "invalid nil part to add");
    }
    // lock and add
    wrlock ();
    try {
      // get the part key id and check
      Uuid kid = part->getkid ();
      // make sure the id is unique
      if (exists (kid) == true) {
	throw Exception ("internal-error", "duplicate unique id found",
			 kid.tostring ());
      }
      // add the part in the set
      if (p_cset == nullptr) p_cset = new Set;
      p_cset->add (part);
      // unlock and return the key id
      unlock ();
      return kid;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a part in this collection
  
  bool Collection::setpart (Part* part) {
    // check for nil part
    if (part == nullptr) return false;
    // lock and set
    wrlock ();
    try {
      // get the part key id and check
      Uuid kid = part->getkid ();
      // get the source part by kid
      Part* sprt = getby (kid);
      if (sprt == nullptr) {
	unlock ();
	return false;
      }
      // check if the part can be swapped
      if (sprt->isswap (part) == false) {
	unlock ();
	return false;
      }
      // replace the part
      bool result = p_cset->replace (part, sprt);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a part at position

  Part* Collection::getat (const long pidx) const {
    rdlock ();
    try {
      // get the part at position
      Object* obj = (p_cset == nullptr) ? nullptr : p_cset->get (pidx);
      auto part = dynamic_cast <Part*> (obj);
      if (part == nullptr) {
	throw Exception ("internal-error", "invalid object in collection",
			 Object::repr (obj));
      }
      unlock ();
      return part;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a part by kid

  Part* Collection::getby (const Uuid& kid) const {
    rdlock ();
    try {
      // get the collection length
      long slen = (p_cset == nullptr) ? 0L : p_cset->length ();
      // loop in the set
      for (long k = 0L; k < slen; k++) {
	Object* obj = p_cset->get (k);
	auto part = dynamic_cast <Part*> (obj);
	if (part == nullptr) {
	  throw Exception ("internal-error", "invalid object in collection",
			   Object::repr (obj));
	}
	// check for valid key id
	if (part->iskid (kid) == true) {
	  unlock ();
	  return part;
	}
      }
      unlock ();
      return nullptr;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a part by kid
  
  Part* Collection::getby (const String& kid) const {
    rdlock ();
    try {
      // get the uuid by string
      Uuid uuid = kid;
      // get the part by uuid
      Part* result = getby (uuid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // delete a part at position

  bool Collection::delat (const long pidx) {
    rdlock ();
    try {
      // get the part at position
      Object* obj = (p_cset == nullptr) ? nullptr : p_cset->get (pidx);
      auto part = dynamic_cast <Part*> (obj);
      if (part == nullptr) {
	throw Exception ("internal-error", "invalid object in collection",
			 Object::repr (obj));
      }
      bool result = remove (part->getkid());
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove a part by uuid

  bool Collection::remove (const Uuid& kid) {
    rdlock ();
    try {
      // get the set length
      long slen = (p_cset == nullptr) ? 0L : p_cset->length ();
      // loop in the set
      for (long k = 0L; k < slen; k++) {
	// get the part
	Object* obj = p_cset->get (k);
	auto part = dynamic_cast <Part*> (obj);
	if (part == nullptr) {
	  throw Exception ("internal-error", "invalid object in collection",
			   Object::repr (obj));
	}
	// check for valid key id
	if (part->iskid (kid) == true) {
	  p_cset->remove (obj);
	  unlock ();
	  return true;
	}
      }
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // remove a part by string kid

  bool Collection::remove (const String& kid) {
    rdlock ();
    try {
      // create the uuid by string
      Uuid uuid = kid;
      // remove by uuid
      bool result = remove (uuid);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // merge a collection into this one

  void Collection::merge (const Collection& colo) {
    wrlock ();
    try {
      long clen = colo.length ();
      for (long k = 0L; k < clen; k++) addpart (colo.getat (k));
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
  static const long QUARK_ZONE_LENGTH = 10;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_RESET   = zone.intern ("reset");
  static const long QUARK_GETAT   = zone.intern ("get-at");
  static const long QUARK_GETBY   = zone.intern ("get-by-kid");
  static const long QUARK_MERGE   = zone.intern ("merge");
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_EMPTYP  = zone.intern ("empty-p");
  static const long QUARK_REMOVE  = zone.intern ("remove");
  static const long QUARK_EXISTSP = zone.intern ("exists-p");
  static const long QUARK_ADDPART = zone.intern ("add-part");
  static const long QUARK_SETPART = zone.intern ("set-part");

  // create a new object in a generic way

  Object* Collection::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new Collection;
    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Collection (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Collection (name, info);
    }
    throw Exception ("argument-error",
                     "too many argument with collection constructor");
  }

  // return true if the given quark is defined

  bool Collection::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Part::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Collection::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_EMPTYP) return new Boolean (empty  ());
      if (quark == QUARK_LENGTH) return new Integer (length ());
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_ADDPART) {
	Object* obj = argv->get (0);
	auto part = dynamic_cast <Part*> (obj);
	if (part == nullptr) {
	  throw Exception ("type-error", "invalid object with collection add",
			   Object::repr (obj));
	}
	Uuid kid = addpart (part);
	return new Uuid (kid);
      }
      if (quark == QUARK_SETPART) {
	Object* obj = argv->get (0);
	auto part = dynamic_cast <Part*> (obj);
	if (part == nullptr) {
	  throw Exception ("type-error", "invalid object with collection set",
			   Object::repr (obj));
	}
	return new Boolean (setpart (part));
      }
      if (quark == QUARK_GETAT) {
	long pidx = argv->getlong (0);
	rdlock ();
	try {
	  Object* result = getat (pidx);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_GETBY) {
	try {
	  Object* obj = argv->get (0);
	  Object* result = nullptr;
	  // check for a uuid
	  auto uuid = dynamic_cast<Uuid*>(obj);
	  if (uuid != nullptr) result = getby (*uuid);
	  // check for a string
	  auto suid = dynamic_cast<String*>(obj);
	  if (suid != nullptr) result = getby (*suid);
	  if ((uuid == nullptr) && (suid == nullptr)) {
	    throw Exception ("type-error",
			     "invalid object for collection getby",
			     Object::repr (obj));
	  } 
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
      if (quark == QUARK_EXISTSP) {
	Object* obj = argv->get(0);
	// check for a uuid
	auto uuid = dynamic_cast<Uuid*>(obj);
	if (uuid != nullptr) return new Boolean (exists(*uuid));
	// check for a string
	auto suid = dynamic_cast<String*>(obj);
	if (suid != nullptr) return new Boolean (exists(*suid));
	throw Exception ("type-error", "invalid object for collection kid-p",
			 Object::repr (obj));
      }
      if (quark == QUARK_REMOVE) {
	Object* obj = argv->get(0);
	// check for a uuid
	auto uuid = dynamic_cast<Uuid*>(obj);
	if (uuid != nullptr) return new Boolean (remove(*uuid));
	// check for a string
	auto suid = dynamic_cast<String*>(obj);
	if (suid != nullptr) return new Boolean (remove(*suid));
	throw Exception ("type-error", "invalid object for collection remove",
			 Object::repr (obj));
      }
      if (quark == QUARK_MERGE) {
	Object* obj = argv->get (0);
	auto colo = dynamic_cast <Collection*> (obj);
	if (colo == nullptr) {
	  throw Exception ("type-error", "invalid object with collection merge",
			   Object::repr (obj));
	}
	merge (*colo);
	return nullptr;
      }
    }
    // call the part method
    return Part::apply (zobj, nset, quark, argv);
  }
}
