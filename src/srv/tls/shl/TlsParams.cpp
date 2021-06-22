// ---------------------------------------------------------------------------
// - TlsParams.cpp                                                           -
// - afnix:tls service - tls parameters class implementation                 -
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

#include "Byte.hpp"
#include "Vector.hpp"
#include "Socket.hpp"
#include "Boolean.hpp"
#include "Integer.hpp"
#include "TlsTypes.hxx"
#include "TlsParams.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the plist parameters name and info
  static const String PN_PRM_VMAJ = "TLS-PRM-VMAJ";
  static const String PI_PRM_VMAJ = "TLS PARAMETER MAJOR VERSION";
  static const String PN_PRM_VMIN = "TLS-PRM-VMIN";
  static const String PI_PRM_VMIN = "TLS PARAMETER MINOR VERSION";
  static const String PN_PRM_CERT = "TLS-PRM-CERT";
  static const String PI_PRM_CERT = "TLS PARAMETER CERTIFICATE NAME";
  static const String PN_PRM_CKEY = "TLS-PRM-CKEY";
  static const String PI_PRM_CKEY = "TLS PARAMETER CERTIFICATE KEY";
  static const String PN_PRM_HOST = "TLS-PRM-HOST";
  static const String PI_PRM_HOST = "TLS PARAMETER HOST NAME";
  static const String PN_PRM_PORT = "TLS-PRM-PORT";
  static const String PI_PRM_PORT = "TLS PARAMETER HOST PORT";
  static const String PN_PRM_DEXT = "TLS-PRM-DEXT";
  static const String PI_PRM_DEXT = "TLS PARAMETER DEBUG EXTENSION";

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tls parameters

  TlsParams::TlsParams (void) {
    reset ();
  }
  
  // create a tls parameters by host and port

  TlsParams::TlsParams (const String& host, const t_word port) {
    reset ();
    d_host = host;
    d_port = port;
  }

  // copy construct this tls parameters

  TlsParams::TlsParams (const TlsParams& that) {
    that.rdlock ();
    try {
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_cert = that.d_cert;
      d_ckey = that.d_ckey;
      d_host = that.d_host;
      d_port = that.d_port;
      d_dext = that.d_dext;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // assign a tls parameters to this one

  TlsParams& TlsParams::operator = (const TlsParams& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_vmaj = that.d_vmaj;
      d_vmin = that.d_vmin;
      d_cert = that.d_cert;
      d_ckey = that.d_ckey;
      d_host = that.d_host;
      d_port = that.d_port;
      d_dext = that.d_dext;
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
  
  String TlsParams::repr (void) const {
    return "TlsParams";
  }

  // clone this object

  Object* TlsParams::clone (void) const {
    return new TlsParams (*this);
  }

  // reset the tls parameters

  void TlsParams::reset (void) {
    wrlock ();
    try {
      d_vmaj = TLS_VMAJ_DEF;
      d_vmin = TLS_VMIN_DEF;
      d_cert = "";
      d_ckey = "";
      d_host = "";
      d_port = 0;
      d_dext = false;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls parameters as a plist

  Plist TlsParams::getinfo (void) const {
    rdlock ();
    try {
      // create a result plist
      Plist plst;
      // set the result plist
      plst.add (PN_PRM_VMAJ, PI_PRM_VMAJ, (t_long) d_vmaj);
      plst.add (PN_PRM_VMIN, PI_PRM_VMIN, (t_long) d_vmin);
      plst.add (PN_PRM_CERT, PI_PRM_CERT, d_cert);
      plst.add (PN_PRM_CKEY, PI_PRM_CKEY, d_ckey);
      plst.add (PN_PRM_HOST, PI_PRM_HOST, d_host);
      plst.add (PN_PRM_PORT, PI_PRM_PORT, (t_long) d_port);
      plst.add (PN_PRM_DEXT, PI_PRM_DEXT, d_dext);
      // here it is
      unlock ();
      return plst;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls major version

  t_byte TlsParams::getvmaj (void) const {
    rdlock ();
    try {
      t_byte result = d_vmaj;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls minor version

  t_byte TlsParams::getvmin (void) const {
    rdlock ();
    try {
      t_byte result = d_vmin;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tls version

  void TlsParams::setvers (const t_byte vmaj, const t_byte vmin) {
    wrlock ();
    try {
      // check version
      if (tls_vers_valid (vmaj, vmin) == false) {
	String vers = tls_vers_tostring (vmaj, vmin);
	throw Exception ("tls-error", "invalid tls version", vers);
      }
      d_vmaj = vmaj;
      d_vmin = vmin;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the tls certificate list

  void TlsParams::setcert (const String& cert) {
    wrlock ();
    try {
      d_cert = cert;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls certificate list

  String TlsParams::getcert (void) const {
    rdlock ();
    try {
      String result = d_cert;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the tls certificate key

  void TlsParams::setckey (const String& ckey) {
    wrlock ();
    try {
      d_ckey = ckey;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls certificate key

  String TlsParams::getckey (void) const {
    rdlock ();
    try {
      String result = d_ckey;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the tls certificate list and key

  void TlsParams::setcert (const String& cert, const String& ckey) {
    wrlock ();
    try {
      d_cert = cert;
      d_ckey = ckey;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the tls host name

  void TlsParams::sethost (const String& host) {
    wrlock ();
    try {
      d_host = host;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls host name

  String TlsParams::gethost (void) const {
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

  // set the tls host port

  void TlsParams::setport (const long port) {
    wrlock ();
    try {
      d_port = port;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls host port

  long TlsParams::getport (void) const {
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

  // set the debug extension flag

  void TlsParams::setdext (const bool dext) {
    wrlock ();
    try {
      d_dext = dext;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the debug extension flag

  bool TlsParams::getdext (void) const {
    rdlock ();
    try {
      bool result = d_dext;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the tls server socket parameter

  SockParams TlsParams::tosprms (void) const {
    rdlock();
    try {
      // create a result socket params
      SockParams prms (d_host, d_port);
      // here it is
      unlock ();
      return prms;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the tls client socket parameter

  SockParams TlsParams::tocprms (void) const {
    rdlock();
    try {
      // create a result socket params
      SockParams prms (d_host, d_port);
      // here it is
      unlock ();
      return prms;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 15;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_SETVERS = zone.intern ("set-version");
  static const long QUARK_GETVMAJ = zone.intern ("get-major-version");
  static const long QUARK_GETVMIN = zone.intern ("get-minor-version");
  static const long QUARK_SETCERT = zone.intern ("set-certificate");
  static const long QUARK_GETCERT = zone.intern ("get-certificate");
  static const long QUARK_SETCKEY = zone.intern ("set-certificate-key");
  static const long QUARK_GETCKEY = zone.intern ("get-certificate-key");
  static const long QUARK_SETHOST = zone.intern ("set-host");
  static const long QUARK_GETHOST = zone.intern ("get-host");
  static const long QUARK_SETPORT = zone.intern ("set-port");
  static const long QUARK_GETPORT = zone.intern ("get-port");
  static const long QUARK_SETDEXT = zone.intern ("set-debug-extension");
  static const long QUARK_GETDEXT = zone.intern ("get-debug-extension");
  static const long QUARK_TOSPRMS = zone.intern ("to-server-sock-params");
  static const long QUARK_TOCPRMS = zone.intern ("to-client-sock-params");

  // create a new object in a generic way

  Object* TlsParams::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) return new TlsParams;
    // check for 2 arguments
    if (argc == 2) {
      String host = argv->getstring (0);
      t_word port = argv->getlong   (1);
      return new TlsParams (host, port);
    }
    // too many arguments
    throw Exception ("argument-error", 
		     "too many argument with tls parameters");
  }

  // return true if the given quark is defined

  bool TlsParams::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? TlsInfos::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* TlsParams::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETVMAJ) return new Byte    (getvmaj ());
      if (quark == QUARK_GETVMIN) return new Byte    (getvmin ());
      if (quark == QUARK_GETCERT) return new String  (getcert ());
      if (quark == QUARK_GETCKEY) return new String  (getckey ());
      if (quark == QUARK_GETHOST) return new String  (gethost ());
      if (quark == QUARK_GETPORT) return new Integer (getport ());
      if (quark == QUARK_GETDEXT) return new Boolean (getdext ());
      if (quark == QUARK_TOSPRMS) return new SockParams (tosprms ());
      if (quark == QUARK_TOCPRMS) return new SockParams (tocprms ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SETCERT) {
	String cert = argv->getstring (0);
	setcert (cert);
	return nullptr;
      }
      if (quark == QUARK_SETCKEY) {
	String ckey = argv->getstring (0);
	setckey (ckey);
	return nullptr;
      }
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
      if (quark == QUARK_SETDEXT) {
	bool dext = argv->getbool (0);
	setdext (dext);
	return nullptr;
      }
    }
    // check for 2 arguments
    if (argc == 2) {
      if (quark == QUARK_SETVERS) {
	t_byte vmaj = argv->getbyte (0);
	t_byte vmin = argv->getbyte (1);
	setvers (vmaj, vmin);
	return nullptr;
      }
      if (quark == QUARK_SETCERT) {
	String cert = argv->getstring (0);
	String ckey = argv->getstring (1);
	setcert (cert, ckey);
	return nullptr;
      }
    }
    // call the tls info method
    return TlsInfos::apply (zobj, nset, quark, argv);
  }
}
