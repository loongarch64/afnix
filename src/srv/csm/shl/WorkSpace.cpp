// ---------------------------------------------------------------------------
// - WorkSpace.cpp                                                           -
// - afnix:csm module - abstract workspace class implementation              -
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
#include "Boolean.hpp"
#include "WorkSpace.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // default workspace name, infor and flags
  static const String WS_NAME_DEF = "";
  static const String WS_INFO_DEF = "";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default workspace

  WorkSpace::WorkSpace (void) {
    d_name = WS_NAME_DEF;
    d_info = WS_NAME_DEF;
  }

  // create a workspace by name

  WorkSpace::WorkSpace (const String& name) {
    d_name = name;
    d_info = WS_NAME_DEF;
  }

  // create a workspace by name and info

  WorkSpace::WorkSpace (const String& name, const String& info) {
    d_name = name;
    d_info = info;
  }

  // get the workspace name

  String WorkSpace::getname (void) const {
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

  // set the workspace name

  void WorkSpace::setname (const String& name) {
    wrlock ();
    try {
      d_name = name;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the workspace info

  String WorkSpace::getinfo (void) const {
    rdlock ();
    try {
      String result = d_info;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the workspace info

  void WorkSpace::setinfo (const String& info) {
    wrlock ();
    try {
      d_info = info;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if an entity exists by zone and string uri

  bool WorkSpace::exists (const String& zone, const String& suri) const {
    rdlock ();
    WorkZone* wzon = nullptr;
    try {
      // get a workzone
      Object::iref (wzon = getzone (zone));
      if (wzon == nullptr) {
	throw Exception ("workspace-error", "cannot access workzone", zone);
      }
      // check by uri
      bool result = wzon->exists (suri);
      Object::dref (wzon);
      unlock ();
      return result;
    } catch (...) {
      Object::dref (wzon);
      unlock ();
      throw;
    }
  }

  // get an input stream by zone and string uri

  InputStream* WorkSpace::getis (const String& zone, const String& suri) const {
    rdlock ();
    WorkZone* wzon = nullptr;
    try {
      // get a workzone
      Object::iref (wzon = getzone (zone));
      if (wzon == nullptr) {
	throw Exception ("workspace-error", "cannot access workzone", zone);
      }
      // get the input stream
      InputStream* is = wzon->getis (suri);
      Object::dref (wzon);
      unlock ();
      return is;
    } catch (...) {
      Object::dref (wzon);
      unlock ();
      throw;
    }
  }

  // get an output stream by zone and normalized uri

  OutputStream* WorkSpace::getos (const String& zone,
				  const String& suri) const {
    rdlock ();
    WorkZone* wzon = nullptr;
    try {
      // get a workzone
      Object::iref (wzon = getzone (zone));
      if (wzon == nullptr) {
	throw Exception ("workspace-error", "cannot access workzone", zone);
      }
      // get the output stream
      OutputStream* os = wzon->getos (suri);
      Object::dref (wzon);
      unlock ();
      return os;
    } catch (...) {
      Object::dref (wzon);
      unlock ();
      throw;
    }
  }

 
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 13;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_ZONEP    = zone.intern ("zone-p");
  static const long QUARK_GETIS    = zone.intern ("get-input-stream");
  static const long QUARK_GETOS    = zone.intern ("get-output-stream");
  static const long QUARK_TONURI   = zone.intern ("to-normalized-uri");
  static const long QUARK_REMOVE   = zone.intern ("remove");
  static const long QUARK_EXISTSP  = zone.intern ("exists-p");
  static const long QUARK_ADDZONE  = zone.intern ("add-zone");
  static const long QUARK_GETZONE  = zone.intern ("get-zone");
  static const long QUARK_SETNAME  = zone.intern ("set-name");
  static const long QUARK_GETINFO  = zone.intern ("get-info");
  static const long QUARK_SETINFO  = zone.intern ("set-info");
  static const long QUARK_GETELST  = zone.intern ("get-entity-list");
  static const long QUARK_TOEPTBL  = zone.intern ("to-entity-table");

  // return true if the given quark is defined

  bool WorkSpace::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Nameable::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark
  
  Object* WorkSpace::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETINFO) return new String  (getinfo ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETNAME) {
	String name = argv->getstring (0);
	setname (name);
	return nullptr;
      }
      if (quark == QUARK_SETINFO) {
	String info = argv->getstring (0);
	setinfo (info);
	return nullptr;
      }
      if (quark == QUARK_ZONEP) {
	String zone = argv->getstring (0);
	return new Boolean (iszone (zone));
      }
      if (quark == QUARK_REMOVE) {
	String zone = argv->getstring (0);
	return new Boolean (rmzone (zone));
      }
      if (quark == QUARK_ADDZONE) {
	String zone = argv->getstring (0);
	return addzone (zone);
      }
      if (quark == QUARK_GETZONE) {
	String zone = argv->getstring (0);
	return getzone (zone);
      }
      if (quark == QUARK_GETELST) {
	String zone = argv->getstring (0);
	return getelst (zone);
      }
      if (quark == QUARK_TOEPTBL) {
	String zone = argv->getstring (0);
	return toeptbl (zone);
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_EXISTSP) {
	// get the zone
	String zone = argv->getstring (0);
	// check for a a uri
	Object* obj = argv->get (1);
	Uri*    uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return new Boolean (exists (zone, *uri));
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return new Boolean (exists (zone, *sobj));
	// invalid object
	throw Exception ("type-error", "invalid object for workspace exists",
			 Object::repr (obj));
      }
      if (quark == QUARK_TONURI) {
	String zone = argv->getstring (0);
	String name = argv->getstring (1);
	return new String (tonuri (zone, name));
      }
      if (quark == QUARK_GETIS) {
	// get the zone
	String zone = argv->getstring (0);
	// check for a a uri
	Object* obj = argv->get (1);
	Uri*    uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return getis (zone, *uri);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return getis (zone, *sobj);
	// invalid object
	throw Exception ("type-error", 
			 "invalid object for workspace input stream",
			 Object::repr (obj));
      }
      if (quark == QUARK_GETOS) {
	// get the zone
	String zone = argv->getstring (0);
	// check for a a uri
	Object* obj = argv->get (1);
	Uri*    uri = dynamic_cast <Uri*> (obj);
	if (uri != nullptr) return getos (zone, *uri);
	// check for a string
	String* sobj = dynamic_cast <String*> (obj);
	if (sobj != nullptr) return getos (zone, *sobj);
	// invalid object
	throw Exception ("type-error", 
			 "invalid object for workspace output stream",
			 Object::repr (obj));
      }
    }
    // call the nameable method
    return Nameable::apply (zobj, nset, quark, argv);
  }
}
