// ---------------------------------------------------------------------------
// - AsnOctets.cpp                                                           -
// - afnix:itu module - asn octet string node class implementation           -
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
#include "Evaluable.hpp"
#include "AsnOctets.hpp"
#include "QuarkZone.hpp"
#include "Exception.hpp"
#include "AsnBuffer.hpp"
#include "InputMapped.hpp"

namespace afnix {

  // -------------------------------------------------------------------------
  // - private section                                                       -
  // -------------------------------------------------------------------------

  // the cer mode indefinite threshold in bytes
  static const long ASN_ICLF_SIZE = 1000;

  // this procedure converts an octet string vector into a buffer
  static Buffer asn_octs_cvtn (const Vector& vnod) {
    // get the node length
    long vlen = vnod.length ();
    // the result buffer
    Buffer result;
    // loop in the bit string vector
    for (long i = 0; i < vlen; i++) {
      Object* obj = vnod.get (i);
      // check for an asn bit string
      AsnOctets* node = dynamic_cast <AsnOctets*> (obj);
      if (node == nullptr) {
        throw Exception ("asn-error", "invalid object in asn octet vector",
                         Object::repr (obj));
      }
      // add into the final buffer
      result.add (node->tobuffer ());
    }
    return result;
  }

  // -------------------------------------------------------------------------
  // - protected section                                                     -
  // -------------------------------------------------------------------------

  // create an asn octet string by tag

  AsnOctets::AsnOctets (const t_octa tagn) : AsnNode (tagn) {
    reset ();
  }

  // write the node body into a buffer

  void AsnOctets::wbody (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // get the buffer size
      long blen = d_octs.length ();
      // write the content
      switch (encr) {
      case ASN_BER:
	// add the data
	buf.add (d_octs);
        break;
      case ASN_DER:
        // check for valid mode
        if ((d_cstf == true) || (d_iclf == true)) {
          throw Exception ("asn-error", "invalid asn node state in der mode");
        }
        // add the data
	buf.add (d_octs);
        break;
      case ASN_CER:
        // check for valid mode
        if (((d_cstf == false) || (d_iclf == false)) && 
            (blen > ASN_ICLF_SIZE)) {
          throw Exception ("asn-error", "invalid asn node state in cer mode");
        }
        if (((d_cstf == true) || (d_iclf == true)) && 
            (blen <= ASN_ICLF_SIZE)) {
          throw Exception ("asn-error", "invalid asn node state in cer mode");
        }
        // check for primitive mode
        if (d_cstf == false) {
          // add the data
	  buf.add (d_octs);
          break;
        }
	// process in indefinite mode
	Buffer sb (ASN_ICLF_SIZE);
	for (long i = 0; i < blen; i++) {
	  sb.add (d_octs.get (i));
	  if ((sb.length () == ASN_ICLF_SIZE) || (i == (blen-1))) {
	    AsnOctets aon (sb);
	    aon.write (encr, buf);
	    sb.reset ();
	  }
	}
        break;
      }
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write the node body into an output stream

  void AsnOctets::wbody (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // get the buffer size
      long blen = d_octs.length ();
      // write the content
      switch (encr) {
      case ASN_BER:
	// write the data
	for (long i = 0; i < blen; i++) {
	  os.write (d_octs.get (i));
	}
	break;
      case ASN_DER:
        // check for valid mode
        if ((d_cstf == true) || (d_iclf == true)) {
          throw Exception ("asn-error", "invalid asn node state in der mode");
        }
	// write the data
	for (long i = 0; i < blen; i++) {
	  os.write (d_octs.get (i));
	}
        break;
      case ASN_CER:
        // check for valid mode
        if (((d_cstf == false) || (d_iclf == false)) && 
            (blen > ASN_ICLF_SIZE)) {
          throw Exception ("asn-error", "invalid asn node state in cer mode");
        }
        if (((d_cstf == true) || (d_iclf == true)) && 
            (blen <= ASN_ICLF_SIZE)) {
          throw Exception ("asn-error", "invalid asn node state in cer mode");
        }
        // check for primitive mode
        if (d_cstf == false) {
	  // write the data
	  for (long i = 0; i < blen; i++) {
	    os.write (d_octs.get (i));
	  }
          break;
        }
	// process in indefinite mode
	Buffer sb (ASN_ICLF_SIZE);
	for (long i = 0; i < blen; i++) {
	  sb.add (d_octs.get (i));
	  if ((sb.length () == ASN_ICLF_SIZE) || (i == (blen-1))) {
	    AsnOctets aon (sb);
	    aon.write (encr, os);
	    sb.reset ();
	  }
	}
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

  // create a default asn octet string

  AsnOctets::AsnOctets (void) : AsnNode (ASN_UNIV_OCTS) {
    reset ();
  }

  // create an asn octet string by string

  AsnOctets::AsnOctets (const String& sval) : AsnNode (ASN_UNIV_OCTS) {
    reset ();
    d_octs.add (sval);
  }

  // create an asn octet string by buffer

  AsnOctets::AsnOctets (const Buffer& obuf) : AsnNode (ASN_UNIV_OCTS) {
    reset ();
    d_octs = obuf;
  }

  // create an octet string node by node and buffer

  AsnOctets::AsnOctets (const AsnNode& node, 
			const Buffer&  cbuf) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_OCTS) || (d_iclf != false)) {
      throw Exception ("asn-error", "invalid asn octet string node state");
    }
    // process in primitive mode
    if (d_cstf == false) {
      d_octs = cbuf;
    } else {
      // parse the buffer as a node vector
      Vector vnod = AsnBuffer::tovnod (cbuf);
      // create the buffer from the node vector
      d_octs = asn_octs_cvtn (vnod);
    }
  }

  // create an octet string node by node and content node vector

  AsnOctets::AsnOctets (const AsnNode& node,
			const Vector&  vnod) : AsnNode (node) {
    // check the node validity
    if ((d_tagn != ASN_UNIV_OCTS) || (d_cstf != true) || (d_iclf != true)) {
      throw Exception ("asn-error", "invalid asn octet string node state");
    }
    // create the buffer from the node vector
    d_octs = asn_octs_cvtn (vnod);
  }

  // copy construct this asn octet string

  AsnOctets::AsnOctets (const AsnOctets& that) {
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_octs = that.d_octs;
      that.unlock ();
    } catch (...) {
      that.unlock ();
      throw;
    }
  }

  // return the node class name

  String AsnOctets::repr (void) const {
    return "AsnOctets";
  }

  // return a clone of this object

  Object* AsnOctets::clone (void) const {
    return new AsnOctets (*this);
  }
  
  // assign an asn octet string to this one

  AsnOctets& AsnOctets::operator = (const AsnOctets& that) {
    // check for self assign
    if (this == &that) return *this;
    // lock and assign
    wrlock ();
    that.rdlock ();
    try {
      AsnNode::operator = (that);
      d_octs = that.d_octs;
      unlock ();
      that.unlock ();
      return *this;
    } catch (...) {
      unlock ();
      that.unlock ();
      throw;
    }
  }

  // reset this asn octet string node

  void AsnOctets::reset (void) {
    wrlock ();
    try {
      // reset the base node
      AsnNode::reset ();
      // adjust the tag and value
      d_tagn = ASN_UNIV_OCTS;
      d_octs.reset ();
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the asn octet string content length

  t_long AsnOctets::getclen (const t_encr encr) const {
    rdlock ();
    try {
      // preset the initial result
      t_long result = d_octs.length ();
      // adjust with the der mode - force to 0 (i.e indefinite length)
      if ((encr == ASN_CER) && (result > ASN_ICLF_SIZE)) result = 0LL;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into a buffer by encoding rule

  void AsnOctets::write (const t_encr encr, Buffer& buf) const {
    rdlock ();
    try {
      // check for the ber mode
      if (encr == ASN_BER) AsnNode::write (encr, buf);
      // check for the der mode
      if (encr == ASN_DER) {
	// check if we need to adjust the node
	if ((d_cstf == true) || (d_iclf == true)) {
	  // fix the asn octet string node
	  AsnOctets abn = *this;
	  abn.d_cstf = false;
	  abn.d_iclf = false;
	  // write the node
	  abn.write (encr, buf);
	} else {
	  AsnNode::write (encr, buf);
	}
      }
      // check for cer mode
      if (encr == ASN_CER) {
	// get the content length
	long clen = d_octs.length ();
	// check for adjustment
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (clen > ASN_ICLF_SIZE)) {
	  // fix the asn octet string node
	  AsnOctets aon = *this;
	  aon.d_cstf = true;
	  aon.d_iclf = true;
	  // write the node
	  aon.write (encr, buf);
	} else {
	  if (((d_cstf == true) || (d_iclf == true)) && 
	      (clen <= ASN_ICLF_SIZE)) {
	    // fix the asn octet string node
	    AsnOctets aon = *this;
	    aon.d_cstf = false;
	    aon.d_iclf = false;
	    // write the node
	    aon.write (encr, buf);
	  } else {
	    AsnNode::write (encr, buf);
	  }
	}
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // write a node into an output stream by encoding rule

  void AsnOctets::write (const t_encr encr, OutputStream& os) const {
    rdlock ();
    try {
      // check for the ber mode
      if (encr == ASN_BER) AsnNode::write (encr, os);
      // check for the der mode
      if (encr == ASN_DER) {
	// check if we need to adjust the node
	if ((d_cstf == true) || (d_iclf == true)) {
	  // fix the asn octet string node
	  AsnOctets aon = *this;
	  aon.d_cstf = false;
	  aon.d_iclf = false;
	  // write the node
	  aon.write (encr, os);
	} else {
	  AsnNode::write (encr, os);
	}
      }
      // check for cer mode
      if (encr == ASN_CER) {
	// get the content length
	long clen = d_octs.length ();
	// check for adjustment
	if (((d_cstf == false) || (d_iclf == false)) && 
	    (clen > ASN_ICLF_SIZE)) {
	  // fix the asn octet string node
	  AsnOctets aon = *this;
	  aon.d_cstf = true;
	  aon.d_iclf = true;
	  // write the node
	  aon.write (encr, os);
	} else {
	  if (((d_cstf == true) || (d_iclf == true)) && 
	      (clen <= ASN_ICLF_SIZE)) {
	    // fix the asn octet string node
	    AsnOctets aon = *this;
	    aon.d_cstf = false;
	    aon.d_iclf = false;
	    // write the node
	    aon.write (encr, os);
	  } else {
	    AsnNode::write (encr, os);
	  }
	}
      }
      // unlock and return
      unlock ();
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node buffer value

  Buffer AsnOctets::tobuffer (void) const {
    rdlock ();
    try {
      Buffer result = d_octs;
      unlock ();
      return result;
    } catch (...) {
      unlock ();
      throw;
    }
  }

  // get the node buffer octet string

  String AsnOctets::tostring (void) const {
    rdlock ();
    try {
      String result = d_octs.format ();
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
  static const long QUARK_TOOBUF = zone.intern ("to-buffer");

  // create a new object in a generic way
 
  Object* AsnOctets::mknew (Vector* argv) {
    long argc = (argv == nullptr) ? 0 : argv->length ();
    // check for 0 argument
    if (argc == 0) return new AsnOctets;
    // check for 1 argument
    if (argc == 1) {
      Object* obj = argv->get (0);
      // check for a string
      String* sobj = dynamic_cast <String*> (obj);
      if (sobj != nullptr) return new AsnOctets (*sobj);
      // check for a buffer
      Buffer* bobj = dynamic_cast <Buffer*> (obj);
      if (bobj != nullptr) return new AsnOctets (*bobj);
      // invalid type
      throw Exception ("type-error", 
		       "invalid object with asn octet string constructor",
		       Object::repr (obj));
    }
    // too many arguments
    throw Exception ("argument-error",
                     "too many argument with asn octet string constructor");
  }

  // return true if the given quark is defined

  bool AsnOctets::isquark (const long quark, const bool hflg) const {
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

  Object* AsnOctets::apply (Evaluable* zobj, Nameset* nset, const long quark,
			  Vector* argv) {
    // get the number of arguments
    long argc = (argv == nullptr) ? 0 : argv->length ();

    // check for 0 argument
    if (argc == 0) {
      if (quark == QUARK_TOOBUF) return new Buffer (tobuffer ());
    }
    // call the asn node method
    return AsnNode::apply (zobj, nset, quark, argv);
  }
}
