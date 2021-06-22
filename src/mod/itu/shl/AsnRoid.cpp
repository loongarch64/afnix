// ---------------------------------------------------------------------------
// - AsnRoid.cpp                                                             -
// - afnix:itu module - asn relative oid node class implementation           -
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
#include "AsnRoid.hpp"
#include "Integer.hpp"
#include "AsnUtils.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure converts an oid vector into a buffer
  static Buffer oid_to_buf (const Oid& oid) {
    // create a buffer result
    Buffer buf;
    // get the number of sid
    long nsid = oid.length ();
    // add the sid to the buffer
    for (long i = 0; i < nsid; i++) AsnUtils::aidbuf (buf, oid.get (i));
    // done
    return buf;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnRoid::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get a body buffer
      Buffer oib = oid_to_buf (d_roid);
      // add it to the buffer
      switch (encr) {
      case ASN_BER:
	buf.add (oib);
	break;
      case ASN_CER:
      case ASN_DER:
	buf.add (oib);
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnRoid::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get a body buffer
      Buffer oib = oid_to_buf (d_roid);
      // add it the stream
      switch (encr) {
      case ASN_BER:
	while (oib.empty() == false) os.write (oib.read ());
	break;
      case ASN_CER:
      case ASN_DER:
	while (oib.empty() == false) os.write (oib.read ());
	break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn oid

  AsnRoid::AsnRoid (void) : AsnNode (ASN_UNIV_ROID) {
    d_roid.reset ();
  }

  // create an asn oid by node and buffer

  AsnRoid::AsnRoid (const AsnNode& node, const Buffer& cbuf) : AsnNode (node) {
    // reset the oid
    d_roid.reset ();
    // check the node validity
    if ((d_tagn != ASN_UNIV_ROID) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn relative oid node state");
    }
    // we need here a dynamic buffer
    Buffer buf = cbuf;
    // loop in the content buffer to extract the asn identifier
    while (buf.empty () == false) {
      // decode the asn id
      t_octa aid = AsnUtils::bufaid (buf);
      // add the aid to the node
      add (aid);
    }
  }

  // copy construct this asn oid

  AsnRoid::AsnRoid (const AsnRoid& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_roid = that.d_roid;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnRoid::repr (void) const {
    return "AsnRoid";
  }

  // return a clone of this object

  Object* AsnRoid::clone (void) const {
    return new AsnRoid (*this);
  }
  
  // assign an asn oid to this one

  AsnRoid& AsnRoid::operator = (const AsnRoid& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_roid = that.d_roid;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this oid node

  void AsnRoid::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_ROID;
      // fix the oid
      d_roid.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn oid content length

  t_long AsnRoid::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // get the oid buffer
      Buffer    buf = oid_to_buf (d_roid);
      t_long result = buf.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of sid

  long AsnRoid::getnsid (void) const {
    rdlock ();
    try {
      long result = d_roid.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sid by index

  t_octa AsnRoid::getsid (const long index) const {
    rdlock ();
    try {
      t_octa result = d_roid.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a sid to the oid node

  void AsnRoid::add (const t_octa sid) {
    wrlock ();
    try {
      d_roid.add (sid);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a the oid object
  
  Oid AsnRoid::getoid (void) const {
    rdlock ();
    try {
      Oid result = d_roid;
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
  static const long QUARK_ZONE_LENGTH = 4;
  static QuarkZone  zone (QUARK_ZONE_LENGTH);

  // the object supported quarks
  static const long QUARK_GETOID = zone.intern ("get-oid");
  static const long QUARK_SIDADD = zone.intern ("sid-add");
  static const long QUARK_SIDGET = zone.intern ("sid-get");
  static const long QUARK_SIDLEN = zone.intern ("sid-length");

  // create a new object in a generic way
 
  Object* AsnRoid::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create the result oid
    AsnRoid* result = new AsnRoid;
    // loop in the integer arguments
    for (long i = 0; i < argc; i++) {
      // get the associated sid
      t_octa sid = argv->getocta (i);
      // add it to the oid
      result->add (sid);
    }
    // here it is
    return result;
  }
  
  // return true if the given quark is defined
  
  bool AsnRoid::isquark (const long quark, const bool hflg) const {
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

  Object* AsnRoid::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_GETOID) return new Oid (getoid ());
      if (quark == QUARK_SIDLEN) return new Integer (getnsid ());
    }
    // check for 1 argument
    if (argc == 1) {
      if (quark == QUARK_SIDADD) {
	t_octa sid = argv->getocta (0);
	add (sid);
	return nullptr;
      }
      if (quark == QUARK_SIDGET) {
	long index = argv->getlong (0);
	t_octa sid = getsid (index);
	return new Integer (sid);
      }
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
