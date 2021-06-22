// ---------------------------------------------------------------------------
// - AsnBoolean.cpp                                                          -
// - afnix:itu module - asn boolean node class implementation                -
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
#include "Boolean.hpp"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBoolean.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the asn boolean false value
  static const t_byte ASN_BOOL_FALSE = 0x00;
  // the asn boolean true value in CER/DER
  static const t_byte ASN_BOOL_TRUE  = 0xFF;
  // the asn primitive boolean content length
  static const t_long ASN_BOOL_CLEN = 1LL;

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnBoolean::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      switch (encr) {
      case ASN_BER:
	buf.add ((char) d_bval);
	break;
      case ASN_CER:
      case ASN_DER:
	if (d_bval == ASN_BOOL_FALSE) {
	  buf.add ((char) ASN_BOOL_FALSE);
	} else {
	  buf.add ((char) ASN_BOOL_TRUE);
	}	  
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnBoolean::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      switch (encr) {
      case ASN_BER:
	os.write ((char) d_bval);
	break;
      case ASN_CER:
      case ASN_DER:
	if (d_bval == ASN_BOOL_FALSE) {
	  os.write ((char) ASN_BOOL_FALSE);
	} else {
	  os.write ((char) ASN_BOOL_TRUE);
	}	  
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

  // create a default asn boolean

  AsnBoolean::AsnBoolean (void) : AsnNode (ASN_UNIV_BOOL) {
    reset ();
  }
  
  // create an asn boolean by value

  AsnBoolean::AsnBoolean (const bool bval) : AsnNode (ASN_UNIV_BOOL) {
    reset ();
    d_bval = bval ? ASN_BOOL_TRUE : ASN_BOOL_FALSE;
  }

  // create a boolean node by node and buffer

  AsnBoolean::AsnBoolean (const AsnNode& node, 
			  const Buffer& cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_BOOL) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn boolean node state");
    }
    // check the content length
    if (cbuf.length () != ASN_BOOL_CLEN) {
      throw Exception ("asn-error", "invalid asn boolean content length");
    }
    // bind the content
    d_bval = (t_byte) cbuf.get ();
  }

  // copy construct this asn boolean

  AsnBoolean::AsnBoolean (const AsnBoolean& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_bval = that.d_bval;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnBoolean::repr (void) const {
    return "AsnBoolean";
  }

  // return a clone of this object

  Object* AsnBoolean::clone (void) const {
    return new AsnBoolean (*this);
  }
  
  // assign an asn boolean to this one

  AsnBoolean& AsnBoolean::operator = (const AsnBoolean& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_bval = that.d_bval;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this boolean node

  void AsnBoolean::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_BOOL;
      d_bval = ASN_BOOL_FALSE;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn boolean content length

  t_long AsnBoolean::getclen (const t_encr encr) const {
    rdlock ();
    try {
      t_long result = ASN_BOOL_CLEN;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node boolean value

  bool AsnBoolean::tobool (void) const {
    rdlock ();
    try {
      bool result = d_bval;
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
  static const long QUARK_TOBOOL = zone.intern ("to-boolean");

  // create a new object in a generic way
 
  Object* AsnBoolean::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnBoolean;
    // check for 1 argument
    if (argc == 1) {
      bool bval = argv->getbool (0);
      return new AsnBoolean (bval);
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn boolean constructor");
  }

  // return true if the given quark is defined

  bool AsnBoolean::isquark (const long quark, const bool hflg) const {
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

  Object* AsnBoolean::apply (Evaluable* zobj, Nameset* nset, const long quark,
			      Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOBOOL) return new Boolean (tobool ());
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
