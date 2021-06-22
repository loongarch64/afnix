// ---------------------------------------------------------------------------
// - TcpSocket.cpp                                                           -
// - afnix:net module - tcp socket class implementation                      -
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
#include "Boolean.hpp"
#include "TcpSocket.hpp"
#include "QuarkZone.hpp"
#include "csio.hpp"
#include "cnet.hpp"
#include "cerr.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default tcp socket

  TcpSocket::TcpSocket (void) {
    create ();
  }

  // create a socket by id 

  TcpSocket::TcpSocket (const int sid) {
    d_sid = sid;
    if (d_sid < 0) throw Exception ("tcp-error", "invalid tcp socket");
  }

  // create a tcp socket by flag

  TcpSocket::TcpSocket (const bool cflg) {
    if (cflg == true) create ();
  }

  // return the class name

  String TcpSocket::repr (void) const {
    return "TcpSocket";
  }

  // return true if the eos flag is set

  bool TcpSocket::iseos (void) const {
    wrlock ();
    try {
      if (d_sbuf.length () != 0) {
	unlock ();
	return false;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, 0);
      if (status == false) {
	unlock ();
	return false;
      }
      // read in the character - might be the eos
      char c = nilc;
      if (c_read (d_sid, &c, 1) <= 0) {
	unlock ();
	return true;
      }
      d_sbuf.pushback (c);
      unlock ();
      return false;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // check if we can read one character

  bool TcpSocket::valid (void) const {
    wrlock ();
    try {
      if (d_sbuf.length () != 0) {
	unlock ();
	return true;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, d_tout);
      if (status == false) {
	unlock ();
	return false;
      }
      // read in the character - might be the eos
      char c = nilc;
      if (c_read (d_sid, &c, 1) <= 0) {
	unlock ();
	return false;
      }
      d_sbuf.pushback (c);
      unlock ();
      return true;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // read one character from the socket

  char TcpSocket::read (void) {
    wrlock ();
    try {
      // check if we can read a character
      if (valid () == false) {
	unlock ();
	return eosc;
      }
      // check the pushback buffer first
      if (d_sbuf.empty () == false) {
	char result = d_sbuf.read ();
	unlock ();
	return result;
      }
      // read the next character in case valid
      // does not push a character (safety code only)
      char c = nilc;
      long code = c_read (d_sid, &c, 1);
      if (code < 0L) throw Error ("read-error", c_errmsg (code), code);
      // check for eos
      if (code == 0) c = eosc;
      unlock ();
      return c;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // copy the tcp socket into a buffer

  long TcpSocket::copy (char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and fill
    wrlock ();
    try {
      // check the pushback buffer first
      long result = d_sbuf.copy (rbuf, size);
      if (result == size) {
	unlock ();
	return result;
      }
      // check if we can read one character
      bool status = c_rdwait (d_sid, d_tout);
      if (status == false) {
	unlock ();
	return result;
      }
      // read by block
      long code = c_read (d_sid, &rbuf[result], size-result);
      if (code < 0L) throw Error ("read-error", c_errmsg (code), code);
      // update result and unlock
      result+= code;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write one character to the socket
  
  long TcpSocket::write (const char value) {
    wrlock ();
    try {
      long code = c_write (d_sid, &value, 1);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a data buffer to the socket

  long TcpSocket::write (const char* data) {
    // lock and write
    wrlock ();
    try {
      // check for size first
      long size = Ascii::strlen (data);
      if (size == 0) {
	unlock ();
	return 0;
      }
      long code = c_write (d_sid, data, size);
      if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
      unlock ();
      return code;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a character array to the socket

  long TcpSocket::write (const char* rbuf, const long size) {
    // check argument first
    if ((rbuf == nullptr) || (size <= 0)) return 0;
    // lock and write
    wrlock ();
    try {
      long result = 0;
      while (result != size) {
	long code = c_write (d_sid, &rbuf[result], size-result);
	if (code < 0L) throw Error ("write-error", c_errmsg (code), code);
	if (code == 0) break;
	result += code;
      }
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the input stream channel
  
  InputStream* TcpSocket::getis (void) {
    wrlock ();
    try {
      InputStream* result = new TcpSocket (c_dup (d_sid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the output stream channel
  
  OutputStream* TcpSocket::getos (void) {
    wrlock ();
    try {
      OutputStream* result = new TcpSocket (c_dup (d_sid));
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // get the socket protocol

  String TcpSocket::getprotocol (void) const {
    return "tcp";
  }

  // create a default socket

  void TcpSocket::create (void) {
    wrlock();
    try {
      if (d_sid != -1) {
	throw Exception ("tcp-error", "existing socket with create");
      }
      d_sid = c_ipsocktcp (IAPF_XANY);
      if (d_sid < 0) {
	long code = d_sid; d_sid = -1;
	throw Error ("tcp-error", c_errmsg (code), code);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // create a tcp socket by address family

  void TcpSocket::create (const Address& addr) {
    wrlock ();
    try {
      if (d_sid != -1) {
	throw Exception ("tcp-error", "tcp socket already created");
      }
      d_sid = c_ipsocktcp (addr.p_addr);
      if (d_sid < 0) {
	long code = d_sid; d_sid = -1;
	throw Error ("tcp-error", c_errmsg (code), code);
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // set the socket keepalive parameters

  bool TcpSocket::setkalv (const long kidl,const long kcnt,const long kitv) {
    wrlock ();
    try {
      bool result = c_setkalv (d_sid, (int) kidl, (int) kcnt, (int) kitv);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // reset the socket keepalive parameters

  void TcpSocket::rstkalv (void) {
    wrlock ();
    try {
      c_rstkalv (d_sid);
      unlock ();
    } catch (...) {
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
  static const long QUARK_CREATE  = zone.intern ("create");
  static const long QUARK_SETKALV = zone.intern ("set-keepalive");
  static const long QUARK_RSTKALV = zone.intern ("reset-keepalive");

  // create a new object in a generic way

  Object* TcpSocket::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new TcpSocket;
    // check for 1 argument
    if (argc == 1) {
      bool cflg = argv->getbool (0);
      return new TcpSocket (cflg);
    }
    throw Exception ("argument-error", 
		     "too many arguments with tcp socket constructor");
  }

  // return true if the given quark is defined

  bool TcpSocket::isquark (const long quark, const bool hflg) const {
    rdlock ();
    try {
      if (zone.exists (quark) == true) {
	unlock ();
	return true;
      }
      bool result = hflg ? Socket::isquark (quark, hflg) : false;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }
  
  // apply this object with a set of arguments and a quark

  Object* TcpSocket::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_CREATE) {
	create ();
	return nullptr;
      }
      if (quark == QUARK_RSTKALV) {
	rstkalv ();
	return nullptr;
      }
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_CREATE) {
	Object* obj = argv->get (0);
	auto addr = dynamic_cast <Address*> (obj);
        if (addr == nullptr) { 
          throw Exception ("argument-error", "invalid object with create",
			   Object::repr (obj));
	}
        create (*addr);
        return nullptr;
      }
    }
    // dispatch 3 arguments
    if (argc == 3) {
      if (quark == QUARK_SETKALV) {
	long kidl = argv->getlong (0);
	long kcnt = argv->getlong (1);
	long kitv = argv->getlong (2);
	return new Boolean (setkalv (kidl, kcnt, kitv));
      }
    }
    // call the socket method
    return Socket::apply (zobj, nset, quark, argv);
  }
}
