// ---------------------------------------------------------------------------
// - AsnUnvs.cpp                                                             -
// - afnix:itu module - asn universal string node class implementation       -
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
#include "AsnUnvs.hpp"
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
  static Buffer asn_unvs_cvtn (const Vector& vnod) {
    // get the node length
    long vlen = vnod.length ();
    // the result buffer
    Buffer result;
    // loop in the bit string vector
    for (long i = 0; i < vlen; i++) {
      Object* obj = vnod.get (i);
      // check for an asn bit string
      AsnUnvs* node = dynamic_cast <AsnUnvs*> (obj);
      if (node == nullptr) {
        throw Exception ("asn-error", 
			 "invalid object in asn universal string vector",
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

  AsnUnvs::AsnUnvs (void) : AsnOctets (ASN_UNIV_UNVS) {
    reset ();
  }

  // create an asn string by value

  AsnUnvs::AsnUnvs (const String& sval) : AsnOctets (ASN_UNIV_UNVS) {
    // reset the buffer
    reset ();
    // add the string in quad network mode
    long slen = sval.length ();
    for (long i = 0; i < slen; i++) d_octs.add ((t_quad) sval[i], false);
  }
  
  // create a bmp string node by node and buffer

  AsnUnvs::AsnUnvs (const AsnNode& node, 
		    const Buffer&  cbuf) : AsnOctets (ASN_UNIV_UNVS) {
    // copy the base node
    AsnNode::operator = (node);
    // check the node validity
    if ((d_tagn != ASN_UNIV_UNVS) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn universal string node state");
    }
    // process in primitive mode
    if (d_cstf == false) {
      d_octs = cbuf;
    } else {
      // parse the buffer as a node vector
      Vector vnod = AsnBuffer::tovnod (cbuf);
      // set the base buffer from the node vector
      d_octs = asn_unvs_cvtn (vnod);
    }
    // make sure we have the right length
    if ((d_octs.length () % 4) != 0) {
      throw Exception ("asn-error",
		       "invalid buffer length with asn universal string");
    }
  }

  // create an asn string node by node and content node vector

  AsnUnvs::AsnUnvs (const AsnNode& node,
		    const Vector&  vnod) : AsnOctets (ASN_UNIV_UNVS) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_UNVS) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn universal string node state");
    }
    // copy the base node
    AsnNode::operator = (node);
    // set the base buffer from the node vector
    d_octs = asn_unvs_cvtn (vnod);
    // make sure we have the right length
    if ((d_octs.length () % 4) != 0) {
      throw Exception ("asn-error",
		       "invalid buffer length with asn universal string");
    }
  }

  // copy construct this string node

  AsnUnvs::AsnUnvs (const AsnUnvs& that) {
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

  String AsnUnvs::repr (void) const {
    return "AsnUnvs";
  }

  // return a clone of this object

  Object* AsnUnvs::clone (void) const {
    return new AsnUnvs (*this);
  }

  // reset this string node

  void AsnUnvs::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnOctets::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_UNVS;
      // copy the string value
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a string representation

  String AsnUnvs::tostring (void) const {
    rdlock ();
    try {
      // make sure we have the right length
      if ((d_octs.length () % 4) != 0) {
	throw Exception ("asn-error",
			 "invalid buffer length with asn universal string");
      }
      // make a buffer working copy
      Buffer buf = d_octs;
      // prepare result
      String result;
      // get the equivalent buffer length
      long clen = buf.length () / 4;
      for (long i = 0; i < clen; i++) result += buf.readq (false);
      // unlock and return
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
 
  Object* AsnUnvs::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnUnvs;
    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new AsnUnvs (sval);
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn universal string constructor");
  }

  // return true if the given quark is defined

  bool AsnUnvs::isquark (const long quark, const bool hflg) const {
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

  Object* AsnUnvs
::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
