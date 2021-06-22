// ---------------------------------------------------------------------------
// - AsnInteger.cpp                                                          -
// - afnix:itu module - asn integer node class implementation                -
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
#include "AsnNode.hxx"
#include "Integer.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnInteger.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnInteger::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get the relatif byte size
      long cbsz = d_rval.getbbs ();
      // process the null value
      if (cbsz == 0) {
	buf.add (nilc);
	unlock ();
	return;
      }
      // prepare signed buffer
      t_byte sbuf[cbsz];
      long size = d_rval.tosbuf (sbuf, cbsz);
      // write into the buffer
      for (long i = 0; i < size; i++) buf.add ((char) sbuf[i]);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnInteger::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get the relatif byte size
      long cbsz = d_rval.getbbs ();
      // process the null value
      if (cbsz == 0) {
	os.write (nilc);
	unlock ();
	return;
      }
      // prepare signed buffer
      t_byte sbuf[cbsz];
      long size = d_rval.tosbuf (sbuf, cbsz);
      // write into the buffer
      for (long i = 0; i < size; i++) os.write ((char) sbuf[i]);
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn integer

  AsnInteger::AsnInteger (void) : AsnNode (ASN_UNIV_INTG) {
    reset ();
  }
  
  // create an asn integer by value

  AsnInteger::AsnInteger (const Relatif& rval) : AsnNode (ASN_UNIV_INTG) {
    reset ();
    d_rval = rval;
  }

  // create an integer node by node and buffer

  AsnInteger::AsnInteger (const AsnNode& node, 
			  const Buffer&  cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_INTG) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn integer node state");
    }
    // check the content length
    t_long clen = cbuf.length ();
    if (clen < 1) {
      throw Exception ("asn-error", "invalid asn integer content length");
    }
    // bind the content
    t_byte sbuf[clen];
    for (long i = 0; i < clen; i++) sbuf[i] = cbuf.get (i);
    d_rval = Relatif (sbuf, clen, true);
  }

  // copy construct this asn integer

  AsnInteger::AsnInteger (const AsnInteger& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_rval = that.d_rval;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnInteger::repr (void) const {
    return "AsnInteger";
  }

  // return a clone of this object

  Object* AsnInteger::clone (void) const {
    return new AsnInteger (*this);
  }
  
  // assign an asn integer to this one

  AsnInteger& AsnInteger::operator = (const AsnInteger& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_rval = that.d_rval;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this integer node

  void AsnInteger::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_INTG;
      d_rval = 0;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node content length

  t_long AsnInteger::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // get the byte buffer size
      long cbsz = d_rval.getbbs ();
      // check for null or 1
      if (cbsz < 2) return 1;
      // adjust for signed buffer
      t_byte sbuf[cbsz];
      long result = d_rval.tosbuf (sbuf, cbsz);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node integer as a relatif

  Relatif AsnInteger::torelatif (void) const {
    rdlock ();
    try {
      Relatif result = d_rval;
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
  static const long QUARK_TOREL = zone.intern ("to-relatif");

  // create a new object in a generic way
 
  Object* AsnInteger::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnInteger;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for an integer
      Integer* iobj = dynamic_cast <Integer*> (obj);
      if (iobj != nullptr) {
	t_long ival = iobj->tolong ();
	return new AsnInteger (ival);
      }
      // check for a relatif
      Relatif* zobj = dynamic_cast <Relatif*> (obj);
      if (zobj != nullptr) return new AsnInteger (*zobj);
      // invalid object
      throw Exception ("type-error", "invalid object with asn integer",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn integer constructor");
  }

  // return true if the given quark is defined

  bool AsnInteger::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? AsnNode::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* AsnInteger::apply (Evaluable* zobj, Nameset* nset, const long quark,
			     Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOREL) return new Relatif (torelatif ());
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
