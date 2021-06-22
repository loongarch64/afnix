// ---------------------------------------------------------------------------
// - AsnOid.cpp                                                              -
// - afnix:itu module - asn oid node class implementation                    -
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
#include "AsnOid.hpp"
#include "AsnNode.hxx"
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
    // check for 0 sid
    if (nsid == 0) return buf;
    // check for 1 sid
    if (nsid == 1) {
      t_octa xsid = oid.get (0) * 40;
      AsnUtils::aidbuf (buf, xsid);
      return buf;
    }
    // add the first 2 sid
    t_octa xsid = oid.get (0);
    t_octa ysid = oid.get (1);
    if (((xsid == 0) || (xsid == 1)) && (ysid > 39)) {
      throw Exception ("asn-error", "out of range sid for primary oid");
    }
    t_octa sid = xsid * 40 + ysid;
    AsnUtils::aidbuf (buf, sid);
    // add the sid to the buffer
    for (long i = 2; i < nsid; i++) AsnUtils::aidbuf (buf, oid.get (i));
    // done
    return buf;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnOid::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get a body buffer
      Buffer oib = oid_to_buf (d_oid);
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

  void AsnOid::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get a body buffer
      Buffer oib = oid_to_buf (d_oid);
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

  AsnOid::AsnOid (void) : AsnNode (ASN_UNIV_OIDN) {
    d_oid.reset ();
  }

  // create an asn oid by node and buffer

  AsnOid::AsnOid (const AsnNode& node, const Buffer& cbuf) : AsnNode (node) {
    // reset the oid
    d_oid.reset ();
    // check the node validity
    if ((d_tagn != ASN_UNIV_OIDN) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn oid node state");
    }
    // we need here a dynamic buffer
    Buffer buf = cbuf;
    // read the first identifier
    if (buf.empty () == false) {
      // decode the asn id
      t_octa aid = AsnUtils::bufaid (buf);
      // extract x and y component
      t_octa xid = aid / 40;
      t_octa yid = aid % 40;
      // add the first two ids
      add (xid);
      add (yid);
    }
    // loop in the content buffer to extract the remaining asn identifier
    while (buf.empty () == false) {
      // decode the asn id
      t_octa aid = AsnUtils::bufaid (buf);
      // add the aid to the node
      add (aid);
    }
  }

  // copy construct this asn oid

  AsnOid::AsnOid (const AsnOid& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_oid = that.d_oid;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnOid::repr (void) const {
    return "AsnOid";
  }

  // return a clone of this object

  Object* AsnOid::clone (void) const {
    return new AsnOid (*this);
  }
  
  // assign an asn oid to this one

  AsnOid& AsnOid::operator = (const AsnOid& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_oid = that.d_oid;
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

  void AsnOid::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_OIDN;
      // fix the oid
      d_oid.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn oid content length

  t_long AsnOid::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // get the oid buffer
      Buffer    buf = oid_to_buf (d_oid);
      t_long result = buf.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the number of sid

  long AsnOid::getnsid (void) const {
    rdlock ();
    try {
      long result = d_oid.length ();
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a sid by index

  t_octa AsnOid::getsid (const long index) const {
    rdlock ();
    try {
      t_octa result = d_oid.get (index);
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // add a sid to the oid node

  void AsnOid::add (const t_octa sid) {
    wrlock ();
    try {
      d_oid.add (sid);
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a the oid object
  
  Oid AsnOid::getoid (void) const {
    rdlock ();
    try {
      Oid result = d_oid;
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
 
  Object* AsnOid::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create the result oid
    AsnOid* result = new AsnOid;
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
  
  bool AsnOid::isquark (const long quark, const bool hflg) const {
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

  Object* AsnOid::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
