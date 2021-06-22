// ---------------------------------------------------------------------------
// - Datagram.cpp                                                            -
// - afnix:net module - datagram class implementation                        -
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
#include "Integer.hpp"
#include "Datagram.hpp"
#include "QuarkZone.hpp"
#include "cnet.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default datagram
  
  Datagram::Datagram (const int sid, t_word dport, const t_byte* daddr,
		      const char* buf, const long size) {
    d_sid   = sid;
    p_daddr = c_cpaddr (daddr);
    d_dport = dport;
    d_sbuf.add (buf, size);
  }

  // destroy this datagram
  
  Datagram::~Datagram (void) {
    close ();
    delete [] p_daddr;
  }

  // return the class name

  String Datagram::repr (void) const {
    return "Datagram";
  }

  // close this datagram

  bool Datagram::close (void) {
    wrlock ();
    try {
      if (Object::uref (this) == true) d_sid = -1;
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }	
  }

  // read a character from the datagram

  char Datagram::read (void) {
    wrlock ();
    try {
      char result = d_sbuf.read ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write one character to the datagram sender
  
  long Datagram::write (const char value) {
    wrlock ();
    try {
      long code = c_ipsendto (d_sid, d_dport, p_daddr, &value, 1);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a buffer to the datagram sender
  
  long Datagram::write (const char* data) {
    wrlock ();
    try {
      // get data size
      long size = Ascii::strlen (data);
      // send to at once
      long code = c_ipsendto (d_sid, d_dport, p_daddr, data, size);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character array to the datagram sender
  
  long Datagram::write (const char* rbuf, const long size) {
    wrlock ();
    try {
      long code = c_ipsendto (d_sid, d_dport, p_daddr, rbuf, size);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return true if the datagram buffer is empty

  bool Datagram::iseos (void) const {
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

  // return true if the datagram buffer is not empty

  bool Datagram::valid (void) const {
    rdlock ();
    try {
      bool result = (d_sbuf.length () != 0);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the peer address

  Address* Datagram::getpeeraddr (void) const {
    rdlock ();
    try {
      Address* result = new Address (p_daddr);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // return the peer port

  t_word Datagram::getpeerport (void) const {
    rdlock ();
    try {
      t_word result = d_dport;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the datagram protocol

  String Datagram::getprotocol (void) const {
    return "udp";
  }
}
