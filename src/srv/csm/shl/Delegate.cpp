// ---------------------------------------------------------------------------
// - Delegate.cpp                                                             -
// - afnix:csm service - delegate blob class implementation                  -
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
#include "Delegate.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the blob request plist
  static const String PN_BLB_ADDR = "PN-BLB-ADDR";
  static const String PI_BLB_ADDR = "BLOB DELEGATE ADDRESS";
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a nil delegate

  Delegate::Delegate (void) {
    d_addr = "";
  }

  // create an anonymous delegate

  Delegate::Delegate (Object* aobj) : Carrier (aobj) {
    d_addr = "";
  }

  // create a delegate object by name

  Delegate::Delegate (Object* aobj,
		      const String& name) : Carrier (aobj, name) {
    d_addr = "";
  }

  // create a delegate object by name and info

  Delegate::Delegate (Object* aobj, const String& name,
		      const String& info) : Carrier (aobj, name, info) {
    d_addr = "";
  }

  // create a delegate object by rid, name and info

  Delegate::Delegate (Object* aobj, const String& rid, const String& name,
		      const String& info) : Carrier (aobj, rid, name, info) {
    d_addr = "";
  }
  
  // create a delegate object by rid, name, info and address

  Delegate::Delegate (Object* aobj, const String& rid, const String& name,
		      const String& info,const String& addr) :
    Carrier (aobj, rid, name, info) {
    d_addr = addr;
  }
  
  // copy construct this delegate object

  Delegate::Delegate (const Delegate& that) {
    that.rdlock ();
    try {
      Carrier::operator = (that);      
      d_addr = that.d_addr;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a registered object to this one

  Delegate& Delegate::operator = (const Delegate& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy base object
      Carrier::operator = (that);
      // set local data
      d_addr  = that.d_addr;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }
  
  // return the delegate class name

  String Delegate::repr (void) const {
    return "Delegate";
  }

  // return a clone of this object

  Object* Delegate::clone (void) const {
    return new Delegate (*this);
  }

  // return the serial did

  t_word Delegate::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Delegate::getsid (void) const {
    return SRL_DBLB_SID;
  }
  
  // serialize this delegate

  void Delegate::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base object
      Carrier::wrstream (os);
      // serialize locally
      d_addr.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this delegate

  void Delegate::rdstream (InputStream& is) {
    wrlock ();
    try {
      // deserialize the base object
      Carrier::rdstream (is);
      // deserialize locally
      d_addr.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request an information list
  
  Plist Delegate::getplst (void) const {
    rdlock ();
    try {
      Plist result = Carrier::getplst ();;
      result.add (PN_BLB_ADDR, PI_BLB_ADDR, d_addr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the delegate address

  void Delegate::setaddr (const String& addr) {
    wrlock ();
    try {
      d_addr = addr;
      unlock ();
    } catch (...) {
      unlock();
      throw;
    }
  }

  // get the delegate address

  String Delegate::getaddr (void) const {
    rdlock ();
    try {
      String result = d_addr;
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
  static const long QUARK_ZONE_LENGTH = 2;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETADDR = zone.intern ("set-address");
  static const long QUARK_GETADDR = zone.intern ("get-address");

  // create a new object in a generic way

  Object* Delegate::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default delegate object
    if (argc == 0) return new Delegate;
    // check for 1 argument
    if (argc == 1) {
      Object* aobj = argv->get (0);
      return new Delegate (aobj);
    }
    // check for 2 arguments
    if (argc == 2) {
      Object* aobj = argv->get (0);
      String  name = argv->getstring (1);
      return new Delegate (aobj, name);
    }
    // check for 3 arguments
    if (argc == 3) {
      Object* aobj = argv->get (0);
      String  name = argv->getstring (1);
      String  info = argv->getstring (2);
      return new Delegate (aobj, name, info);
    }
    // check for 4 arguments
    if (argc == 4) {
      Object* aobj = argv->get (0);
      String   rid = argv->getstring (1);
      String  name = argv->getstring (2);
      String  info = argv->getstring (3);
      return new Delegate (aobj, rid, name, info);
    }
    // check for 5 arguments
    if (argc == 5) {
      Object* aobj = argv->get (0);
      String   rid = argv->getstring (1);
      String  name = argv->getstring (2);
      String  info = argv->getstring (3);
      String  addr = argv->getstring (4);
      return new Delegate (aobj, rid, name, info, addr);
    }
    throw Exception ("argument-error",
                     "too many argument with delegate constructor");
  }

  // return true if the given quark is defined
  
  bool Delegate::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Carrier::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark
  
  Object* Delegate::apply (Evaluable* zobj, Nameset* nset, const long quark,
			   Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETADDR) return new String (getaddr());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETADDR) {
	String addr = argv->getstring(0);
	setaddr (addr);
	return nullptr;
      }
    }
    // call the carrier method
    return Carrier::apply (zobj, nset, quark, argv);
  }
}
