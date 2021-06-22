// ---------------------------------------------------------------------------
// - AsnEoc.cpp                                                              -
// - afnix:itu module - asn eoc node class implementation                    -
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
#include "AsnEoc.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the asn primitive eoc content length
  static const t_long ASN_PEOC_CLEN = 0LL;

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // write the node body into a buffer
  void AsnEoc::wbody (const t_encr encr, Buffer& buf) const {}

  // write the node footer into an output stream
  void AsnEoc::wbody (const t_encr encr, OutputStream& os) const {}

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create an empty node

  AsnEoc::AsnEoc (void) {}

  // copy construct this asn eoc

  AsnEoc::AsnEoc (const AsnEoc& that) {
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

  String AsnEoc::repr (void) const {
    return "AsnEoc";
  }

  // return a clone of this object

  Object* AsnEoc::clone (void) const {
    return new AsnEoc (*this);
  }

  // assign an asn eoc to this one

  AsnEoc& AsnEoc::operator = (const AsnEoc& that) {
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

  // get the asn boolean content length

  t_long AsnEoc::getclen (const t_encr encr) const {
    rdlock ();
    try {
      t_long result = ASN_PEOC_CLEN;
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
 
  Object* AsnEoc::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // create a default eoc node object
    if (argc == 0) return new AsnEoc;
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn eoc constructor");
  }
}
