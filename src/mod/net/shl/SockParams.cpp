// ---------------------------------------------------------------------------
// - Sockopt.cpp                                                          -
// - afnix:net module - socket options class implementation                  -
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
#include "Integer.hpp"
#include "Boolean.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SockParams.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the default host
  static const String SOK_HOST_DEF = Address::loopname();
  // the default port
  static const long   SOK_PORT_DEF = 0;
  // the default address reuse
  static const bool   SOK_RADR_DEF = true;
  // the default port reuse
  static const bool   SOK_RPRT_DEF = false;
  // the default broadcast parameter
  static const bool   SOK_BCST_DEF = false;
  // the default bypass routing table
  static const bool   SOK_DNTR_DEF = false;
  // the default keepalive
  static const bool   SOK_KLIV_DEF = false;
  // the default connection linger time
  static const long   SOK_LIGT_DEF = 0;
  // the default receive buffer size
  static const long   SOK_RSIZ_DEF = 0;
  // the default send buffer size
  static const long   SOK_SSIZ_DEF = 0;
  // the default socket hop limit
  static const long   SOK_SHOP_DEF = 0;
  // the default multicast use loopback
  static const bool   SOK_MLBK_DEF = false;
  // the default multicast hop limit
  static const long   SOK_MHOP_DEF = 0;
  // the deault maximum segment size
  static const long   SOK_MSIZ_DEF = 0;
  // the default disable naggle algorithm flag
  static const bool   SOK_NDLY_DEF = false;
  // the default socket backlog
  static const long   SOK_BLOG_DEF = 5;

  // -------------------------------------------------------------------------
  // - class section                                                        -
  // -------------------------------------------------------------------------

  // create a default socket parameters

  SockParams::SockParams (void) {
    d_addr = Address (SOK_HOST_DEF);
    d_port = SOK_PORT_DEF;
    d_radr = SOK_RADR_DEF;
    d_rprt = SOK_RPRT_DEF;
    d_bcst = SOK_BCST_DEF;
    d_dntr = SOK_DNTR_DEF;
    d_kliv = SOK_KLIV_DEF;
    d_ligt = SOK_LIGT_DEF;
    d_rsiz = SOK_RSIZ_DEF;
    d_ssiz = SOK_SSIZ_DEF;
    d_shop = SOK_SHOP_DEF;
    d_mlbk = SOK_MLBK_DEF;
    d_mhop = SOK_MHOP_DEF;
    d_msiz = SOK_MSIZ_DEF; 
    d_ndly = SOK_NDLY_DEF;
    d_blog = SOK_BLOG_DEF;
  }

  // create a socket parameters by host and port

  SockParams::SockParams (const String& host, const t_word port) {
    d_addr = Address (host);
    d_port = port;
    d_radr = SOK_RADR_DEF;
    d_rprt = SOK_RPRT_DEF;
    d_bcst = SOK_BCST_DEF;
    d_dntr = SOK_DNTR_DEF;
    d_kliv = SOK_KLIV_DEF;
    d_ligt = SOK_LIGT_DEF;
    d_rsiz = SOK_RSIZ_DEF;
    d_ssiz = SOK_SSIZ_DEF;
    d_shop = SOK_SHOP_DEF;
    d_mlbk = SOK_MLBK_DEF;
    d_mhop = SOK_MHOP_DEF;
    d_msiz = SOK_MSIZ_DEF; 
    d_ndly = SOK_NDLY_DEF;
    d_blog = SOK_BLOG_DEF;
  }

    // create a socket parameters by host and port

  SockParams::SockParams (const String& host, const t_word port,
			  const String& sapf) {
    d_addr = Address (host, false, sapf);
    d_port = port;
    d_radr = SOK_RADR_DEF;
    d_rprt = SOK_RPRT_DEF;
    d_bcst = SOK_BCST_DEF;
    d_dntr = SOK_DNTR_DEF;
    d_kliv = SOK_KLIV_DEF;
    d_ligt = SOK_LIGT_DEF;
    d_rsiz = SOK_RSIZ_DEF;
    d_ssiz = SOK_SSIZ_DEF;
    d_shop = SOK_SHOP_DEF;
    d_mlbk = SOK_MLBK_DEF;
    d_mhop = SOK_MHOP_DEF;
    d_msiz = SOK_MSIZ_DEF; 
    d_ndly = SOK_NDLY_DEF;
    d_blog = SOK_BLOG_DEF;
  }

  // create a socket parameters by address and port

  SockParams::SockParams (const Address& addr, const t_word port) {
    d_addr = addr;
    d_port = port;
    d_radr = SOK_RADR_DEF;
    d_rprt = SOK_RPRT_DEF;
    d_bcst = SOK_BCST_DEF;
    d_dntr = SOK_DNTR_DEF;
    d_kliv = SOK_KLIV_DEF;
    d_ligt = SOK_LIGT_DEF;
    d_rsiz = SOK_RSIZ_DEF;
    d_ssiz = SOK_SSIZ_DEF;
    d_shop = SOK_SHOP_DEF;
    d_mlbk = SOK_MLBK_DEF;
    d_mhop = SOK_MHOP_DEF;
    d_msiz = SOK_MSIZ_DEF; 
    d_ndly = SOK_NDLY_DEF;
    d_blog = SOK_BLOG_DEF;
  }
  
  // copy construct this address

  SockParams::SockParams (const SockParams& that) {
    that.rdlock ();
    try {
      d_addr = that.d_addr;
      d_port = that.d_port;
      d_radr = that.d_radr;
      d_rprt = that.d_rprt;
      d_bcst = that.d_bcst;
      d_dntr = that.d_dntr;
      d_kliv = that.d_kliv;
      d_ligt = that.d_ligt;
      d_rsiz = that.d_rsiz;
      d_ssiz = that.d_ssiz;
      d_shop = that.d_shop;
      d_mlbk = that.d_mlbk;
      d_mhop = that.d_mhop;
      d_msiz = that.d_msiz;
      d_ndly = that.d_ndly;
      d_blog = that.d_blog;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // return the class name

  String SockParams::repr (void) const {
    return "SockParams";
  }
  
  // return a clone of this object

  Object* SockParams::clone (void) const {
    return new SockParams (*this);
  }
  
  // assign an obect to this one

  SockParams& SockParams::operator = (const SockParams& that) {
    // check for self assignation
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      d_addr = that.d_addr;
      d_port = that.d_port;
      d_radr = that.d_radr;
      d_rprt = that.d_rprt;
      d_bcst = that.d_bcst;
      d_dntr = that.d_dntr;
      d_kliv = that.d_kliv;
      d_ligt = that.d_ligt;
      d_rsiz = that.d_rsiz;
      d_ssiz = that.d_ssiz;
      d_shop = that.d_shop;
      d_mlbk = that.d_mlbk;
      d_mhop = that.d_mhop;
      d_msiz = that.d_msiz;
      d_ndly = that.d_ndly;
      d_blog = that.d_blog;
      unlock();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock();
      that.unlock ();
      throw;
    }
  }

  // set a boolean socket option

  bool SockParams::setopt (const t_so opt, const bool val) {
    wrlock ();
    try {
      bool status = true;
      switch (opt) {
      case SO_RADR:
	d_radr = val;
	break;
      case SO_RPRT:
	d_rprt = val;
	break;
      case SO_BCST:
	d_bcst = val;
	break;
      case SO_DNTR:
	d_dntr = val;
	break;
      case SO_KLIV:
	d_kliv = val;
	break;
      case SO_MLBK:
	d_mlbk = val;
	break;
      case SO_NDLY:
	d_bcst = val;
	break;
      default:
	status = false;
	break;
      } 
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a socket option with a value

  bool SockParams::setopt (const t_so opt, const long val) {
    wrlock ();
    try {
      bool status = true;
      switch (opt) {
      case SO_LIGT:
	d_ligt = val;
	break;
      case SO_RSIZ:
	d_rsiz = val;
	break;
      case SO_SSIZ:
	d_ssiz = val;
	break;
      case SO_SHOP:
	d_shop = val;
	break;
      case SO_MHOP:
	d_mhop = val;
	break;
      case SO_MSIZ:
	d_msiz = val;
	break;
      default:
	status = false;
	break;
      } 
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a boolean socket option

  bool SockParams::getbopt (const t_so opt) const {
    rdlock ();
    try {
      bool result = false;
      switch (opt) {
      case SO_RADR:
	result = d_radr;
	break;
      case SO_RPRT:
	result = d_rprt;
	break;
      case SO_BCST:
	result = d_bcst;
	break;
      case SO_DNTR:
	result = d_dntr;
	break;
      case SO_KLIV:
	result = d_kliv;
	break;
      case SO_MLBK:
	result = d_mlbk;
	break;
      case SO_NDLY:
	result = d_bcst;
	break;
      default:
	throw Exception ("socket-error", "invalid option for getbopt");
	break;
      } 
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get a socket option value

  long SockParams::getlopt (const t_so opt) const {
    rdlock ();
    try {
      long result = 0L;
      switch (opt) {
      case SO_LIGT:
	result = d_ligt;
	break;
      case SO_RSIZ:
	result = d_rsiz;
	break;
      case SO_SSIZ:
	result = d_ssiz;
	break;
      case SO_SHOP:
	result = d_shop;
	break;
      case SO_MHOP:
	result = d_mhop;
	break;
      case SO_MSIZ:
	result = d_msiz;
	break;
      default:
	throw Exception ("socket-error", "invalid option for getlopt");
	break;
      } 
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the address parameter

  void SockParams::setaddr (const Address& addr) {
    wrlock ();
    try {
      d_addr = addr;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the host parameter

  void SockParams::sethost (const String& host) {
    wrlock ();
    try {
      d_addr = Address (host);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the address parameter

  Address SockParams::getaddr (void) const {
    rdlock ();
    try {
      Address result = d_addr;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set the port parameter

  void SockParams::setport (const t_word port) {
    wrlock ();
    try {
      d_port = port;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the port parameter

  t_word SockParams::getport (void) const {
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
  
  // set the backlog parameter

  void SockParams::setblog (const long blog) {
    wrlock ();
    try {
      d_blog = blog;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the backlog parameter

  long SockParams::getblog (void) const {
    rdlock ();
    try {
      long result = d_blog;
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
  static const long QUARK_GETADDR = zone.intern ("get-address");
  static const long QUARK_SETADDR = zone.intern ("set-address");
  static const long QUARK_SETHOST = zone.intern ("set-host");
  static const long QUARK_GETPORT = zone.intern ("get-port");
  static const long QUARK_SETPORT = zone.intern ("set-port");

  // create a new object in a generic way

  Object* SockParams::mknew (Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // check for 0 argument
    if (argc == 0) return new SockParams;
    // check for 2 arguments
    if (argc == 2) {
      Object* obj = argv->get(0);
      t_word port = argv->getlong (1);
      // check for a string
      String* host = dynamic_cast <String*> (obj);
      if (host != nullptr) return new SockParams (*host, port);
      // check for an address
      Address* addr = dynamic_cast <Address*> (obj);
      if (addr != nullptr) return new SockParams (*addr, port);
      // invalid arguments
      throw Exception ("type-error",
		       "invalid arguments in socket parameters constructor",
		       Object::repr (obj));
    }
    // check for 3 arguments
    if (argc == 3) {
      String host = argv->getstring(0);
      t_word port = argv->getlong (1);
      String sapf = argv->getstring (2);
      return new SockParams (host, port, sapf);
    }
    // too many arguments
    throw Exception ("argument-error", 
		     "too many argument with socket parameters");
  }
  
  // return true if the given quark is defined

  bool SockParams::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Sockopt::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* SockParams::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETADDR) return new Address (getaddr ());
      if (quark == QUARK_GETPORT) return new Integer (getport ());
    }
    // dispatch 1 arguments
    if (argc == 1) {
      if (quark == QUARK_SETADDR) {
	Object*   obj = argv->get (0);
	Address* addr = dynamic_cast<Address*> (obj);
	if (addr == nullptr) {
	  throw Exception ("type-error", "invalid object as address",
			   Object::repr (obj));
	}
	setaddr (*addr);
	return nullptr;
      }
      if (quark == QUARK_SETHOST) {
	String host = argv->getstring (0);
	sethost (host);
	return nullptr;
      }
      if (quark == QUARK_SETPORT) {
	t_word port = argv->getlong (0);
	setport (port);
	return nullptr;
      }
    }
    // call the socket option method
    return Sockopt::apply (zobj, nset, quark, argv);
  }
}
