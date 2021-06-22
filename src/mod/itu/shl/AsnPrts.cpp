// ---------------------------------------------------------------------------
// - AsnPrts.cpp                                                             -
// - afnix:itu module - asn printable string node class implementation       -
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
#include "AsnPrts.hpp"
#include "AsnNode.hxx"
#include "AsnUtils.hpp"
#include "Evaluable.hpp"
#include "AsnBuffer.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // this procedure converts an octet string vector into a buffer
  static Buffer asn_prts_cvtn (const Vector& vnod) {
    // get the node length
    long vlen = vnod.length ();
    // the result buffer
    Buffer result;
    // loop in the bit string vector
    for (long i = 0; i < vlen; i++) {
      Object* obj = vnod.get (i);
      // check for an asn bit string
      AsnPrts* node = dynamic_cast <AsnPrts*> (obj);
      if (node == nullptr) {
        throw Exception ("asn-error", 
			 "invalid object in asn printable string vector",
                         Object::repr (obj));
      }
      // add into the final buffer
      result.add (node->tobuffer ());
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - class section                                                         -
  // -------------------------------------------------------------------------

  // create a default asn string

  AsnPrts::AsnPrts (void) : AsnOctets (ASN_UNIV_PRTS) {
    reset ();
  }

  // create an asn string by value

  AsnPrts::AsnPrts (const String& sval) : AsnOctets (ASN_UNIV_PRTS) {
    // reset the buffer
    reset ();
    // check the string argument
    if (AsnUtils::isprts (sval) == false) {
      throw Exception ("asn-error", "invalid asn printable string", sval);
    }
    // simply add it into the base buffer - the default mode is BYTE
    d_octs.add (sval);
  }
  
  // create an asn printable string node by node and buffer

  AsnPrts::AsnPrts (const AsnNode& node, 
		    const Buffer&  cbuf) : AsnOctets (ASN_UNIV_PRTS) {
    // copy the base node
    AsnNode::operator = (node);
    // check the node validity
    if ((d_tagn != ASN_UNIV_PRTS) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn printable string node state");
    }
    // process in primitive mode
    if (d_cstf == false) {
      d_octs = cbuf;
    } else {
      // parse the buffer as a node vector
      Vector vnod = AsnBuffer::tovnod (cbuf);
      // set the base buffer from the node vector
      d_octs = asn_prts_cvtn (vnod);
    }
  }

  // create an asn string node by node and content node vector

  AsnPrts::AsnPrts (const AsnNode& node,
		    const Vector&  vnod) : AsnOctets (ASN_UNIV_PRTS) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_PRTS) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn printable string node state");
    }
    // copy the base node
    AsnNode::operator = (node);
    // set the base buffer from the node vector
    d_octs = asn_prts_cvtn (vnod);
  }
  
  // copy construct this string node

  AsnPrts::AsnPrts (const AsnPrts& that) {
    that.rdlock ();
    try {
      AsnOctets::operator = (that);
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }
  
  // return the node class name

  String AsnPrts::repr (void) const {
    return "AsnPrts";
  }

  // return a clone of this object

  Object* AsnPrts::clone (void) const {
    return new AsnPrts (*this);
  }

  // reset this string node

  void AsnPrts::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnOctets::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_PRTS;
      // copy the string value
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a string representation

  String AsnPrts::tostring (void) const {
    rdlock ();
    try {
      String result = d_octs.tostring ();
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
  static const long QUARK_TOSTRING = zone.intern ("to-string");

  // create a new object in a generic way
 
  Object* AsnPrts::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnPrts;
    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new AsnPrts (sval);
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn printable string constructor");
  }

  // return true if the given quark is defined

  bool AsnPrts::isquark (const long quark, const bool hflg) const {
    rdlock ();
    if (zone.exists (quark) == true) {
      unlock ();
      return true;
    }
    bool result = hflg ? AsnOctets::isquark (quark, hflg) : false;
    unlock ();
    return result;
  }

  // apply this object with a set of arguments and a quark

  Object* AsnPrts::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOSTRING) return new String (tostring ());
    }
    // call the asn octets method
    return AsnOctets::apply (zobj, nset, quark, argv);
  }
}
