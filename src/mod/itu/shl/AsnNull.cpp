// ---------------------------------------------------------------------------
// - AsnNull.cpp                                                             -
// - afnix:itu module - asn null node class implementation                   -
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
#include "AsnNull.hpp"
#include "AsnNode.hxx"
#include "Evaluable.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the asn primitive null content length
  static const t_long ASN_NULL_CLEN = 0LL;

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer

  void AsnNull::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnNull::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn null

  AsnNull::AsnNull (void) : AsnNode (ASN_UNIV_NULL) {
    reset ();
  }
  
  // create a null node by node and buffer

  AsnNull::AsnNull (const AsnNode& node, const Buffer& cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_NULL) || (d_cstf != false) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn null node state");
    }
    // check the content length
    if (cbuf.length () != ASN_NULL_CLEN) {
      throw Exception ("asn-error", "invalid asn null content length");
    }
  }

  // copy construct this asn null

  AsnNull::AsnNull (const AsnNull& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnNull::repr (void) const {
    return "AsnNull";
  }

  // return a clone of this object

  Object* AsnNull::clone (void) const {
    return new AsnNull (*this);
  }
  
  // assign an asn null to this one

  AsnNull& AsnNull::operator = (const AsnNull& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this null node

  void AsnNull::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_NULL;
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn null content length

  t_long AsnNull::getclen (const t_encr encr) const {
    rdlock ();
    try {
      t_long result = ASN_NULL_CLEN;
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

  // create a new object in a generic way
 
  Object* AsnNull::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnNull;
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn null constructor");
  }
}
