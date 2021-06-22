// ---------------------------------------------------------------------------
// - AliasTable.cpp                                                          -
// - standard object library - alias table class implementation              -
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
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AliasTable.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty alias table

  AliasTable::AliasTable (void) {
    reset ();
  }

  // create an alias table by data

  AliasTable::AliasTable (const long size, const char** data) {
    reset ();
    if ((size > 0L) && (data != nullptr) && ((size % 2) == 0)) {
      for (long k = 0L; k < size; k+= 2) {
	add (data[k], data[k+1]);
      }
    }
  }
  
  // return the class name

  String AliasTable::repr (void) const {
    return "AliasTable";
  }

  // reset this alias table

  void AliasTable::reset (void) {
    wrlock ();
    try {
      d_dtbl.reset ();
      d_rtbl.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
	
  // check if the alias table is empty

  bool AliasTable::empty (void) const {
    rdlock ();
    try {
      bool result = d_dtbl.empty ();
      if (d_rtbl.empty () != result) {
	throw Exception ("internal-error", "inconsistent alias table");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a name exists

  bool AliasTable::exists (const String& name) const {
    rdlock ();
    try {
      bool result = d_dtbl.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a alias name exists

  bool AliasTable::isalias (const String& name) const {
    rdlock ();
    try {
      bool result = d_rtbl.exists (name);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add an alias by name

  void AliasTable::add (const String& name, const String& anam) {
    wrlock ();
    try {
      d_dtbl.add (name, new String (anam));
      d_rtbl.add (anam, new String (name));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a name to an alias

  String AliasTable::map (const String& name) const {
    rdlock ();
    try {
      // get the alias by name
      Object*  obj = d_dtbl.get (name);
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj == nullptr) {
	throw Exception ("alias-error", "cannot find alias by name", name);
      }
      String result = *sobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property by name to an alias

  Property AliasTable::map (const Property& prop) const {
    rdlock ();
    try {
      // prepare the perty result
      Property result = prop;
      // get the alias by name
      Object*  obj = d_dtbl.get (prop.getname ());
      String* sobj = dynamic_cast <String*> (obj);
      // map the property name
      if (sobj != nullptr) result.setname (*sobj);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a plist by name 

  Plist AliasTable::map (const Plist& plst) const {
    rdlock ();
    try {
      // prepare the plist result
      Plist result;
      // loop in the plist
      long plen = plst.length ();
      for (long k = 0; k < plen; k++) {
	Property* prop = plst.get (k);
	if (prop == nullptr) continue;
	result.add (new Property (map (*prop)));
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map an alias to a name

  String AliasTable::remap (const String& name) const {
    rdlock ();
    try {
      // get the name by alias
      Object*  obj = d_rtbl.get (name);
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj == nullptr) {
	throw Exception ("alias-error", "cannot find name by alias", name);
      }
      String result = *sobj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // map a property by alias to a name

  Property AliasTable::remap (const Property& prop) const {
    rdlock ();
    try {
      // prepare the perty result
      Property result = prop;
      // get the alias by name
      Object*  obj = d_rtbl.get (prop.getname ());
      String* sobj = dynamic_cast <String*> (obj);
      // map the property name
      if (sobj != nullptr) result.setname (*sobj);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // remap a plist by alias name 

  Plist AliasTable::remap (const Plist& plst) const {
    rdlock ();
    try {
      // prepare the plist result
      Plist result;
      // loop in the plist
      long plen = plst.length ();
      for (long k = 0; k < plen; k++) {
	Property* prop = plst.get (k);
	if (prop == nullptr) continue;
	result.add (new Property (remap (*prop)));
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
  static const long QUARK_ZONE_LENGTH = 5;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_MAP    = zone.intern ("map");
  static const long QUARK_REMAP  = zone.intern ("remap");
  static const long QUARK_RESET  = zone.intern ("reset");
  static const long QUARK_EXISTP = zone.intern ("exists-p");
  static const long QUARK_ALIASP = zone.intern ("alias-p");

  // create a new object in a generic way

  Object* AliasTable::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check 0 argument
    if (argc == 0) return new AliasTable;
    // invalid arguments
    throw Exception ("argument-error", "too many argument for alias table");
  }

  // return true if the given quark is defined

  bool AliasTable::isquark (const long quark, const bool hflg) const {
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

  Object* AliasTable::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_RESET) {
	reset ();
	return nullptr;
      }
    }
    
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_EXISTP) {
	String name = argv->getstring (0);
	return new Boolean (exists (name));
      }
      if (quark == QUARK_ALIASP) {
	String name = argv->getstring (0);
	return new Boolean (isalias (name));
      }
      if (quark == QUARK_MAP) {
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new String (map (*sobj));
	// check for a property
	Property* prop = dynamic_cast <Property*> (obj);
	if (prop != nullptr) return new Property (map (*prop));
	// check for a plist
	Plist* plst = dynamic_cast <Plist*> (obj);
	if (plst != nullptr) return new Plist (map (*plst));
	// invalid type
	throw Exception ("type-error", "invalid object with map",
			 Object::repr (obj));
      }
      if (quark == QUARK_REMAP) {
	Object* obj = argv->get (0);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new String (remap (*sobj));
	// check for a property
	Property* prop = dynamic_cast <Property*> (obj);
	if (prop != nullptr) return new Property (remap (*prop));
	// check for a plist
	Plist* plst = dynamic_cast <Plist*> (obj);
	if (plst != nullptr) return new Plist (remap (*plst));
	// invalid type
	throw Exception ("type-error", "invalid object with remap",
			 Object::repr (obj));
      }
    }
    // call the object method
    return Object::apply (zobj, nset, quark, argv);
  }
}
