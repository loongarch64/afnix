// ---------------------------------------------------------------------------
// - AsnUtfs.cpp                                                             -
// - afnix:itu module - asn utf string node class implementation             -
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
#include "AsnUtfs.hpp"
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

  // this procedure converts an utf string vector into a buffer
  static Buffer asn_utfs_cvtn (const Vector& vnod) {
    // get the node length
    long vlen = vnod.length ();
    // the result buffer
    Buffer result;
    // loop in the bit string vector
    for (long i = 0; i < vlen; i++) {
      Object* obj = vnod.get (i);
      // check for an asn bit string
      AsnUtfs* node = dynamic_cast <AsnUtfs*> (obj);
      if (node == nullptr) {
        throw Exception ("asn-error", 
			 "invalid object in asn utf string vector",
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

  // create a default asn utf string

  AsnUtfs::AsnUtfs (void) : AsnOctets (ASN_UNIV_UTFS) {
    reset ();
  }

  // create an asn utf string by value

  AsnUtfs::AsnUtfs (const String& sval) : AsnOctets (ASN_UNIV_UTFS) {
    // reset the buffer
    reset ();
    // simply add it into the base buffer in mode UTF8
    d_octs.add (sval);
  }
  
  // create an utf string node by node and buffer

  AsnUtfs::AsnUtfs (const AsnNode& node, 
		    const Buffer&  cbuf) : AsnOctets (ASN_UNIV_UTFS) {
    // copy the base node
    AsnNode::operator = (node);
    // check the node validity
    if ((d_tagn != ASN_UNIV_UTFS) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn utf string node state");
    }
    // process in primitive mode
    if (d_cstf == false) {
      d_octs = cbuf;
    } else {
      // parse the buffer as a node vector
      Vector vnod = AsnBuffer::tovnod (cbuf);
      // set the base buffer from the node vector
      d_octs = asn_utfs_cvtn (vnod);
    }
  }

  // create an asn string node by node and content node vector

  AsnUtfs::AsnUtfs (const AsnNode& node,
		    const Vector&  vnod) : AsnOctets (ASN_UNIV_UTFS) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_UTFS) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn utf string node state");
    }
    // copy the base node
    AsnNode::operator = (node);
    // set the base buffer from the node vector
    d_octs = asn_utfs_cvtn (vnod);
  }
  
  // copy construct this string node

  AsnUtfs::AsnUtfs (const AsnUtfs& that) {
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

  String AsnUtfs::repr (void) const {
    return "AsnUtfs";
  }

  // return a clone of this object

  Object* AsnUtfs::clone (void) const {
    return new AsnUtfs (*this);
  }

  // reset this string node

  void AsnUtfs::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnOctets::reset ();
      // force the buffer in utf mode
      d_octs.setemod (Encoding::EMOD_UTF8);
      // adjust the tag and value
      d_tagn = ASN_UNIV_UTFS;
      // copy the string value
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get a string representation

  String AsnUtfs::tostring (void) const {
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
 
  Object* AsnUtfs::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnUtfs;
    // check for 1 argument
    if (argc == 1) {
      String sval = argv->getstring (0);
      return new AsnUtfs (sval);
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn utf string constructor");
  }

  // return true if the given quark is defined

  bool AsnUtfs::isquark (const long quark, const bool hflg) const {
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

  Object* AsnUtfs::apply (Evaluable* zobj, Nameset* nset, const long quark,
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
