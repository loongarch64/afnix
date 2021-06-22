// ---------------------------------------------------------------------------
// - Tracker.cpp                                                             -
// - afnix:csm service - tracker blob class implementation                  -
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
#include "Tracker.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {
  
  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default host name
  static const String PV_BLB_HOST = "localhost";
  // the default host port
  static const long   PV_BLB_PORT = 0L;
  // the default host protocol
  static const String PV_BLB_PRTO = "tcp";

  // the blob request plist
  static const String PN_BLB_HOST = "PN-BLB-HOST";
  static const String PI_BLB_HOST = "TRACKER HOST NAME";
  static const String PN_BLB_PORT = "PN-BLB-PORT";
  static const String PI_BLB_PORT = "TRACKER HOST NAME";
  static const String PN_BLB_PRTO = "PN-BLB-PORT";
  static const String PI_BLB_PRTO = "TRACKER PROTOCOL";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------
  
  // create a default tracker

  Tracker::Tracker (void) {
    d_host = PV_BLB_HOST;
    d_port = PV_BLB_PORT;
    d_prto = PV_BLB_PRTO;
  }

  // create a tracker by name

  Tracker::Tracker (const String& name) : Blob (name) {
    d_host = PV_BLB_HOST;
    d_port = PV_BLB_PORT;
    d_prto = PV_BLB_PRTO;
  }

  // create a tracker by name and info

  Tracker::Tracker (const String& name, const String& info) :
    Blob (name, info) {
    d_host = PV_BLB_HOST;
    d_port = PV_BLB_PORT;
    d_prto = PV_BLB_PRTO;
  }

  // copy construct this tracker

  Tracker::Tracker (const Tracker& that) {
    that.rdlock ();
    try {
      // copy the base blob
      Blob::operator = (that);
      // copy locally
      d_host = that.d_host;
      d_port = that.d_port;
      d_prto = that.d_prto;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a tracker to this one

  Tracker& Tracker::operator = (const Tracker& that) {
    // check for self-assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      // copy the base blob
      Blob::operator = (that);
      // copy locally
      d_host = that.d_host;
      d_port = that.d_port;
      d_prto = that.d_prto;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // get the class name

  String Tracker::repr (void) const {
    return "Tracker";
  }

  // return a clone of this object

  Object* Tracker::clone (void) const {
    return new Tracker (*this);
  }

  // return the serial did
  
  t_word Tracker::getdid (void) const {
    return SRL_DEOD_CSM;
  }

  // return the serial sid

  t_word Tracker::getsid (void) const {
    return SRL_TCKR_SID;
  }
  
  // serialize this tracker

  void Tracker::wrstream (OutputStream& os) const {
    rdlock ();
    try {
      // serialize the base object
      Blob::wrstream (os);
      // serialize locally
      d_host.wrstream (os);
      Serial::wrlong (d_port, os);
      d_prto.wrstream (os);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // deserialize this tracker

  void Tracker::rdstream (InputStream& is) {
    wrlock ();
    try {
      //deserialize the base object
      Blob::rdstream (is);
      // deserialize locally
      d_host.rdstream (is);
      d_port = Serial::rdlong (is);
      d_prto.rdstream (is);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // request an information list

  Plist Tracker::getplst (void) const {
    rdlock ();
    try {
      Plist result = Blob::getplst ();;
      result.add (PN_BLB_HOST, PI_BLB_HOST, d_host);
      result.add (PN_BLB_PORT, PI_BLB_PORT, (t_long) d_port);
      result.add (PN_BLB_PRTO, PI_BLB_PRTO, d_prto);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the tracker host name

  void Tracker::sethost (const String& host) {
    wrlock ();
    try {
      d_host = host;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tracker host name

  String Tracker::gethost (void) const {
    rdlock ();
    try {
      String result = d_host;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tracker port

  void Tracker::setport (const long port) {
    wrlock ();
    try {
      d_port = port;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tracker port

  long Tracker::getport (void) const {
    rdlock ();
    try {
      long result = d_port;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tracker protocol

  void Tracker::setprto (const String& prto) {
    wrlock ();
    try {
      d_prto = prto;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tracker protocol

  String Tracker::getprto (void) const {
    rdlock ();
    try {
      String result = d_prto;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if a tracker has the same authority

  bool Tracker::isauth (const Tracker& tckr) const {
    rdlock ();
    try {
      bool result = ((tckr.gethost () == d_host) && 
		     (tckr.getport () == d_port) &&
		     (tckr.getprto () == d_prto));
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
  static const long QUARK_SETHOST = zone.intern ("set-host");
  static const long QUARK_GETHOST = zone.intern ("get-host");
  static const long QUARK_SETPORT = zone.intern ("set-port");
  static const long QUARK_GETPORT = zone.intern ("get-port");
  static const long QUARK_SETPRTO = zone.intern ("set-protocol");
  static const long QUARK_GETPRTO = zone.intern ("get-protocol");
  static const long QUARK_ISAUTHP = zone.intern ("authority-p");

  // create a new object in a generic way

  Object* Tracker::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String name = argv->getstring (0);
      return new Tracker (name);
    }
    // check for 2 arguments
    if (argc == 2) {
      String name = argv->getstring (0);
      String info = argv->getstring (1);
      return new Tracker (name, info);
    }
    // invalid arguments
    throw Exception ("argument-error", 
                     "invalid arguments with tracker constructor");
  }

  // return true if the given quark is defined

  bool Tracker::isquark (const long quark, const bool hflg) const {
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
  
  Object* Tracker::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETHOST) return new String  (gethost ());
      if (quark == QUARK_GETPORT) return new Integer (getport ());
      if (quark == QUARK_GETPRTO) return new String  (getprto ());
    }
    if (argc == 1) {
      if (quark == QUARK_SETHOST) {
        String host = argv->getstring (0);
        sethost (host);
        return nullptr;
      }
      if (quark == QUARK_SETPORT) {
        long port = argv->getlong (0);
        setport (port);
        return nullptr;
      }
      if (quark == QUARK_SETPRTO) {
        String prto = argv->getstring (0);
        setprto (prto);
        return nullptr;
      }
      if (quark == QUARK_ISAUTHP) {
	Object* obj = argv->get (0);
	Tracker*   xas = dynamic_cast <Tracker*> (obj);
	if (xas == nullptr) {
	  throw Exception ("type-error", "invalid object for authority-p",
			   Object::repr (obj));
	}
	return new Boolean (isauth (*xas));
      }		   
    }
    // call the blob methods
    return Blob::apply (zobj, nset, quark, argv);
  }
}
