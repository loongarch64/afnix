// ---------------------------------------------------------------------------
// - Socket.cpp                                                              -
// - afnix:net module - socket class implementation                          -
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

#include "Item.hpp"
#include "Regex.hpp"
#include "Vector.hpp"
#include "Socket.hpp"
#include "Integer.hpp"
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "cnet.hpp"
#include "csio.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // generate an authority by address and port
  static String to_authority (const Address* addr, const long port) {
    // initialize result
    String result = "0.0.0.0:0";
    if (addr == nullptr) return result;
    // check port value
    if (port < 0) {
      throw Exception ("socket-error", "invalid negative port number");
    }
    // get canonical address
    String cnam = addr->getcanon ();
    // check for IPV6
    Regex re = "<$x:>+";
    if (re == cnam) {
      result  = '[';
      result += cnam;
      result += ']';
    } else {
      result = cnam;
    }
    // add the port
    result += ':';
    result += port;
    // here it is
    return result;
  }
  
  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default socket

  Socket::Socket (void) {
    d_sid = -1;
  }

  // create a socket by id

  Socket::Socket (const int sid) {
    d_sid = sid;
  }

  // destroy this socket

  Socket::~Socket (void) {
    close ();
  }

  // return the class name

  String Socket::repr (void) const {
    return "Socket";
  }

  // close this socket 
  
  bool Socket::close (void) {
    wrlock ();
    try {
      if ((d_sid == -1) || (Object::uref (this) == false)) {
	unlock ();
	return true;
      }
      bool status = c_close (d_sid);
      d_sid = -1;
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the stream descriptor

  int Socket::getsid (void) const {
    rdlock ();
    try {
      int result = d_sid;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we have an ipv6 socket

  bool Socket::isipv6 (void) const {
    rdlock ();
    try {
      bool result = c_isipv6 (d_sid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if we can broadcast message

  bool Socket::isbcast (void) const {
    rdlock ();
    try {
      bool result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the socket is open

  bool Socket::isopen (void) const {
    rdlock ();
    try {
      bool status = (d_sid != -1);
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the socket address

  Address* Socket::getsockaddr (void) const {
    rdlock ();
    t_byte* addr = nullptr;
    try {
      // get the socket address
      addr = c_ipsockaddr (d_sid);
      // map an address object
      Address* result = (addr == nullptr) ? nullptr : new Address (addr);
      delete [] addr;
      unlock ();
      return result;
    } catch (...) {
      delete [] addr;
      unlock ();
      throw;
    }
  }

  // return the socket port

  t_word Socket::getsockport (void) const {
    rdlock ();
    try {
      t_word result = c_ipsockport (d_sid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the socket authority

  String Socket::getsockauth (void) const {
    rdlock ();
    Address* addr = nullptr;
    try {
      // get the socket address
      addr = getsockaddr ();
      // get the socket port
      long port = getsockport ();
      // map it to an authority
      String result = to_authority (addr, port);
      delete addr;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete addr;
      unlock ();
      throw;
    }
  }

  // return the peer address

  Address* Socket::getpeeraddr (void) const {
    rdlock ();
    t_byte* addr = nullptr;
    try {
      // get the peer address
      addr = c_ippeeraddr (d_sid);
      // map the socket address
      Address* result = (addr == nullptr) ? nullptr : new Address (addr);
      delete [] addr;
      unlock ();
      return result;
    } catch (...) {
      delete [] addr;
      unlock ();
      throw;
    }
  }

  // return the peer port

  t_word Socket::getpeerport (void) const {
    rdlock ();
    try {
      t_word result = c_ippeerport (d_sid);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the peer authority

  String Socket::getpeerauth (void) const {
    rdlock ();
    Address* addr = nullptr;
    try {
      // get the peer address
      addr = getpeeraddr ();
      // get the peer port
      long port = getpeerport ();
      // map it to an authority
      String result = to_authority (addr, port);
      delete addr;
      // unlock and return
      unlock ();
      return result;
    } catch (...) {
      delete addr;
      unlock ();
      throw;
    }
  }

  // detach the stream descriptor
  
  int Socket::detach (void) {
    wrlock ();
    try {
      int result = d_sid;
      d_sid = -1;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // force a socket shutdown
  
  bool Socket::shutdown (void) {
    wrlock ();
    try {
      if (d_sid == -1) {
	unlock ();
	return true;
      }
      bool status = c_close (d_sid);
      d_sid = -1;
      unlock ();
      return status;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // shutdown this socket

  bool Socket::shutdown (const bool mode) {
    wrlock ();
    try {
      bool result = false;
      if (mode == false) result = c_ipshut (d_sid, SHUT_RECV);
      if (mode == true)  result = c_ipshut (d_sid, SHUT_SEND);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a socket by parameters

  bool Socket::setopt (const SockParams& prms) {
    wrlock ();
    try {
      // preset result
      bool result = true;
      // check for reuse address
      bool radr = prms.getbopt (SO_RADR);
      if (radr == true) result = result && setopt (SO_RADR, radr);
      // check for reuse port
      bool rprt = prms.getbopt (SO_RPRT);
      if (rprt == true) result = result && setopt (SO_RPRT, rprt);
      // check for broadcast
      bool bcst = prms.getbopt (SO_BCST);
      if (bcst == true) result = result && setopt (SO_BCST, bcst);
      // check for bypass routing table
      bool dntr = prms.getbopt (SO_DNTR);
      if (dntr == true) result = result && setopt (SO_DNTR, dntr);
      // check for keepalve
      bool kliv = prms.getbopt (SO_KLIV);
      if (kliv == true) result = result && setopt (SO_KLIV, kliv);
      // check for linger time
      long ligt = prms.getlopt (SO_LIGT);
      if (ligt != 0) result = result && setopt (SO_LIGT, ligt);
      // check for receive buffer size
      long rsiz = prms.getlopt (SO_RSIZ);
      if (rsiz != 0) result = result && setopt (SO_RSIZ, rsiz);
      // check for send size
      long ssiz = prms.getlopt (SO_SSIZ);
      if (ssiz != 0) result = result && setopt (SO_SSIZ, ssiz);
      // check for sockethop limit
      long shop = prms.getlopt (SO_SHOP);
      if (shop != 0) result = result && setopt (SO_SHOP, shop);
      // check for multicast lookpback
      bool mlbk = prms.getbopt (SO_MLBK);
      if (mlbk == true) result = result && setopt (SO_MLBK, mlbk);
      // check for multicast hop limit
      long mhop = prms.getlopt (SO_MHOP);
      if (mhop != 0) result = result && setopt (SO_MHOP, mhop);
      // check for segment size
      long msiz = prms.getlopt (SO_MSIZ);
      if (msiz != 0) result = result && setopt (SO_MSIZ, msiz);
      // check for disable naggle algorithm
      long ndly = prms.getbopt (SO_NDLY);
      if (ndly == true) result = result && setopt (SO_NDLY, ndly);
      // here it is
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // set a socket option

  bool Socket::setopt (const t_so opt, const bool val) {
    wrlock ();
    try {
      bool result = false;
      switch (opt) {
      case SO_RADR:
	result = c_ipsetopt (d_sid, SOPT_RADR, val);
	break;
      case SO_RPRT:
	result = c_ipsetopt (d_sid, SOPT_RPRT, val);
	break;
      case SO_BCST:
	result = c_ipsetopt (d_sid, SOPT_BCST, val);
	break;
      case SO_DNTR:
	result = c_ipsetopt (d_sid, SOPT_DNTR, val);
	break;
      case SO_KLIV:
	result = c_ipsetopt (d_sid, SOPT_KLIV, val);
	break;
      case SO_LIGT:
	result = c_ipsetopt (d_sid, SOPT_LIGT, val);
	break;
      case SO_MLBK:
	result = c_ipsetopt (d_sid, SOPT_MLBK, val);
	break;
      case SO_NDLY:
	result = c_ipsetopt (d_sid, SOPT_NDLY, val);
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set a socket option with a value

  bool Socket::setopt (const t_so opt, const long val) {
    wrlock ();
    try {
      bool result = false;
      switch (opt) {
      case SO_LIGT:
	result = c_ipsetopt (d_sid, SOPT_LIGT, val);
	break;
      case SO_RSIZ:
	result = c_ipsetopt (d_sid, SOPT_RSIZ, val);
	break;
      case SO_SSIZ:
	result = c_ipsetopt (d_sid, SOPT_SSIZ, val);
	break;
      case SO_SHOP:
	result = c_ipsetopt (d_sid, SOPT_SHOP, val);
	break;
      case SO_MHOP:
	result = c_ipsetopt (d_sid, SOPT_MHOP, val);
	break;
      case SO_MSIZ:
	result = c_ipsetopt (d_sid, SOPT_MSIZ, val);
	break;
      default:
	break;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a boolean socket option

  bool Socket::getbopt (const t_so opt) const {
    rdlock ();
    try {
      bool result = false;
      bool status = false;
      switch (opt) {
      case SO_RADR:
	status = c_ipgetopt (d_sid, SOPT_RADR, result);
	break;
      case SO_RPRT:
	status = c_ipgetopt (d_sid, SOPT_RPRT, result);
	break;
      case SO_BCST:
	status = c_ipgetopt (d_sid, SOPT_BCST, result);
	break;
      case SO_DNTR:
	status = c_ipgetopt (d_sid, SOPT_DNTR, result);
	break;
      case SO_KLIV:
	result = c_ipgetopt (d_sid, SOPT_KLIV, result);
	break;
      case SO_LIGT:
	status = c_ipgetopt (d_sid, SOPT_LIGT, result);
	break;
      case SO_MLBK:
	status = c_ipgetopt (d_sid, SOPT_MLBK, result);
	break;
      case SO_NDLY:
	status = c_ipgetopt (d_sid, SOPT_NDLY, result);
	break;
      default:
	status = false;
	break;
      }
      if (status == false) {
	throw Exception ("socket-error", "cannot get boolean option");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a socket option  value

  long Socket::getlopt (const t_so opt) const {
    wrlock ();
    try {
      long result = 0L;
      bool status = false;
      switch (opt) {
      case SO_LIGT:
	status = c_ipgetopt (d_sid, SOPT_LIGT, result);
	break;
      case SO_RSIZ:
	status = c_ipgetopt (d_sid, SOPT_RSIZ, result);
	break;
      case SO_SSIZ:
	status = c_ipgetopt (d_sid, SOPT_SSIZ, result);
	break;
      case SO_SHOP:
	status = c_ipgetopt (d_sid, SOPT_SHOP, result);
	break;
      case SO_MHOP:
	status = c_ipgetopt (d_sid, SOPT_MHOP, result);
	break;
      case SO_MSIZ:
	status = c_ipgetopt (d_sid, SOPT_MSIZ, result);
	break;
      default:
	status = false;
	break;
      }
      if (status == false) {
	throw Exception ("socket-error", "cannot get option value");
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // connect this socket by port and address

  bool Socket::connect (t_word port, const Address& addr) {
    rdlock ();
    try {
      bool result = c_ipconnect (d_sid, port, addr.p_addr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // connect this socket by port and aliases address

  bool Socket::connect (t_word port, const Address& addr, const bool alsf) {
    wrlock ();
    addr.rdlock ();
    try {
      // start with canonical address
      bool status = c_ipconnect (d_sid, port, addr.p_addr);
      if ((status == true) || (alsf == false)) {
	addr.unlock ();
	unlock ();
	return status;
      }
      // use aliases address mode
      for (long i = 0; i < addr.d_size; i++) {
	status = c_ipconnect (d_sid, port, addr.p_aals[i].p_aadr);
	if (status == true) {
	  addr.unlock ();
	  unlock ();
	  return true;
	}
      }
      // connection failure
      addr.unlock ();
      unlock ();
      return false;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // bind a socket with a port

  bool Socket::bind (t_word port) {
    rdlock ();
    try {
      bool result = c_ipbind (d_sid, port);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // bind a socket with a port and an address

  bool Socket::bind (t_word port, const Address& addr) {
    rdlock ();
    addr.rdlock ();
    try {
      bool result = c_ipbind (d_sid, port, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - object section                                                        -
  // -------------------------------------------------------------------------

  // the quark zone
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);
  
  // the object supported quarks
  static const long QUARK_BIND      = zone.intern ("bind");
  static const long QUARK_CONNECT   = zone.intern ("connect");
  static const long QUARK_SHUTDOWN  = zone.intern ("shutdown");

  // return true if the given quark is defined

  bool Socket::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? SocketStream::isquark (quark, hflg) : false;
      if (result == false) {
	result = hflg ? Sockopt::isquark (quark, hflg) : false;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // apply this object with a set of arguments and a quark

  Object* Socket::apply (Evaluable* zobj, Nameset* nset, const long quark,
			 Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_SHUTDOWN) return new Boolean (shutdown    ());
    }

    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_BIND) {
	long port = argv->getlong (0);
	bind (port);
	return nullptr;
      }
      if (quark == QUARK_SHUTDOWN) {
	bool mode = argv->getbool (0);
	return new Boolean (shutdown (mode));
      }
    }
    // dispatch 2 arguments
    if (argc == 2) {
      if (quark == QUARK_BIND) {
	long port = argv->getlong (0);
	Address* addr = dynamic_cast <Address*> (argv->get (1));
	if (addr == nullptr) 
	  throw Exception ("argument-error", "address expected with bind");
	bind (port, *addr);
	return nullptr;
      }
      if (quark == QUARK_CONNECT) {
	long port = argv->getlong (0);
	Address* addr = dynamic_cast <Address*> (argv->get (1));
	if (addr == nullptr) 
	  throw Exception ("argument-error", "address expected with connect");
	connect (port, *addr);
	return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_CONNECT) {
	long port = argv->getlong (0);
	Address* addr = dynamic_cast <Address*> (argv->get (1));
	if (addr == nullptr) 
	  throw Exception ("argument-error", "address expected with connect");
	bool alsf = argv->getbool (2);
	connect (port, *addr, alsf);
	return nullptr;
      }
    }
    // check for socket stream quark
    if (SocketStream::isquark (quark, true) == true)
      return SocketStream::apply (zobj, nset, quark, argv);
    // call the socket option method
    return Sockopt::apply (zobj, nset, quark, argv);
  }
}
