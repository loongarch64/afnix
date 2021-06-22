// ---------------------------------------------------------------------------
// - UdpSocket.cpp                                                           -
// - afnix:net module - udp socket socket implementation                     -
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

#include "Ascii.hpp"
#include "Error.hpp"
#include "Vector.hpp"
#include "UdpSocket.hpp"
#include "QuarkZone.hpp"
#include "cnet.hpp"
#include "csio.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the maximum datagram size
  static const long UDP_BUFFER_SIZE = 65508;

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default udp socket

  UdpSocket::UdpSocket (void) {
    d_sid  = c_ipsockudp (IAPF_XANY);
    if (d_sid < 0) {
      long code = d_sid; d_sid = -1;
      throw Error ("udp-error", c_errmsg (code), code);
    }
    p_buf  = new char[UDP_BUFFER_SIZE];
    d_port = 0;
  }

  // create an udp socket by socket id

  UdpSocket::UdpSocket (const int sid) {
    d_sid = sid;
    if (d_sid < 0) {
      d_sid = -1;
      throw Exception ("udp-error", "invalid upd socket");
    }
    p_buf  = new char[UDP_BUFFER_SIZE];
    d_port = 0;
  }

  // create an udp socket by flag

  UdpSocket::UdpSocket (const bool cflg) {
    if (cflg == true) {
      d_sid  = c_ipsockudp (IAPF_XANY);
      if (d_sid < 0) {
	long code = d_sid; d_sid = -1;
	throw Error ("udp-error", c_errmsg (code), code);
      }
    }
    p_buf  = new char[UDP_BUFFER_SIZE];
    d_port = 0;
  }

  // destroy this udp socket

  UdpSocket::~UdpSocket (void) {
    delete [] p_buf;
  }

  // return the class name

  String UdpSocket::repr (void) const {
    return "UdpSocket";
  }

  // return true if we can broadcast message

  bool UdpSocket::isbcast (void) const {
    rdlock ();
    try {
      bool result = true;
      if (c_isipv6 (d_sid) == true) result = false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // join a multicast group by address

  bool UdpSocket::join (const Address& addr) {
    wrlock ();
    addr.rdlock ();
    try {
      bool result = c_ipjoin (d_sid, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();
      throw;
    }
  }

  // drop from multicast group by address

  bool UdpSocket::drop (const Address& addr) {
    wrlock ();
    addr.rdlock ();
    try {
      bool result = c_ipdrop (d_sid, addr.p_addr);
      addr.unlock ();
      unlock ();
      return result;
    } catch (...) {
      addr.unlock ();
      unlock ();      
      throw;
    }
  }

  // return true if the udp client buffer is empty

  bool UdpSocket::iseos (void) const {
    rdlock ();
    try {
      bool result = (d_sbuf.length () == 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if we can read a character

  bool UdpSocket::valid (void) const {
    rdlock ();
    try {
      // first check in the buffer
      if (d_sbuf.length () != 0) {
	unlock ();
	return true;
      }
      // check if we can read one character
      bool result = c_rdwait (d_sid, d_tout);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read one character from a udp message

  char UdpSocket::read (void) {
    wrlock ();
    try {
      // check the pushback buffer first
      if (d_sbuf.length () != 0) {
	unlock ();
	return d_sbuf.read ();
      }
      // read the datagram
      long count  = 0;
      if (d_addr.p_addr == nullptr)
	count = c_iprecv (d_sid, p_buf, UDP_BUFFER_SIZE);
      else
	count = c_iprecvfr (d_sid, d_port, d_addr.p_addr, p_buf, 
			    UDP_BUFFER_SIZE);
      if (count < 0) {
	throw Error ("read-error", c_errmsg (count), count);
      }
      if (count == 0) {
	throw Exception ("read-error", "cannot read udp datagram");
      }
      // update the buffer with the data
      d_sbuf.add (p_buf, count);
      char result = d_sbuf.read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read a buffer by size from a udp message

  Buffer* UdpSocket::read (const long size) {
    wrlock ();
    try {
      Buffer* result = new Buffer;
      // check the pushback buffer first
      long blen = d_sbuf.length ();
      long rlen = size;
      if (blen > 0) {
	if (blen < size) {
	  for (long i = 0; i < blen; i++) {
	    result->add (d_sbuf.read ());
	    rlen--;
	  }
	}
	if (blen > size) {
	  for (long i = 0; i < size; i++) result->add (d_sbuf.read ());
	  unlock ();
	  return result;
	}
      }
      // now fill the result buffer by reading
      if (rlen <= 0) {
	unlock ();
	return result;
      }
      long count = 0;
      if (d_addr.p_addr == nullptr)
	count = c_iprecv (d_sid, p_buf, UDP_BUFFER_SIZE);
      else
	count = c_iprecvfr (d_sid, d_port, d_addr.p_addr, p_buf, 
			    UDP_BUFFER_SIZE);
      if (count < 0) {
	throw Error ("read-error", c_errmsg (count), count);
      }
      // place the remaining in the result buffer
      rlen = (rlen <= count) ? rlen : count;
      for (long i = 0; i < rlen; i++) result->add (p_buf[i]);
      // place the rest of the datagram in the socket buffer
      for (long i = rlen; i < count; i++) d_sbuf.add (p_buf[i]);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send one character in a udp message
  
  long UdpSocket::write (const char value) {
    wrlock ();
    try {
      long result = 0;
      if (d_addr.p_addr == nullptr) {
	result = c_ipsend (d_sid, &value, 1);
      } else {
	result = c_ipsendto (d_sid, d_port, d_addr.p_addr, &value, 1);
      }
      if (result < 0) throw Error ("write-error", c_errmsg (result), result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send a buffer in a udp message
  
  long UdpSocket::write (const char* value) {
    wrlock ();
    try {
      long size   = Ascii::strlen (value);
      long result = 0L;
      if (d_addr.p_addr == nullptr) {
	result = c_ipsend (d_sid, value, size);
      } else {
	result = c_ipsendto (d_sid, d_port, d_addr.p_addr, value, size);
      }
      if (result < 0) throw Error ("write-error", c_errmsg (result), result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // send a buffer by size in a udp message
  
  long UdpSocket::write (const char* rbuf, const long size) {
    wrlock ();
    try {
      long result = 0L;
      if (d_addr.p_addr == nullptr) {
	result = c_ipsend (d_sid, rbuf, size);
      } else {
	result = c_ipsendto (d_sid, d_port, d_addr.p_addr, rbuf, size);
      }
      if (result < 0) throw Error ("write-error", c_errmsg (result), result);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the input stream channel
  
  InputStream* UdpSocket::getis (void) {
    wrlock ();
    try {
      InputStream* result = new UdpSocket (c_dup (d_sid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the output stream channel
  
  OutputStream* UdpSocket::getos (void) {
    wrlock ();
    try {
      OutputStream* result = new UdpSocket (c_dup (d_sid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the socket protocol

  String UdpSocket::getprotocol (void) const {
    return "udp";
  }

  // create an udp socket by address family

  void UdpSocket::create (const Address& addr) {
    wrlock ();
    try {
      if (d_sid != -1) {
	throw Exception ("udp-error", "udp socket already created");
      }
      d_sid = c_ipsockudp (addr.p_addr);
      if (d_sid < 0) {
	long code = d_sid; d_sid = -1;
	throw Error ("udp-error", c_errmsg (code), code);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // accept a new datagram from this udp socket

  Datagram* UdpSocket::accept (void) const {
    rdlock ();
    t_byte* daddr = nullptr;
    try {
      // get the socket address length
      long alen = isipv6 () ? 16 : 4;
      // create a new address 
      daddr = new t_byte[alen+1];
      daddr[0]      = alen;
      t_word  dport = 0;
      // get the receiving buffer
      long code = c_iprecvfr (d_sid, dport, daddr, p_buf, UDP_BUFFER_SIZE);
      if (code < 0) throw Error ("accept-error", c_errmsg (code), code);
      // create a new datagram
      Datagram* dg = new Datagram (d_sid, dport, daddr, p_buf, code);
      delete [] daddr;
      unlock ();
      return dg;
    } catch (...) {
      delete [] daddr;
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
  static const long QUARK_ACCEPT = zone.intern ("accept");

  // create a new object in a generic way

  Object* UdpSocket::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new UdpSocket;
    // check for 1 argument
    if (argc == 1) {
      bool cflg = argv->getbool (0);
      return new UdpSocket (cflg);
    }
    throw Exception ("argument-error", 
		     "too many arguments with udp socket constructor");
  }

  // return true if the given quark is defined

  bool UdpSocket::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Socket::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* UdpSocket::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_ACCEPT) return accept ();
    }
    // call the socket method
    return Socket::apply (zobj, nset, quark, argv);
  }
}
