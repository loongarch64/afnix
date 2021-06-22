// ---------------------------------------------------------------------------
// - Carrier.cpp                                                             -
// - afnix:csm service - object carrier blob class implementation           -
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
#include "Carrier.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the blob request plist
  static const String PN_BLB_CAGO = "PN-BLB-CAGO";
  static const String PI_BLB_CAGO = "BLOB CARGO";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil carrier

  Carrier::Carrier (void) {
    p_aobj = nullptr;
  }

  // create an anonymous carrier

  Carrier::Carrier (Object* aobj) {
    Object::iref (p_aobj = aobj);
  }

  // create a carrier object by name

  Carrier::Carrier (Object* aobj, const String& name) : Blob (name) {
    Object::iref (p_aobj = aobj);
  }

  // create a carrier object by name and info

  Carrier::Carrier (Object* aobj, const String& name,
		    const String& info) : Blob (name, info) {
    Object::iref (p_aobj = aobj);
  }

  // create a carrier object by rid, name and info

  Carrier::Carrier (Object* aobj, const String& rid, const String& name,
		    const String& info) : Blob (rid, name, info) {
    Object::iref (p_aobj = aobj);
  }
  
  // copy construct this carrier object

  Carrier::Carrier (const Carrier& that) {
    that.rdlock ();
    try {
      Blob::operator = (that);      
      Object::iref (p_aobj = that.p_aobj);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // destroy this registered object

  Carrier::~Carrier (void) {
    Object::dref (p_aobj);
  }

  // assign a registered object to this one

  Carrier& Carrier::operator = (const Carrier& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base object
      Blob::operator = (that);
      // protect the copy
      Object::iref (that.p_aobj);
      // remove old object
      Object::dref (p_aobj);
      // set local data
      p_aobj  = that.p_aobj;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the carrier class name

  String Carrier::repr (void) const {
    return "Carrier";
  }

  // return a clone of this object

  Object* Carrier::clone (void) const {
    return new Carrier (*this);
  }

  // return the serial did

  t_word Carrier::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Carrier::getsid (void) const {
    return SRL_CBLB_SID;
  }
  
  // serialize this carrier

  void Carrier::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base object
      Blob::wrstream (os);
      // serialize locally
      if (p_aobj == nullptr) {
	Serial::wrnilid (os);
      } else {
	Serial* sobj = dynamic_cast <Serial*> (p_aobj);
	if (sobj == nullptr) {
	  throw Exception ("serial-error", "cannot serialize object", 
			   p_aobj->repr ());
	}
	sobj->serialize (os);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this carrier

  void Carrier::rdstream (InputStream& is) {
    wrlock ();
    try {
      //deserialize the base object
      Blob::rdstream (is);
      // deserialize locally
      Object::iref (p_aobj = Serial::deserialize (is));
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // request an information list
  
  Plist Carrier::getplst (void) const {
    rdlock ();
    try {
      Plist result = Blob::getplst ();;
      result.add (PN_BLB_CAGO, PI_BLB_CAGO, Object::repr (p_aobj));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the carrier object

  Object* Carrier::getobj (void) const {
    rdlock ();
    try {
      Object* result = p_aobj;
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
  static const long QUARK_ZONE_LENGTH = 1;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETOBJ  = zone.intern ("get-object");

  // create a new object in a generic way

  Object* Carrier::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default carrier object
    if (argc == 0) return new Carrier;
    // check for 1 argument
    if (argc == 1) {
      Object* aobj = argv->get (0);
      return new Carrier (aobj);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* aobj = argv->get (0);
      String  name = argv->getstring (1);
      return new Carrier (aobj, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      Object* aobj = argv->get (0);
      String  name = argv->getstring (1);
      String  info = argv->getstring (2);
      return new Carrier (aobj, name, info);
    }
    // check for 4 arguments
    if (argc == 4) {
      Object* aobj = argv->get (0);
      String   rid = argv->getstring (1);
      String  name = argv->getstring (2);
      String  info = argv->getstring (3);
      return new Carrier (aobj, rid, name, info);
    }
    throw Exception ("argument-error",
                     "too many argument with carrier constructor");
  }

  // return true if the given quark is defined
  
  bool Carrier::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Blob::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Carrier::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETOBJ) {
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
    }
    // call the blob method
    return Blob::apply (zobj, nset, quark, argv);
  }
}
