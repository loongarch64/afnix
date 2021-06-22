// ---------------------------------------------------------------------------
// - MultiPart.cpp                                                           -
// - afnix:nwg module - multipart content class implementation               -
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
#include "Evaluable.hpp"
#include "MultiPart.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "SinglePart.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a create a default multipart

  MultiPart::MultiPart (const String& bnds) {
    d_bnds = bnds;
  }
  
  // create a multipart by boundary and buffer
  
  MultiPart::MultiPart (const String& bnds, Buffer& mbuf) {
    d_bnds = bnds;
    parse (mbuf);
  }
  
  // return the class name
  
  String MultiPart::repr (void) const {
    return "MultiPart";
  }
  
  // reset this multipart

  void MultiPart::reset (void) {
    wrlock ();
    try {
      d_mprt.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the boundary string

  String MultiPart::getbnds (void) const {
    rdlock ();
    try {
      String result = d_bnds;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // parse a buffer and generate a vector of single parts

  void MultiPart::parse (Buffer& mbuf) {
    wrlock ();
    try {
      // reset locally
      reset ();
      // create the boundary line
      String cbnd = String ("--") + d_bnds;
      String ebnd = cbnd + "--";
      // loop until empty
      while (mbuf.empty () == false) {
	// get the boundary line
	String line = mbuf.readln ();
	// check for end of buffer
	if (line == ebnd) {
	  if (mbuf.empty () == true) break;
	  throw Exception ("multipart-error", 
			   "non empty buffer at end of multipart boundary");
	}
	// check for boundary content
	if (line != cbnd) {
	  throw Exception ("multipart-error", 
			   "invalid bounday in multipart content");
	}
	// create a single part buffer
	SinglePart* part = new SinglePart ();
	// parse the header into the single part
	part->hparse (mbuf);
	// get the buffer upto a boundary
	Buffer* pbuf = mbuf.cpbnds (cbnd);
	if (pbuf == nullptr) {
	  throw Exception ("internal-error", "nil buffer in copy boundary");
	}
	// normaly the last character must be lf or crlf that we can remove
	if (pbuf->rmcrnl () == false) {
	  throw Exception ("multipart-error", 
			   "inconsistent single part buffer at boundary");
	}
	// set the single part buffer and add it to the multipart
	part->setpbuf (pbuf);
	d_mprt.add (part);
      }
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the multipart length

  long MultiPart::length (void) const {
    rdlock ();
    try {
      long result = d_mprt.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a single part by index

  SinglePart* MultiPart::get (const long index) const {
    rdlock ();
    try {
      SinglePart* result = dynamic_cast <SinglePart*> (d_mprt.get (index));
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
  static const long QUARK_ZONE_LENGTH = 3;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_LENGTH  = zone.intern ("length");
  static const long QUARK_GETSPRT = zone.intern ("get-single-part");
  static const long QUARK_GETBNDS = zone.intern ("get-boundary-string");

  // create a new object in a generic way

  Object* MultiPart::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 1 argument
    if (argc == 1) {
      String bnds  = argv->getstring (0);
      return new MultiPart (bnds);
    }
    // check for 2 arguments
    if (argc == 2) {
      String  bnds = argv->getstring (0);
      Object*  obj = argv->get (1);
      Buffer* mbuf = dynamic_cast <Buffer*> (obj);
      if (mbuf == nullptr) {
	throw Exception ("type-error", "invalid object with multipart",
			 Object::repr (obj));
      }
      return new MultiPart (bnds, *mbuf);
    }
    // invalid arguments
    throw Exception ("argument-error", "invalid arguments with multipart");
  }

  // return true if the given quark is defined

  bool MultiPart::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? Object::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* MultiPart::apply (Evaluable* zobj, Nameset* nset, const long quark,
			    Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();
    
    // dispatch 0 argument
    if (argc == 0) {
      if (quark == QUARK_LENGTH)  return new Integer (length  ());
      if (quark == QUARK_GETBNDS) return new String  (getbnds ());
    }
    // dispatch 1 argument
    if (argc == 1) {
      if (quark == QUARK_GETSPRT) {
	rdlock ();
	try {
	  long index = argv->getlong (0);
	  Object* result = get (index);
	  zobj->post (result);
	  unlock ();
	  return result;
	} catch (...) {
	  unlock ();
	  throw;
	}
      }
    }
    // call the buffer method
    return Object::apply (zobj, nset, quark, argv);
  }
}
